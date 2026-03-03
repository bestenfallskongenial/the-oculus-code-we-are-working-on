#ifndef OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H
#define OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H

// TEMP INVENTORY HEADER
// Purpose:
// - Gather variables/arrays/structs used across the 04-oculus_close_and_closer files.
// - Show confusing or mismatching names in-context so naming can be unified.
// - This file is intentionally documentation-first and not wired into build yet.

#include <stdint.h>
#include <stddef.h>

// ---------------------------------------------------------------------------------------------------------------------
// 1) Structs encountered in this sample
// ---------------------------------------------------------------------------------------------------------------------

struct RGB
{
    uint8_t red;
    uint8_t grn;
    uint8_t blu;
};

struct glsl_states
{
    // EGL/window handles + shader/program/texture/uniform tables
    uint32_t screen_width;
    uint32_t screen_height;

    // NOTE: exact VC/EGL/GLES types intentionally omitted here to keep this temp header lightweight.
    // In production, keep the original full typed struct from kernel.h.

    unsigned gl_vsh_id_placeholder;
    unsigned gl_fsh_id_placeholder;
    unsigned gl_prg_id_placeholder;
    unsigned gl_tex_id_placeholder;
};

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
    unsigned program;
    unsigned atlas_texture;
    int a_vertex;
    int u_atlas;
    int u_tile_count;
    int u_tile_rect;
    int u_tile_index;
    int tile_index[16];
    MenuTileRect tile_rect[16];
};

// ---------------------------------------------------------------------------------------------------------------------
// 2) Core mode / IO model (target naming)
// ---------------------------------------------------------------------------------------------------------------------

// Canonical buffers used by menu_final.cpp / util.cpp / features.cpp.
extern int   g_centralModeBuffer[16][33];
extern int   g_inOutMatrixInt[8][16];
extern float g_inOutMatrixFlt[8][16];

extern uint8_t menu_map_max[12];
extern bool    menu_pickup_flag[16];
extern bool    g_channel_mode_capability[8][6];

// Current selection / slot state
extern int g_currentProgramBuffer;
extern int g_current_gl_program;
extern int g_last_gl_program;
extern int g_current_menu_layer;

// ---------------------------------------------------------------------------------------------------------------------
// 3) Timing / BPM / LFO state (currently split across old/new names)
// ---------------------------------------------------------------------------------------------------------------------

extern unsigned long g_resultBPM[2];
extern unsigned long g_nextBeatTime[2];
extern unsigned long g_intervalCalculated[2];
extern unsigned long g_lastBpmCalculation[2];

extern unsigned long g_lastCircleBuffer[2];
extern unsigned long g_nextCircleBuffer[2];
extern unsigned long g_elapsedMicroseconds[2];
extern unsigned long g_cycleLength[2];
extern int           g_sampleIndex[2];

extern int g_activeBpmChannel;

// ---------------------------------------------------------------------------------------------------------------------
// 4) Menu/button/input state
// ---------------------------------------------------------------------------------------------------------------------

extern unsigned int g_buttons_states[2][5];
extern unsigned long g_currentTime;
extern unsigned long g_double_click_time;
extern unsigned long g_long_click_time;

extern int g_menu_mode_new;
extern int g_menu_mode_old;

extern bool is_hold_for_2_sec_a;
extern bool is_hold_for_2_sec_b;

// ---------------------------------------------------------------------------------------------------------------------
// 5) Filesystem/media scanning/loading state
// ---------------------------------------------------------------------------------------------------------------------

extern const char *g_SufVsh[];
extern const char *g_SufOmf[];
extern const char *g_SufFsh[];
extern const char *g_SufOmt[];
extern const char *g_SufTex[];
extern const char *g_SufVid[];

extern char *g_ScnVsh[];
extern char *g_ScnOmf[];
extern char *g_ScnFsh[];
extern char *g_ScnOmt[];
extern char *g_ScnTex[];
extern char *g_ScnVid[];

extern unsigned g_bytVsh[];
extern unsigned g_bytOmf[];
extern unsigned g_bytFsh[];
extern unsigned g_bytOmt[];
extern unsigned g_bytTex[];
extern unsigned g_bytVid[];

extern int filecounter[5][6];

// ---------------------------------------------------------------------------------------------------------------------
// 6) Render/menu overlay helper globals from "the functions.cpp"
// ---------------------------------------------------------------------------------------------------------------------

extern char *MENU_FSH_EXTENSIONS[1];
extern char *MENU_TEX_EXTENSIONS[1];

extern char *SCANED_FILES_MENU_FSH[1];
extern char *SCANED_FILES_MENU_TEX[1];
extern unsigned MENU_FSH_LOADED_BYTES[1];
extern unsigned MENU_TEX_LOADED_BYTES[1];

extern float MENU_GPU_ORIGIN[2];
extern float MENU_GPU_TILE_SIZE[2];
extern float MENU_GPU_BACKGROUND_SCALE[2];
extern float MENU_GPU_REL_POS[16][2];
extern float MENU_GPU_REL_SIZE[16][2];
extern int   MENU_GPU_TILE_COUNT;
extern MenuGpuState g_menu_state;

// ---------------------------------------------------------------------------------------------------------------------
// 7) Known confusing / mismatching names (rename candidates)
// ---------------------------------------------------------------------------------------------------------------------

// Old                            -> Candidate canonical name
// -------------------------------------------------------------------------------------------------

// active_bpm_channel             -> g_activeBpmChannel
// g_nextBeatTime                 -> g_nextBeatTime
// g_inOutMatrixInt / g_inOutMatrixInt  -> ONE spelling only (recommend g_inOutMatrixInt)
// g_inOutMatrixFlt / g_inOutMatrixFlt  -> ONE spelling only (recommend g_inOutMatrixFlt)
// g_current_gl_program                 -> g_current_gl_program
// g_validTextureCount / g_validTextureCount -> one global symbol (pick one prefix strategy)
// m_bufferOmt / m_bufferOmt      -> one member spelling
// m_bufferVid / m_bufferVid    -> one member spelling
// maxUsb / maxUsb                -> one enum token spelling
// trL,trH vs trL,trH             -> one enum token spelling

// Wrapper/API drift to normalize:
// wrapper_from_sd   vs wrapper_load_sd
// wrapper_load_usb  vs wrapper_init_usb
// buttonPing(int,int) is called without args in wrappers.cpp

#endif // OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H