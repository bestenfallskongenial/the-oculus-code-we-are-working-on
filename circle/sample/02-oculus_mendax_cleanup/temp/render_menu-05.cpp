#include "kernel.h"

namespace GPU_rendered_menu
{
    // 8x8 atlas, 1024x1024 texture => 128x128 tile size
    static const int ATLAS_GRID = 8;
    static const int ATLAS_SIZE = 1024;
    static const int TILE_SIZE  = ATLAS_SIZE / ATLAS_GRID;

    enum TileIndex
    {
        // 0..15 mode icons
        TILE_ADC = 0,
        TILE_TRG,
        TILE_BPM,
        TILE_LF1,
        TILE_LF2,
        TILE_AUD,
        TILE_TEX,
        TILE_FRM,
        TILE_PRG,
        TILE_MODE_9,
        TILE_MODE_10,
        TILE_MODE_11,
        TILE_MODE_12,
        TILE_MODE_13,
        TILE_MODE_14,
        TILE_MODE_15,

        // 16..23 extra mode placeholders
        TILE_EXTRA_0 = 16,
        TILE_EXTRA_1,
        TILE_EXTRA_2,
        TILE_EXTRA_3,
        TILE_EXTRA_4,
        TILE_EXTRA_5,
        TILE_EXTRA_6,
        TILE_EXTRA_7,

        // 24..31 waveforms
        TILE_WAVE_0 = 24,
        TILE_WAVE_1,
        TILE_WAVE_2,
        TILE_WAVE_3,
        TILE_WAVE_4,
        TILE_WAVE_5,
        TILE_WAVE_6,
        TILE_WAVE_7,

        // 32..39 signs/additional
        TILE_SIGN_DASH = 32,
        TILE_SIGN_PIPE,
        TILE_SIGN_STAR,
        TILE_SIGN_DOT_SMALL,
        TILE_SIGN_ARROW_L,
        TILE_SIGN_ARROW_R,
        TILE_SIGN_PLUS,
        TILE_SIGN_MINUS,

        // 40..49 digits 0..9
        TILE_D0 = 40,
        TILE_D1,
        TILE_D2,
        TILE_D3,
        TILE_D4,
        TILE_D5,
        TILE_D6,
        TILE_D7,
        TILE_D8,
        TILE_D9,

        // 50 dot, 51 bpm icon
        TILE_DOT = 50,
        TILE_BPM_ICON = 51,

        // 52..55 backgrounds
        TILE_BG_A = 52,
        TILE_BG_B = 53,
        TILE_BG_BPM = 54,
        TILE_BG_SPARE = 55
    };

    struct Vec2
    {
        float x;
        float y;
    };

    struct UVRect
    {
        float u0, v0;
        float u1, v1;
    };

    struct SlotLayout
    {
        Vec2 ch[8];
        Vec2 bpm6[6]; // [icon][d0][d1][d2][dot][d3]
        Vec2 bgA;
        Vec2 bgB;
        Vec2 bgBpm;
        Vec2 halfTileNdc; // base tile half-size
    };

    // Exposed layout: edit only these positions for re-arrangement.
    static SlotLayout g_layout =
    {
        {
            { -0.77f,  0.42f }, { -0.55f,  0.42f }, { -0.33f,  0.42f }, { -0.11f,  0.42f },
            {  0.11f,  0.42f }, {  0.33f,  0.42f }, {  0.55f,  0.42f }, {  0.77f,  0.42f }
        },
        {
            { -0.22f, -0.44f }, { -0.06f, -0.44f }, {  0.07f, -0.44f },
            {  0.20f, -0.44f }, {  0.31f, -0.44f }, {  0.42f, -0.44f }
        },
        { -0.44f,  0.42f },
        {  0.44f,  0.42f },
        {  0.12f, -0.44f },
        {  0.07f,  0.07f }
    };

    struct MenuInput
    {
        unsigned char ch_mode[8];
        unsigned char bpm_glyph[6]; // explicit tile indices
        unsigned char g_menu_mode_new;
        bool button_event;
    };

    struct DrawCmd
    {
        unsigned char tile;
        unsigned char layer; // 0=bg, 1=fg
        Vec2 center;
        Vec2 scale;
    };

    struct DrawList
    {
        DrawCmd cmd[24];
        unsigned count;
    };

    struct Renderer
    {
        GLuint program;
        GLuint vbo;
        GLint a_pos;
        GLint a_uv;
        GLint u_tex;
        bool ready;
    };

    struct Vertex
    {
        float x, y;
        float u, v;
    };

    static unsigned char mode_to_tile(unsigned char mode)
    {
        if (mode <= 15)
            return mode;
        return TILE_SIGN_DASH;
    }

    static UVRect tile_uv(unsigned char tile)
    {
        const int col = tile % ATLAS_GRID;
        const int row = tile / ATLAS_GRID;
        const float inv = 1.0f / (float)ATLAS_GRID;

        UVRect r;
        r.u0 = (float)col * inv;
        r.v0 = (float)row * inv;
        r.u1 = r.u0 + inv;
        r.v1 = r.v0 + inv;
        return r;
    }

    static void push(DrawList &out, unsigned char tile, unsigned char layer, Vec2 center, Vec2 scale)
    {
        if (out.count >= 24)
            return;

        out.cmd[out.count].tile   = tile;
        out.cmd[out.count].layer  = layer;
        out.cmd[out.count].center = center;
        out.cmd[out.count].scale  = scale;
        out.count++;
    }

    void build_menu_draw_list(const MenuInput &in, DrawList &out)
    {
        out.count = 0;

        Vec2 s1 = { 1.0f, 1.0f };
        Vec2 sA = s1;
        Vec2 sB = s1;
        Vec2 sP = s1;

        const float highlight = 1.05f;
        if (in.g_menu_mode_new == 1) { sA.x = highlight; sA.y = highlight; }
        if (in.g_menu_mode_new == 2) { sB.x = highlight; sB.y = highlight; }
        if (in.g_menu_mode_new == 3 || in.button_event) { sP.x = highlight; sP.y = highlight; }

        push(out, TILE_BG_A,   0, g_layout.bgA,   sA);
        push(out, TILE_BG_B,   0, g_layout.bgB,   sB);
        push(out, TILE_BG_BPM, 0, g_layout.bgBpm, sP);

        for (unsigned i = 0; i < 8; ++i)
            push(out, mode_to_tile(in.ch_mode[i]), 1, g_layout.ch[i], s1);

        for (unsigned i = 0; i < 6; ++i)
            push(out, in.bpm_glyph[i], 1, g_layout.bpm6[i], s1);
    }

    static GLuint compile_shader(GLenum type, const char *src)
    {
        GLuint shader = glCreateShader(type);
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

    bool init_renderer(Renderer &r)
    {
        r.program = 0;
        r.vbo = 0;
        r.a_pos = -1;
        r.a_uv = -1;
        r.u_tex = -1;
        r.ready = false;

        const char *vs_src =
            "attribute vec2 a_pos;\n"
            "attribute vec2 a_uv;\n"
            "varying vec2 v_uv;\n"
            "void main() {\n"
            "  v_uv = a_uv;\n"
            "  gl_Position = vec4(a_pos, 0.0, 1.0);\n"
            "}\n";

        const char *fs_src =
            "precision mediump float;\n"
            "uniform sampler2D u_tex;\n"
            "varying vec2 v_uv;\n"
            "void main() {\n"
            "  gl_FragColor = texture2D(u_tex, v_uv);\n"
            "}\n";

        GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_src);
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_src);
        if (!vs || !fs)
            return false;

        r.program = glCreateProgram();
        glAttachShader(r.program, vs);
        glAttachShader(r.program, fs);
        glLinkProgram(r.program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLint linked = 0;
        glGetProgramiv(r.program, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            glDeleteProgram(r.program);
            r.program = 0;
            return false;
        }

        r.a_pos = glGetAttribLocation(r.program, "a_pos");
        r.a_uv  = glGetAttribLocation(r.program, "a_uv");
        r.u_tex = glGetUniformLocation(r.program, "u_tex");
        if (r.a_pos < 0 || r.a_uv < 0 || r.u_tex < 0)
            return false;

        glGenBuffers(1, &r.vbo);
        if (!r.vbo)
            return false;

        r.ready = true;
        return true;
    }

    void shutdown_renderer(Renderer &r)
    {
        if (r.vbo)
            glDeleteBuffers(1, &r.vbo);
        if (r.program)
            glDeleteProgram(r.program);

        r.program = 0;
        r.vbo = 0;
        r.a_pos = -1;
        r.a_uv = -1;
        r.u_tex = -1;
        r.ready = false;
    }

    static void build_quad(const DrawCmd &c, Vertex out[6])
    {
        const UVRect uv = tile_uv(c.tile);

        const float hx = g_layout.halfTileNdc.x * c.scale.x;
        const float hy = g_layout.halfTileNdc.y * c.scale.y;

        const float x0 = c.center.x - hx;
        const float y0 = c.center.y - hy;
        const float x1 = c.center.x + hx;
        const float y1 = c.center.y + hy;

        out[0] = (Vertex){ x0, y0, uv.u0, uv.v1 };
        out[1] = (Vertex){ x1, y0, uv.u1, uv.v1 };
        out[2] = (Vertex){ x1, y1, uv.u1, uv.v0 };
        out[3] = (Vertex){ x0, y0, uv.u0, uv.v1 };
        out[4] = (Vertex){ x1, y1, uv.u1, uv.v0 };
        out[5] = (Vertex){ x0, y1, uv.u0, uv.v0 };
    }

    void render_menu(const Renderer &r, GLuint atlas_texture, const DrawList &list)
    {
        if (!r.ready || !atlas_texture || list.count == 0)
            return;

        glUseProgram(r.program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas_texture);
        glUniform1i(r.u_tex, 0);

        glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
        glEnableVertexAttribArray((GLuint)r.a_pos);
        glEnableVertexAttribArray((GLuint)r.a_uv);
        glVertexAttribPointer((GLuint)r.a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
        glVertexAttribPointer((GLuint)r.a_uv,  2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (unsigned pass = 0; pass < 2; ++pass)
        {
            for (unsigned i = 0; i < list.count; ++i)
            {
                if (list.cmd[i].layer != pass)
                    continue;

                Vertex quad[6];
                build_quad(list.cmd[i], quad);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        glDisable(GL_BLEND);
        glDisableVertexAttribArray((GLuint)r.a_pos);
        glDisableVertexAttribArray((GLuint)r.a_uv);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // helper: encode bpm x10 (e.g. 1200 => 120.0)
    static unsigned char digit_tile(unsigned char d)
    {
        return (d <= 9) ? (unsigned char)(TILE_D0 + d) : (unsigned char)TILE_SIGN_DASH;
    }

    void encode_bpm_x10(unsigned short bpm_x10, unsigned char out6[6])
    {
        unsigned short v = bpm_x10;

        unsigned char d3 = (unsigned char)(v % 10); v /= 10;
        unsigned char d2 = (unsigned char)(v % 10); v /= 10;
        unsigned char d1 = (unsigned char)(v % 10); v /= 10;
        unsigned char d0 = (unsigned char)(v % 10);

        out6[0] = TILE_BPM_ICON;
        out6[1] = digit_tile(d0);
        out6[2] = digit_tile(d1);
        out6[3] = digit_tile(d2);
        out6[4] = TILE_DOT;
        out6[5] = digit_tile(d3);
    }
}