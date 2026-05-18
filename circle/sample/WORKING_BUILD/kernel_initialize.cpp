#include "kernel.h"


    #undef __DEBUG_LOG__
//  #define __DEBUG_LOG__

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

                    }

                if (bOK)
                    {
                    bOK = startupScreen();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "System Info Log                                    DONE");

                    }                

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_EMMC.Initialize()                                DONE");

                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.Initialize()                              DONE");

                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.UpdatePlugAndPlay()                       DONE");

                    }

                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "InitDMA Buffer                                     DONE");

                    }

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "InitMEM Buffer                                     DONE");

                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "m_VCHIQ.Initialize()                               DONE");

                    }

                    if (bOK)
                    {
                            bcm_host_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "bcm_host_init()                                    DONE");

                    }    

                    if (bOK)
                    {
                            initOGL(&m_ogl);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "initOGL(&m_ogl)                                    DONE");

                    }

                if (bOK)
                    {
                    bOK =   SPI_init();
                  storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "SPI_init()                                         DONE");

                    }

                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "SMI_Init(LED_PIN)                                  DONE");

                    }

                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);
                    storeLog( m_logBuffer, m_logBufferIndex, "1/100 sec", m_Timer.GetTicks(), "WS2812_Init(LED_COUNT)                             DONE");

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
