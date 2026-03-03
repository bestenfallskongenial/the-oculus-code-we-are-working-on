Makefile                                                                                                            // ***** !!!!! *****


README.md                   


bufferToScreen.cpp          buffer_to_screen_plot                           (   unsigned        x,                  // !!! not tested !!!
                                                                                unsigned        y, 
                                                                                u32 color )
                            buffer_to_screen_draw_char                      (   char            ch,
                                                                                unsigned        charCol,
                                                                                unsigned        charRow,
                                                                                u32             fgColor,
                                                                                u32             bgColor )
                            buffer_to_screen_init                           (   void )
                            buffer_to_screen_clear_screen                   (   u32 bgColor)
                            buffer_to_screen_draw_buffer_segment_to_screen  (   const char     *pSourceBuffer,
                                                                                u32             startIndex,
                                                                                u32             endIndex,
                                                                                unsigned        startCol,
                                                                                unsigned        startRow,
                                                                                u32             fgColor,
                                                                                u32             bgColor )
                            buffer_to_screen_get_grid                       (   unsigned       &cols, 
                                                                                unsigned       &rows )

color_table.cpp             const unsigned char g_rgb_color_table[49][3]                                            // ***** !!!!! *****


features.cpp                randomVec8                                      (   uint32_t        p_seed )
                            calculateBPM                                    (   unsigned long   p_triggerTimeClockA, 
                                                                                unsigned long   p_triggerTimeClockB )
                            predictedNextBeat                               ()
                            sampleWaveTable                                 ()
                            audioEnergy                                     (   float           p_adcvalue    ) // shall i pass the channel instead?

filesystem.cpp              Mount                                           (   const char*     p_deviceName )
                            UnMount                                         ()
                            openFile                                        (   const char     *p_fileName )
                            loadFile                                        (   char           *p_buffer,       // destination buffer for the file data
                                                                                unsigned        p_bufferSize )
                            saveBuffer                                      (   const char*     p_fileName,
                                                                                const char*     p_buffer,
                                                                                unsigned        p_bufferSize )
                            closeFile                                       ()
                            bulkLoad                                        (   char*           p_fileNameArray[],  // where we have stored the filenames from the root directory scan
                                                                                unsigned        p_loadedBytes[],    // where we store the size in bytes for each file 
                                                                                char**          p_bufferArray,      // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                                int             p_maxFiles,         // how many files we are allowed to process ( os limitations )
                                                                                int            &p_validFiles,       // counts successful loads <- directly modified in the function, we dont need to return it
                                                                                unsigned        p_fileSize )        // maximum size for each file
                            IsValidFile(                                        const char*     pFileName,
                                                                                const char*     extension )
                            scanRoot                                        (   char**          p_fileNameArray,    // where we store the valid filenames we find
                                                                                const char*     p_fileExtension[],  // the array of valid file extensions for this type of file
                                                                                int             p_extentionCount,   // how many valid file extensions we have in the array above
                                                                                int            &p_scannedFiles,     // our counter of found files, important <- has to be initialised with 0
                                                                                unsigned        p_maxFiles )  
                            updateUSB                                       (   const char*     p_deviceName )
                            removeUSB                                       (   CDevice        *f_partitionName,    // we could also unmount the filesystem here if we wanted to be extra safe,right?
                                                                                void           *p_pContext )

filesystem_user.cpp         util_save_modes_file                            ()                                      // this is not wanted/needed anymore


gfx_OGL.cpp                 gfx_init_OGL                                    (   glsl_states    *m_glsl )


gfx_debug.cpp               shaderLog                                       (   GLint           shader, 
                                                                                int             shaderIndex )
                            programLog                                      (   GLint           shader, 
                                                                                int             program_index )
                            gfx_check                                       (   const char*     file,
                                                                                unsigned        line )


gfx_init.cpp                initVshaders                                    (   glsl_states    *m_glsl,             // ***** !!!!! *****
                                                                                int             p_fromFile, 
                                                                                int             p_toFile)
                            initOshader                                     (   glsl_states    *m_glsl )
                            initFshaders                                    (   glsl_states    *m_glsl, 
                                                                                int             p_fromFile, 
                                                                                int             p_toFile) 
                            initOprogram                                    (   glsl_states    *m_glsl)             // ***** !!!!! *****
                            initFprograms                                   (   glsl_states    *m_glsl, 
                                                                                int             p_fromFile, 
                                                                                int             p_toFile)  
                            initOuniforms                                   (   glsl_states    *m_glsl)             // ***** !!!!! *****
                            initFuniforms                                   (   glsl_states    *m_glsl, 
                                                                                int             p_fromFile, 
                                                                                int             p_toFile) 
                            initOtexture                                    (   glsl_states     *m_glsl)            // ***** !!!!! *****
                            initUtextures                                   (   glsl_states    *m_glsl, 
                                                                                int             p_fromFile, 
                                                                                int             p_toFile)  
                            initVbuffer                                     (   glsl_states    *m_glsl) 

gfx_render.cpp              gfx_render_shader_a                             (   glsl_states    *m_glsl)             // ***** !!!!! *****
                            gfx_render_shader_b                             (   glsl_states    *m_glsl)

helpers.cpp                 checkUpdate                                     ()                                      // ***** !!!!! *****


kernel.cpp                  CKernel::CKernel                                (   void )                              // ***** !!!!! *****
                            CKernel::~CKernel                               (   void )


kernel.h                                                                                                            // ***** !!!!! *****


kernel_initialize.cpp       Initialize                                      (   void )                              // ***** !!!!! *****


kernel_run.cpp              Run                                             (   void )                              // ***** !!!!! *****


memory.cpp                  wrapperDMAallocate                              ()
                            wrapperMEMallocate                              ()
                            wrapperDMAcleanUp                               ()
                            wrapperMEMcleanUp                               ()
                            initMEMbuffer                                   (   size_t          count,              // ***** !!!!! *****
                                                                                size_t          bufferSize ) 
                            initDMAbuffer                                   (   size_t          count,              // ***** !!!!! *****
                                                                                size_t          bufferSize,
                                                                                char**          blockBaseOut,
                                                                                char**          rawBlockOut,
                                                                                size_t*         alignedSizeOut )
                            clearMEMbuffer                                  (   char**          buffers, 
                                                                                size_t          count ) 
                            clearDMAbuffer                                  (   char**          buffers, 
                                                                                char*           rawBlock )


menu.fsh                    GLSL for my overlay texture atlas menu


menu_final.cpp              resetMenuPickupFlags                            ()                                      // ***** !!!!! *****
                            modeMenuAssignGroup                             (   uint8_t         menu_id, 
                                                                                uint8_t         base)
                            applyModeToChannel                              (   int             channel)            // ***** !!!!! *****
                            modeADC                                         (   int             channel) 
                            modeTRG                                         (   int             channel)
                            modeBPM                                         (   int             channel)
                            modeLF1                                         (   int             channel)
                            modeLF2                                         (   int             channel)


util.cpp                    prepParameters                                  ()
                            chooseProgram                                   (   int             p_channel )
                            chooseTexture                                   (   int             p_channel )
                            chooseVideo                                     (   int             p_channel )
                            storeModes                                      ()                                      // ***** !!!!! *****
                            readADC                                         () 
                            buttonPing                                      (   int             p_btn_id,           // ***** !!!!! *****
                                                                                int             pin )
                            storeLog                                        (   char*           buffer, 
                                                                                u32&            index,
                                                                                const char*     label,
                                                                                u32             value1, 
                                                                                u32             value2,
                                                                                u32             value3, 
                                                                                u32             value4 )
                            storeMsg                                        (   char*           buffer,
                                                                                u32&            index,
                                                                                const char*     label,
                                                                                const void*     tx_msg,
                                                                                u32             total_size)
                            nextline                                        (   char*           buffer,
                                                                                u32&            index )

wavetable.cpp               unsigned long g_waveTable[WAVEFORMS][WAVESAMPLES] 


wrappers.cpp                wrapper_from_sd                                 ()
                            wrapper_load_usb                                ()
                            wrapper_init_gl_sd                              ()
                            wrapper_init_gl_usb                             ()
                            wrapper_io                                      ()
                            wrapper_modes                                   ()

okay, here now the plan - we need to connect the loose ends
- there was the plan to get rid of CString ( #include <circle/string.h> ) screen ( #include <circle/screen.h>) and the logger ( #include <circle/logger.h> )
- to replace it with a mechanism that logs into a buffer and either write it to a file or show it on screen
new functions here are:

saveBuffer() and buffer_to_screen_*()

- to integrate a texture atlas driven menu running as a second pass fragment shader:

menu.fsh
initOshader()
initOprogram()
initOuniforms()
initOtexture() 

with an independent set of uniforms and parameters, look at the file circle/sample/04-oculus_close_and_closer/the functions.cpp 

driven by an unified array g_centralModeBuffer[][] ( the old mode_storage_buffers ) where we store all variables/modes/settings for each channel

- i also combined the input/mode/output values into a single matrix inOutMatrixInt[][] and inOutMatrixFlt[][]

and i want to do it with the timing related arrays from circle/sample/01-oculus_mendax/global.h too, like in features.cpp

also do i create wrappers for the different steps inside of the CKernel::Run() to organise the steps load - init and io - apply modes - render  better
i have also restructure some of the functions ( i guess i can do more in filesystem.cpp and init_gfx.cpp that i ay not need to pass as much parameters )

as said in  need to get this all together, means for example create the propper kernel.h - arrays variables etc, check for consistency 
create the logic how to navigate the menus circle/sample/04-oculus_close_and_closer/menu_final.cpp - mainly menu_id and buttonPing

the idea is to have my two buttons as followed ( important note here! when no button is pressed the physical input goes into the adc, 
    
is a button pressed long the four pots are routed into the first four adc channels, look at circle/sample/01-oculus_mendax/io.cpp how i did it in the past ):

on single tab event on the upper button is counted as potential tab bpm, means we need to store it,
a hold opens up "menu layer A" ( upper button ) and "menu layer B" ( lower button ) means modeMenuAssignGroup() will map the adc to this group of parameters ( for A and B its the modes )
hold the lower button and single press the upper one will circle through additional menu layers like lfo settings, 
attenuation and sensitivity settings, filesystem operations ( i need a flag base mechanism for load setup data or initialise the firmware update here )

the loop pipeline is something like

read adc -> 
