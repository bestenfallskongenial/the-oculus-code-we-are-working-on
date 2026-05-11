#include "kernel.h"

boolean			CKernel::Initialize						(	void )
{
                boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    }

                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
                    m_Timer.MsDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    }

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    m_Timer.MsDelay(1000); 
                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    m_Timer.MsDelay(1000); 
                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    m_Timer.MsDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = SPI_init();
                    }

                if (bOK)
                    {
                    bOK = SMI_Init(LED_PIN);        // example GPIO18, choose your actual WS2812 SMI GPIO
                    }

                if (bOK)
                    {
                    bOK = WS2812_Init(LED_COUNT);      // example LED count, choose your actual count
                    }

                if (bOK)
                    {
                    bOK = wrapperInitDMA();
                    }

                if (bOK)
                    {
                    bOK = wrapperInitMEM();
                    }

                return bOK;
}