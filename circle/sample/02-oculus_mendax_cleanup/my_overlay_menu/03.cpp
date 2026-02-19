#include "kernel.h"
#include "global.h"

// Deterministic, integration-ready GPU menu overlay module.
// - No changes to user shader pipeline.
// - One dedicated menu fragment shader.
// - Atlas-driven tiles (8x8, 64 tiles).
// - CPU uploads only tile indices + fixed slot rectangles.

namespace
{
    static const int MENU_GPU_MAX_SLOTS  = 20;

    // Slot rectangle: x, y, w, h in normalized screen space [0..1], relative to origin.
    struct SlotRect
    {
        float x;
        float y;
        float w;
        float h;
    };

    struct MenuGpuFrameData
    {
        int slotCount;
        int tileIndex[MENU_GPU_MAX_SLOTS];
    };

    struct MenuGpuState
    {
        bool initialized;

        GLuint program;
        GLuint vertexShader;
        GLuint fragmentShader;

        GLint aVertex;
        GLint uAtlas;
        GLint uOrigin;
        GLint uSlotCount;
        GLint uTileIndex;
        GLint uSlotRect;

        GLuint atlasTexture;

        // Fixed layout (relative to origin) to avoid per-frame CPU layout math.
        SlotRect slots[MENU_GPU_MAX_SLOTS];
        int slotCount;
    };

    static MenuGpuState g_menuGpu = {0};

    static const char* kMenuVertexShader =
        "attribute vec4 vertex;\n"
        "varying vec2 tcoord;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
        "    tcoord = vertex.zw;\n"
        "}\n";

    static const char* kMenuFragmentShader =
        "precision mediump float;\n"
        "varying vec2 tcoord;\n"
        "uniform sampler2D u_atlas;\n"
        "uniform vec2 u_origin;\n"
        "uniform int u_slotCount;\n"
        "uniform int u_tileIndex[20];\n"
        "uniform vec4 u_slotRect[20];\n"
        "\n"
        "vec2 atlas_uv_from_tile(int idx, vec2 localUv)\n"
        "{\n"
        "    float fidx = float(idx);\n"
        "    float tileY = floor(fidx / 8.0);\n"
        "    float tileX = fidx - tileY * 8.0;\n"
        "    vec2 base = vec2(tileX, tileY) / 8.0;\n"
        "    return base + localUv / 8.0;\n"
        "}\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    vec4 outColor = vec4(0.0);\n"
        "\n"
        "    for (int i = 0; i < 20; ++i)\n"
        "    {\n"
        "        if (i >= u_slotCount)\n"
        "        {\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        vec4 r = u_slotRect[i];\n"
        "        vec2 p = tcoord - (u_origin + r.xy);\n"
        "\n"
        "        if (p.x < 0.0 || p.y < 0.0 || p.x > r.z || p.y > r.w)\n"
        "        {\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        vec2 localUv = vec2(p.x / r.z, p.y / r.w);\n"
        "        vec2 atlasUv = atlas_uv_from_tile(u_tileIndex[i], localUv);\n"
        "        vec4 tileColor = texture2D(u_atlas, atlasUv);\n"
        "\n"
        "        // Last matching slot wins (deterministic painter ordering).\n"
        "        outColor = tileColor;\n"
        "    }\n"
        "\n"
        "    gl_FragColor = outColor;\n"
        "}\n";

    static int menu_gpu_clamp_tile_index(int index)
    {
        if (index < 0)
        {
            return 0;
        }
        if (index > 63)
        {
            return 63;
        }
        return index;
    }

    static GLuint menu_gpu_compile_shader(GLenum type, const char* source)
    {
        GLuint shader = glCreateShader(type);
        if (shader == 0)
        {
            return 0;
        }

        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);

        GLint ok = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (ok != GL_TRUE)
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    static bool menu_gpu_link_program(MenuGpuState* s)
    {
        s->program = glCreateProgram();
        if (s->program == 0)
        {
            return false;
        }

        glAttachShader(s->program, s->vertexShader);
        glAttachShader(s->program, s->fragmentShader);
        glLinkProgram(s->program);

        GLint ok = GL_FALSE;
        glGetProgramiv(s->program, GL_LINK_STATUS, &ok);
        if (ok != GL_TRUE)
        {
            glDeleteProgram(s->program);
            s->program = 0;
            return false;
        }

        s->aVertex    = glGetAttribLocation (s->program, "vertex");
        s->uAtlas     = glGetUniformLocation(s->program, "u_atlas");
        s->uOrigin    = glGetUniformLocation(s->program, "u_origin");
        s->uSlotCount = glGetUniformLocation(s->program, "u_slotCount");
        s->uTileIndex = glGetUniformLocation(s->program, "u_tileIndex");
        s->uSlotRect  = glGetUniformLocation(s->program, "u_slotRect");

        return true;
    }

    static void menu_gpu_setup_default_layout(MenuGpuState* s)
    {
        // Basic deterministic layout:
        // - 8 mode icons in two rows of four.
        // - 4 BPM digits + dot + BPM icon on a lower strip.
        // - 6 background/activity tiles (same index, scalable tiles).

        int n = 0;

        // Activity background tiles (scalable).
        s->slots[n++] = (SlotRect){0.00f, 0.00f, 0.23f, 0.23f};
        s->slots[n++] = (SlotRect){0.25f, 0.00f, 0.23f, 0.23f};
        s->slots[n++] = (SlotRect){0.52f, 0.00f, 0.23f, 0.23f};
        s->slots[n++] = (SlotRect){0.77f, 0.00f, 0.23f, 0.23f};

        // 8 mode icons.
        s->slots[n++] = (SlotRect){0.03f, 0.03f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.12f, 0.03f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.03f, 0.12f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.12f, 0.12f, 0.08f, 0.08f};

        s->slots[n++] = (SlotRect){0.55f, 0.03f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.64f, 0.03f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.55f, 0.12f, 0.08f, 0.08f};
        s->slots[n++] = (SlotRect){0.64f, 0.12f, 0.08f, 0.08f};

        // BPM strip: d0 d1 d2 d3 dot icon.
        s->slots[n++] = (SlotRect){0.20f, 0.30f, 0.06f, 0.10f};
        s->slots[n++] = (SlotRect){0.27f, 0.30f, 0.06f, 0.10f};
        s->slots[n++] = (SlotRect){0.34f, 0.30f, 0.06f, 0.10f};
        s->slots[n++] = (SlotRect){0.41f, 0.30f, 0.06f, 0.10f};
        s->slots[n++] = (SlotRect){0.48f, 0.30f, 0.03f, 0.10f};
        s->slots[n++] = (SlotRect){0.53f, 0.30f, 0.08f, 0.10f};

        // Remaining slots reserved for extension.
        while (n < MENU_GPU_MAX_SLOTS)
        {
            s->slots[n++] = (SlotRect){0.0f, 0.0f, 0.0f, 0.0f};
        }

        s->slotCount = 18;
    }

    static void menu_gpu_extract_bpm_digits(unsigned long bpmA, unsigned long bpmB, int outDigits[4])
    {
        // Deterministic conversion to four decimal digits (ABCD):
        // - first two digits from bpmA (00..99),
        // - next two digits from bpmB (00..99).
        unsigned int a = static_cast<unsigned int>(bpmA % 100UL);
        unsigned int b = static_cast<unsigned int>(bpmB % 100UL);

        outDigits[0] = static_cast<int>((a / 10U) % 10U);
        outDigits[1] = static_cast<int>(a % 10U);
        outDigits[2] = static_cast<int>((b / 10U) % 10U);
        outDigits[3] = static_cast<int>(b % 10U);
    }

    static void menu_gpu_build_frame_data(MenuGpuFrameData* outFrame)
    {
        outFrame->slotCount = g_menuGpu.slotCount;

        // Background tile index for activity feedback (fixed index placeholder).
        outFrame->tileIndex[0] = 32;
        outFrame->tileIndex[1] = 32;
        outFrame->tileIndex[2] = 32;
        outFrame->tileIndex[3] = 32;

        // Mode tiles from mode_storage_buffers.
        outFrame->tileIndex[4]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH0_MODE][current_buffer]);
        outFrame->tileIndex[5]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH1_MODE][current_buffer]);
        outFrame->tileIndex[6]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH2_MODE][current_buffer]);
        outFrame->tileIndex[7]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH3_MODE][current_buffer]);
        outFrame->tileIndex[8]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH4_MODE][current_buffer]);
        outFrame->tileIndex[9]  = menu_gpu_clamp_tile_index(mode_storage_buffers[CH5_MODE][current_buffer]);
        outFrame->tileIndex[10] = menu_gpu_clamp_tile_index(mode_storage_buffers[CH6_MODE][current_buffer]);
        outFrame->tileIndex[11] = menu_gpu_clamp_tile_index(mode_storage_buffers[CH7_MODE][current_buffer]);

        int bpmDigits[4] = {0, 0, 0, 0};
        menu_gpu_extract_bpm_digits(resultBPM[0], resultBPM[1], bpmDigits);

        // Digit tiles are expected at atlas indices 0..9.
        outFrame->tileIndex[12] = menu_gpu_clamp_tile_index(bpmDigits[0]);
        outFrame->tileIndex[13] = menu_gpu_clamp_tile_index(bpmDigits[1]);
        outFrame->tileIndex[14] = menu_gpu_clamp_tile_index(bpmDigits[2]);
        outFrame->tileIndex[15] = menu_gpu_clamp_tile_index(bpmDigits[3]);

        // Dot tile + BPM icon tile.
        outFrame->tileIndex[16] = 10;
        outFrame->tileIndex[17] = 11;

        for (int i = 18; i < MENU_GPU_MAX_SLOTS; ++i)
        {
            outFrame->tileIndex[i] = 0;
        }
    }
}

// Public integration-ready entry points (not wired yet).
bool menu_gpu_init(GLuint atlasTexture)
{
    MenuGpuState* s = &g_menuGpu;

    s->vertexShader   = menu_gpu_compile_shader(GL_VERTEX_SHADER,   kMenuVertexShader);
    s->fragmentShader = menu_gpu_compile_shader(GL_FRAGMENT_SHADER, kMenuFragmentShader);

    if (s->vertexShader == 0 || s->fragmentShader == 0)
    {
        return false;
    }

    if (!menu_gpu_link_program(s))
    {
        return false;
    }

    s->atlasTexture = atlasTexture;
    menu_gpu_setup_default_layout(s);
    s->initialized = true;

    return true;
}

void menu_gpu_shutdown(void)
{
    MenuGpuState* s = &g_menuGpu;

    if (s->program != 0)
    {
        glDeleteProgram(s->program);
        s->program = 0;
    }
    if (s->vertexShader != 0)
    {
        glDeleteShader(s->vertexShader);
        s->vertexShader = 0;
    }
    if (s->fragmentShader != 0)
    {
        glDeleteShader(s->fragmentShader);
        s->fragmentShader = 0;
    }

    s->initialized = false;
}

void menu_gpu_render(GLuint fullscreenQuadVbo, float originX, float originY)
{
    MenuGpuState* s = &g_menuGpu;
    if (!s->initialized)
    {
        return;
    }

    MenuGpuFrameData frame;
    menu_gpu_build_frame_data(&frame);

    glUseProgram(s->program);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVbo);

    glVertexAttribPointer(static_cast<GLuint>(s->aVertex), 4, GL_FLOAT, 0, 16, 0);
    glEnableVertexAttribArray(static_cast<GLuint>(s->aVertex));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s->atlasTexture);

    if (s->uAtlas != -1)     glUniform1i(s->uAtlas, 0);
    if (s->uOrigin != -1)    glUniform2f(s->uOrigin, originX, originY);
    if (s->uSlotCount != -1) glUniform1i(s->uSlotCount, frame.slotCount);
    if (s->uTileIndex != -1) glUniform1iv(s->uTileIndex, MENU_GPU_MAX_SLOTS, frame.tileIndex);
    if (s->uSlotRect != -1)  glUniform4fv(s->uSlotRect, MENU_GPU_MAX_SLOTS, reinterpret_cast<const GLfloat*>(s->slots));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_BLEND);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}