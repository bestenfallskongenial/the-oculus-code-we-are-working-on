#include "kernel.h"
#include "global.h"

// Consolidated second-pass overlay draw path.
// This file intentionally does not redefine menu scan/load globals or init wrappers.

namespace
{
    static const int MENU_GPU_TILE_COUNT = 16;

    struct MenuTileRect
    {
                float x;
                float y;
                float w;
                float h;
    };

    static const float kMenuOrigin[2] = { 0.05f, 0.08f };
    static const float kMenuTileSize[2] = { 0.08f, 0.08f };
    static const float kMenuBackgroundScale[2] = { 1.0f, 1.0f };

    static const float kMenuRelPos[MENU_GPU_TILE_COUNT][2] =
    {
                {0.0f, 0.0f}, {1.1f, 0.0f}, {2.2f, 0.0f}, {3.3f, 0.0f},
                {0.0f, 1.1f}, {1.1f, 1.1f}, {2.2f, 1.1f}, {3.3f, 1.1f},
                {0.0f, 2.6f}, {0.9f, 2.6f}, {1.8f, 2.6f}, {2.7f, 2.6f},
                {3.6f, 2.6f}, {4.1f, 2.6f},
                {-0.1f, -0.1f},
                {-0.1f, 2.5f}
    };

    static const float kMenuRelSize[MENU_GPU_TILE_COUNT][2] =
    {
                {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
                {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
                {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f},
                {0.35f, 0.95f}, {0.75f, 0.95f},
                {4.4f, 2.2f},
                {5.8f, 1.2f}
    };
}

void            CKernel::gpu_render_menu_draw()
{
                MenuTileRect tile_rect[MENU_GPU_TILE_COUNT];
                int tile_index[MENU_GPU_TILE_COUNT];

                const float ox = kMenuOrigin[0];
                const float oy = kMenuOrigin[1];
                const float tw = kMenuTileSize[0];
                const float th = kMenuTileSize[1];

                for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
                {
                    float sx = kMenuRelSize[i][0];
                    float sy = kMenuRelSize[i][1];

                    if (i == 14 || i == 15)
                    {
                        sx *= kMenuBackgroundScale[0];
                        sy *= kMenuBackgroundScale[1];
                    }

                    tile_rect[i].x = ox + kMenuRelPos[i][0] * tw;
                    tile_rect[i].y = oy + kMenuRelPos[i][1] * th;
                    tile_rect[i].w = sx * tw;
                    tile_rect[i].h = sy * th;
                }

                tile_index[0] = g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
                tile_index[1] = g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
                tile_index[2] = g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
                tile_index[3] = g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
                tile_index[4] = g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
                tile_index[5] = g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
                tile_index[6] = g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
                tile_index[7] = g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

                const unsigned long bpm0 = g_resultBPM[0] % 10000UL;
                const unsigned long bpm1 = g_resultBPM[1] % 10UL;

                tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
                tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
                tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
                tile_index[11] = (int) (bpm0 % 10UL);
                tile_index[12] = 10;
                tile_index[13] = (int) bpm1;
                tile_index[14] = 48;
                tile_index[15] = 49;

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glBindBuffer(GL_ARRAY_BUFFER, m_glsl.gl_buf);
                glUseProgram(m_glsl.gl_omp_id[0]);

                glVertexAttribPointer(m_glsl.gl_vtx, 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(m_glsl.gl_vtx);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_glsl.gl_omt_id[0]);

                if (m_glsl.u_atlas[0] != -1)
                    glUniform1i(m_glsl.u_atlas[0], 0);
                if (m_glsl.u_tile_count[0] != -1)
                    glUniform1i(m_glsl.u_tile_count[0], MENU_GPU_TILE_COUNT);
                if (m_glsl.u_tile_rect[0] != -1)
                    glUniform4fv(m_glsl.u_tile_rect[0], MENU_GPU_TILE_COUNT, (const GLfloat *) tile_rect);

                GLfloat index_values[MENU_GPU_TILE_COUNT];
                for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
                    index_values[i] = (GLfloat) tile_index[i];

                if (m_glsl.u_tile_index[0] != -1)
                    glUniform1fv(m_glsl.u_tile_index[0], MENU_GPU_TILE_COUNT, index_values);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glDisable(GL_BLEND);
}