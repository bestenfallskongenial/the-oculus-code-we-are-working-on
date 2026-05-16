#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{
/*
                g_currentTime = m_Timer.GetClockTicks();    
 
                randomVec8              ( g_currentTime );

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );
                msDelay(1000);
*/
                while (/*m_resetFlag == false*/ 1)
                    {

                    g_currentTime = m_Timer.GetClockTicks(); 

                //  bufferScreenClear();

                    if (!m_SD_has_load) 
                        {
/*                            
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';
*/
                        wrapper_from_sd();
/*
                        randomVec8              ( g_currentTime );            
                                                    
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );

                        bufferScreenDraw(   "load and log from/to sd",
                                                    0,
                                                    sizeof("load and log from/to sd"),
                                                    0,
                                                    0,
                                                    0xFFFFFFFF );

                        msDelay(1000);        
                        bufferScreenClear();  
*/
                        m_SD_has_load = true;
                        }
/*
                        if (m_bStorageAttached == true) bufferScreenDraw( "m_bStorageAttached set", 0, sizeof("m_bStorageAttached set"), 0, 20, 0xFFFFFFFF );    
                        msDelay(100);    
*/
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)
                        {
/*
                        bufferScreenDraw( "is this loop running forever", 0, sizeof("is this loop running forever"), 0, 21, 0xFFFFFFFF );    
                        msDelay(1000);    
                             
                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0'; 
*/
                        wrapper_load_usb();          
/*                       
                        bufferScreenDraw(   "load from usb",
                                                    0,
                                                    sizeof("load from usb"),
                                                    0,
                                                    0,
                                                    0xFFFFFFFF );
*/
                        randomVec8              ( g_currentTime );            

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    make83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );

                      //msDelay(1000);    
                      //bufferScreenClear();
                        m_USB_has_load = true;   
                                               
                        }

                    }

                return ShutdownHalt;
}
