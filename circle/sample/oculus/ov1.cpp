#include "kernel.h"
#include "global.h"

// Canonical menu overlay state for second-pass shader rendering.
// Consolidated from gpu_menu_A/B/C without duplicating existing filesystem/memory/init flows.
// Shader compile/link and atlas upload are handled by gfx_init.cpp + filesystem.cpp + memory.cpp.

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

    struct MenuGpuState
    {
        bool initialized;
        int tile_index[MENU_GPU_TILE_COUNT];
        MenuTileRect tile_rect[MENU_GPU_TILE_COUNT];
    };

    MenuGpuState g_menu_state = { false, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };

    static void rebuild_layout_from_globals()
    {
        const float ox = MENU_GPU_ORIGIN[0];
        const float oy = MENU_GPU_ORIGIN[1];
        const float tw = MENU_GPU_TILE_SIZE[0];
        const float th = MENU_GPU_TILE_SIZE[1];

        for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
        {
            float sx = MENU_GPU_REL_SIZE[i][0];
            float sy = MENU_GPU_REL_SIZE[i][1];

            if (i == 14 || i == 15)
            {
                sx *= MENU_GPU_BACKGROUND_SCALE[0];
                sy *= MENU_GPU_BACKGROUND_SCALE[1];
            }

            g_menu_state.tile_rect[i].x = ox + MENU_GPU_REL_POS[i][0] * tw;
            g_menu_state.tile_rect[i].y = oy + MENU_GPU_REL_POS[i][1] * th;
            g_menu_state.tile_rect[i].w = sx * tw;
            g_menu_state.tile_rect[i].h = sy * th;
        }
    }

    static void update_indices_from_runtime_state()
    {
        g_menu_state.tile_index[0] = g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
        g_menu_state.tile_index[1] = g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
        g_menu_state.tile_index[2] = g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
        g_menu_state.tile_index[3] = g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
        g_menu_state.tile_index[4] = g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
        g_menu_state.tile_index[5] = g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
        g_menu_state.tile_index[6] = g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
        g_menu_state.tile_index[7] = g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

        const unsigned long bpm0 = g_resultBPM[0] % 10000UL;
        const unsigned long bpm1 = g_resultBPM[1] % 10UL;

        g_menu_state.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
        g_menu_state.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
        g_menu_state.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
        g_menu_state.tile_index[11] = (int) (bpm0 % 10UL);
        g_menu_state.tile_index[12] = 10;
        g_menu_state.tile_index[13] = (int) bpm1;
        g_menu_state.tile_index[14] = 48;
        g_menu_state.tile_index[15] = 49;
    }
}

// menu asset extension groups used by existing scan/load wrappers
const char *MENU_FSH_EXTENSIONS[1] = { "fsh" };
const char *MENU_TEX_EXTENSIONS[1] = { "bmp" };

char *SCANED_FILES_MENU_FSH[1] = { 0 };
char *SCANED_FILES_MENU_TEX[1] = { 0 };

unsigned MENU_FSH_LOADED_BYTES[1] = { 0 };
unsigned MENU_TEX_LOADED_BYTES[1] = { 0 };

// global menu layout controls
float MENU_GPU_ORIGIN[2] = { 0.05f, 0.08f };
float MENU_GPU_TILE_SIZE[2] = { 0.08f, 0.08f };
float MENU_GPU_BACKGROUND_SCALE[2] = { 1.0f, 1.0f };

float MENU_GPU_REL_POS[16][2] =
{
    {0.0f, 0.0f}, {1.1f, 0.0f}, {2.2f, 0.0f}, {3.3f, 0.0f},
    {0.0f, 1.1f}, {1.1f, 1.1f}, {2.2f, 1.1f}, {3.3f, 1.1f},

    {0.0f, 2.6f}, {0.9f, 2.6f}, {1.8f, 2.6f}, {2.7f, 2.6f},
    {3.6f, 2.6f}, {4.1f, 2.6f},

    {-0.1f, -0.1f},
    {-0.1f, 2.5f}
};

float MENU_GPU_REL_SIZE[16][2] =
{
    {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
    {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},

    {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f},
    {0.35f, 0.95f}, {0.75f, 0.95f},

    {4.4f, 2.2f},
    {5.8f, 1.2f}
};

void CKernel::gpu_render_menu_rebuild_layout()
{
    rebuild_layout_from_globals();
    g_menu_state.initialized = true;
}

void CKernel::gpu_render_menu_update_indices()
{
    if (!g_menu_state.initialized)
        rebuild_layout_from_globals();

    update_indices_from_runtime_state();
    g_menu_state.initialized = true;
}

// second-pass overlay draw helper following gfx_render.cpp render pass style
void CKernel::gpu_render_menu_draw(GLuint fullscreen_vbo)
{
    if (!g_menu_state.initialized)
    {
        rebuild_layout_from_globals();
        update_indices_from_runtime_state();
        g_menu_state.initialized = true;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);
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
        glUniform4fv(m_glsl.u_tile_rect[0], MENU_GPU_TILE_COUNT, (const GLfloat *) g_menu_state.tile_rect);

    GLfloat index_values[MENU_GPU_TILE_COUNT];
    for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
        index_values[i] = (GLfloat) g_menu_state.tile_index[i];

    if (m_glsl.u_tile_index[0] != -1)
        glUniform1fv(m_glsl.u_tile_index[0], MENU_GPU_TILE_COUNT, index_values);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);

}

void CKernel::gpu_render_menu_shutdown()
{
    g_menu_state = MenuGpuState{ false, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };
}