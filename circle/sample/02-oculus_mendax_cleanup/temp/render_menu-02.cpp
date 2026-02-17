#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <GLES2/gl2.h>

namespace gpu_texture_atlas_menu
{
    static const int ATLAS_SIZE_PX = 1024;
    static const int ATLAS_GRID    = 8;
    static const int TILE_SIZE_PX  = ATLAS_SIZE_PX / ATLAS_GRID; // 128

    static const int CHANNEL_COUNT = 8;
    static const int BPM_COUNT     = 6;
    static const int BG_COUNT      = 3;
    static const int MAX_TILES     = CHANNEL_COUNT + BPM_COUNT + BG_COUNT;

    // 8x8 atlas index contract (64 tiles total)
    enum AtlasTile : uint8_t
    {
        // 0..15: regular modes/icons
        TILE_MODE_ADC = 0,
        TILE_MODE_TRG,
        TILE_MODE_BPM,
        TILE_MODE_LF1,
        TILE_MODE_LF2,
        TILE_MODE_AUD,
        TILE_MODE_TEX,
        TILE_MODE_FRM,
        TILE_MODE_PRG,
        TILE_MODE_X0,
        TILE_MODE_X1,
        TILE_MODE_X2,
        TILE_MODE_X3,
        TILE_MODE_X4,
        TILE_MODE_X5,
        TILE_MODE_X6,

        // 16..23: extra mode placeholders
        TILE_EXTRA_0 = 16,
        TILE_EXTRA_1,
        TILE_EXTRA_2,
        TILE_EXTRA_3,
        TILE_EXTRA_4,
        TILE_EXTRA_5,
        TILE_EXTRA_6,
        TILE_EXTRA_7,

        // 24..31: waveforms
        TILE_WAVE_0 = 24,
        TILE_WAVE_1,
        TILE_WAVE_2,
        TILE_WAVE_3,
        TILE_WAVE_4,
        TILE_WAVE_5,
        TILE_WAVE_6,
        TILE_WAVE_7,

        // 32..39: signs / helper icons
        TILE_SIGN_DASH = 32,
        TILE_SIGN_PIPE,
        TILE_SIGN_STAR,
        TILE_SIGN_DOT_SMALL,
        TILE_SIGN_ARROW_LEFT,
        TILE_SIGN_ARROW_RIGHT,
        TILE_SIGN_PLUS,
        TILE_SIGN_MINUS,

        // 40..49: digits 0..9
        TILE_DIGIT_0 = 40,
        TILE_DIGIT_1,
        TILE_DIGIT_2,
        TILE_DIGIT_3,
        TILE_DIGIT_4,
        TILE_DIGIT_5,
        TILE_DIGIT_6,
        TILE_DIGIT_7,
        TILE_DIGIT_8,
        TILE_DIGIT_9,

        // 50..51: decimal point + BPM icon
        TILE_DECIMAL_DOT = 50,
        TILE_BPM_ICON    = 51,

        // 52..55: 4 backgrounds
        TILE_BG_GROUP_A = 52,
        TILE_BG_GROUP_B = 53,
        TILE_BG_BPM     = 54,
        TILE_BG_SPARE   = 55,

        // 56..63 reserved
        TILE_RESERVED_56 = 56,
        TILE_RESERVED_57,
        TILE_RESERVED_58,
        TILE_RESERVED_59,
        TILE_RESERVED_60,
        TILE_RESERVED_61,
        TILE_RESERVED_62,
        TILE_RESERVED_63
    };

    struct Vec2
    {
        float x;
        float y;
    };

    struct TileLayout
    {
        Vec2 centerNdc;
        Vec2 sizeNdc;
    };

    // exposed + centralized placement (relative to center in NDC)
    struct MenuLayout
    {
        TileLayout ch[CHANNEL_COUNT];
        TileLayout bpm[BPM_COUNT];
        TileLayout bg[BG_COUNT];
    };

    static MenuLayout g_layout =
    {
        // channel icons (8)
        {
            {{-0.78f,  0.42f}, {0.16f, 0.16f}},
            {{-0.56f,  0.42f}, {0.16f, 0.16f}},
            {{-0.34f,  0.42f}, {0.16f, 0.16f}},
            {{-0.12f,  0.42f}, {0.16f, 0.16f}},
            {{ 0.12f,  0.42f}, {0.16f, 0.16f}},
            {{ 0.34f,  0.42f}, {0.16f, 0.16f}},
            {{ 0.56f,  0.42f}, {0.16f, 0.16f}},
            {{ 0.78f,  0.42f}, {0.16f, 0.16f}}
        },

        // bpm/icon group (6): [icon][d0][d1][d2][dot][d3]
        {
            {{-0.24f, -0.45f}, {0.14f, 0.14f}},
            {{-0.08f, -0.45f}, {0.11f, 0.14f}},
            {{ 0.04f, -0.45f}, {0.11f, 0.14f}},
            {{ 0.16f, -0.45f}, {0.11f, 0.14f}},
            {{ 0.27f, -0.45f}, {0.06f, 0.14f}},
            {{ 0.37f, -0.45f}, {0.11f, 0.14f}}
        },

        // backgrounds A/B/BPM
        {
            {{-0.45f,  0.42f}, {0.95f, 0.22f}},
            {{ 0.45f,  0.42f}, {0.95f, 0.22f}},
            {{ 0.08f, -0.45f}, {0.82f, 0.20f}}
        }
    };

    struct MenuInput
    {
        uint8_t channelMode[CHANNEL_COUNT]; // mode_storage_buffers[CHx_MODE][current_buffer]
        uint8_t bpmGlyph[BPM_COUNT];        // 6 direct atlas tile indices
        uint8_t menuMode;                   // g_menu_mode_new
        bool buttonEvent;                   // highlight BPM background
    };

    struct DrawItem
    {
        uint8_t atlasIndex;
        TileLayout layout;
        uint8_t layer; // 0 = bg, 1 = fg
    };

    struct DrawList
    {
        DrawItem items[MAX_TILES];
        uint32_t count;
    };

    struct Renderer
    {
        GLuint program;
        GLint  aPos;
        GLint  aUV;
        GLint  uAtlas;
        GLint  uColor;

        GLuint vbo;
        GLuint atlasTexture;

        DrawList list;
    };

    static inline uint8_t mode_to_tile(uint8_t mode)
    {
        switch (mode)
        {
            case 0:  return TILE_MODE_ADC;
            case 1:  return TILE_MODE_TRG;
            case 2:  return TILE_MODE_BPM;
            case 3:  return TILE_MODE_LF1;
            case 4:  return TILE_MODE_LF2;
            case 5:  return TILE_MODE_AUD;
            case 6:  return TILE_MODE_TEX;
            case 7:  return TILE_MODE_FRM;
            case 8:  return TILE_MODE_PRG;
            case 9:  return TILE_MODE_X0;
            case 10: return TILE_MODE_X1;
            case 11: return TILE_MODE_X2;
            case 12: return TILE_MODE_X3;
            case 13: return TILE_MODE_X4;
            case 14: return TILE_MODE_X5;
            case 15: return TILE_MODE_X6;
            default: return TILE_SIGN_DASH;
        }
    }

    static inline void push_item(DrawList& list, uint8_t tile, const TileLayout& layout, uint8_t layer)
    {
        if (list.count >= MAX_TILES)
            return;

        list.items[list.count].atlasIndex = tile;
        list.items[list.count].layout     = layout;
        list.items[list.count].layer      = layer;
        list.count++;
    }

    void build_menu_draw_list(const MenuInput& in, DrawList& out)
    {
        out.count = 0;

        TileLayout bgA = g_layout.bg[0];
        TileLayout bgB = g_layout.bg[1];
        TileLayout bgP = g_layout.bg[2];

        // 5% highlight scaling for backgrounds
        const float highlight = 1.05f;
        if (in.menuMode == 1)
        {
            bgA.sizeNdc.x *= highlight;
            bgA.sizeNdc.y *= highlight;
        }
        if (in.menuMode == 2)
        {
            bgB.sizeNdc.x *= highlight;
            bgB.sizeNdc.y *= highlight;
        }
        if (in.menuMode == 3 || in.buttonEvent)
        {
            bgP.sizeNdc.x *= highlight;
            bgP.sizeNdc.y *= highlight;
        }

        push_item(out, TILE_BG_GROUP_A, bgA, 0);
        push_item(out, TILE_BG_GROUP_B, bgB, 0);
        push_item(out, TILE_BG_BPM,     bgP, 0);

        for (uint8_t i = 0; i < CHANNEL_COUNT; ++i)
        {
            push_item(out, mode_to_tile(in.channelMode[i]), g_layout.ch[i], 1);
        }

        for (uint8_t i = 0; i < BPM_COUNT; ++i)
        {
            push_item(out, in.bpmGlyph[i], g_layout.bpm[i], 1);
        }
    }

    static GLuint compile_shader(GLenum type, const char* src)
    {
        GLuint shader = glCreateShader(type);
        if (!shader)
            return 0;

        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);

        GLint ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    static GLuint create_program()
    {
        static const char* vs =
            "attribute vec2 a_pos;\n"
            "attribute vec2 a_uv;\n"
            "varying vec2 v_uv;\n"
            "void main()\n"
            "{\n"
            "    v_uv = a_uv;\n"
            "    gl_Position = vec4(a_pos.xy, 0.0, 1.0);\n"
            "}\n";

        static const char* fs =
            "precision mediump float;\n"
            "varying vec2 v_uv;\n"
            "uniform sampler2D u_atlas;\n"
            "uniform vec4 u_color;\n"
            "void main()\n"
            "{\n"
            "    vec4 texel = texture2D(u_atlas, v_uv);\n"
            "    gl_FragColor = texel * u_color;\n"
            "}\n";

        GLuint v = compile_shader(GL_VERTEX_SHADER, vs);
        GLuint f = compile_shader(GL_FRAGMENT_SHADER, fs);
        if (!v || !f)
        {
            if (v) glDeleteShader(v);
            if (f) glDeleteShader(f);
            return 0;
        }

        GLuint p = glCreateProgram();
        glAttachShader(p, v);
        glAttachShader(p, f);
        glBindAttribLocation(p, 0, "a_pos");
        glBindAttribLocation(p, 1, "a_uv");
        glLinkProgram(p);

        glDeleteShader(v);
        glDeleteShader(f);

        GLint ok = 0;
        glGetProgramiv(p, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glDeleteProgram(p);
            return 0;
        }

        return p;
    }

    bool init_renderer(Renderer& r, GLuint atlasTexture)
    {
        memset(&r, 0, sizeof(r));

        r.program = create_program();
        if (!r.program)
            return false;

        r.aPos   = glGetAttribLocation(r.program, "a_pos");
        r.aUV    = glGetAttribLocation(r.program, "a_uv");
        r.uAtlas = glGetUniformLocation(r.program, "u_atlas");
        r.uColor = glGetUniformLocation(r.program, "u_color");

        glGenBuffers(1, &r.vbo);
        if (!r.vbo)
            return false;

        r.atlasTexture = atlasTexture;
        return true;
    }

    void set_atlas_texture(Renderer& r, GLuint atlasTexture)
    {
        r.atlasTexture = atlasTexture;
    }

    static void build_quad_vertices(const DrawItem& item, float out[6 * 4])
    {
        const float cx = item.layout.centerNdc.x;
        const float cy = item.layout.centerNdc.y;
        const float hx = item.layout.sizeNdc.x * 0.5f;
        const float hy = item.layout.sizeNdc.y * 0.5f;

        const float x0 = cx - hx;
        const float x1 = cx + hx;
        const float y0 = cy - hy;
        const float y1 = cy + hy;

        const float inv = 1.0f / (float)ATLAS_GRID;
        const int col = item.atlasIndex % ATLAS_GRID;
        const int row = item.atlasIndex / ATLAS_GRID;

        const float u0 = col * inv;
        const float v0 = row * inv;
        const float u1 = u0 + inv;
        const float v1 = v0 + inv;

        // 2 triangles, interleaved: x,y,u,v
        const float vtx[24] =
        {
            x0, y0, u0, v1,
            x1, y0, u1, v1,
            x1, y1, u1, v0,

            x0, y0, u0, v1,
            x1, y1, u1, v0,
            x0, y1, u0, v0
        };

        memcpy(out, vtx, sizeof(vtx));
    }

    void render_menu(const Renderer& r, const DrawList& list)
    {
        if (!r.program || !r.vbo || !r.atlasTexture)
            return;

        glUseProgram(r.program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, r.atlasTexture);
        if (r.uAtlas >= 0) glUniform1i(r.uAtlas, 0);
        if (r.uColor >= 0) glUniform4f(r.uColor, 1.0f, 1.0f, 1.0f, 1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
        glEnableVertexAttribArray((GLuint)r.aPos);
        glEnableVertexAttribArray((GLuint)r.aUV);
        glVertexAttribPointer((GLuint)r.aPos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
        glVertexAttribPointer((GLuint)r.aUV,  2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // stable layering: backgrounds, then icons
        for (uint8_t layer = 0; layer < 2; ++layer)
        {
            for (uint32_t i = 0; i < list.count; ++i)
            {
                if (list.items[i].layer != layer)
                    continue;

                float quad[6 * 4];
                build_quad_vertices(list.items[i], quad);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        glDisableVertexAttribArray((GLuint)r.aPos);
        glDisableVertexAttribArray((GLuint)r.aUV);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_BLEND);
    }

    void shutdown_renderer(Renderer& r)
    {
        if (r.vbo)
        {
            glDeleteBuffers(1, &r.vbo);
            r.vbo = 0;
        }
        if (r.program)
        {
            glDeleteProgram(r.program);
            r.program = 0;
        }
        r.atlasTexture = 0;
        memset(&r.list, 0, sizeof(r.list));
    }
}