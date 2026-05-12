#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{
                uint32_t    start_time_fps_calculation = m_Timer.GetClockTicks();   

                randomVec8              ( start_time_fps_calculation );

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );
                m_Timer.MsDelay(1000);
/*
                bufferToScreenDrawBuffer(   "save buffer success",
                                            0,
                                            sizeof("save buffer success"),
                                            0,
                                            0,
                                            0xFFFFFFFF );
                                            m_Timer.MsDelay(1000); 

                bufferToScreenDrawBuffer(   m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            0,
                                            0xFFFFFFFF );

                m_Timer.MsDelay(1000); 
*/
                while (m_m_resetFlag == false)
                    {
                    start_time_fps_calculation = m_Timer.GetClockTicks(); 

                    if (!m_SD_has_load) 
                        {
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        wrapper_from_sd();

                        randomVec8              ( start_time_fps_calculation );            

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );
                        m_Timer.MsDelay(1000);

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
                        m_Timer.MsDelay(1000);    

                        m_USB_has_load = true;                            
                        }

                    }

                return ShutdownHalt;
}
