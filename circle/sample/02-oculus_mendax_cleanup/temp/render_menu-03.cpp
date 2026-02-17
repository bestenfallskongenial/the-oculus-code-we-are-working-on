#include <stdint.h>
#include <string.h>
#include <GLES2/gl2.h>

namespace GPUAtlasMenu
{
    // ==========================================================================================
    // Atlas geometry (8x8 on 1024x1024)
    // ==========================================================================================
    static const int ATLAS_W  = 1024;
    static const int ATLAS_H  = 1024;
    static const int GRID_W   = 8;
    static const int GRID_H   = 8;
    static const int TILE_W   = ATLAS_W / GRID_W; // 128
    static const int TILE_H   = ATLAS_H / GRID_H; // 128

    // ==========================================================================================
    // Icon indices in atlas (exposed  easy to reorder)
    // ==========================================================================================
    enum AtlasTileIndex : uint8_t
    {
        // 0..15  : mode icons
        IDX_MODE_ADC = 0,
        IDX_MODE_TRG,
        IDX_MODE_BPM,
        IDX_MODE_LF1,
        IDX_MODE_LF2,
        IDX_MODE_AUD,
        IDX_MODE_TEX,
        IDX_MODE_FRM,
        IDX_MODE_PRG,
        IDX_MODE_EXTRA0,
        IDX_MODE_EXTRA1,
        IDX_MODE_EXTRA2,
        IDX_MODE_EXTRA3,
        IDX_MODE_EXTRA4,
        IDX_MODE_EXTRA5,
        IDX_MODE_EXTRA6,

        // 16..23 : extra modes placeholders
        IDX_EXTRA_0 = 16,
        IDX_EXTRA_1,
        IDX_EXTRA_2,
        IDX_EXTRA_3,
        IDX_EXTRA_4,
        IDX_EXTRA_5,
        IDX_EXTRA_6,
        IDX_EXTRA_7,

        // 24..31 : waveforms
        IDX_WAVE_0 = 24,
        IDX_WAVE_1,
        IDX_WAVE_2,
        IDX_WAVE_3,
        IDX_WAVE_4,
        IDX_WAVE_5,
        IDX_WAVE_6,
        IDX_WAVE_7,

        // 32..39 : signs/additional
        IDX_SIGN_DASH = 32,
        IDX_SIGN_PIPE,
        IDX_SIGN_STAR,
        IDX_SIGN_DOT_SMALL,
        IDX_SIGN_ARROW_L,
        IDX_SIGN_ARROW_R,
        IDX_SIGN_PLUS,
        IDX_SIGN_MINUS,

        // 40..49 : digits 0..9
        IDX_DIGIT_0 = 40,
        IDX_DIGIT_1,
        IDX_DIGIT_2,
        IDX_DIGIT_3,
        IDX_DIGIT_4,
        IDX_DIGIT_5,
        IDX_DIGIT_6,
        IDX_DIGIT_7,
        IDX_DIGIT_8,
        IDX_DIGIT_9,

        // 50..51 : dot  BPM badge/icon
        IDX_DOT = 50,
        IDX_BPM_ICON = 51,

        // 52..55 : backgrounds
        IDX_BG_A = 52,
        IDX_BG_B = 53,
        IDX_BG_BPM = 54,
        IDX_BG_SPARE = 55
    };

    // ==========================================================================================
    // Layout exposed (relative to overlay center, NDC-like [-1..1])
    // ==========================================================================================
    struct Vec2
    {
        float x;
        float y;
    };

    struct Layout
    {
        Vec2 channelPos[8];  // CH0..CH7
        Vec2 bpmPos[6];      // [icon][d1][d2][d3][dot][d4]
        Vec2 bgA;
        Vec2 bgB;
        Vec2 bgBpm;
    };

    // Editable defaults.
    Layout g_layout =
    {
        // channels
        {
            {-0.77f,  0.42f},
            {-0.55f,  0.42f},
            {-0.33f,  0.42f},
            {-0.11f,  0.42f},
            { 0.11f,  0.42f},
            { 0.33f,  0.42f},
            { 0.55f,  0.42f},
            { 0.77f,  0.42f}
        },
        // bpm row
        {
            {-0.22f, -0.44f}, // icon
            {-0.06f, -0.44f}, // d1
            { 0.07f, -0.44f}, // d2
            { 0.20f, -0.44f}, // d3
            { 0.31f, -0.44f}, // dot
            { 0.42f, -0.44f}  // d4
        },
        // backgrounds
        {-0.44f,  0.42f},
        { 0.44f,  0.42f},
        { 0.12f, -0.44f}
    };

    // ==========================================================================================
    // Public input (no state-machine, no fuzzy logic)
    // ==========================================================================================
    struct Input
    {
        uint8_t channelMode[8]; // mode_storage_buffers[CHx_MODE][current_buffer]
        uint8_t bpmDigits[4];   // e.g. 1200 for "120.0"
        uint8_t g_menu_mode_new; // 1=A, 2=B, 3=BPM/LFO
        bool    buttonEvent;     // pulse BPM bg
    };

    // ==========================================================================================
    // Internal draw item
    // ==========================================================================================
    struct DrawItem
    {
        uint8_t atlasIndex;
        Vec2 center;
        Vec2 scale;
        float tileW; // ndc width
        float tileH; // ndc height
        uint8_t layer; // 0 bg, 1 fg
    };

    static const int MAX_ITEMS = 24;

    // ==========================================================================================
    // Helpers
    // ==========================================================================================
    static inline uint8_t digit_to_tile(uint8_t d)
    {
        return (d <= 9) ? (uint8_t)(IDX_DIGIT_0  d) : (uint8_t)IDX_SIGN_DASH;
    }

    static inline uint8_t mode_to_tile(uint8_t mode)
    {
        switch (mode)
        {
            case 0:  return IDX_MODE_ADC;
            case 1:  return IDX_MODE_TRG;
            case 2:  return IDX_MODE_BPM;
            case 3:  return IDX_MODE_LF1;
            case 4:  return IDX_MODE_LF2;
            case 5:  return IDX_MODE_AUD;
            case 6:  return IDX_MODE_TEX;
            case 7:  return IDX_MODE_FRM;
            case 8:  return IDX_MODE_PRG;
            case 9:  return IDX_MODE_EXTRA0;
            case 10: return IDX_MODE_EXTRA1;
            case 11: return IDX_MODE_EXTRA2;
            case 12: return IDX_MODE_EXTRA3;
            case 13: return IDX_MODE_EXTRA4;
            case 14: return IDX_MODE_EXTRA5;
            case 15: return IDX_MODE_EXTRA6;
            default: return IDX_SIGN_DASH;
        }
    }

    static inline void tile_uv(uint8_t idx, float &u0, float &v0, float &u1, float &v1)
    {
        const int col = idx % GRID_W;
        const int row = idx / GRID_W;
        const float du = 1.0f / (float)GRID_W;
        const float dv = 1.0f / (float)GRID_H;
        u0 = col * du;
        v0 = row * dv;
        u1 = u0  du;
        v1 = v0  dv;
    }

    static inline void push_item(DrawItem* items, int& count, uint8_t idx, Vec2 c, Vec2 s, float w, float h, uint8_t layer)
    {
        if (count >= MAX_ITEMS) return;
        items[count].atlasIndex = idx;
        items[count].center = c;
        items[count].scale = s;
        items[count].tileW = w;
        items[count].tileH = h;
        items[count].layer = layer;
        count;
    }

    static void build_items(const Input& in, DrawItem* items, int& count)
    {
        count = 0;

        const float ICON_W = 0.16f;
        const float ICON_H = 0.16f;
        const float BG_W   = 0.68f;
        const float BG_H   = 0.26f;
        const float BPM_BG_W = 0.78f;
        const float BPM_BG_H = 0.24f;

        Vec2 bgA = {1.0f, 1.0f};
        Vec2 bgB = {1.0f, 1.0f};
        Vec2 bgP = {1.0f, 1.0f};

        const float H = 1.05f; // 5%
        if (in.g_menu_mode_new == 1) { bgA.x = H; bgA.y = H; }
        if (in.g_menu_mode_new == 2) { bgB.x = H; bgB.y = H; }
        if (in.g_menu_mode_new == 3 || in.buttonEvent) { bgP.x = H; bgP.y = H; }

        // backgrounds
        push_item(items, count, IDX_BG_A,   g_layout.bgA,   bgA, BG_W, BG_H, 0);
        push_item(items, count, IDX_BG_B,   g_layout.bgB,   bgB, BG_W, BG_H, 0);
        push_item(items, count, IDX_BG_BPM, g_layout.bgBpm, bgP, BPM_BG_W, BPM_BG_H, 0);

        // channels
        for (int i = 0; i < 8; i)
        {
            push_item(items, count, mode_to_tile(in.channelMode[i]), g_layout.channelPos[i], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        }

        // bpm row: icon  d1 d2 d3 dot d4 (120.0)
        push_item(items, count, IDX_BPM_ICON,                 g_layout.bpmPos[0], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        push_item(items, count, digit_to_tile(in.bpmDigits[0]), g_layout.bpmPos[1], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        push_item(items, count, digit_to_tile(in.bpmDigits[1]), g_layout.bpmPos[2], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        push_item(items, count, digit_to_tile(in.bpmDigits[2]), g_layout.bpmPos[3], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        push_item(items, count, IDX_DOT,                        g_layout.bpmPos[4], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
        push_item(items, count, digit_to_tile(in.bpmDigits[3]), g_layout.bpmPos[5], (Vec2){1.0f,1.0f}, ICON_W, ICON_H, 1);
    }

    // ==========================================================================================
    // GL renderer state
    // ==========================================================================================
    struct Renderer
    {
        GLuint program;
        GLuint vbo;
        GLint  a_pos;
        GLint  a_uv;
        GLint  u_tex;
        GLuint atlasTex;
        bool   ready;
    };

    static Renderer g_renderer = {0,0,-1,-1,-1,0,false};

    static GLuint compile_shader(GLenum type, const char* src)
    {
        GLuint sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, 0);
        glCompileShader(sh);
        GLint ok = 0;
        glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            glDeleteShader(sh);
            return 0;
        }
        return sh;
    }

    static GLuint create_program()
    {
        static const char* VS =
            "attribute vec2 a_pos;\n"
            "attribute vec2 a_uv;\n"
            "varying vec2 v_uv;\n"
            "void main(){ v_uv=a_uv; gl_Position=vec4(a_pos,0.0,1.0); }\n";

        static const char* FS =
            "precision mediump float;\n"
            "uniform sampler2D u_tex;\n"
            "varying vec2 v_uv;\n"
            "void main(){ gl_FragColor = texture2D(u_tex, v_uv); }\n";

        GLuint v = compile_shader(GL_VERTEX_SHADER, VS);
        GLuint f = compile_shader(GL_FRAGMENT_SHADER, FS);
        if (!v || !f) return 0;

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

    bool init(GLuint atlasTextureId)
    {
        if (g_renderer.ready) return true;

        g_renderer.program = create_program();
        if (!g_renderer.program) return false;

        glGenBuffers(1, &g_renderer.vbo);
        if (!g_renderer.vbo)
        {
            glDeleteProgram(g_renderer.program);
            memset(&g_renderer, 0, sizeof(g_renderer));
            return false;
        }

        g_renderer.a_pos = 0;
        g_renderer.a_uv  = 1;
        g_renderer.u_tex = glGetUniformLocation(g_renderer.program, "u_tex");
        g_renderer.atlasTex = atlasTextureId;
        g_renderer.ready = true;
        return true;
    }

    void shutdown()
    {
        if (g_renderer.vbo) glDeleteBuffers(1, &g_renderer.vbo);
        if (g_renderer.program) glDeleteProgram(g_renderer.program);
        memset(&g_renderer, 0, sizeof(g_renderer));
    }

    // Draw one quad from atlas at given center/size.
    static void draw_item(const DrawItem& it)
    {
        const float hw = 0.5f * it.tileW * it.scale.x;
        const float hh = 0.5f * it.tileH * it.scale.y;

        const float x0 = it.center.x - hw;
        const float y0 = it.center.y - hh;
        const float x1 = it.center.x  hw;
        const float y1 = it.center.y  hh;

        float u0, v0, u1, v1;
        tile_uv(it.atlasIndex, u0, v0, u1, v1);

        // tri strip: (x0,y0) (x1,y0) (x0,y1) (x1,y1)
        // each vertex: pos.xy  uv.xy
        const float vtx[16] =
        {
            x0, y0,  u0, v1,
            x1, y0,  u1, v1,
            x0, y1,  u0, v0,
            x1, y1,  u1, v0
        };

        glBindBuffer(GL_ARRAY_BUFFER, g_renderer.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STREAM_DRAW);

        glVertexAttribPointer(g_renderer.a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)0);
        glVertexAttribPointer(g_renderer.a_uv,  2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));
        glEnableVertexAttribArray(g_renderer.a_pos);
        glEnableVertexAttribArray(g_renderer.a_uv);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // Main draw entry.
    // Atlas must already be uploaded in GPU memory (atlasTextureId), no per-frame uploads here.
    bool render(const Input& in)
    {
        if (!g_renderer.ready || g_renderer.atlasTex == 0) return false;

        DrawItem items[MAX_ITEMS];
        int itemCount = 0;
        build_items(in, items, itemCount);

        glUseProgram(g_renderer.program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_renderer.atlasTex);
        glUniform1i(g_renderer.u_tex, 0);

        // alpha blending for transparent icons/backgrounds
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Draw layer 0 then layer 1.
        for (int layer = 0; layer <= 1; layer)
        {
            for (int i = 0; i < itemCount; i)
            {
                if (items[i].layer == layer)
                {
                    draw_item(items[i]);
                }
            }
        }

        glDisableVertexAttribArray(g_renderer.a_pos);
        glDisableVertexAttribArray(g_renderer.a_uv);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    // Optional helper if you want to switch atlas texture at runtime.
    void set_atlas_texture(GLuint atlasTextureId)
    {
        g_renderer.atlasTex = atlasTextureId;
    }
}