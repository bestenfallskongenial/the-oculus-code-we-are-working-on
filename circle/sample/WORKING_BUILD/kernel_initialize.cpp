#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

#define MY_BUFFER   m_logBuffer
#define MY_INDEX    m_logBufferIndex    

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_Interrupt.Initialize()                           DONE");
#endif
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_Timer.Initialize()                               DONE");
#endif
                    }
                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "CLogger::SetRawSink(CKernel::LoggerSink, this)     DONE");
#endif
                    bOK = m_Logger.Initialize(0);
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_Logger.Initialize(0)                             DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK = frameBufferInit();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "frameBufferInit()                                  DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "System Info Log                                    DONE");
#endif
                    }                
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_EMMC.Initialize()                                DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
#ifdef __DEBUG_LOG__                    
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.Initialize()                              DONE");
#endif
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_USBHCI.UpdatePlugAndPlay()                       DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "InitDMA Buffer                                     DONE");
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "InitDMA Buffer                                     DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "InitMEM Buffer                                     DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "m_VCHIQ.Initialize()                               DONE");
#endif
                    }
                    if (bOK)
                    {
                    bcm_host_init();
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "bcm_host_init()                                    DONE");
#endif
                    }    
                    if (bOK)
                    {
                    initOGL(&m_ogl);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "initOGL(&m_ogl)                                    DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SPI_init();
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "SPI_init()                                         DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "SMI_Init(LED_PIN)                                  DONE");
#endif
                    }
                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "WS2812_Init(LED_COUNT)                             DONE");
#endif
                    }
                if (bOK)
                    {
                    GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF)            DONE");
#endif
                    GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP)             DONE");
#endif
                    GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP);
#ifdef __DEBUG_LOG__
                    storeLog( MY_BUFFER, MY_INDEX, "1/100 sec", m_Timer.GetTicks(), "GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP)             DONE");
#endif
                    }
                nextline(   MY_BUFFER, MY_INDEX);

                return bOK;
}
