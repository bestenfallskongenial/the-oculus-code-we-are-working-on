#include <GLES2/gl2.h>

namespace GPU_rendered_menu
{
    // 8x8 atlas, 1024x1024
    const int ATLAS_GRID = 8;
    const int ATLAS_SIZE = 1024;

    enum AtlasTile
    {
        // 0..15 main modes/icons
        TILE_MODE_ADC = 0,
        TILE_MODE_TRG,
        TILE_MODE_BPM,
        TILE_MODE_LF1,
        TILE_MODE_LF2,
        TILE_MODE_AUD,
        TILE_MODE_TEX,
        TILE_MODE_FRM,
        TILE_MODE_PRG,
        TILE_MODE_EX0,
        TILE_MODE_EX1,
        TILE_MODE_EX2,
        TILE_MODE_EX3,
        TILE_MODE_EX4,
        TILE_MODE_EX5,
        TILE_MODE_EX6,

        // 16..23 extra modes placeholders
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

        // 32..39 signs / icons
        TILE_SIGN_DASH = 32,
        TILE_SIGN_PIPE,
        TILE_SIGN_STAR,
        TILE_SIGN_DOT_SMALL,
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

        TILE_DOT = 50,
        TILE_BPM_ICON = 51,

        // 4 backgrounds
        TILE_BG_A = 52,
        TILE_BG_B,
        TILE_BG_BPM,
        TILE_BG_SPARE
    };

    struct Vec2
    {
        float x;
        float y;
    };

    struct UVRect
    {
        float u0;
        float v0;
        float u1;
        float v1;
    };

    struct MenuLayout
    {
        Vec2 ch[8];
        Vec2 bpm[6]; // icon,d0,d1,d2,dot,d3
        Vec2 bgA;
        Vec2 bgB;
        Vec2 bgBpm;
    };

    // central layout table (easy to change)
    static const MenuLayout LAYOUT =
    {
        {
            { -0.77f, 0.42f }, { -0.55f, 0.42f }, { -0.33f, 0.42f }, { -0.11f, 0.42f },
            {  0.11f, 0.42f }, {  0.33f, 0.42f }, {  0.55f, 0.42f }, {  0.77f, 0.42f }
        },
        {
            { -0.22f, -0.44f }, { -0.06f, -0.44f }, { 0.07f, -0.44f },
            {  0.20f, -0.44f }, {  0.31f, -0.44f }, { 0.42f, -0.44f }
        },
        { -0.44f,  0.42f },
        {  0.44f,  0.42f },
        {  0.12f, -0.44f }
    };

    struct MenuInput
    {
        unsigned char channelMode[8];
        unsigned char bpmGlyph[6];
        int g_menu_mode_new;  // 1/2/3
        bool button_event;
    };

    struct DrawItem
    {
        unsigned char tile;
        Vec2 center;
        Vec2 scale;
        int layer; // 0 background, 1 foreground
    };

    struct DrawList
    {
        DrawItem item[24]; // 3 bg + 8 ch + 6 bpm = 17
        int count;
    };

    struct Vertex
    {
        float x;
        float y;
        float u;
        float v;
    };

    static unsigned char mode_to_tile(unsigned char mode)
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
            case 9:  return TILE_MODE_EX0;
            case 10: return TILE_MODE_EX1;
            case 11: return TILE_MODE_EX2;
            case 12: return TILE_MODE_EX3;
            case 13: return TILE_MODE_EX4;
            case 14: return TILE_MODE_EX5;
            case 15: return TILE_MODE_EX6;
            default: return TILE_SIGN_DASH;
        }
    }

    static UVRect tile_uv(unsigned char tile)
    {
        int col = tile % ATLAS_GRID;
        int row = tile / ATLAS_GRID;
        float inv = 1.0f / (float)ATLAS_GRID;

        UVRect r;
        r.u0 = (float)col * inv;
        r.v0 = (float)row * inv;
        r.u1 = r.u0 + inv;
        r.v1 = r.v0 + inv;
        return r;
    }

    static void push_item(DrawList& out, unsigned char tile, Vec2 center, Vec2 scale, int layer)
    {
        if (out.count >= 24)
            return;

        out.item[out.count].tile = tile;
        out.item[out.count].center = center;
        out.item[out.count].scale = scale;
        out.item[out.count].layer = layer;
        out.count++;
    }

    // deterministic list builder: no extra menu logic
    void build_draw_list(const MenuInput& in, DrawList& out)
    {
        out.count = 0;

        Vec2 one = { 1.0f, 1.0f };
        Vec2 bgA = one;
        Vec2 bgB = one;
        Vec2 bgP = one;

        const float H = 1.05f;
        if (in.g_menu_mode_new == 1) { bgA.x = H; bgA.y = H; }
        if (in.g_menu_mode_new == 2) { bgB.x = H; bgB.y = H; }
        if (in.g_menu_mode_new == 3 || in.button_event) { bgP.x = H; bgP.y = H; }

        push_item(out, TILE_BG_A,   LAYOUT.bgA,   bgA, 0);
        push_item(out, TILE_BG_B,   LAYOUT.bgB,   bgB, 0);
        push_item(out, TILE_BG_BPM, LAYOUT.bgBpm, bgP, 0);

        for (int i = 0; i < 8; ++i)
            push_item(out, mode_to_tile(in.channelMode[i]), LAYOUT.ch[i], one, 1);

        for (int i = 0; i < 6; ++i)
            push_item(out, in.bpmGlyph[i], LAYOUT.bpm[i], one, 1);
    }

    static void quad_vertices(const DrawItem& item, Vertex out[6])
    {
        const float baseHalfW = 0.07f;
        const float baseHalfH = 0.07f;

        float hx = baseHalfW * item.scale.x;
        float hy = baseHalfH * item.scale.y;

        float x0 = item.center.x - hx;
        float y0 = item.center.y - hy;
        float x1 = item.center.x + hx;
        float y1 = item.center.y + hy;

        UVRect uv = tile_uv(item.tile);

        out[0] = { x0, y0, uv.u0, uv.v1 };
        out[1] = { x1, y0, uv.u1, uv.v1 };
        out[2] = { x1, y1, uv.u1, uv.v0 };
        out[3] = { x0, y0, uv.u0, uv.v1 };
        out[4] = { x1, y1, uv.u1, uv.v0 };
        out[5] = { x0, y1, uv.u0, uv.v0 };
    }

    // renders overlay immediately from atlas in GPU memory
    // caller owns shader program and attribute/uniform locations
    void render_from_atlas(
        GLuint atlas_texture,
        const DrawList& list,
        GLint a_pos,
        GLint a_uv,
        GLint u_tex,
        GLuint scratch_vbo)
    {
        if (!atlas_texture || list.count <= 0)
            return;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas_texture);
        glUniform1i(u_tex, 0);

        glEnableVertexAttribArray((GLuint)a_pos);
        glEnableVertexAttribArray((GLuint)a_uv);

        glBindBuffer(GL_ARRAY_BUFFER, scratch_vbo);
        glVertexAttribPointer((GLuint)a_pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
        glVertexAttribPointer((GLuint)a_uv,  2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 2));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int pass = 0; pass < 2; ++pass)
        {
            for (int i = 0; i < list.count; ++i)
            {
                if (list.item[i].layer != pass)
                    continue;

                Vertex v[6];
                quad_vertices(list.item[i], v);
                glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        glDisable(GL_BLEND);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableVertexAttribArray((GLuint)a_pos);
        glDisableVertexAttribArray((GLuint)a_uv);
    }

    // helper: encode 120.0 style output into 6 glyph slots
    void encode_bpm_x10(unsigned short bpm_x10, unsigned char out6[6])
    {
        unsigned short v = bpm_x10;
        unsigned char d3 = (unsigned char)(v % 10); v /= 10;
        unsigned char d2 = (unsigned char)(v % 10); v /= 10;
        unsigned char d1 = (unsigned char)(v % 10); v /= 10;
        unsigned char d0 = (unsigned char)(v % 10);

        out6[0] = TILE_BPM_ICON;
        out6[1] = (unsigned char)(TILE_DIGIT_0 + d0);
        out6[2] = (unsigned char)(TILE_DIGIT_0 + d1);
        out6[3] = (unsigned char)(TILE_DIGIT_0 + d2);
        out6[4] = TILE_DOT;
        out6[5] = (unsigned char)(TILE_DIGIT_0 + d3);
    }
}