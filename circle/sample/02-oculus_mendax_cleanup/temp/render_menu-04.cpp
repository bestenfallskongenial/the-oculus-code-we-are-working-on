namespace gpu_texture_atlas_menu
{
    // ============================================================================================
    // Atlas contract
    // ============================================================================================
    static const int ATLAS_SIZE_PX = 1024;
    static const int ATLAS_GRID    = 8;
    static const int TILE_SIZE_PX  = ATLAS_SIZE_PX / ATLAS_GRID; // 128
    static const int TILE_COUNT    = ATLAS_GRID * ATLAS_GRID;    // 64

    // 8x8 tile indices (explicit  easy to re-map)
    enum AtlasTile : uint8_t
    {
        // 0..15 : main mode icons
        TILE_MODE_ADC = 0,
        TILE_MODE_TRG,
        TILE_MODE_BPM,
        TILE_MODE_LF1,
        TILE_MODE_LF2,
        TILE_MODE_AUD,
        TILE_MODE_TEX,
        TILE_MODE_FRM,
        TILE_MODE_PRG,
        TILE_MODE_EXT0,
        TILE_MODE_EXT1,
        TILE_MODE_EXT2,
        TILE_MODE_EXT3,
        TILE_MODE_EXT4,
        TILE_MODE_EXT5,
        TILE_MODE_EXT6,

        // 16..23 : extra modes placeholders
        TILE_EXTRA_0 = 16,
        TILE_EXTRA_1,
        TILE_EXTRA_2,
        TILE_EXTRA_3,
        TILE_EXTRA_4,
        TILE_EXTRA_5,
        TILE_EXTRA_6,
        TILE_EXTRA_7,

        // 24..31 : waveforms
        TILE_WAVE_0 = 24,
        TILE_WAVE_1,
        TILE_WAVE_2,
        TILE_WAVE_3,
        TILE_WAVE_4,
        TILE_WAVE_5,
        TILE_WAVE_6,
        TILE_WAVE_7,

        // 32..39 : signs / additional
        TILE_SIGN_DASH = 32,
        TILE_SIGN_PIPE,
        TILE_SIGN_STAR,
        TILE_SIGN_DOT_SMALL,
        TILE_SIGN_ARROW_L,
        TILE_SIGN_ARROW_R,
        TILE_SIGN_PLUS,
        TILE_SIGN_MINUS,

        // 40..49 : digits 0..9
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

        // 50..51 : "." and bpm icon
        TILE_DOT = 50,
        TILE_BPM_ICON = 51,

        // 52..55 : backgrounds
        TILE_BG_A   = 52,
        TILE_BG_B   = 53,
        TILE_BG_BPM = 54,
        TILE_BG_3   = 55,

        // 56..63 : spare
        TILE_SPARE_56 = 56,
        TILE_SPARE_57,
        TILE_SPARE_58,
        TILE_SPARE_59,
        TILE_SPARE_60,
        TILE_SPARE_61,
        TILE_SPARE_62,
        TILE_SPARE_63
    };

    struct Vec2 { float x, y; };
    struct UVRect { float u0, v0, u1, v1; };

    static inline UVRect atlas_uv(uint8_t tileIndex)
    {
        const int col = tileIndex % ATLAS_GRID;
        const int row = tileIndex / ATLAS_GRID;
        const float inv = 1.0f / (float)ATLAS_GRID;
        UVRect r;
        r.u0 = (float)col * inv;
        r.v0 = (float)row * inv;
        r.u1 = r.u0  inv;
        r.v1 = r.v0  inv;
        return r;
    }

    // ============================================================================================
    // Overlay layout (positions relative to center in NDC)
    // ============================================================================================
    struct OverlayLayout
    {
        Vec2 ch[8];     // 8 channel slots
        Vec2 bpm6[6];   // [icon][d0][d1][d2][dot][d3]
        Vec2 bgA;
        Vec2 bgB;
        Vec2 bgBpm;
        Vec2 tileHalfNdc; // half-size of one icon in NDC (base size)
    };

    // EDIT HERE to move layout without touching renderer code.
    static const OverlayLayout kLayout =
    {
        // ch[8]
        {
            { -0.77f,  0.42f }, { -0.55f,  0.42f }, { -0.33f,  0.42f }, { -0.11f,  0.42f },
            {  0.11f,  0.42f }, {  0.33f,  0.42f }, {  0.55f,  0.42f }, {  0.77f,  0.42f }
        },
        // bpm6
        {
            { -0.22f, -0.44f }, { -0.06f, -0.44f }, {  0.07f, -0.44f },
            {  0.20f, -0.44f }, {  0.31f, -0.44f }, {  0.42f, -0.44f }
        },
        // backgrounds
        { -0.44f,  0.42f },
        {  0.44f,  0.42f },
        {  0.12f, -0.44f },
        // base icon half-size in NDC (width/height)
        {  0.07f,  0.07f }
    };

    // ============================================================================================
    // Input / draw-list
    // ============================================================================================
    struct MenuInput
    {
        uint8_t chMode[8];      // mode_storage_buffers[CHx_MODE][current_buffer]
        uint8_t bpmGlyph[6];    // atlas tile indices for [icon][d0][d1][d2][dot][d3]
        uint8_t g_menu_mode_new; // 1,2,3
        bool    buttonEvent;     // highlight BPM group on button event
    };

    struct DrawItem
    {
        uint8_t tileIndex;
        Vec2    center;
        Vec2    scale;   // 1.0 nominal
        uint8_t layer;   // 0 background, 1 foreground
    };

    struct DrawList
    {
        DrawItem item[24];
        uint32_t count;
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
            case 9:  return TILE_MODE_EXT0;
            case 10: return TILE_MODE_EXT1;
            case 11: return TILE_MODE_EXT2;
            case 12: return TILE_MODE_EXT3;
            case 13: return TILE_MODE_EXT4;
            case 14: return TILE_MODE_EXT5;
            case 15: return TILE_MODE_EXT6;
            default: return TILE_SIGN_DASH;
        }
    }

    static inline void push(DrawList& dl, uint8_t tile, Vec2 c, Vec2 s, uint8_t layer)
    {
        if (dl.count >= (sizeof(dl.item) / sizeof(dl.item[0]))) return;
        dl.item[dl.count].tileIndex = tile;
        dl.item[dl.count].center    = c;
        dl.item[dl.count].scale     = s;
        dl.item[dl.count].layer     = layer;
        dl.count;
    }

    // No extra logic / no state machine: direct mapping only.
    void build_draw_list(const MenuInput& in, DrawList& out)
    {
        out.count = 0;

        Vec2 s1 = { 1.0f, 1.0f };
        Vec2 bgA = s1, bgB = s1, bgP = s1;

        const float highlight = 1.05f; // 5%
        if (in.g_menu_mode_new == 1) { bgA.x = highlight; bgA.y = highlight; }
        if (in.g_menu_mode_new == 2) { bgB.x = highlight; bgB.y = highlight; }
        if (in.g_menu_mode_new == 3 || in.buttonEvent) { bgP.x = highlight; bgP.y = highlight; }

        // backgrounds
        push(out, TILE_BG_A,   kLayout.bgA,   bgA, 0);
        push(out, TILE_BG_B,   kLayout.bgB,   bgB, 0);
        push(out, TILE_BG_BPM, kLayout.bgBpm, bgP, 0);

        // channel icons
        for (uint8_t i = 0; i < 8; i)
            push(out, mode_to_tile(in.chMode[i]), kLayout.ch[i], s1, 1);

        // bpm symbols/icons
        for (uint8_t i = 0; i < 6; i)
            push(out, in.bpmGlyph[i], kLayout.bpm6[i], s1, 1);
    }

    // ============================================================================================
    // GPU renderer (atlas stays in GPU memory, immediate draw of quads)
    // ============================================================================================
    struct Renderer
    {
        GLuint program;
        GLuint vbo;
        GLint  a_pos;
        GLint  a_uv;
        GLint  u_tex;
        bool   ready;
    };

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

    static GLuint link_program(GLuint vs, GLuint fs)
    {
        GLuint p = glCreateProgram();
        glAttachShader(p, vs);
        glAttachShader(p, fs);
        glLinkProgram(p);
        GLint ok = 0;
        glGetProgramiv(p, GL_LINK_STATUS, &ok);
        if (!ok)
        {
            glDeleteProgram(p);
            return 0;
        }
        return p;
    }

    bool init_renderer(Renderer& r)
    {
        memset(&r, 0, sizeof(r));

        static const char* kVS =
            "attribute vec2 a_pos;\n"
            "attribute vec2 a_uv;\n"
            "varying vec2 v_uv;\n"
            "void main(){ v_uv = a_uv; gl_Position = vec4(a_pos, 0.0, 1.0); }\n";

        static const char* kFS =
            "precision mediump float;\n"
            "uniform sampler2D u_tex;\n"
            "varying vec2 v_uv;\n"
            "void main(){ gl_FragColor = texture2D(u_tex, v_uv); }\n";

        GLuint vs = compile_shader(GL_VERTEX_SHADER, kVS);
        GLuint fs = compile_shader(GL_FRAGMENT_SHADER, kFS);
        if (!vs || !fs) return false;

        r.program = link_program(vs, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
        if (!r.program) return false;

        r.a_pos = glGetAttribLocation(r.program, "a_pos");
        r.a_uv  = glGetAttribLocation(r.program, "a_uv");
        r.u_tex = glGetUniformLocation(r.program, "u_tex");
        if (r.a_pos < 0 || r.a_uv < 0 || r.u_tex < 0) return false;

        glGenBuffers(1, &r.vbo);
        if (!r.vbo) return false;

        r.ready = true;
        return true;
    }

    void shutdown_renderer(Renderer& r)
    {
        if (r.vbo) glDeleteBuffers(1, &r.vbo);
        if (r.program) glDeleteProgram(r.program);
        memset(&r, 0, sizeof(r));
    }

    struct Vtx { float x, y, u, v; };

    // Build one quad in clip-space using layout center  scale and atlas tile UVs.
    static void build_quad(const DrawItem& it, Vtx out6[6])
    {
        const UVRect uv = atlas_uv(it.tileIndex);
        const float hx = kLayout.tileHalfNdc.x * it.scale.x;
        const float hy = kLayout.tileHalfNdc.y * it.scale.y;

        const float x0 = it.center.x - hx;
        const float y0 = it.center.y - hy;
        const float x1 = it.center.x  hx;
        const float y1 = it.center.y  hy;

        // two triangles
        out6[0] = { x0, y0, uv.u0, uv.v1 };
        out6[1] = { x1, y0, uv.u1, uv.v1 };
        out6[2] = { x1, y1, uv.u1, uv.v0 };
        out6[3] = { x0, y0, uv.u0, uv.v1 };
        out6[4] = { x1, y1, uv.u1, uv.v0 };
        out6[5] = { x0, y1, uv.u0, uv.v0 };
    }

    // Draw draw-list from atlas texture directly on current framebuffer.
    // Integration timing/call site intentionally left to caller.
    void render_draw_list(const Renderer& r, GLuint atlasTexture, const DrawList& dl)
    {
        if (!r.ready || !atlasTexture || dl.count == 0) return;

        glUseProgram(r.program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlasTexture);
        glUniform1i(r.u_tex, 0);

        glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
        glEnableVertexAttribArray((GLuint)r.a_pos);
        glEnableVertexAttribArray((GLuint)r.a_uv);
        glVertexAttribPointer((GLuint)r.a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), (const void*)0);
        glVertexAttribPointer((GLuint)r.a_uv,  2, GL_FLOAT, GL_FALSE, sizeof(Vtx), (const void*)(sizeof(float) * 2));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // pass 0 backgrounds, pass 1 foreground
        for (int pass = 0; pass < 2; pass)
        {
            for (uint32_t i = 0; i < dl.count; i)
            {
                if ((int)dl.item[i].layer != pass) continue;

                Vtx quad[6];
                build_quad(dl.item[i], quad);
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

    // ============================================================================================
    // Optional helpers for BPM glyph encoding
    // ============================================================================================
    static inline uint8_t digit_tile(uint8_t d)
    {
        return (d <= 9) ? (uint8_t)(TILE_DIGIT_0  d) : (uint8_t)TILE_SIGN_DASH;
    }

    // e.g. 1200 => "120.0" as [icon][1][2][0][.][0]
    void encode_bpm_x10(uint16_t bpm_x10, uint8_t out6[6])
    {
        if (!out6) return;
        uint16_t v = bpm_x10;
        uint8_t d3 = (uint8_t)(v % 10); v /= 10;
        uint8_t d2 = (uint8_t)(v % 10); v /= 10;
        uint8_t d1 = (uint8_t)(v % 10); v /= 10;
        uint8_t d0 = (uint8_t)(v % 10);

        out6[0] = TILE_BPM_ICON;
        out6[1] = digit_tile(d0);
        out6[2] = digit_tile(d1);
        out6[3] = digit_tile(d2);
        out6[4] = TILE_DOT;
        out6[5] = digit_tile(d3);
    }
}