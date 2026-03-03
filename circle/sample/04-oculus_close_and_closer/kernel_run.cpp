#include kernel.h

TShutdownMode   CKernel::Run(void)
{
                prepParameters();                                                     // we prep the parameters in the system arrays

                wrapper_load_sd();                                                     // we than load the files from sd

                wrapper_init_gl_sd();                                                     // we than init the glsl system and parse the media files from sd
/*
                CString bufferVCSM = m_SharedMemory.m_DebugCharArray;                       // we now use storeBuffer() instead of all CString operations!
                CString bufferMMAL =  m_H264Decoder.m_DebugCharArray;
*/
                filesystem_save_log_file( "emmc1-1", FILENAME_VCSM_LOG, bufferVCSM);        // this is for debug purposes because vc4 is a blackbox we are reverse-engineering, so we need to have a log of the vcsm calls and the mmal calls to see if we can find out what is causing the crash after ~ 22 minutes of runtime, because the theory is that there is an memory violation in the vcsm or mmal code that causes the crash, so we need to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash, and if we can find out what is causing the crash, we can try to fix it or work around it, but for now we just want to have a log of the vcsm and mmal calls to see if we can find out what is causing the crash
                filesystem_save_log_file( "emmc1-1", FILENAME_MMAL_LOG, bufferMMAL);

                while (m_resetFlag == false)                                                // the actual mainloop - we loop while the reset flag is false
                    {
                    if(updateUSB("umsd1") == true && m_USBhasLoadOnes == false) // usb attach detection - do we need the flag when we manage to a) clean up the gl system and the filesystem properly ( soft reset )
                        {
                        wrapper_init_usb();                                                 // if true we run the load from usb function
                        wrapper_init_gl_usb();                                              // we than init the glsl system and parse the media files from sd

                        filesystem_save_log_file( "umsd1-1", FILENAME_GL_LOG, g_log_string);
                        }
                    start_time_fps_calculation = m_Timer.GetClockTicks();                   // for the "fps break" - shall we make this our main timestamp?

                    wrapper_io();
                    wrapper_modes();
                    wrapper_render();
                    }
}

