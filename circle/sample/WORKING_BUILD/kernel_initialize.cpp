#include "kernel.h"

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 0, "m_Interrupt.Initialize()                           DONE");
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 1, "m_Timer.Initialize()                               DONE");
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 2, "CLogger::SetRawSink(CKernel::LoggerSink, this)     DONE");
                    bOK = m_Logger.Initialize(0);
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 3, "m_Logger.Initialize(0)                             DONE");
                    }


                if (bOK)
                    {
                    bOK = frameBufferInit();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 4, "frameBufferInit()                                  DONE");
                  //bufferScreenDraw( "frameBufferInit() done", 0, sizeof("frameBufferInit() done"), 0, 4, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = startupScreen();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 5, "System Info Log                                    DONE");
                  //msDelay(1000);
                    }                

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 6, "m_EMMC.Initialize()                                DONE");
                  //bufferScreenDraw( "m_EMMC.Initialize() done", 0, sizeof("m_EMMC.Initialize() done"), 0, 5, 0xFFFFFFFF );
                  //msDelay(1000); 
                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 7, "m_USBHCI.Initialize()                              DONE");
                  //bufferScreenDraw( "m_USBHCI.Initialize() done", 0, sizeof("m_USBHCI.Initialize() done"), 0, 6, 0xFFFFFFFF );
                  //msDelay(1000); 
                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 8, "m_USBHCI.UpdatePlugAndPlay()                       DONE");
                  //bufferScreenDraw( "m_USBHCI.UpdatePlugAndPlay() done", 0, sizeof("m_USBHCI.UpdatePlugAndPlay() done"), 0, 7, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
                    storeLog( m_logBuffer, m_logBufferIndex, "step", 9, "InitDMA Buffer                                     DONE");
                  //bufferScreenDraw( "InitDMA Buffer done", 0, sizeof("InitDMA Buffer done"), 0, 8, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",10, "InitMEM Buffer                                     DONE");
                  //bufferScreenDraw( "InitMEM Buffer done", 0, sizeof("InitMEM Buffer done"), 0, 9, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();       // <--- breaking point !! but why?!
                    storeLog( m_logBuffer, m_logBufferIndex, "step",11, "m_VCHIQ.Initialize()                               DONE");
                  //bufferScreenDraw( "m_VCHIQ.Initialize() done", 0, sizeof("m_VCHIQ.Initialize() done"), 0, 10, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                    if (bOK)
                    {
                            bcm_host_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",12, "bcm_host_init()                                    DONE");
                  //bufferScreenDraw( "bcm_host_init() done", 0, sizeof("bcm_host_init() done"), 0, 11, 0xFFFFFFFF );
                  //msDelay(1000);
                    }    

                    if (bOK)
                    {
                            initOGL(&m_ogl);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",13, "initOGL(&m_ogl)                                    DONE");
                  //bufferScreenDraw( "initOGL(&m_ogl) done", 0, sizeof("initOGL(&m_ogl) done"), 0, 12, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SPI_init();
                  storeLog( m_logBuffer, m_logBufferIndex, "step",14, "SPI_init()                                         DONE");
                //bufferScreenDraw( "SPI_init() done", 0, sizeof("SPI_init() done"), 0, 13, 0xFFFFFFFF );
                 //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);        // example GPIO18, choose your actual WS2812 SMI GPIO
                    storeLog( m_logBuffer, m_logBufferIndex, "step",15, "SMI_Init(LED_PIN)                                  DONE");
                  //bufferScreenDraw( "SMI_Init(LED_PIN) done", 0, sizeof("SMI_Init(LED_PIN) done"), 0, 14, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);      // example LED count, choose your actual count
                    storeLog( m_logBuffer, m_logBufferIndex, "step",16, "WS2812_Init(LED_COUNT)                             DONE");
                  //bufferScreenDraw( "WS2812_Init(LED_COUNT) done", 0, sizeof("WS2812_Init(LED_COUNT) done"), 0, 15, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",16, "GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF)            DONE");
                    GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",16, "GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP)             DONE");
                    GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",16, "GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP)             DONE");
                    }

                return bOK;
}
