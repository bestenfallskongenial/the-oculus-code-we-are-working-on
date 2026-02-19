#include "kernel.h"
#include "global.h"
#include <cstring>

// -----------------------------------------------------------------------------
// menu_gpu.cpp
//
// Standalone, deterministic GPU menu overlay module.
//
// This file is intentionally not integrated into the render loop yet.
// It provides the minimum building blocks for:
//   1) a system-only menu fragment shader,
//   2) fixed tile layout (relative to an origin),
//   3) CPU -> tile-index packing,
//   4) one overlay draw pass on the same back buffer.
// -----------------------------------------------------------------------------

namespace
{
    static const int MENU_GPU_MAX_TILES = 20;
    static const int MENU_GPU_ATLAS_GRID = 8;

    // Atlas tile assignment (example/default mapping).
    // Integration can change these constants if the atlas packing differs.
    static const int TILE_DIGIT_BASE = 32;      // 32..41 = '0'..'9'
    static const int TILE_DOT = 42;
    static const int TILE_BPM_ICON = 43;
    static const int TILE_CURSOR_LEFT = 44;
    static const int TILE_CURSOR_RIGHT = 45;

    static const char* kMenuOverlayFragmentShader =
        "precision mediump float;\n"
        "varying vec2 tcoord;\n"
        "uniform sampler2D u_menu_atlas;\n"
        "uniform vec2 u_resolution;\n"
        "uniform vec4 u_tile_rect[20];\n"   // x, y, w, h in pixels
        "uniform float u_tile_index[20];\n" // 0..63, <0 = disabled
        "uniform float u_alpha;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec2 fragPx = tcoord * u_resolution;\n"
        "    vec4 outColor = vec4(0.0);\n"
        "\n"
        "    for (int i = 0; i < 20; ++i)\n"
        "    {\n"
        "        float idx = u_tile_index[i];\n"
        "        if (idx < 0.0)\n"
        "        {\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        vec4 r = u_tile_rect[i];\n"
        "        bool inside = (fragPx.x >= r.x) && (fragPx.x < (r.x + r.z)) &&\n"
        "                      (fragPx.y >= r.y) && (fragPx.y < (r.y + r.w));\n"
        "\n"
        "        if (!inside)\n"
        "        {\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        vec2 localUv = vec2((fragPx.x - r.x) / r.z, (fragPx.y - r.y) / r.w);\n"
        "\n"
        "        float col = mod(idx, 8.0);\n"
        "        float row = floor(idx / 8.0);\n"
        "\n"
        "        vec2 atlasUv = vec2((col + localUv.x) / 8.0, (row + localUv.y) / 8.0);\n"
        "        vec4 sampleColor = texture2D(u_menu_atlas, atlasUv);\n"
        "\n"
        "        if (sampleColor.a > 0.001)\n"
        "        {\n"
        "            outColor = sampleColor;\n"
        "        }\n"
        "    }\n"
        "\n"
        "    outColor.a *= u_alpha;\n"
        "    gl_FragColor = outColor;\n"
        "}\n";

    struct MenuRect
    {
        float x;
        float y;
        float w;
        float h;
    };

    struct MenuGpuFrame
    {
        float tileIndex[MENU_GPU_MAX_TILES];
        MenuRect tileRect[MENU_GPU_MAX_TILES];
    };

    struct MenuGpuState
    {
        GLuint glProgram;
        GLuint glFragmentShader;
        GLuint glAtlasTexture;

        GLint uMenuAtlas;
        GLint uResolution;
        GLint uTileRect;
        GLint uTileIndex;
        GLint uAlpha;

        float originX;
        float originY;
        float tilePx;

        bool initialized;
    };

    static int menu_digit_to_tile(int digit)
    {
        if (digit < 0 || digit > 9)
        {
            return TILE_DIGIT_BASE;
        }
        return TILE_DIGIT_BASE + digit;
    }

    static void menu_gpu_reset_frame(MenuGpuFrame* frame)
    {
        for (int i = 0; i < MENU_GPU_MAX_TILES; ++i)
        {
            frame->tileIndex[i] = -1.0f;
            frame->tileRect[i].x = 0.0f;
            frame->tileRect[i].y = 0.0f;
            frame->tileRect[i].w = 0.0f;
            frame->tileRect[i].h = 0.0f;
        }
    }

    static void menu_gpu_set_tile(MenuGpuFrame* frame,
                                  int slot,
                                  float tileIndex,
                                  float x,
                                  float y,
                                  float w,
                                  float h)
    {
        if (slot < 0 || slot >= MENU_GPU_MAX_TILES)
        {
            return;
        }

        frame->tileIndex[slot] = tileIndex;
        frame->tileRect[slot].x = x;
        frame->tileRect[slot].y = y;
        frame->tileRect[slot].w = w;
        frame->tileRect[slot].h = h;
    }

    static bool menu_gpu_compile_overlay_program(MenuGpuState* state, GLuint sharedVertexShader)
    {
        state->glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(state->glFragmentShader, 1, &kMenuOverlayFragmentShader, 0);
        glCompileShader(state->glFragmentShader);

        GLint compileOk = GL_FALSE;
        glGetShaderiv(state->glFragmentShader, GL_COMPILE_STATUS, &compileOk);
        if (compileOk != GL_TRUE)
        {
            return false;
        }

        state->glProgram = glCreateProgram();
        glAttachShader(state->glProgram, sharedVertexShader);
        glAttachShader(state->glProgram, state->glFragmentShader);
        glLinkProgram(state->glProgram);

        GLint linkOk = GL_FALSE;
        glGetProgramiv(state->glProgram, GL_LINK_STATUS, &linkOk);
        if (linkOk != GL_TRUE)
        {
            return false;
        }

        state->uMenuAtlas = glGetUniformLocation(state->glProgram, "u_menu_atlas");
        state->uResolution = glGetUniformLocation(state->glProgram, "u_resolution");
        state->uTileRect = glGetUniformLocation(state->glProgram, "u_tile_rect");
        state->uTileIndex = glGetUniformLocation(state->glProgram, "u_tile_index");
        state->uAlpha = glGetUniformLocation(state->glProgram, "u_alpha");

        return true;
    }

    static void menu_gpu_build_frame(const MenuGpuState* state,
                                     const int modeStorage[8],
                                     const int resultBPM[2],
                                     int activeBpmChannel,
                                     MenuGpuFrame* outFrame)
    {
        // Layout slots:
        //   0..3   : channel A modes (icons)
        //   4..7   : channel B modes (icons)
        //   8..11  : BPM digits
        //   12     : dot
        //   13     : BPM icon
        //   14     : left selector cursor
        //   15     : right selector cursor
        //   16..19 : reserved

        const float ox = state->originX;
        const float oy = state->originY;
        const float t = state->tilePx;

        menu_gpu_reset_frame(outFrame);

        // 2x2 left block (A channels)
        menu_gpu_set_tile(outFrame, 0, (float)modeStorage[0], ox + 0.0f * t, oy + 0.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 1, (float)modeStorage[1], ox + 1.0f * t, oy + 0.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 2, (float)modeStorage[2], ox + 0.0f * t, oy + 1.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 3, (float)modeStorage[3], ox + 1.0f * t, oy + 1.0f * t, t, t);

        // 2x2 right block (B channels)
        menu_gpu_set_tile(outFrame, 4, (float)modeStorage[4], ox + 4.0f * t, oy + 0.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 5, (float)modeStorage[5], ox + 5.0f * t, oy + 0.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 6, (float)modeStorage[6], ox + 4.0f * t, oy + 1.0f * t, t, t);
        menu_gpu_set_tile(outFrame, 7, (float)modeStorage[7], ox + 5.0f * t, oy + 1.0f * t, t, t);

        // Bottom strip: "123.4 O"
        const float stripY = oy + 3.0f * t;
        menu_gpu_set_tile(outFrame, 8,  (float)menu_digit_to_tile(resultBPM[0] / 100), ox + 0.0f * t, stripY, t, t);
        menu_gpu_set_tile(outFrame, 9,  (float)menu_digit_to_tile((resultBPM[0] / 10) % 10), ox + 1.0f * t, stripY, t, t);
        menu_gpu_set_tile(outFrame, 10, (float)menu_digit_to_tile(resultBPM[0] % 10),        ox + 2.0f * t, stripY, t, t);
        menu_gpu_set_tile(outFrame, 11, (float)menu_digit_to_tile(resultBPM[1] % 10),        ox + 4.0f * t, stripY, t, t);
        menu_gpu_set_tile(outFrame, 12, (float)TILE_DOT,                                      ox + 3.0f * t, stripY, t, t);
        menu_gpu_set_tile(outFrame, 13, (float)TILE_BPM_ICON,                                 ox + 6.0f * t, stripY, t, t);

        if (activeBpmChannel == 0)
        {
            menu_gpu_set_tile(outFrame, 14, (float)TILE_CURSOR_LEFT, ox + 6.8f * t, stripY, t * 0.8f, t);
        }
        else
        {
            menu_gpu_set_tile(outFrame, 15, (float)TILE_CURSOR_RIGHT, ox + 7.0f * t, stripY, t * 0.8f, t);
        }
    }
}

// -----------------------------------------------------------------------------
// Public entry points for later integration in kernel/gfx flow.
// -----------------------------------------------------------------------------

bool menu_gpu_init(MenuGpuState* state, GLuint sharedVertexShader, GLuint atlasTexture)
{
    if (state == 0)
    {
        return false;
    }

    memset(state, 0, sizeof(MenuGpuState));
    state->originX = 24.0f;
    state->originY = 24.0f;
    state->tilePx = 32.0f;
    state->glAtlasTexture = atlasTexture;

    state->initialized = menu_gpu_compile_overlay_program(state, sharedVertexShader);
    return state->initialized;
}

void menu_gpu_shutdown(MenuGpuState* state)
{
    if (state == 0)
    {
        return;
    }

    if (state->glProgram != 0)
    {
        glDeleteProgram(state->glProgram);
        state->glProgram = 0;
    }

    if (state->glFragmentShader != 0)
    {
        glDeleteShader(state->glFragmentShader);
        state->glFragmentShader = 0;
    }

    state->initialized = false;
}

void menu_gpu_set_origin(MenuGpuState* state, float x, float y)
{
    if (state == 0)
    {
        return;
    }

    state->originX = x;
    state->originY = y;
}

void menu_gpu_set_tile_size(MenuGpuState* state, float tilePixels)
{
    if (state == 0)
    {
        return;
    }

    if (tilePixels <= 0.0f)
    {
        return;
    }

    state->tilePx = tilePixels;
}

void menu_gpu_render_overlay(MenuGpuState* state,
                             GLuint vbo,
                             GLint vertexAttrib,
                             int screenWidth,
                             int screenHeight,
                             const int modeStorage[8],
                             const int resultBPM[2],
                             int activeBpmChannel,
                             float alpha)
{
    if (state == 0 || !state->initialized)
    {
        return;
    }

    MenuGpuFrame frame;
    menu_gpu_build_frame(state, modeStorage, resultBPM, activeBpmChannel, &frame);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer((GLuint)vertexAttrib, 4, GL_FLOAT, 0, 16, 0);
    glEnableVertexAttribArray((GLuint)vertexAttrib);

    glUseProgram(state->glProgram);

    if (state->uResolution != -1)
    {
        glUniform2f(state->uResolution, (GLfloat)screenWidth, (GLfloat)screenHeight);
    }

    if (state->uTileIndex != -1)
    {
        glUniform1fv(state->uTileIndex, MENU_GPU_MAX_TILES, frame.tileIndex);
    }

    if (state->uTileRect != -1)
    {
        glUniform4fv(state->uTileRect, MENU_GPU_MAX_TILES, (const GLfloat*)frame.tileRect);
    }

    if (state->uAlpha != -1)
    {
        glUniform1f(state->uAlpha, alpha);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->glAtlasTexture);
    if (state->uMenuAtlas != -1)
    {
        glUniform1i(state->uMenuAtlas, 0);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_BLEND);
}