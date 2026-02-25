here the attempt to create a clean and extended version of my oculus mendax code.
some changes :

clean up and separation of the menu.cpp means we need to rework the 
new integrated button code
addition of an overlay texture atlas based means we need to rework  
the filesystem, the global variables etc,
i also need to init a second parser instance not to confuse my code that much


FILENAME                |      FUNCTIONS / CONTENT                                              |  REMARKS DESCIPTION

Makefile

README.md               -   this file here

buttons.cpp                         

                        -   button_ping                 (int btn_id)                            one of the 3 versions 

                        /   consume_button              (int btn_id, int &var)                  is mute / demo / concept

color_table.cpp         -   contains the color table for the ws2812 leds

filesystem.cpp                      

                        -   filesystem_open_file        (   const char *p_fileName)
 
                        -   filesystem_load_file        (   char *buffer, 
                                                            unsigned bufferSize, 
                                                            int mode)

                        -   filesystem_close_file       ()
 
                        -   filesystem_process_files    (   char* p_fileNameArray[], 
                                                            unsigned p_loadedBytes[], 
                                                            char** p_bufferArray, 
                                                            int p_maxFiles, 
                                                            int p_validFiles, 
                                                            unsigned p_fileSize, 
                                                            int mode)
!!!!!
                        -   filesystem_mount            (   const char* p_deviceName,             OKAY, here is where the "magic" happens!!!
                                                            char* fileNamesVsh[],           
                                                            unsigned vStotalLoadedBytes[], 
                                                            int maxVshaderFiles,

                                                            char* fileNamesOmf[],          *   NEW
                                                            unsigned foverlayLoadedBytes[],     *   NEW
                                                            int maxFoverlayFiles,               *   NEW

                                                            char* fileNamesFsh[], 
                                                            unsigned fStotalLoadedBytes[], 
                                                            int maxFshaderFiles,

                                                            char* fileNamesOmt[],        *   NEW            
                                                            unsigned tXoverlayLoadedBytes[],    *   NEW
                                                            int maxTexOverlayFiles,             *   NEW

                                                            char* fileNamesTex[], 
                                                            unsigned tXtotalLoadedBytes[], 
                                                            int maxTextureFiles,

                                                            char* fileNamesVid[]  , 
                                                            unsigned vItotalLoadedBytes[], 
                                                            int maxVideoFiles)
!!!!!
                        -   filesystem_save_log_file    (   const char* p_deviceName, 
                                                            const char* p_fileName, 
                                                            const CString& p_str_to_save)

                        -   filesystem_IsValidFileType  (   const char* p_fileName, 
                                                            const char* p_fileExtension)

                        -   filesystem_ScanRootDir      (   char** fileArray, 
                                                            const char* p_fileExtension[], 
                                                            int p_extentionCount, 
                                                            unsigned p_maxFiles )

                        -   filesystem_update_USB       (   const char* deviceType)

                        -   filesystem_remove_USB       (   CDevice *pDevice, 
                                                            void *p_pContext)

filesystem_user.cpp      

                        -   util_check_for_update       ()

                        -   util_save_modes_file        ()

                        -   filesystem_load_kernel      (   const char* p_deviceName, 
                                                            const char* p_fileName, 
                                                            unsigned p_fileIndex)

                        -   filesystem_save_kernel      (   const char* p_deviceName, 
                                                            const char* p_fileName, 
                                                            unsigned p_fileIndex)

                        -   GenerateH264ParserInfo      (   int p_fileIndex)

                        -   GenerateBmpParserInfo       (   int p_fileIndex)

                        -   GenerateBmpOverlayInfo      (   int p_fileIndex)                     *   NEW

                        -   parser_h264                 (   int p_fromFile, 
                                                            int p_toFile)

                        -   parser_bmp                  (   int p_fromFile, 
                                                            int p_toFile)

                        -   parser_overlay_bmp          (   int p_fileIndex)                     *   NEW

gfx.cpp                                                                                         *   look up circle/sample/03-next_try/temp/the functions.cpp!!!

                        -   gfx_init_vshaders           (   CUBE_STATE_T *state, 
                                                            int p_fromFile, 
                                                            int p_toFile) 

                        -   gfx_init_overlay_fshader    (   CUBE_STATE_T *state )               *   NEW

                        -   gfx_init_fshaders           (   CUBE_STATE_T *state, 
                                                            int p_fromFile, 
                                                            int p_toFile) 

                        -   gfx_init_overlay_program    (   CUBE_STATE_T *state)                *   NEW

                        -   gfx_init_programs           (   CUBE_STATE_T *state, 
                                                            int p_fromFile, 
                                                            int p_toFile)       

                        -   gfx_init_overlay_uniforms   (   CUBE_STATE_T *state)                *   NEW

                        -   gfx_init_uniforms           (   CUBE_STATE_T *state, 
                                                            int p_fromFile, 
                                                            int p_toFile)

                        -   gfx_init_overlay_texture    (   CUBE_STATE_T *state)                *   NEW

                        -   gfx_init_textures           (   CUBE_STATE_T *state, 
                                                            int p_fromFile, 
                                                            int p_toFile)       

                        -   gfx_init_v_buffer           (   CUBE_STATE_T *state) 

                        -   gfx_render_shader_a         (   CUBE_STATE_T *state)

                        /   we need the "render the overlay display fragment shader"!!!         *   look up circle/sample/03-next_try/temp/the functions.cpp!!!

                        -   gfx_render_shader_b         (   CUBE_STATE_T* state)


gfx_OGL.cpp                         

                        -   gfx_init_OGL                (   CUBE_STATE_T *state)

gfx_debug.cpp                       

                        -   gfx_shader_log              (   GLint shader, 
                                                            int shaderIndex)

                        -   gfx_program_log             (   GLint shader, 
                                                            int program_index) 

                        -   gfx_check                   (   const char *file, 
                                                            unsigned line)

global.cpp                          

                        - contains the global variables BUT i decided to "re-migrate" them into the kernel.h file ( where there where before )

io.cpp                              
                            
                        /   io_init_pickup_buffer       ()

                        -   io_read_ADC                 () 

                        -   io_event_button_A           (   BUTTONS::TEvent Event, 
                                                            void *pParam)

                        -   io_event_button_B           (   BUTTONS::TEvent Event, 
                                                            void *pParam)

kernel.cpp                          

                        *   CKernel                     (void)

                        -   Initialize                  (void)

                        -   Run                         (void)
                                    
kernel.h                * 

memory.cpp                          
!!!!!
                        -   memory_allocate             ()                                      * we add here the new buffers for the overlay menu code

                        -   memory_clean_up             ()                                      *
!!!!!
                        -   memory_init_buffer          (   size_t count, 
                                                            size_t bufferSize) 

                        -   memory_init_dma_buffer      (   size_t count, 
                                                            size_t bufferSize,
                                                            char** blockBaseOut,
                                                            char** rawBlockOut,
                                                            size_t* alignedSizeOut)

                        -   memory_clear_buffer         (   char** buffers, 
                                                            size_t count) 

                        -   memory_clear_dma_buffer     (   char** buffers, 
                                                            char* rawBlock)

                                    
menu.fsh                *   this is the new shader for my overly texture atlas driven menu!!   
                                    
menu_final.cpp         we created a new clean menu functions, separate into different smaller functions 

                        +   menu_reset_pickup_flags     ()                                      *

                        +   menu_mode_assign_group      (   uint8_t menu_id,                    *
                                                            uint8_t base)

                        +   apply_mode_to_channel       (   int channel)                        *

                        *   here we need the function that maps the g_centralModeBuffer content depending on the button input and choosen menu layer

                        /   modeADC                     (   int channel)                        simple version

                        +   modeADC                     (   int channel)

                        +   modeTRG                     (   int channel)

                        +   modeBPM                     (   int channel) 

                        /   modePOT                     ()                                      not implemented jet

                        +   modeLF1                     ()

                        +   modeLF2                     ()

                        ?   frameVIDEO                                                          new potential modes

                        ?   indexVIDEO

                        ?   singleSHADER

                        ?   extTIME

                        ?   extCLK
                                    
util.cpp                            

                        -   util_prep_parameters        ()

                        -   util_choose_program         ()                                      

                        -   util_choose_texture         ()

                        -   util_store_program          () 

                        -   util_random_vec8            (   uint32_t p_seed)           

                        -   util_calculate_BPM          (   unsigned long p_triggerTimeClockA, 
                                                            unsigned long p_triggerTimeClockB) 

                        -   util_determine_bpm_source   ()

                        -   util_update_predicted_beat  ()

                        -   util_LFO                    ()

                        ?   util_audio_energy           (   float p_adcvalue)                     <-- the old audio engine - i MUST FIND THE REWORK!!!

wavetable.cpp                       -   contains the wave tables for the lfo´s 


there are still functions missing here, whole files too!

menu_led.cpp                        *   concerned with the ws2812 leds

okay, we should start to to name variables i a REALLY clean and deterministic way,
like prefixes for them g_for global, p_ for parameter ( in functions ), also 
