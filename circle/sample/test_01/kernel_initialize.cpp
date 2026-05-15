#include "kernel.h"

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;
                
                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",0)
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",1)
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",2)
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",3)
                //  msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",4)
                    }

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",5)
                //  msDelay(1000); 
                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",6)
                //  msDelay(1000); 
                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    storeLog( m_logBuffer, m_logBufferIndex, "step",7)
                //  msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",8)
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",9)
                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",10)
                    }

                    if (bOK)
                    {
                            bcm_host_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",11)
                    }    

                    if (bOK)
                    {
                            initOGL(&m_ogl);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",12)
                    }

                if (bOK)
                    {
                    bOK =   SPI_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",13)
                    }

                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);        // example GPIO18, choose your actual WS2812 SMI GPIO
                    storeLog( m_logBuffer, m_logBufferIndex, "step",14)
                    }

                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);      // example LED count, choose your actual count
                    storeLog( m_logBuffer, m_logBufferIndex, "step",15)
                    }

                return bOK;
}