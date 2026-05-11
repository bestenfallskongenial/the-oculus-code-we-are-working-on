#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{
                uint32_t    start_time_fps_calculation = m_Timer.GetClockTicks();   

                randomVec8              ( start_time_fps_calculation );

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            /*make83FileName("TXT"),*/
                                            "loga.txt",
                                            m_logBuffer,
                                            m_logBufferIndex );
                m_Timer.MsDelay(1000);

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

m_logBufferIndex = 0;
m_logBuffer[0] = '\0';

                if (!m_SD_has_load) 
                    {
                    wrapper_from_sd();
                    m_SD_has_load = true;
                    }
                if (updateUSB("umsd1") == true && m_USB_has_load == false)
                    { 
                    wrapper_load_usb();
                    m_USB_has_load = true;                            
                    }

                bufferToScreenDrawBuffer(   "wrapper successful",
                                            0,
                                            sizeof("wrapper successful"),
                                            0,
                                            0,
                                            0xFFFFFFFF );
                m_Timer.MsDelay(1000); 

                            start_time_fps_calculation = m_Timer.GetClockTicks();     

                randomVec8              ( start_time_fps_calculation );            

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            /*make83FileName("TXT"),*/
                                            "logb.txt",
                                            m_logBuffer,
                                            m_logBufferIndex );
                m_Timer.MsDelay(1000);    

                bufferToScreenDrawBuffer(   "save buffer success",
                                            0,
                                            sizeof("save buffer success"),
                                            0,
                                            0,
                                            0xFFFFFFFF );
                m_Timer.MsDelay(1000); 
                
m_logBufferIndex = 0;
m_logBuffer[0] = '\0';

                bufferToScreenDrawBuffer(   m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            0,
                                            0xFFFFFFFF );
                                         
                while (m_resetFlag == false)
                    {
                    }

                return ShutdownHalt;
}
