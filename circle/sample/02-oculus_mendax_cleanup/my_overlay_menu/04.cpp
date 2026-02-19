#include "kernel.h"
#include "global.h"

namespace
{
    static const int MENU_GPU_ATLAS_GRID = 8;
    static const int MENU_GPU_MAX_TILES  = 20;

    struct MenuTileRect
    {
        float x;
        float y;
        float w;
        float h;
    };

    struct MenuGpuState
    {
        bool   is_initialized;
        GLuint program;
        GLuint atlas_texture;
        GLint  a_vertex;
        GLint  u_atlas;
        GLint  u_tile_count;
        GLint  u_tile_rect;
        GLint  u_tile_index;
        int    tile_count;
        int    tile_index[MENU_GPU_MAX_TILES];
        MenuTileRect tile_rect[MENU_GPU_MAX_TILES];
    };

    static MenuGpuState g_menu_gpu = { false, 0, 0, -1, -1, -1, -1, -1, 0, {0}, {{0.0f,0.0f,0.0f,0.0f}} };

    static const char* MENU_GPU_FRAGMENT_SHADER =
        "precision mediump float;\n"
        "varying vec2 tcoord;\n"
        "uniform sampler2D u_menu_atlas;\n"
        "uniform int u_menu_tile_count;\n"
        "uniform vec4 u_menu_tile_rect[20];\n"
        "uniform float u_menu_tile_index[20];\n"
        "const float kAtlasGrid = 8.0;\n"
        "void main()\n"
        "{\n"
        "    vec4 out_color = vec4(0.0, 0.0, 0.0, 0.0);\n"
        "    for (int i = 0; i < 20; ++i)\n"
        "    {\n"
        "        if (i >= u_menu_tile_count)\n"
        "            break;\n"
        "\n"
        "        vec4 rect = u_menu_tile_rect[i];\n"
        "        float in_x = step(rect.x, tcoord.x) * step(tcoord.x, rect.x + rect.z);\n"
        "        float in_y = step(rect.y, tcoord.y) * step(tcoord.y, rect.y + rect.w);\n"
        "\n"
        "        if (in_x * in_y > 0.5)\n"
        "        {\n"
        "            float local_x = (tcoord.x - rect.x) / rect.z;\n"
        "            float local_y = (tcoord.y - rect.y) / rect.w;\n"
        "\n"
        "            float idx = floor(u_menu_tile_index[i] + 0.5);\n"
        "            float tile_x = mod(idx, kAtlasGrid);\n"
        "            float tile_y = floor(idx / kAtlasGrid);\n"
        "\n"
        "            vec2 uv = vec2((tile_x + local_x) / kAtlasGrid,\n"
        "                           (tile_y + local_y) / kAtlasGrid);\n"
        "\n"
        "            vec4 tile_color = texture2D(u_menu_atlas, uv);\n"
        "            out_color = mix(out_color, tile_color, tile_color.a);\n"
        "        }\n"
        "    }\n"
        "\n"
        "    gl_FragColor = out_color;\n"
        "}\n";

    static inline int clamp_int(int v, int lo, int hi)
    {
        if (v < lo) return lo;
        if (v > hi) return hi;
        return v;
    }

    static void set_rect(int slot, float x, float y, float w, float h)
    {
        g_menu_gpu.tile_rect[slot].x = x;
        g_menu_gpu.tile_rect[slot].y = y;
        g_menu_gpu.tile_rect[slot].w = w;
        g_menu_gpu.tile_rect[slot].h = h;
    }

    static void configure_fixed_layout()
    {
        const float ox = 0.05f;
        const float oy = 0.08f;
        const float s  = 0.08f;
        const float g  = 0.01f;

        // 8 channel icons: 2 rows x 4 columns.
        set_rect(0, ox + (s + g) * 0.0f, oy + (s + g) * 0.0f, s, s);
        set_rect(1, ox + (s + g) * 1.0f, oy + (s + g) * 0.0f, s, s);
        set_rect(2, ox + (s + g) * 2.0f, oy + (s + g) * 0.0f, s, s);
        set_rect(3, ox + (s + g) * 3.0f, oy + (s + g) * 0.0f, s, s);

        set_rect(4, ox + (s + g) * 0.0f, oy + (s + g) * 1.0f, s, s);
        set_rect(5, ox + (s + g) * 1.0f, oy + (s + g) * 1.0f, s, s);
        set_rect(6, ox + (s + g) * 2.0f, oy + (s + g) * 1.0f, s, s);
        set_rect(7, ox + (s + g) * 3.0f, oy + (s + g) * 1.0f, s, s);

        // BPM readout area: 4 digits + dot + icon.
        const float by = oy + (s + g) * 2.5f;
        const float bw = s * 0.75f;
        const float bh = s * 0.95f;

        set_rect(8,  ox + 0.00f, by, bw, bh);   // digit 1
        set_rect(9,  ox + 0.08f, by, bw, bh);   // digit 2
        set_rect(10, ox + 0.16f, by, bw, bh);   // digit 3
        set_rect(11, ox + 0.24f, by, bw, bh);   // digit 4
        set_rect(12, ox + 0.32f, by, bw * 0.45f, bh); // dot
        set_rect(13, ox + 0.36f, by, bw, bh);   // bpm icon

        // Two scalable background tiles (left/right blocks).
        set_rect(14, ox - 0.01f, oy - 0.01f, (s + g) * 4.0f + 0.02f, (s + g) * 2.0f + 0.02f);
        set_rect(15, ox - 0.01f, by - 0.01f,  0.46f, bh + 0.02f);

        g_menu_gpu.tile_count = 16;
    }

    static void update_dynamic_tile_indices()
    {
        // CH0..CH7 mode icons.
        g_menu_gpu.tile_index[0] = clamp_int(mode_storage_buffers[CH0_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[1] = clamp_int(mode_storage_buffers[CH1_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[2] = clamp_int(mode_storage_buffers[CH2_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[3] = clamp_int(mode_storage_buffers[CH3_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[4] = clamp_int(mode_storage_buffers[CH4_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[5] = clamp_int(mode_storage_buffers[CH5_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[6] = clamp_int(mode_storage_buffers[CH6_MODE][current_buffer], 0, 63);
        g_menu_gpu.tile_index[7] = clamp_int(mode_storage_buffers[CH7_MODE][current_buffer], 0, 63);

        // BPM numeric value from resultBPM[0], optional decimal from resultBPM[1].
        unsigned long bpm_int = resultBPM[0] % 10000UL;
        unsigned long bpm_dec = resultBPM[1] % 10UL;

        g_menu_gpu.tile_index[8]  = (int)((bpm_int / 1000UL) % 10UL);
        g_menu_gpu.tile_index[9]  = (int)((bpm_int / 100UL)  % 10UL);
        g_menu_gpu.tile_index[10] = (int)((bpm_int / 10UL)   % 10UL);
        g_menu_gpu.tile_index[11] = (int)( bpm_int           % 10UL);

        g_menu_gpu.tile_index[12] = 10; // dot glyph assumed at tile 10.
        g_menu_gpu.tile_index[13] = (int)bpm_dec;

        // Background tiles (fixed indices; atlas-specific).
        g_menu_gpu.tile_index[14] = 48;
        g_menu_gpu.tile_index[15] = 49;
    }

    static GLuint compile_fragment_shader_or_zero(const char* src)
    {
        GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
        if (shader == 0)
            return 0;

        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }
}

bool menu_gpu_init(GLuint shared_vertex_shader, GLuint atlas_texture)
{
    if (g_menu_gpu.is_initialized)
        return true;

    if (shared_vertex_shader == 0 || atlas_texture == 0)
        return false;

    GLuint fsh = compile_fragment_shader_or_zero(MENU_GPU_FRAGMENT_SHADER);
    if (fsh == 0)
        return false;

    GLuint prg = glCreateProgram();
    glAttachShader(prg, shared_vertex_shader);
    glAttachShader(prg, fsh);
    glLinkProgram(prg);
    glDeleteShader(fsh);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(prg, GL_LINK_STATUS, &link_ok);
    if (link_ok == GL_FALSE)
    {
        glDeleteProgram(prg);
        return false;
    }

    g_menu_gpu.program      = prg;
    g_menu_gpu.atlas_texture = atlas_texture;
    g_menu_gpu.a_vertex     = glGetAttribLocation(prg, "vertex");
    g_menu_gpu.u_atlas      = glGetUniformLocation(prg, "u_menu_atlas");
    g_menu_gpu.u_tile_count = glGetUniformLocation(prg, "u_menu_tile_count");
    g_menu_gpu.u_tile_rect  = glGetUniformLocation(prg, "u_menu_tile_rect");
    g_menu_gpu.u_tile_index = glGetUniformLocation(prg, "u_menu_tile_index");

    configure_fixed_layout();
    update_dynamic_tile_indices();

    g_menu_gpu.is_initialized = true;
    return true;
}

void menu_gpu_update()
{
    if (!g_menu_gpu.is_initialized)
        return;

    update_dynamic_tile_indices();
}

void menu_gpu_render(const CKernel::CUBE_STATE_T* state, GLuint fullscreen_vbo)
{
    if (!g_menu_gpu.is_initialized || state == 0)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);

    glUseProgram(g_menu_gpu.program);

    if (g_menu_gpu.a_vertex >= 0)
    {
        glVertexAttribPointer(g_menu_gpu.a_vertex, 4, GL_FLOAT, 0, 16, 0);
        glEnableVertexAttribArray(g_menu_gpu.a_vertex);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_menu_gpu.atlas_texture);

    if (g_menu_gpu.u_atlas >= 0)
        glUniform1i(g_menu_gpu.u_atlas, 0);

    if (g_menu_gpu.u_tile_count >= 0)
        glUniform1i(g_menu_gpu.u_tile_count, g_menu_gpu.tile_count);

    if (g_menu_gpu.u_tile_rect >= 0)
        glUniform4fv(g_menu_gpu.u_tile_rect, g_menu_gpu.tile_count, (const GLfloat*)g_menu_gpu.tile_rect);

    if (g_menu_gpu.u_tile_index >= 0)
    {
        GLfloat idx_as_float[MENU_GPU_MAX_TILES];
        for (int i = 0; i < g_menu_gpu.tile_count; ++i)
            idx_as_float[i] = (GLfloat)g_menu_gpu.tile_index[i];

        glUniform1fv(g_menu_gpu.u_tile_index, g_menu_gpu.tile_count, idx_as_float);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_BLEND);
}

void menu_gpu_shutdown()
{
    if (g_menu_gpu.program != 0)
        glDeleteProgram(g_menu_gpu.program);

    g_menu_gpu = MenuGpuState{ false, 0, 0, -1, -1, -1, -1, -1, 0, {0}, {{0.0f,0.0f,0.0f,0.0f}} };
}