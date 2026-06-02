#include "kernel.h"

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex   

TShutdownMode CKernel::Run(void)
{
                unsigned r = 0;
                unsigned g = 127;
                unsigned b = 186;

                g_inOutMatrixFlt[0][OUT] = 0.5f;
                g_inOutMatrixFlt[1][OUT] = 0.5f;
                g_inOutMatrixFlt[2][OUT] = 0.5f;
                g_inOutMatrixFlt[3][OUT] = 0.5f;

                g_inOutMatrixFlt[4][OUT] = 0.5f;
                g_inOutMatrixFlt[5][OUT] = 0.5f;
                g_inOutMatrixFlt[6][OUT] = 0.5f;
                g_inOutMatrixFlt[7][OUT] = 0.5f;

                while (/*m_resetFlag == false*/ 1)
                    {
                    g_currentTime = m_Timer.GetClockTicks(); 

                    if (!m_SD_has_load) 
                        {
                        wrapper_from_sd();
                        wrapper_parser_a();
                        wrapper_init_gl_sd();

                        m_SD_has_load = true;
                        }

                    if (updateUSB("umsd1") == true && m_USB_has_load == false)
                        {
                        wrapper_load_usb();   

                        wrapper_parser_b();
                        wrapper_init_gl_usb();
       
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                  /*gen83FileName("TXT"*/
                                                    "bootlog.txt",
                                                    m_logBuffer,            // stores the pre-init buffer
                                                    m_logBufferIndex );

                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "GLSL.txt",
                                                    m_bufferLog[1],
                                                    m_bufferLogIndex[1] );

                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "parser.txt",
                                                    m_bufferLog[0],
                                                    m_bufferLogIndex[0] );

                                                    msDelay(100);
                        saveFromBufferM         (   PARTITION_NAME_SD,
                                                    "vc04.txt",
                                                    m_bufferLog[2],
                                                    m_bufferLogIndex[2] );

                        m_USB_has_load = true;   

                        bufferScreenDraw( "we are done here", 0, sizeof("we are done here"), 0, 20, 0xFFFFFFFF );
                        }
        if ( m_SD_has_load && m_USB_has_load == true ) 
        {
                            m_logBufferIndex = 0;

                            bufferScreenClear();
                            readAndConvertADC();
                            adc_AdvanceIndex();
                            ADCDebug();

                            g_inOutMatrixFlt[0][OUT] = g_inOutMatrixFlt[0][VAL];
                            g_inOutMatrixFlt[1][OUT] = g_inOutMatrixFlt[1][VAL];
                            g_inOutMatrixFlt[2][OUT] = g_inOutMatrixFlt[2][VAL];
                            g_inOutMatrixFlt[3][OUT] = g_inOutMatrixFlt[3][VAL];

                            g_inOutMatrixFlt[4][OUT] = g_inOutMatrixFlt[4][VAL];
                            g_inOutMatrixFlt[5][OUT] = g_inOutMatrixFlt[5][VAL];
                            g_inOutMatrixFlt[6][OUT] = g_inOutMatrixFlt[6][VAL];
                            g_inOutMatrixFlt[7][OUT] = g_inOutMatrixFlt[7][VAL];
                        //  logInOutRuntime();
        }
                    WS2812_SetLED(0, r, g, b);
                    WS2812_SetLED(1, b, r, g);
                    WS2812_SetLED(2, g, b, r);
                    WS2812_SetLED(3, (r*2)%255, g, b);

                    WS2812_Update();
                    
                    r = (r + 1) % 256;
                    g = (g + 2) % 256;
                    b = (b + 3) % 256;

                //  buttonPing(0, SW_PIN_A);
                //  buttonPing(1, SW_PIN_B);

                //  logButtonStatesRuntime();

                    randomVec8(g_currentTime);

                    g_current_gl_program = 0;

                    g_centralModeBuffer[g_current_gl_program][FRM_MODE] = 0;

                    get_gl_time( m_Timer.GetClockTicks() );

                    frmBufferSet(&m_vtx);

                    setUniPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            0);

                    drawGLsPrg();

                    frmRateBreak(false);

                    frmBufferSwap(&m_ogl);

                    msDelay(25);

                //  if ( checkUpdate() == true ) bufferScreenDraw( "update present", 0, sizeof("update present"), 0, 21, 0xFFFFFFFF );
                    }

                return ShutdownHalt;
}
