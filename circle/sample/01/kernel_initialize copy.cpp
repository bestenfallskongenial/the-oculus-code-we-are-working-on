// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean         CKernel::Initialize (void)
{
                bool bOK = true;

    if (bOK)
     {
         my_GPIO_SetOutput(CS_PIN);               // new
         my_GPIO_SetPull(CS_PIN, GPIO_PULL_OFF);  // new
         my_GPIO_Write(CS_PIN, LOW);              // new
     }
                    }
                if (bOK)
                    {

                    }   
                if (bOK)
                    {
                    my_SPI_init();
                    my_SMI_SetupTiming()
                    my_SMI_SetupDMA()
                    my_SMI_Init(CTRL_PIN),
                    }   
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
                    bOK = m_EMMC.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); 
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    m_Timer.MsDelay(1000);  
                    }
                if (bOK)
                    {
                    bOK = wrapperInitDMA();
                    bOK = wrapperInitMEM();
                    }
                if (bOK)
                    {
                    bOK = wrapperInitVCSMstruct();
                    bOK = wrapperInitMMALstruct();
                    }                    
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize();
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                {
                    bcm_host_init();  // we need to initialize the bcm_host before the graphics
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    initOGL   (&m_glsl); // we need to initialize the graphics before the shared memory
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                    {
                    bOK = m_SPIMaster.Initialize();
                    }

                return bOK;
                }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------