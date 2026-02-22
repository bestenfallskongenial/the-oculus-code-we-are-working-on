#include "kernel.h"
#include <circle/string.h> 
#include <stdint.h>
/*
< extern        < type                 < name                                   < size
        <const
*/   
extern          CString                 g_barsString;
extern          CString                 g_log_string;
extern          CString                 g_error_string;
extern          CString                 g_modes;

extern          bool                    noTargetFPS;
extern          bool                    adc_pot_routing;
extern          bool                    resetFlag;
extern          bool                    g_show_bank;

extern          bool                    is_hold_for_2_sec_a;
extern          bool                    is_hold_for_2_sec_b;

extern          bool                    is_hold_for_4_sec_a;
extern          bool                    is_hold_for_4_sec_b;
extern          bool                    timer_mode;
extern          bool                    single_tex_mode;

extern          float                   current_fps;
extern          unsigned long           last_swap_duration;
extern          int                     g_last_gl_program;  // Store last active shader program

extern          int                     g_currentProgramBuffer;

extern          int                     g_loaded_vsh_old;
extern          int                     g_loaded_fsh_old;
extern          int                     g_loaded_tex_old;
extern          int                     g_loaded_vid_old;
extern          int                     g_loaded_vsh_new;
extern          int                     g_loaded_fsh_new;
extern          int                     g_loaded_tex_new;
extern          int                     g_loaded_vid_new;   

extern          int                     g_scanned_vsh;
extern          int                     g_scanned_fsh;
extern          int                     g_scanned_tex;
extern          int                     g_scanned_vid; 

extern bool           a_is_hold;
extern bool           b_is_hold;

extern          int                     g_current_gl_program; // display_debug value
extern          int                     gl_current_tex;
extern          int                     g_linked_programs_counter;
extern          int                     g_audio_source_channel;

extern          int                     g_menu_mode_new;                    // Current menu mode (0, 1, or 2)
extern          int                     g_menu_mode_old;
extern          int                     g_current_texture;
extern          int                     g_activeBpmChannel;
extern          int                     g_attenuation;
extern          unsigned                g_hFile;  // extern          bool                    showMenuFlag;

extern          unsigned long           start_time_fps_calculation;
extern          unsigned long           end_time_fps_calculation;
extern          unsigned long           total_time_fps_calculation;

extern          unsigned long           currentTime;
extern          unsigned long           targetTime;
extern          unsigned long           delayTime;


extern          float                   /* u_audioSmoothBand */[4]; 
extern          float                   GLtime;
extern          float                   g_opaque;

//  Define file names on the sd card and usb stick
extern  const   char                    *g_vidLogNames                          [VID_FILES_ON_SD+VID_FILES_ON_USB+4];
extern  const   char                    *g_texLogNames                          [TEX_FILES_ON_SD+TEX_FILES_ON_USB+4];

extern  const   char                    *g_vhsExtensions[VSH_VALID_SUFFIX_COUNT]; 
extern  const   char                    *g_fhsExtensions[FSH_VALID_SUFFIX_COUNT];
extern  const   char                    *g_texExtensions[TEX_VALID_SUFFIX_COUNT];
extern  const   char                    *g_vidExtensions[VID_VALID_SUFFIX_COUNT];

extern          char                   *g_vshScannedFileNames[40];
extern          char                   *g_fshScannedFileNames[40];
extern          char                   *g_texScannedFileNames[40];
extern          char                   *g_vidScannedFileNames[40];

extern          unsigned                g_vshLoadedBytes[VSH_FILES_ON_SD + VSH_FILES_ON_USB];
extern          unsigned                g_fshLoadedBytes[FSH_FILES_ON_SD + FSH_FILES_ON_USB];
extern          unsigned                g_texLoadedBytes[TEX_FILES_ON_SD + TEX_FILES_ON_USB+4];
extern          unsigned                g_vidLoadedBytes[VID_FILES_ON_SD + VID_FILES_ON_USB]; 
/* Define arrays for bitmap metadata
extern          bool                    TEX_FILE_STATUS                         [TEX_FILES_ON_SD + TEX_FILES_ON_USB];                                
extern          uint32_t                TEX_FILE_SIZE                           [TEX_FILES_ON_SD + TEX_FILES_ON_USB];           // File size 
extern          uint32_t                TEX_FILE_BM_OFFSET                      [TEX_FILES_ON_SD + TEX_FILES_ON_USB];           // Offset 
extern          uint32_t                TEX_FILE_X_DIM                          [TEX_FILES_ON_SD + TEX_FILES_ON_USB];           // Width 
extern          uint32_t                TEX_FILE_Y_DIM                          [TEX_FILES_ON_SD + TEX_FILES_ON_USB];           // Height 
extern          uint32_t                TEX_FILE_BM_SIZE                        [TEX_FILES_ON_SD + TEX_FILES_ON_USB];           // Image size 
//  Define the g_lfoMultiplier for my LFO ( static when used inside the LFO function ) */
extern          int                     g_lfoMultiplier                              [LFO_MULTIPLIERS];
extern          unsigned long           g_lastCircleBuffer                      [LFO_INSTANCES];
extern          unsigned long           g_nextCircleBuffer                      [LFO_INSTANCES];
extern          float                   g_lfoFltOut                        [LFO_INSTANCES];
extern          int                     g_lfoMltTmp                         [LFO_INSTANCES];

extern          int                     adc_raw_value                           [ADC_CHANNELS];
extern          int                     adc_int_value                           [ADC_CHANNELS];
extern          float                   adc_float_value                         [ADC_CHANNELS];
extern          int                     output_int_value                        [ADC_CHANNELS];
extern          float                   output_float_value                      [ADC_CHANNELS];                    // Array of output values per channel
extern          int                     g_randomIntegerValue                        [ADC_CHANNELS];
extern          float                   g_randomFloatValue                      [ADC_CHANNELS];                    // Array of u_seed_a values per channel

//extern          float                   float_lfo_output                        [LFO_INSTANCES];         // realtime output for 2 x 4 lfo waves ( sine, saw, tri, ... )
extern          int                     g_lfoIntOut                          [LFO_INSTANCES];
extern          bool                    m_shaderStatusFlags                     [FSH_FILES_ON_SD+FSH_FILES_ON_USB]; 
extern          bool                    menu_pickup_flag                        [ADC_CHANNELS+ADC_CHANNELS];                 // Array tracking if pot value has been "picked up"
extern          bool                    input_threshold_flag                    [ADC_CHANNELS];             // Array tracking threshold state per channel
extern          unsigned long           g_resultBPM                               [2];
extern          int                     g_input_mode                            [ADC_CHANNELS];             // Array storing operation mode for each channel
extern          int                     g_threshold_high                          [ADC_CHANNELS];                    // Array of upper threshold values per channel
extern          int                     g_threshold_low                           [ADC_CHANNELS];                     // Array of lower threshold values per channel
extern          int                     g_menu_pickup_buffer                      [ADC_CHANNELS+ADC_CHANNELS];               // Array storing previous pot values for pickup detection
extern          unsigned long           current_time_for_bpm;                      // Array storing timing information per channel
extern          unsigned long           g_nextBeatTime                          [2];                     // Next beat timestamp for BPM mode
extern          unsigned long           g_intervalCalculated                         [2];
extern          unsigned long           g_lastBpmCalculation              [2];
extern          unsigned long           g_lastValidCalculationBuffer                   [2];
extern          unsigned long           g_intervalBuffer                          [2][3];
extern          unsigned char           led_col_new                             [20][3];
extern  const   unsigned char           g_rgb_color_table                                 [49][3]; 

extern          unsigned long           g_waveTable                               [WAVEFORMS][WAVESAMPLES];


extern          unsigned long g_elapsedMicroseconds[LFO_INSTANCES];
extern          unsigned long g_cycleLength[LFO_INSTANCES];
extern          int g_sampleIndex[LFO_INSTANCES];

extern          int LFO_mode[4];

extern          int g_sensitivityNew;
extern          int g_sensitivityOld;

extern          unsigned long     loaded_bytes_kernel[2];

extern          int g_centralModeBuffer[16][33];
extern          int shader_has_stored_params[33];
extern          int audio_sample[2];

extern          CString h264_info;  // <- needs to be global, no return of this function!

extern          int g_validTextureCount;
/*
new menu_input_mode

[8 ( for adc inputs ) + 4 ( LFO ) + 4 ( extra mode flags )][progs on sd + progs on usb]
 menu_input_mode        LFO_mode    g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer],g_centralModeBuffer[TEX_MODE][g_currentProgramBuffer],  adc_attenuation    

eventual also the variable buffers ***_old???
*/

