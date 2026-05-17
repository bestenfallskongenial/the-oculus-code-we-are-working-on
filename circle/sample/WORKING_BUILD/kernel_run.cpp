#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{

unsigned r = 0;
unsigned g = 127;
unsigned b = 186;
/*
                g_currentTime = m_Timer.GetClockTicks();    
 
                randomVec8              ( g_currentTime );

                saveFromBufferM         (   PARTITION_NAME_SD,
                                            gen83FileName("TXT"),
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
                                                    gen83FileName("TXT"),
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
                        wrapper_init_gl_sd();

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
                        
                        wrapper_init_gl_usb();
/*                       
                        bufferScreenDraw(   "load from usb",
                                                    0,
                                                    sizeof("load from usb"),
                                                    0,
                                                    0,
                                                    0xFFFFFFFF );
*/
                    //  randomVec8              ( g_currentTime );            

                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    gen83FileName("TXT"),
                                                    m_logBuffer,
                                                    m_logBufferIndex );

                      //msDelay(1000);    
                      //bufferScreenClear();
                        m_USB_has_load = true;   
                        bufferScreenDraw( "we are done here", 0, sizeof("we are done here"), 0, 13, 0xFFFFFFFF );
                        }

                        WS2812_SetLED(0, r, g, b);
                        WS2812_SetLED(1, b, r, g);
                        WS2812_SetLED(2, g, b, r);
                        WS2812_SetLED(3, (r*2)%255, g, b);

                        WS2812_Update();
                        
                        r = (r + 1) % 256;
                        g = (g + 2) % 256;
                        b = (b + 3) % 256;

                        buttonPing(0, SW_PIN_A);
                        buttonPing(1, SW_PIN_B);

                        logButtonStatesRuntime();

                        randomVec8(g_currentTime);

                        readAndConvertADC();
                        adc_AdvanceIndex();

                        logInOutRuntime();

                        msDelay(25);

                        if ( checkUpdate() == true ) bufferScreenDraw( "update present", 0, sizeof("update present"), 0, 20, 0xFFFFFFFF );
                    }

                return ShutdownHalt;
}
