// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    #include "kernel.h"     
//  #include <linux/kernel.h>                                                                           // here or in the kernel.h? i move it to kernel.h, seems to be an artifact
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //  #define LOG_NAME "VCSM"                                                                 // dead code ?!

            //  static  const char              FromKernel[] = "kernel";                                // for logging? here not in kernel.h?
                

                CKernel::CKernel()
                  : gE_FrameBuffer( m_Options.GetWidth(),                                               // my framebuffer
                                    m_Options.GetHeight(),
                                    32, 0, TRUE),                                                       
                    m_Interrupt (),                                                                     // i assume someone needs interupts here, not me!
	                m_Timer (&m_Interrupt),                                                             // this i use frequently for timeouts, delays           
                    m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),                                         // my CEMMCDevice device
	                m_USBHCI (&m_Interrupt, &m_Timer, TRUE),                                            // my CUSBHCI device, i assume the usb device
	            //  m_VCHIQ (CMemorySystem::Get (), &m_Interrupt),                                      // my VCHIQ device, for video and gpu communication - does it need m_Screen?
                    m_VCHIQ (&m_Memory, &m_Interrupt),                                                  // old!
                    m_SMITxDMA(DMA_CHANNEL_LITE)                                                        // new                 
                    {
                    m_ActLED.Blink (5);	
                    }
                    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------                    
                CKernel::~CKernel (void)                                                                // my deconstructor
                    {
                    wrapperMemoryCleanUp();                                                             // despite the fact that i dont use it anyway dont i need also a glsl cleanup for example?
                    }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
