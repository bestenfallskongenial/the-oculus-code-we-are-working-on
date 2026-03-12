#pragma once

// TEMP INVENTORY HEADER
// Purpose:
// 1) Gather variables/arrays/structs currently referenced across
//    circle/sample/04-oculus_close_and_closer/*.cpp,*.h
// 2) Show naming conflicts/mismatches in one place before refactor.
//
// NOTE: This file is for integration cleanup planning.
// It is NOT yet wired into the build.

// -------------------------------------------------------------------------------------------------
// Structs found in this sample
// -------------------------------------------------------------------------------------------------
struct RGB;
struct glsl_state;
struct MenuTileRect;
struct MenuGpuState;

// -------------------------------------------------------------------------------------------------
// Arrays / globals already defined in current code (non-exhaustive by type, exhaustive by symbol use)
// -------------------------------------------------------------------------------------------------

// file suffix + scanned-name + loaded-bytes tables
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

// mode/menu/io core
extern unsigned char menu_map_max[];
extern const bool g_channel_mode_capability[][6];
extern int g_centralModeBuffer[][/*program slot*/];
extern int g_currentProgramBuffer;
extern int g_current_menu_layer;
extern int g_menu_mode_new;
extern int g_menu_mode_old;
extern bool g_menuPickUpFlag[];

// unified in/out matrices (new naming appears with and without g_ prefix)
extern int   g_inOutMatrixInt[][/*io_types*/];
extern float g_inOutMatrixFlt[][/*io_types*/];
extern int   g_inOutMatrixInt[][/*io_types*/];
extern float g_inOutMatrixFlt[][/*io_types*/];

// bpm/lfo/timing
extern unsigned long g_resultBPM[];
extern unsigned long g_nextBeatTime[];
extern unsigned long g_intervalCalculated[];
extern unsigned long g_lastBpmCalculation[];
extern unsigned long g_lastBpmCalculationTMP[];
extern unsigned long g_lastCircleBuffer[];
extern unsigned long g_nextCircleBuffer[];
extern unsigned long g_elapsedMicroseconds[];
extern unsigned long g_cycleLength[];
extern int g_sampleIndex[];
extern int g_activeBpmChannel;
extern int g_lfoMultiplier[];
extern int g_lfoMultiplierTMP[];
extern float g_lfoFltOut[];
extern int g_lfoIntOut[];

// random/audio/mode support
extern float g_randomFloatValue[];
extern int g_randomIntegerValue[];
extern int g_sensitivityNew;
extern int g_sensitivityOld;
extern int g_attenuation;
extern int g_modes[];

// runtime selection m_glsl
extern int g_current_gl_program;
extern int g_last_gl_program;
extern int g_loaded_fsh_new;
extern int p_validTextureCount;
extern int p_validVideoCount;
extern bool shader_has_stored_params[];

// button timing/event m_glsl
extern unsigned int g_buttons_states[2][5];
extern unsigned int g_currentTime;
extern unsigned int g_double_click_time;
extern unsigned int g_long_click_time;

// menu overlay globals from "the functions.cpp"
extern char *MENU_FSH_EXTENSIONS[1];
extern char *MENU_TEX_EXTENSIONS[1];
extern unsigned MENU_FSH_LOADED_BYTES[1];
extern unsigned MENU_TEX_LOADED_BYTES[1];
extern float MENU_GPU_ORIGIN[2];
extern float MENU_GPU_TILE_SIZE[2];
extern float MENU_GPU_BACKGROUND_SCALE[2];
extern float MENU_GPU_REL_POS[16][2];
extern float MENU_GPU_REL_SIZE[16][2];
extern int MENU_GPU_TILE_COUNT;
extern MenuGpuState g_menu_state;

// frequently referenced engine members in this sample
// (listed as names only to make cross-file use obvious during cleanup)
// m_pFileSystem, m_glsl, m_videoBuffer, m_bufferKnl, m_bufferVsh, m_bufferFsh, m_bufferOmf,
// m_bufferOmt, m_bufferTex, m_bufferFameA/m_bufferFameB, m_bufferVid,
// m_videoBlockBase/m_videoBlockSize, m_frameBlockBaseA/B, m_textureBlockBase,
// m_USBhasLoadOnes, m_resetFlag, m_shaderStatusFlags, m_bStorageAttached

// -------------------------------------------------------------------------------------------------
// Naming conflicts / mismatches to resolve (with context)
// -------------------------------------------------------------------------------------------------

// 1) Wrapper function mismatch
//    kernel_run.cpp calls: wrapper_load_sd(), wrapper_init_usb()
//    wrappers.cpp defines: wrapper_from_sd(), wrapper_load_usb()

// 2) inOut matrix name mismatch
//    menu_final.cpp/features.cpp use: g_inOutMatrixInt/g_inOutMatrixFlt
//    util.cpp uses:               g_inOutMatrixInt/g_inOutMatrixFlt

// 3) mode buffer name mismatch
//    new path: g_centralModeBuffer + g_currentProgramBuffer
//    old path: g_centralModeBuffer + g_currentProgramBuffer

// 4) BPM name mismatch
//    new path: g_resultBPM
//    old path: g_resultBPM

// 5) enum token mismatch (case/typo)
//    enum in kernel.h: trL, trH, trF
//    use in menu_final.cpp: trL, trH, trF

// 6) filecounter index naming mismatch
//    kernel.h enum: maxUsb
//    wrappers.cpp use: maxUsb

// 7) m_glsl object name mismatch
//    glsl m_glsl member appears as m_glsl in kernel.h
//    wrappers.cpp/gfx paths also use bare "m_glsl" in calls

// 8) omt buffer name mismatch
//    m_bufferOmt vs m_bufferOmt

// 9) buttonPing call mismatch
//    signature: buttonPing(int p_btn_id, int pin)
//    wrapper_io() currently calls buttonPing() without args

// 10) legacy CString/log use still mixed with new buffer log goal
//     Run path still references CString-oriented flow in comments and save calls.


// -------------------------------------------------------------------------------------------------
// Auto-collected symbol index (from current sample sources)
// -------------------------------------------------------------------------------------------------
// - MENU_FSH_EXTENSIONS
// - MENU_FSH_LOADED_BYTES
// - MENU_GPU_BACKGROUND_SCALE
// - MENU_GPU_ORIGIN
// - MENU_GPU_REL_POS
// - MENU_GPU_REL_SIZE
// - MENU_GPU_TILE_COUNT
// - MENU_GPU_TILE_SIZE
// - MENU_TEX_EXTENSIONS
// - MENU_TEX_LOADED_BYTES
// - g_currentProgramBuffer
// - filecounter
// - g_ScnFsh
// - g_ScnOmf
// - g_ScnOmt
// - g_ScnTex
// - g_ScnVid
// - g_ScnVsh
// - g_SufFsh
// - g_SufOmf
// - g_SufOmt
// - g_SufTex
// - g_SufVid
// - g_SufVsh
// - g_activeBpmChannel
// - g_attenuation
// - g_buttons_states
// - g_bytFsh
// - g_bytOmf
// - g_bytOmt
// - g_bytTex
// - g_bytVid
// - g_bytVsh
// - g_centralModeBuffer
// - g_channel_mode_capability
// - g_currentProgramBuffer
// - g_currentTime
// - g_current_gl_program
// - g_current_menu_layer
// - g_cycleLength
// - g_double_click_time
// - g_elapsedMicroseconds
// - g_hFile
// - g_inOutMatrixFlt
// - g_inOutMatrixInt
// - g_intervalCalculated
// - g_lastBpmCalculation
// - g_lastBpmCalculationTMP
// - g_lastCircleBuffer
// - g_last_gl_program
// - g_lfoFltOut
// - g_lfoIntOut
// - g_lfoMultiplier
// - g_lfoMultiplierTMP
// - g_linked_programs_counter
// - g_loaded_fsh_new
// - g_log_string
// - g_long_click_time
// - g_menu_mode_new
// - g_menu_mode_old
// - g_menu_state
// - g_modes
// - g_nextBeatTime
// - g_nextCircleBuffer
// - g_opaque
// - g_randomFloatValue
// - g_randomIntegerValue
// - g_resultBPM
// - g_rgb_color_table
// - g_sampleIndex
// - g_sensitivityNew
// - g_sensitivityOld
// - g_texLogNames
// - p_validTextureCount
// - p_validVideoCount
// - g_vidLogNames
// - g_waveTable
// - g_inOutMatrixFlt
// - g_inOutMatrixInt
// - m_ActLED
// - m_bufferOmt
// - m_Button_A
// - m_Button_B
// - m_ChipSelectPin
// - m_DebugCharArray
// - m_DeviceNameService
// - m_EMMC
// - m_GPIOManager
// - m_H264Decoder
// - m_H264Parser
// - m_H264SystemParser
// - m_Interrupt
// - m_Logger
// - m_MCP300X
// - m_NeoPixels
// - m_Options
// - m_SPIMaster
// - m_Screen
// - m_Serial
// - m_SharedMemory
// - m_TextureA
// - m_Timer
// - m_USBHCI
// - m_USBhasLoadOnes
// - m_VCHIQ
// - m_Watchdog
// - m_bStorageAttached
// - m_bufferFameA
// - m_bufferFameB
// - m_bufferFrA
// - m_bufferFrB
// - m_bufferFrA
// - m_bufferFrB
// - m_bufferFsh
// - m_bufferFshader
// - m_bufferKernel
// - m_bufferKnl
// - m_bufferLog
// - m_bufferOmf
// - m_bufferOmt
// - m_bufferTex
// - m_bufferTexture
// - m_bufferVid
// - m_bufferVid
// - m_bufferVsh
// - m_bufferVshader
// - m_button
// - m_frameBlockBaseA
// - m_frameBlockBaseB
// - m_frameBlockSizeA
// - m_frameBlockSizeB
// - m_frameRawBlockA
// - m_frameRawBlockB
// - m_glsl
// - m_overlayRawBlock
// - m_overlyBlockBase
// - m_overlyBlockSize
// - m_pFileSystem
// - m_resetFlag
// - m_shaderStatusFlags
// - m_tex_data_offset
// - m_tex_height
// - m_tex_valid
// - m_tex_width
// - m_textureBlockBase
// - m_textureBlockSize
// - m_textureRawBlock
// - m_vc_handle
// - m_vc_pointer
// - m_videoBlockBase
// - m_videoBlockSize
// - m_videoBuffer
// - m_videoRawBlock
// - g_menuPickUpFlag
// - g_centralModeBuffer
// - g_resultBPM
// - shader_has_stored_params
// - m_glsl