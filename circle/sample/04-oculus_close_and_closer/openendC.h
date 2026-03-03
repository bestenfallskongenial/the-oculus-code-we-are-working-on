#pragma once

/*
    temp_header.h
    -------------------------------------------------------------------------
    PURPOSE
    - Inventory header for 04-oculus_close_and_closer.
    - Collects variables/arrays/structs that are used across files.
    - Highlights confusing/mismatching names in context so naming can be unified.

    IMPORTANT
    - This file is for integration planning and rename work.
    - It is intentionally documentation-oriented (not a production ABI header yet).
*/

// ============================================================================
// 1) Structs and enums currently in active use
// ============================================================================

struct RGB
{
    unsigned char red;
    unsigned char grn;
    unsigned char blu;
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
    unsigned int program;
    unsigned int atlas_texture;
    int a_vertex;
    int u_atlas;
    int u_tile_count;
    int u_tile_rect;
    int u_tile_index;
    int tile_index[16];
    MenuTileRect tile_rect[16];
};

enum modetable
{
    CH0_MODE = 0, CH1_MODE, CH2_MODE, CH3_MODE,
    CH4_MODE, CH5_MODE, CH6_MODE, CH7_MODE,
    LF1_WAVE, LF2_WAVE, LF1_MULT, LF2_MULT,
    FRM_MODE, TEX_MODE, CLK_MODE, VID_MODE,
    modetablecount
};

enum io_types
{
    raw = 0,
    in,
    out,
    rnd,
    lf1,
    lf2,
    au0,
    au1,
    au2,
    au3,
    trL,
    trH,
    trF,
    io_type_count
};

enum ButtonTSIndex
{
    BTN_STATUS = 0,
    BTN_PRESS_START,
    BTN_DOUBLE,
    BTN_LONG,
    BTN_RELEASE,
    BTN_SINGLE,
    BTN_HOLD_TICK,
    BTN_INDEX_COUNT
};

// ============================================================================
// 2) Confirmed arrays/globals declared in this sample folder
// ============================================================================

// kernel.h
static unsigned char menu_map_max[12] = { 5,5,5,5, 5,5,5,5, 4,4,7,7 };

// NOTE: NUMBER_OF_MODES dimension is currently external/implicit.
// Keep as TODO to bind to a single compile-time constant.

// File suffix tables (kernel.h)
static const char *g_SufVsh[] = { "vsh" };
static const char *g_SufOmf[] = { "omf" };
static const char *g_SufFsh[] = { "fsh" };
static const char *g_SufOmt[] = { "omt" };
static const char *g_SufTex[] = { "bmp" };
static const char *g_SufVid[] = { "264" };

// Scanned filename arrays (kernel.h)
// g_ScnVsh, g_ScnOmf, g_ScnFsh, g_ScnOmt, g_ScnTex, g_ScnVid

// Loaded bytes arrays (kernel.h)
// g_bytVsh, g_bytOmf, g_bytFsh, g_bytOmt, g_bytTex, g_bytVid

// color_table.cpp
// const unsigned char g_rgb_color_table[49][3]

// wavetable.cpp
// unsigned long g_waveTable[WAVEFORMS][WAVESAMPLES]

// util.cpp
// unsigned int g_buttons_states[2][5]

// the functions.cpp (overlay prototype)
// MENU_FSH_EXTENSIONS, MENU_TEX_EXTENSIONS
// SCANED_FILES_MENU_FSH, SCANED_FILES_MENU_TEX
// MENU_FSH_LOADED_BYTES, MENU_TEX_LOADED_BYTES
// MENU_GPU_ORIGIN, MENU_GPU_TILE_SIZE, MENU_GPU_BACKGROUND_SCALE
// MENU_GPU_REL_POS, MENU_GPU_REL_SIZE, MENU_GPU_TILE_COUNT
// MenuGpuState g_menu_state

// bufferToScreen.cpp (file-local static render text state)
// gE_FrameBuffer, gE_CharGenerator, gE_PixelBuffer,
// gE_PitchBytes, gE_ScreenWidth, gE_ScreenHeight,
// gE_CharWidth, gE_CharHeight, gE_Cols, gE_Rows

// ============================================================================
// 3) Referenced globals that look owned by global.h / other modules
// ============================================================================

// Matrix/state
// g_centralModeBuffer
// g_inOutMatrixInt / g_inOutMatrixFlt
// g_inOutMatrixInt / g_inOutMatrixFlt   <-- legacy/non-prefixed variant also used

// Program/media selection
// g_current_gl_program, g_last_gl_program
// g_currentProgramBuffer
// g_loaded_fsh_new
// g_validTextureCount, g_validVideoCount

// Menu runtime state
// g_current_menu_layer
// g_menu_mode_new, g_menu_mode_old
// menu_pickup_flag

// Feature/timing state
// g_resultBPM, g_nextBeatTime, g_activeBpmChannel
// g_intervalCalculated
// g_lastBpmCalculation, g_lastBpmCalculationTMP
// g_lastCircleBuffer, g_nextCircleBuffer
// g_elapsedMicroseconds, g_cycleLength, g_sampleIndex
// g_lfoMultiplier, g_lfoMultiplierTMP
// g_sensitivityNew, g_sensitivityOld
// g_attenuation

// Button runtime timing
// g_currentTime, g_double_click_time, g_long_click_time

// Filesystem/log/debug
// g_hFile
// g_log_string

// ============================================================================
// 4) Naming mismatches to resolve (with context)
// ============================================================================

/*
A) Matrix naming mismatch:
   - New style: g_inOutMatrixInt / g_inOutMatrixFlt (util.cpp)
   - Mixed style: g_inOutMatrixInt / g_inOutMatrixFlt (menu_final.cpp, features.cpp)

   Context examples:
   - util.cpp -> g_inOutMatrixInt[channel][raw]
   - menu_final.cpp -> g_inOutMatrixInt[channel][out]
*/

/*
B) Mode storage naming mismatch:
   - New style: g_centralModeBuffer[...][g_currentProgramBuffer]
   - Legacy style: g_centralModeBuffer[...][g_currentProgramBuffer]

   Context examples:
   - menu_final.cpp uses g_centralModeBuffer
   - the functions.cpp uses g_centralModeBuffer + g_currentProgramBuffer
*/

/*
C) BPM naming mismatch:
   - New style: g_resultBPM
   - Legacy style: g_resultBPM

   Context examples:
   - features.cpp writes g_resultBPM
   - the functions.cpp reads g_resultBPM
*/

/*
D) io enum token mismatch:
   - enum has trL/trH/trF
   - code references trL/trH in menu_final.cpp

   Action:
   - choose ONE casing convention and apply globally.
*/

/*
E) Wrapper naming mismatch:
   - Run() calls wrapper_load_sd / wrapper_init_usb
   - wrappers.cpp defines wrapper_from_sd / wrapper_load_usb

   Action:
   - normalize declarations + definitions + call sites in one pass.
*/

/*
F) menu button API mismatch:
   - function signature: buttonPing(int p_btn_id, int pin)
   - wrapper_io calls buttonPing() without args

   Action:
   - define canonical button IDs/pins in one location and use consistently.
*/

// ============================================================================
// 5) Proposed canonical names (draft mapping table)
// ============================================================================

/*
old/new candidates -> canonical suggestion
----------------------------------------
g_inOutMatrixInt       -> g_inOutMatrixInt
g_inOutMatrixFlt     -> g_inOutMatrixFlt
g_centralModeBuffer -> g_centralModeBuffer
g_currentProgramBuffer       -> g_currentProgramBuffer
g_resultBPM            -> g_resultBPM
trL/trH              -> trL/trH
wrapper_from_sd      -> wrapper_load_sd
wrapper_load_usb     -> wrapper_init_usb_load  (or keep wrapper_load_usb)
*/

// End of temp_header inventory.