#include "kernel.h"

boolean			CKernel::Initialize						(	void )
{
                bool bOK = TRUE;
                
                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",0);
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",1);
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",2);
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",3);
                //  msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",4);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",5);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000); 
                    }

                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",6);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000); 
                    }
                    
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    storeLog( m_logBuffer, m_logBufferIndex, "step",7);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",8);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",9);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",10);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                    if (bOK)
                    {
                            bcm_host_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",11);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }    

                    if (bOK)
                    {
                            initOGL(&m_ogl);
                    storeLog( m_logBuffer, m_logBufferIndex, "step",12);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SPI_init();
                    storeLog( m_logBuffer, m_logBufferIndex, "step",13);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   SMI_Init(LED_PIN);        // example GPIO18, choose your actual WS2812 SMI GPIO
                    storeLog( m_logBuffer, m_logBufferIndex, "step",14);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);      // example LED count, choose your actual count
                    storeLog( m_logBuffer, m_logBufferIndex, "step",15);
                    bufferScreenDraw( "step 1", 0, sizeof("step 1"), 0, 0, 0xFFFFFFFF );
                    msDelay(1000);
                    }

                return bOK;
}