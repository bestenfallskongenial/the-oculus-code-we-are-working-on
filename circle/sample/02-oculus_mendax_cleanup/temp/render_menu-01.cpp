//GPU_render_texture_atlas_menu.cpp
@@ -0,0 1,389 @@
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <GLES2/gl2.h>

namespace gpu_atlas_menu
{
    // =========================================================================
    // Atlas geometry
    // =========================================================================
    static const int ATLAS_W  = 1024;
    static const int ATLAS_H  = 1024;
    static const int ATLAS_N  = 8;          // 8x8 tiles
    static const int TILE_W   = ATLAS_W / ATLAS_N;  // 128
    static const int TILE_H   = ATLAS_H / ATLAS_N;  // 128

    // =========================================================================
    // Tile index contract (8x8 = 64 slots)
    // Requested order:
    //  0..15 : mode icons
    // 16..23 : extra mode placeholders
    // 24..31 : waveform icons
    // 32..39 : signs/additional icons
    // 40..49 : digits 0..9
    // 50     : dot
    // 51..54 : backgrounds (4)
    // =========================================================================
    enum AtlasTile : uint8_t
    {
        // 0..15 mode icons
        TILE_MODE_ADC = 0,
        TILE_MODE_TRG,
        TILE_MODE_BPM,
        TILE_MODE_LF1,
        TILE_MODE_LF2,
        TILE_MODE_AUD,
        TILE_MODE_TEX,
        TILE_MODE_FRM,
        TILE_MODE_PRG,
        TILE_MODE_09,
        TILE_MODE_10,
        TILE_MODE_11,
        TILE_MODE_12,
        TILE_MODE_13,
        TILE_MODE_14,
        TILE_MODE_15,

        // 16..23 extra placeholders
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
        TILE_SIGN_STAR_A,
        TILE_SIGN_STAR_B,
        TILE_SIGN_ARROW_L,
        TILE_SIGN_ARROW_R,
        TILE_SIGN_PLUS,
        TILE_SIGN_MINUS,

        // 40..49 digits
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

        // 50 dot
        TILE_DOT = 50,

        // 51..54 background tiles
        TILE_BG_A = 51,
        TILE_BG_B = 52,
        TILE_BG_BPM = 53,
        TILE_BG_3 = 54
    };

    // =========================================================================
    // Public layout  data
    // Positions are NDC centers (-1..1), exposed for easy change.
    // =========================================================================
    struct Vec2 { float x; float y; };

    struct Layout
    {
        Vec2 channelPos[8];   // 8 channel icons
        Vec2 bpmPos[6];       // [icon][d0][d1][d2][dot][d3]
        Vec2 bgA;
        Vec2 bgB;
        Vec2 bgBpm;
        Vec2 tileSizeNdc;     // base tile size in NDC
    };

    static const Layout kLayout =
    {
        // channel positions
        {
            { -0.78f,  0.44f }, { -0.56f,  0.44f }, { -0.34f,  0.44f }, { -0.12f,  0.44f },
            {  0.12f,  0.44f }, {  0.34f,  0.44f }, {  0.56f,  0.44f }, {  0.78f,  0.44f }
        },
        // bpm positions
        {
            { -0.22f, -0.46f }, { -0.06f, -0.46f }, {  0.07f, -0.46f },
            {  0.20f, -0.46f }, {  0.31f, -0.46f }, {  0.42f, -0.46f }
        },
        // background positions
        { -0.44f,  0.44f },
        {  0.44f,  0.44f },
        {  0.12f, -0.46f },
        // base tile size in NDC
        {  0.16f,  0.16f }
    };

    struct MenuInput
    {
        uint8_t modeByChannel[8];   // from mode_storage_buffers[CHx_MODE][current_buffer]
        uint8_t bpmGlyph[6];        // atlas tile IDs for [icon][d0][d1][d2][dot][d3]
        uint8_t g_menu_mode_new;    // 1=A, 2=B, 3=BPM group
        bool buttonEvent;           // true => pulse/scale BPM background
    };

    struct Renderer
    {
        GLuint program;
        GLuint vbo;
        GLuint atlasTexture;
        GLint  u_tex;
        GLint  a_pos;
        GLint  a_uv;
    };

    static const char* kVS =
        "attribute vec2 a_pos;\n"
        "attribute vec2 a_uv;\n"
        "varying vec2 v_uv;\n"
        "void main() {\n"
        "  gl_Position = vec4(a_pos, 0.0, 1.0);\n"
        "  v_uv = a_uv;\n"
        "}\n";

    static const char* kFS =
        "precision mediump float;\n"
        "uniform sampler2D u_tex;\n"
        "varying vec2 v_uv;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(u_tex, v_uv);\n"
        "}\n";

    // =========================================================================
    // Helpers
    // =========================================================================
    static GLuint compile_shader(GLenum type, const char* src)
    {
        GLuint sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, 0);
        glCompileShader(sh);
        return sh;
    }

    static uint8_t mode_to_tile(uint8_t mode)
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
            case 9:  return TILE_MODE_09;
            case 10: return TILE_MODE_10;
            case 11: return TILE_MODE_11;
            case 12: return TILE_MODE_12;
            case 13: return TILE_MODE_13;
            case 14: return TILE_MODE_14;
            case 15: return TILE_MODE_15;
            default: return TILE_SIGN_DASH;
        }
    }

    static inline void atlas_uv(uint8_t tile, float& u0, float& v0, float& u1, float& v1)
    {
        const int col = tile % ATLAS_N;
        const int row = tile / ATLAS_N;
        const float inv = 1.0f / (float)ATLAS_N;
        u0 = col * inv;
        v0 = row * inv;
        u1 = u0  inv;
        v1 = v0  inv;
    }

    // vertex layout: x, y, u, v
    static inline void push_quad(float* dst, int& cursor, Vec2 c, Vec2 size, uint8_t tile)
    {
        float u0, v0, u1, v1;
        atlas_uv(tile, u0, v0, u1, v1);

        const float hx = size.x * 0.5f;
        const float hy = size.y * 0.5f;
        const float x0 = c.x - hx;
        const float y0 = c.y - hy;
        const float x1 = c.x  hx;
        const float y1 = c.y  hy;

        // tri 1
        dst[cursor] = x0; dst[cursor] = y0; dst[cursor] = u0; dst[cursor] = v1;
        dst[cursor] = x1; dst[cursor] = y0; dst[cursor] = u1; dst[cursor] = v1;
        dst[cursor] = x1; dst[cursor] = y1; dst[cursor] = u1; dst[cursor] = v0;
        // tri 2
        dst[cursor] = x0; dst[cursor] = y0; dst[cursor] = u0; dst[cursor] = v1;
        dst[cursor] = x1; dst[cursor] = y1; dst[cursor] = u1; dst[cursor] = v0;
        dst[cursor] = x0; dst[cursor] = y1; dst[cursor] = u0; dst[cursor] = v0;
    }

    static inline uint8_t digit_to_tile(uint8_t d)
    {
        if (d <= 9) return (uint8_t)(TILE_DIGIT_0  d);
        return TILE_SIGN_DASH;
    }

    // optional helper
    void encode_bpm_120_0(uint16_t bpmTimes10, uint8_t out6[6])
    {
        // [icon][d0][d1][d2][dot][d3]
        uint16_t v = bpmTimes10; // 1200 => 120.0
        const uint8_t d3 = (uint8_t)(v % 10); v /= 10;
        const uint8_t d2 = (uint8_t)(v % 10); v /= 10;
        const uint8_t d1 = (uint8_t)(v % 10); v /= 10;
        const uint8_t d0 = (uint8_t)(v % 10);

        out6[0] = TILE_MODE_BPM;
        out6[1] = digit_to_tile(d0);
        out6[2] = digit_to_tile(d1);
        out6[3] = digit_to_tile(d2);
        out6[4] = TILE_DOT;
        out6[5] = digit_to_tile(d3);
    }

    // =========================================================================
    // API
    // =========================================================================
    bool init_renderer(Renderer& r, const uint8_t* atlasRGBA1024x1024)
    {
        memset(&r, 0, sizeof(r));

        GLuint vs = compile_shader(GL_VERTEX_SHADER, kVS);
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, kFS);
        r.program = glCreateProgram();
        glAttachShader(r.program, vs);
        glAttachShader(r.program, fs);
        glLinkProgram(r.program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        r.a_pos = glGetAttribLocation(r.program, "a_pos");
        r.a_uv  = glGetAttribLocation(r.program, "a_uv");
        r.u_tex = glGetUniformLocation(r.program, "u_tex");

        glGenBuffers(1, &r.vbo);

        glGenTextures(1, &r.atlasTexture);
        glBindTexture(GL_TEXTURE_2D, r.atlasTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            ATLAS_W,
            ATLAS_H,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            atlasRGBA1024x1024
        );
        glBindTexture(GL_TEXTURE_2D, 0);

        return (r.program != 0 && r.vbo != 0 && r.atlasTexture != 0);
    }

    // Call if you replace atlas content at runtime
    void update_atlas_texture(Renderer& r, const uint8_t* atlasRGBA1024x1024)
    {
        glBindTexture(GL_TEXTURE_2D, r.atlasTexture);
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            ATLAS_W,
            ATLAS_H,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            atlasRGBA1024x1024
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Draws directly from atlas in GPU memory, no CPU texture composition.
    void render_menu_overlay(Renderer& r, const MenuInput& in)
    {
        // 17 quads total: 3 bg  8 channels  6 bpm glyphs
        // each quad: 6 vertices, each vertex: 4 floats (x,y,u,v)
        float vertices[17 * 6 * 4];
        int n = 0;

        Vec2 base = kLayout.tileSizeNdc;
        Vec2 bgA  = base;
        Vec2 bgB  = base;
        Vec2 bgP  = base;

        // 5% highlight
        const float H = 1.05f;
        if (in.g_menu_mode_new == 1) { bgA.x *= H; bgA.y *= H; }
        if (in.g_menu_mode_new == 2) { bgB.x *= H; bgB.y *= H; }
        if (in.g_menu_mode_new == 3 || in.buttonEvent) { bgP.x *= H; bgP.y *= H; }

        // backgrounds
        push_quad(vertices, n, kLayout.bgA,   bgA, TILE_BG_A);
        push_quad(vertices, n, kLayout.bgB,   bgB, TILE_BG_B);
        push_quad(vertices, n, kLayout.bgBpm, bgP, TILE_BG_BPM);

        // channel icons
        for (int i = 0; i < 8; i)
        {
            push_quad(vertices, n, kLayout.channelPos[i], base, mode_to_tile(in.modeByChannel[i]));
        }

        // bpm icon  digits
        for (int i = 0; i < 6; i)
        {
            push_quad(vertices, n, kLayout.bpmPos[i], base, in.bpmGlyph[i]);
        }

        glUseProgram(r.program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, r.atlasTexture);
        if (r.u_tex >= 0) glUniform1i(r.u_tex, 0);

        glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(n * (int)sizeof(float)), vertices, GL_DYNAMIC_DRAW);

        if (r.a_pos >= 0)
        {
            glEnableVertexAttribArray((GLuint)r.a_pos);
            glVertexAttribPointer((GLuint)r.a_pos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);
        }
        if (r.a_uv >= 0)
        {
            glEnableVertexAttribArray((GLuint)r.a_uv);
            glVertexAttribPointer((GLuint)r.a_uv, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
        }

        // alpha blend for overlay
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, n / 4);
        glDisable(GL_BLEND);

        if (r.a_pos >= 0) glDisableVertexAttribArray((GLuint)r.a_pos);
        if (r.a_uv  >= 0) glDisableVertexAttribArray((GLuint)r.a_uv);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void shutdown_renderer(Renderer& r)
    {
        if (r.vbo)         glDeleteBuffers(1, &r.vbo);
        if (r.atlasTexture)glDeleteTextures(1, &r.atlasTexture);
        if (r.program)     glDeleteProgram(r.program);
        memset(&r, 0, sizeof(r));
    }
}