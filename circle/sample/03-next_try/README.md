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

                        -   buttonPing                 (int btn_id)                            one of the 3 versions 

                        /   consume_button              (int btn_id, int &var)                  is mute / demo / concept

color_table.cpp         -   contains the color table for the ws2812 leds

filesystem.cpp                      

                        -   openFile        (   const char *p_fileName)
 
                        -   loadFile        (   char *buffer, 
                                                            unsigned bufferSize, 
                                                            int mode)

                        -   closeFile       ()
 
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

                        -   IsValidFile  (   const char* p_fileName, 
                                                            const char* p_fileExtension)

                        -   scanRoot      (   char** fileArray, 
                                                            const char* p_fileExtension[], 
                                                            int p_extentionCount, 
                                                            unsigned p_maxFiles )

                        -   updateUSB       (   const char* deviceType)

                        -   removeUSB       (   CDevice *pDevice, 
                                                            void *p_pContext)

filesystem_user.cpp      

                        -   checkUpdate       ()

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

                        -   initVshaders           (   glsl_states *m_glsl, 
                                                            int p_fromFile, 
                                                            int p_toFile) 

                        -   initOshader    (   glsl_states *m_glsl )               *   NEW

                        -   initFshaders           (   glsl_states *m_glsl, 
                                                            int p_fromFile, 
                                                            int p_toFile) 

                        -   initOprogram    (   glsl_states *m_glsl)                *   NEW

                        -   initFprograms           (   glsl_states *m_glsl, 
                                                            int p_fromFile, 
                                                            int p_toFile)       

                        -   initOuniforms   (   glsl_states *m_glsl)                *   NEW

                        -   initFuniforms           (   glsl_states *m_glsl, 
                                                            int p_fromFile, 
                                                            int p_toFile)

                        -   initOtexture    (   glsl_states *m_glsl)                *   NEW

                        -   initUtextures           (   glsl_states *m_glsl, 
                                                            int p_fromFile, 
                                                            int p_toFile)       

                        -   initVbuffer           (   glsl_states *m_glsl) 

                        -   gfx_render_shader_a         (   glsl_states *m_glsl)

                        /   we need the "render the overlay display fragment shader"!!!         *   look up circle/sample/03-next_try/temp/the functions.cpp!!!

                        -   gfx_render_shader_b         (   glsl_states* m_glsl)


gfx_OGL.cpp                         

                        -   gfx_init_OGL                (   glsl_states *m_glsl)

gfx_debug.cpp                       

                        -   shaderLog              (   GLint shader, 
                                                            int shaderIndex)

                        -   programLog             (   GLint shader, 
                                                            int program_index) 

                        -   gfx_check                   (   const char *file, 
                                                            unsigned line)

global.cpp                          

                        - contains the global variables BUT i decided to "re-migrate" them into the kernel.h file ( where there where before )

io.cpp                              
                            
                        /   io_init_pickup_buffer       ()

                        -   readADC                 () 

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
                        -   wrapperMemoryAllocate             ()                                      * we add here the new buffers for the overlay menu code

                        -   wrapperMemoryCleanUp             ()                                      *
!!!!!
                        -   initMEMbuffer          (   size_t count, 
                                                            size_t bufferSize) 

                        -   initDMAbuffer      (   size_t count, 
                                                            size_t bufferSize,
                                                            char** blockBaseOut,
                                                            char** rawBlockOut,
                                                            size_t* alignedSizeOut)

                        -   clearMEMbuffer         (   char** buffers, 
                                                            size_t count) 

                        -   clearDMAbuffer     (   char** buffers, 
                                                            char* rawBlock)

                                    
menu.fsh                *   this is the new shader for my overly texture atlas driven menu!!   
                                    
menu_final.cpp         we created a new clean menu functions, separate into different smaller functions 

                        +   resetMenuPickupFlags     ()                                      *

                        +   modeMenuAssignGroup      (   uint8_t menu_id,                    *
                                                            uint8_t base)

                        +   applyModeToChannel       (   int channel)                        *

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

                        -   prepParameters        ()

                        -   chooseProgram         ()                                      

                        -   chooseTexture         ()

                        -   storeModes          () 

                        -   randomVec8            (   uint32_t p_seed)           

                        -   calculateBPM          (   unsigned long p_triggerTimeClockA, 
                                                            unsigned long p_triggerTimeClockB) 

                        -   util_determine_bpm_source   ()

                        -   predictedNextBeat  ()

                        -   sampleWaveTable                    ()

                        ?   audioEnergy           (   float p_adcvalue)                     <-- the old audio engine - i MUST FIND THE REWORK!!!

wavetable.cpp                       -   contains the wave tables for the lfo´s 


there are still functions missing here, whole files too!

menu_led.cpp                        *   concerned with the ws2812 leds

okay, we should start to to name variables i a REALLY clean and deterministic way,
like prefixes for them g_for global, p_ for parameter ( in functions ), also 
