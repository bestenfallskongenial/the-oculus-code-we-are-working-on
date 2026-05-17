#include "kernel.h"

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_Interrupt.Initialize()                           DONE");
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_Timer.Initialize()                               DONE");
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "CLogger::SetRawSink(CKernel::LoggerSink, this)     DONE");
                    bOK = m_Logger.Initialize(0);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_Logger.Initialize(0)                             DONE");
                    }


                if (bOK)
                    {
                    bOK = frameBufferInit();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "frameBufferInit()                                  DONE");
                  //bufferScreenDraw( "frameBufferInit() done", 0, sizeof("frameBufferInit() done"), 0, 4, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = startupScreen();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "System Info Log                                    DONE");
                  //msDelay(1000);
                    }                

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_EMMC.Initialize()                                DONE");
                  //bufferScreenDraw( "m_EMMC.Initialize() done", 0, sizeof("m_EMMC.Initialize() done"), 0, 5, 0xFFFFFFFF );
                  //msDelay(1000); 
                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.Initialize()                              DONE");
                  //bufferScreenDraw( "m_USBHCI.Initialize() done", 0, sizeof("m_USBHCI.Initialize() done"), 0, 6, 0xFFFFFFFF );
                  //msDelay(1000); 
                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.UpdatePlugAndPlay()                       DONE");
                  //bufferScreenDraw( "m_USBHCI.UpdatePlugAndPlay() done", 0, sizeof("m_USBHCI.UpdatePlugAndPlay() done"), 0, 7, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "InitDMA Buffer                                     DONE");
                  //bufferScreenDraw( "InitDMA Buffer done", 0, sizeof("InitDMA Buffer done"), 0, 8, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "InitMEM Buffer                                     DONE");
                  //bufferScreenDraw( "InitMEM Buffer done", 0, sizeof("InitMEM Buffer done"), 0, 9, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();       // <--- breaking point !! but why?!
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_VCHIQ.Initialize()                               DONE");
                  //bufferScreenDraw( "m_VCHIQ.Initialize() done", 0, sizeof("m_VCHIQ.Initialize() done"), 0, 10, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                    if (bOK)
                    {
                            bcm_host_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "bcm_host_init()                                    DONE");
                  //bufferScreenDraw( "bcm_host_init() done", 0, sizeof("bcm_host_init() done"), 0, 11, 0xFFFFFFFF );
                  //msDelay(1000);
                    }    

                    if (bOK)
                    {
                            initOGL(&m_ogl);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "initOGL(&m_ogl)                                    DONE");
                  //bufferScreenDraw( "initOGL(&m_ogl) done", 0, sizeof("initOGL(&m_ogl) done"), 0, 12, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SPI_init();
                  storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "SPI_init()                                         DONE");
                //bufferScreenDraw( "SPI_init() done", 0, sizeof("SPI_init() done"), 0, 13, 0xFFFFFFFF );
                 //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);        // example GPIO18, choose your actual WS2812 SMI GPIO
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "SMI_Init(LED_PIN)                                  DONE");
                  //bufferScreenDraw( "SMI_Init(LED_PIN) done", 0, sizeof("SMI_Init(LED_PIN) done"), 0, 14, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);      // example LED count, choose your actual count
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "WS2812_Init(LED_COUNT)                             DONE");
                  //bufferScreenDraw( "WS2812_Init(LED_COUNT) done", 0, sizeof("WS2812_Init(LED_COUNT) done"), 0, 15, 0xFFFFFFFF );
                  //msDelay(1000);
                    }

                if (bOK)
                    {
                    GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF)            DONE");
                    GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP)             DONE");
                    GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP)             DONE");
                    }

                return bOK;
}
