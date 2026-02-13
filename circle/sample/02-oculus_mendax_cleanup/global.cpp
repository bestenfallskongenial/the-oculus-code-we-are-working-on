// STYLE: terse. no prose. 1 line. facts only. no filler.
// we need to check what here is really relevant for the code, we have some corpses here
// also do we need to sort and even rename variables etc on a global scope.
// keep and gathering ALL variables here at one place is mandatory. we can move may variables etc from other files here
// i suggest clear seperation through naming and comments!
#include "kernel.h"
#include "global.h"

#include <circle/string.h> 
#include <stdint.h>
/*
                < type                 < name                                   < size                      < content/value
                                                                                                                    <apostroph
*/   
                CString                 g_barsString;
                CString                 g_log_string;
                CString                 g_error_string;
                CString                 g_modes;

                unsigned                hFile                                                               =     0;  // ?? empty ??
                bool                    noTargetFPS                                                         =  true;
                bool                    adc_pot_routing                                                     = false;
                bool                    resetFlag                                                           = false;
                bool                    g_show_bank                                                         = false;

                bool                    is_hold_for_2_sec_a                                                 = false;
                bool                    is_hold_for_2_sec_b                                                 = false;
                bool                    is_hold_for_4_sec_a                                                 = false;
                bool                    is_hold_for_4_sec_b                                                 = false;
                bool                    timer_mode                                                          = false;
                bool                    single_tex_mode                                                     = false;
                int                     VSH_LOADED_OLD                                                      =     0;
                int                     FSH_LOADED_OLD                                                      =     0;
                int                     TEX_LOADED_OLD                                                      =     0;
                int                     VID_LOADED_OLD                                                      =     0;
                int                     VSH_LOADED_NEW                                                      =     0;
                int                     FSH_LOADED_NEW                                                      =     0;
                int                     TEX_LOADED_NEW                                                      =     0;
                int                     VID_LOADED_NEW                                                      =     0;

                int                     scanned_vsh                                                      =     0;
                int                     scanned_fsh                                                      =     0;
                int                     scanned_tex                                                      =     0;
                int                     scanned_vid                                                      =     0;

bool           a_is_hold = false;
bool           b_is_hold = false;

                float                   current_fps                                                         =     0.0;
                unsigned long           last_swap_duration                                                  =     0;
                int                     last_gl_current_prg = -1;  // Store last active shader program

                int                     current_buffer = 0;

                int                     gl_current_prg                                                      =     0; // display_debug value
                int                     gl_current_tex                                                      =     0;
                int                     linked_programs                                                     =     0;
                int                     audio_source_channel                                                =    -1;                // Tracks which channel is audio source (-1 if none)

                int                     active_bpm_channel                                                  =     0;
                int                     g_menu_mode_new                                                     =     0;                        // Current menu mode (0, 1, or 2)
                int                     g_menu_mode_old                                                     =     0;
                int                     g_current_texture                                                   =     0;
                int                     attenuation                                                         =     2;
                unsigned long           start_time_fps_calculation                                          =     0;
                unsigned long           end_time_fps_calculation                                            =     0;
                unsigned long           total_time_fps_calculation                                          =     0;
// for the fps breack mechanism                
                unsigned long           currentTime = 0;
                unsigned long           targetTime = 0;
                unsigned long           delayTime = 0;
                
                float                   util_audio_smooth_band                  [4]                         =  {0.0f};

                float                   GLtime                                                              =  0.0f;
                float                   opaque                                                              =  1.0f;
// for the log files of my classes
        const   char                    *VID__LOG_NAMES                          [VID_SD+VID_USB+4]                   = { "VIDLOG00.TXT",   "VIDLOG01.TXT",  "VIDLOG02.TXT",   "VIDLOG03.TXT", "VIDLOG04.TXT",   "VIDLOG05.TXT",  "VIDLOG06.TXT",   "VIDLOG07.TXT",  "VIDLOG08.TXT",   "VIDLOG09.TXT",   "VIDLOG10.TXT",   "VIDLOG11.TXT",   "VIDLOG12.TXT",   "VIDLOG13.TXT"    };
        const   char                    *BMP__LOG_NAMES                          [TEX_SD+TEX_USB+4]                   = { "BMPLOG00.TXT",   "BMPLOG01.TXT",  "BMPLOG02.TXT",   "BMPLOG03.TXT", "BMPLOG04.TXT",   "BMPLOG05.TXT",  "BMPLOG06.TXT",   "BMPLOG07.TXT",  "BMPLOG08.TXT",   "BMPLOG09.TXT",   "BMPLOG10.TXT",   "BMPLOG11.TXT",   "BMPLOG12.TXT",   "BMPLOG14.TXT"    };
// list of extensions used in my scanroot directory function
        const   char                    *vhsExtensions[VSH_VALID_SUFFIX_COUNT]={ "vsh" }; 
        const   char                    *fhsExtensions[FSH_VALID_SUFFIX_COUNT]={ "fsh" };
        const   char                    *texExtensions[TEX_VALID_SUFFIX_COUNT]={ "bmp" };
        const   char                    *vidExtensions[VID_VALID_SUFFIX_COUNT]={ "mp4", "264", "mov" }; // despite our efforts we only get raw h264 and even here its unclear if our vc_h264 _decoder uses the correct settings!
// array to store the scanned filenames?
                char                   *SCANED_FILES_VSH                         [40]                       = {   0    };
                char                   *SCANED_FILES_FSH                         [40]                       = {   0    };
                char                   *SCANED_FILES_TEX                         [40]                       = {   0    };
                char                   *SCANED_FILES_VID                         [40]                       = {   0    };
// array to store the length of the loased files
                unsigned                VSH_LOADED_BYTES                        [VSH_SD + VSH_USB]          = {   0    };
                unsigned                FSH_LOADED_BYTES                        [FSH_SD + FSH_USB]          = {   0    };
                unsigned                TEX_LOADED_BYTES                        [TEX_SD + TEX_USB+4]          = {   0    };
                unsigned                VID_LOADED_BYTES                        [VID_SD + VID_USB]          = {   0    }; 
// multipliers for the lfo´s */
                int                     multiplier                              [7]                         = {   64, 32, 16, 8, 4, 2, 1 };
// variables for the LFO calculations
                unsigned long           last_circle_buffer                      [LFO_INSTANCES]             = {   0    };
                unsigned long           next_circle_buffer                      [LFO_INSTANCES]             = {   0    };
                float                   LFO_float_output                        [LFO_INSTANCES]             = {   0    };
                int                     LFO_int_output                          [LFO_INSTANCES]             = {   0    };
                int                     last_multiplier                         [LFO_INSTANCES]             = {   0    };
// arrays for the ADC input and output values
                int                   adc_raw_value                             [ADC_CHANNELS]              = { 0 };
                int                   adc_int_value                             [ADC_CHANNELS]              = { 0 };
                float                 adc_float_value                           [ADC_CHANNELS]              = { 0 };
                int                   output_int_value                          [ADC_CHANNELS]              = { 0 };
                float                 output_float_value                        [ADC_CHANNELS]              = { 0 };                    // Array of output values per channel
                int                   random_int_value                          [ADC_CHANNELS]              = { 0 };
                float                 random_float_value                        [ADC_CHANNELS]              = { 0 };                    // Array of u_seed_a values per channel
//
                int                   g_input_mode                              [ADC_CHANNELS]              = { 0 };             // Array storing operation mode for each channel
                bool                  m_shaderStatusFlags                       [FSH_SD+FSH_USB]; 
                int                   menu_pickup_buffer                        [ADC_CHANNELS+ADC_CHANNELS]              = { 0 };               // Array storing previous pot values for pickup detection
                unsigned long         current_time_for_bpm                                                  = 0;                      // Array storing timing information per channel
                bool                  menu_pickup_flag                          [ADC_CHANNELS+ADC_CHANNELS]              = { 0 };                 // Array tracking if pot value has been "picked up"
                bool                  input_threshold_flag                      [ADC_CHANNELS]              = { 0 };             // Array tracking threshold state per channel
// BPM detection variables
                unsigned long         next_beat_time                            [2]                         = { 0 };                     // Next beat timestamp for BPM mode
                unsigned long         buffer_interval                           [2]                         = { 0 };
                unsigned long         last_valid_bpm_calculation                [2]                         = { 0 };
                unsigned long         last_valid_bpm_buffer                     [2]                         = { 0 };
                unsigned long         resultBPM                                 [2]                         = { 0 };
                unsigned long         intervalBuffer                            [2][3]                      = { 0 };
// Threshold values for input processing
                int                   threshold_high                            [ADC_CHANNELS]              = {   320,  320,  320,  320,  320,  320,  320,  320 };                    // Array of upper threshold values per channel
                int                   threshold_low                             [ADC_CHANNELS]              = {   128,  128,  128,  128,  128,  128,  128,  128 };                     // Array of lower threshold values per channel
// Arrays for LED color values - maybe we externalize this into an additional file
                unsigned char          led_col_new                               [20][3]                     = { 0 };


// array for the update mechanism in filesystem.cpp
unsigned long     loaded_bytes_kernel[2] = { 0 };                                                                                                               
// variables for the framerate break mechanism/calculation/ lfo timing        
        unsigned long elapsedMicroseconds[LFO_INSTANCES];
        unsigned long cycleLength[LFO_INSTANCES];
        int sampleIndex[LFO_INSTANCES];
// LFO mode settings for each channel (0 = off, 1 = sine, 2 = triangle, 3 = square)
        int LFO_mode[LFO_PARAMETERS] = { 0,3,3,3 };

        int sensitivity_new = 0;
        int sensitivity_old = 0;


         int mode_storage_buffers[16][33] = {0};               // array to store the modes and specials for all loaded fshaders?
         int shader_has_stored_params[33] = { 0 };
         int audio_sample[2] = { 0,0};

         CString h264_info;  // <- needs to be global, no return of this function!


         int m_validTextureCount=0;
/*
new menu_input_mode

[8 ( for adc inputs ) + 4 ( LFO ) + 4 ( extra mode flags )][progs on sd + progs on usb]
 menu_input_mode        LFO_mode    mode_storage_buffers[FRM_MODE][current_buffer],mode_storage_buffers[TEX_MODE][current_buffer],  adc_attenuation    

eventual also the variable buffers ***_old???
*/