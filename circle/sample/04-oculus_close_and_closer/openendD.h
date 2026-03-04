#ifndef OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H
#define OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H

/*
 * TEMP INTEGRATION HEADER
 *
 * Goal:
 * - central inventory of globals/arrays/structs currently spread across 04-oculus_close_and_closer
 * - highlight naming collisions (legacy vs new) in one place before final refactor
 *
 * NOTE: This is an integration scratch header and intentionally conservative.
 */

#include "kernel.h"
#include <stdint.h>

// ---------------------------------------------------------------------------------------------------------------------
// Structs currently present in this sample
// ---------------------------------------------------------------------------------------------------------------------

struct RGB
{
    uint8_t red;
    uint8_t grn;
    uint8_t blu;
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
    GLuint program;
    GLuint atlas_texture;
    GLint a_vertex;
    GLint u_atlas;
    GLint u_tile_count;
    GLint u_tile_rect;
    GLint u_tile_index;
    int tile_index[16];
    MenuTileRect tile_rect[16];
};

// ---------------------------------------------------------------------------------------------------------------------
// File scan / file load globals
// ---------------------------------------------------------------------------------------------------------------------

extern const char *g_SufVsh[SUFFIX_VSH];
extern const char *g_SufOmf[SUFFIX_OMF];
extern const char *g_SufFsh[SUFFIX_FSH];
extern const char *g_SufOmt[SUFFIX_OMT];
extern const char *g_SufTex[SUFFIX_TEX];
extern const char *g_SufVid[SUFFIX_VID];

extern char *g_ScnVsh[VSH_FILES_ON_SD + VSH_FILES_ON_USB];
extern char *g_ScnOmf[OMF_FILES_ON_SD + OMF_FILES_ON_USB];
extern char *g_ScnFsh[FSH_FILES_ON_SD + FSH_FILES_ON_USB];
extern char *g_ScnOmt[OMT_FILES_ON_SD + OMT_FILES_ON_USB];
extern char *g_ScnTex[TEX_FILES_ON_SD + TEX_FILES_ON_USB];
extern char *g_ScnVid[VID_FILES_ON_SD + VID_FILES_ON_USB];

extern unsigned g_bytVsh[VSH_FILES_ON_SD + VSH_FILES_ON_USB];
extern unsigned g_bytOmf[OMF_FILES_ON_SD + OMF_FILES_ON_USB];
extern unsigned g_bytFsh[FSH_FILES_ON_SD + FSH_FILES_ON_USB];
extern unsigned g_bytOmt[OMT_FILES_ON_SD + OMT_FILES_ON_USB];
extern unsigned g_bytTex[TEX_FILES_ON_SD + TEX_FILES_ON_USB];
extern unsigned g_bytVid[VID_FILES_ON_SD + VID_FILES_ON_USB];

extern unsigned g_hFile;

// ---------------------------------------------------------------------------------------------------------------------
// Mode + IO + timing state (new model)
// ---------------------------------------------------------------------------------------------------------------------

extern uint8_t menu_map_max[12];
extern const bool g_channel_mode_capability[ADC_CHANNELS][NUMBER_OF_MODES];

extern int g_centralModeBuffer[modetablecount][DEFAULT_SLOT + 1];
extern int g_currentProgramBuffer;

extern int g_inOutMatrixInt[ADC_CHANNELS][io_type_count];
extern float g_inOutMatrixFlt[ADC_CHANNELS][io_type_count];

extern unsigned long g_resultBPM[2];
extern unsigned long g_nextBeatTime[2];
extern unsigned long g_intervalCalculated[2];
extern unsigned long g_lastBpmCalculation[2];
extern unsigned long g_lastBpmCalculationTMP[2];
extern unsigned long g_lastCircleBuffer[2];
extern unsigned long g_nextCircleBuffer[2];
extern unsigned long g_elapsedMicroseconds[2];
extern unsigned long g_cycleLength[2];
extern int g_sampleIndex[2];

extern int g_lfoMultiplier[8];
extern int g_lfoMultiplierTMP[2];
extern float g_lfoFltOut[2];
extern int g_lfoIntOut[2];

extern unsigned long g_waveTable[WAVEFORMS][WAVESAMPLES];

extern int g_sensitivityNew;
extern int g_sensitivityOld;
extern int g_activeBpmChannel;
extern int g_attenuation;

// ---------------------------------------------------------------------------------------------------------------------
// Runtime selection and menu/button state
// ---------------------------------------------------------------------------------------------------------------------

extern int g_current_gl_program;
extern int g_last_gl_program;
extern int g_loaded_fsh_new;
extern int g_validTextureCount;
extern int g_validVideoCount;

extern int g_current_menu_layer;
extern int g_menu_mode_new;
extern int g_menu_mode_old;
extern bool menu_pickup_flag[16];

extern unsigned long g_currentTime;
extern unsigned long g_double_click_time;
extern unsigned long g_long_click_time;
extern unsigned int g_buttons_states[2][5];

// ---------------------------------------------------------------------------------------------------------------------
// Logging + diagnostics buffers
// ---------------------------------------------------------------------------------------------------------------------

extern CString g_log_string;
extern CString g_modes;

extern const char *g_texLogNames[TEX_FILES_ON_SD + TEX_FILES_ON_USB + 4];
extern const char *g_vidLogNames[VID_FILES_ON_SD + VID_FILES_ON_USB + 4];

// ---------------------------------------------------------------------------------------------------------------------
// Overlay menu pass state
// ---------------------------------------------------------------------------------------------------------------------

extern MenuGpuState g_menu_state;


// ---------------------------------------------------------------------------------------------------------------------
// Legacy symbols still referenced by 04 files (from older global.h model)
// ---------------------------------------------------------------------------------------------------------------------

extern int g_centralModeBuffer[16][33];
extern int g_currentProgramBuffer;
extern unsigned long g_resultBPM[2];
extern unsigned long currentTime;
extern unsigned long currentTime;
extern int shader_has_stored_params[33];
extern bool is_hold_for_2_sec_a;
extern bool is_hold_for_2_sec_b;

// ---------------------------------------------------------------------------------------------------------------------
// Naming collision map (legacy symbol -> new symbol candidate)
// ---------------------------------------------------------------------------------------------------------------------

/*
 * 1) Mode storage
 *    - g_centralModeBuffer[row][slot] -> g_centralModeBuffer[row][slot]
 *      Context: "the functions.cpp" still reads g_centralModeBuffer for overlay tile indices.
 *
 * 2) Program slot index
 *    - g_currentProgramBuffer -> g_currentProgramBuffer
 *      Context: overlay path uses g_currentProgramBuffer while menu/mode path uses g_currentProgramBuffer.
 *
 * 3) BPM arrays
 *    - g_resultBPM -> g_resultBPM
 *    - g_nextBeatTime -> g_nextBeatTime
 *    - last_valid_bpm_calculation -> g_lastBpmCalculation
 *      Context: mixed references between legacy global.h and new features/menu code.
 *
 * 4) IO matrix names
 *    - g_inOutMatrixInt/Flt -> g_inOutMatrixInt/Flt
 *      Context: util.cpp mostly uses g_inOutMatrix*, while menu_final.cpp/features.cpp still use inOutMatrix*.
 *
 * 5) Random vectors
 *    - random_int_value / random_float_value -> g_inOutMatrixInt[][rnd] / g_inOutMatrixFlt[][rnd]
 *      Context: modeTRG/modeBPM use rnd matrix lane but comments still reference legacy arrays.
 *
 * 6) Program handles
 *    - g_current_gl_program -> g_current_gl_program
 *      Context: legacy global.h names differ from util.cpp naming.
 *
 * 7) Threshold lane tokens
 *    - trL/trH/trF spelling inconsistency vs enum values trL/trH/trF.
 *      Context: menu_final.cpp uses lower-case variants that do not match kernel.h enum tokens.
 */

#endif // OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H