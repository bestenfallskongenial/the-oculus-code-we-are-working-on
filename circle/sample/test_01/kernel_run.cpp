#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{
                uint32_t    start_time_fps_calculation = m_Timer.GetClockTicks();   
//  we generate 8 rnd vaole 
                randomVec8              ( start_time_fps_calculation );
//  to feed the filename generator and store the bootstrap log to sd
                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );
                msDelay(1000);
/*
                bufferToScreenDrawBuffer(   "save buffer success",
                                            0,
                                            sizeof("save buffer success"),
                                            0,
                                            0,
                                            0xFFFFFFFF );
                                            msDelay(1000); 

                bufferToScreenDrawBuffer(   m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            0,
                                            0xFFFFFFFF );

                msDelay(1000); 
*/
                while (m_resetFlag == false)
                    {
                    start_time_fps_calculation = m_Timer.GetClockTicks(); 

                    if (!m_SD_has_load) 
                        {
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        wrapper_from_sd();

                        randomVec8              ( start_time_fps_calculation );            

                        wrapper_init_gl_sd();

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );

                        msDelay(1000);

                        m_SD_has_load = true;
                        }

                    start_time_fps_calculation = m_Timer.GetClockTicks(); 
                
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)
                        {
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0'; 

                        wrapper_load_usb();                    

                        randomVec8              ( start_time_fps_calculation );            

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );
                        msDelay(1000);    

                        m_USB_has_load = true;                            
                        }

                    }

                return ShutdownHalt;
}
