	#include "kernel.h"

				CKernel::CKernel						(	void )
				:	m_Timer(&m_Interrupt),
                    m_Logger(LOGLEVEL, &m_Timer),
                    gE_FrameBuffer( m_Options.GetWidth(), 
                                    m_Options.GetHeight(), 
                                    32, 
                                    0, 
                                    TRUE ),
                    m_SMITxDMA( DMA_CHANNEL_LITE )      // DMA_CHANNEL_LITE comes from machineinfo.h   
                m_ActLED.Blink(5);
}

				CKernel::~CKernel						(	void )
{
}



void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch;
                    }

                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0';
}

