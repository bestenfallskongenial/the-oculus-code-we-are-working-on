#include "kernel.h"

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex   

TShutdownMode CKernel::Run(void)
{
  
                unsigned r = 0;                                                     // start values for the led testing  
                unsigned g = 127;
                unsigned b = 186;

                g_inOutMatrixFlt[0][OUT] = 0.5f;                                    // start values for the glsl code
                g_inOutMatrixFlt[1][OUT] = 0.5f;
                g_inOutMatrixFlt[2][OUT] = 0.5f;
                g_inOutMatrixFlt[3][OUT] = 0.5f;

                g_inOutMatrixFlt[4][OUT] = 0.5f;
                g_inOutMatrixFlt[5][OUT] = 0.5f;
                g_inOutMatrixFlt[6][OUT] = 0.5f;
                g_inOutMatrixFlt[7][OUT] = 0.5f;

                g_centralModeBuffer[g_gl_program_current][FLAG_TEX] = 1;            // start values for the glsl code

                g_centralModeBuffer[g_gl_program_current][FLAG_TIME] = 0;           // start values for the glsl code

                while (/*m_resetFlag == false*/ 1)
                    {
                    if (!m_SD_has_load)                                             // first load block - get the system files from sd
                        {
                        wrapper_load_sd();
                        wrapper_parser_sd();
                        wrapper_init_gl_sd();

                        m_SD_has_load = true;
                        }
                    if (updateUSB("umsd1") == true && m_USB_has_load == false)      // second load block - get user files from usb AND there is a devide attached
                        {
                        wrapper_load_usb();   

                        wrapper_parser_usb();
                        wrapper_init_gl_usb();

                        m_USB_has_load = true;
                        }
                        
                    if( m_SD_has_load && m_USB_has_load )
                        {
                        m_logBufferIndex = 0;
                        bufferScreenClear();
                        } 

                    g_currentTime = m_Timer.GetClockTicks();                        // here starts the actual runtimeloop
                    
                    readAndConvertADC();
                    adc_ProcessAudio();
                    adc_AdvanceIndex();

                    randomVec8(g_currentTime);

                    buttonPingB( 0, SW_PIN_A, 1, SW_PIN_B );                        // 

                    setLayer(0,1);                                              //  here goes the button consumer and menu code

                    dispatchLayer();
                    

                //  getChannelModeA( 0 );
                //  getChannelModeA( 1 );                    

                //  applyTargetModes();

                //  updateLED();

                //  storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "menu layer", g_menuLayer );

                    logButtonStatesRuntime();

                    frmBufferSet(&m_vtx);                                           // this is the demo code just to see if rendering works

                    setUniPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            0);

                    setTexPrg(&m_ogl,
                            &m_fsh,
                            &m_tex,
                            0,
                            1);                            

                    drawGLsPrg();

                    frmRateBreak(false);

                    frmBufferSwap(&m_ogl); 

                    msDelay(25);
                    }
                return ShutdownHalt;                    
}