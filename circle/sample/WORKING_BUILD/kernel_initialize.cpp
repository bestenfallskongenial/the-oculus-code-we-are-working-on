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
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Interrupt.Initialize()                FAILED");
#endif
                    }       
                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Timer.Initialize()                    FAILED");
#endif
                    }
                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SetRawSink(CKernel::LoggerSink, this)   FAILED");
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_Logger.Initialize(0)                  FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK = frameBufferInit();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "frameBufferInit()                       FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK = startupScreen();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "Hardware Info Log                       FAILED");
#endif
                    }                
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_EMMC.Initialize()                     FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize();
#ifdef __DEBUG_LOG__                    
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_USBHCI.Initialize()                   FAILED");
#endif
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_USBHCI.UpdatePlugAndPlay()            FAILED");
                    #endif
                    }
                if (bOK)
                    {
                    bOK =   wrapperInitDMA();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "InitDMA Buffer                          FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK =   wrapperInitMEM();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "InitMEM Buffer                          FAILED");
#endif
                    }

                if (bOK)
                    {
                    bOK =   m_VCHIQ.Initialize();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "m_VCHIQ.Initialize()                    FAILED");
#endif
                    }
                    if (bOK)
                    {
                    bcm_host_init();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "bcm_host_init()                         FAILED");
#endif
                    }    

                                    if (bOK)                    // decided to gather all vcsm/mmal init code here and clean up the code a little! 
                                        {
                                        getStateVCHI();
                    #ifdef __DEBUG_LOG__
                                    //  if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getStateVCHI()                          FAILED");
                    #endif
                                        }
                                    if (bOK)
                                        {
                                        bOK = initEventsVCOS(m_EventSMEM, "SMEM");
                    #ifdef __DEBUG_LOG__
                                        if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "initEventsVCOS(m_EventSMEM, SMEM)        DONE");
                    #endif
                                        }
                                    if (bOK)
                                        {
                                        bOK = openServiceVCHI (     m_ServiceCreateVCSM,
                                                                    VC_SM_VER,
                                                                    VC_SM_MIN_VER,
                                                                    VCHIQ_MAKE_FOURCC('S','M','E','M'),
                                                                    callbackVCSM,
                                                                    &m_EventSMEM,
                                                                    m_VCHIInstance,
                                                                    m_ServiceHandleVCSM );
                    #ifdef __DEBUG_LOG__
                                        if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "openServiceVCHI('S','M','E','M')         DONE");
                    #endif
                                        }
                                    if (bOK)
                                        {
                                        if (bOK)
                                            {
                                            bOK = importMemoryVCSM  (   m_videoBlockBase, 
                                                                        m_videoBlockSize, 
                                                                        m_input_buffer_handle,
                                                                        m_importTxVCSM_A, 
                                                                        m_importRxVCSM_A );

                                            bOK = lockMemoryVCSM    (   m_input_buffer_handle,
                                                                        m_input_buffer_pointer,
                                                                        m_lockTxVCSM, 
                                                                        m_lockRxVCSM ); 
                    #ifdef __DEBUG_LOG__
                                            if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SMEM-MEM Input allocation                DONE");
                    #endif                                                                       
                                            }
                                        if (bOK)
                                            {
                                            bOK = importMemoryVCSM  (   m_frameBlockBaseA, 
                                                                        m_frameBlockSizeA, 
                                                                        m_output_buffer_handle_a,
                                                                        m_importTxVCSM_B, 
                                                                        m_importRxVCSM_B );                    

                                            bOK = lockMemoryVCSM    (   m_output_buffer_handle_a,
                                                                        m_output_buffer_pointer_a,
                                                                        m_lockTxVCSM, 
                                                                        m_lockRxVCSM );
                    #ifdef __DEBUG_LOG__
                                            if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SMEM-MEM Output A allocation             DONE");
                    #endif                                              
                                            }
                                        if (bOK)
                                            {
                                            bOK = importMemoryVCSM  (   m_frameBlockBaseB, 
                                                                        m_frameBlockSizeB, 
                                                                        m_output_buffer_handle_b,
                                                                        m_importTxVCSM_C, 
                                                                        m_importRxVCSM_C );                    

                                            bOK = lockMemoryVCSM    (   m_output_buffer_handle_b,
                                                                        m_output_buffer_pointer_b,
                                                                        m_lockTxVCSM, 
                                                                        m_lockRxVCSM );   
                    #ifdef __DEBUG_LOG__
                                            if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SMEM-MEM Output B allocation             DONE");
                    #endif                                                                           
                                            }
                                        }

                if (bOK)
                    {
                    bOK = initEventsVCOS( m_EventMMAL, "MMAL" );
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "initEventsVCOS( m_EventMMAL, MMAL )     DONE");
#endif
                    }

                if (bOK)
                    {
                    bOK = openServiceVCHI(      m_ServiceCreateMMAL,
                                                VC_MMAL_VER,
                                                VC_MMAL_MIN_VER,
                                                VCHIQ_MAKE_FOURCC('m','m','a','l'),
                                                callbackMMAL,
                                                &m_EventMMAL,
                                                m_VCHIInstance,
                                                m_ServiceHandleMMAL);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "openServiceVCHI('m','m','a','l')            DONE");
#endif
                    }

                if (bOK)
                    {
                    bOK = createComponent( m_ComponentHandle, m_ComponentCreateTx, m_ComponentCreateRx);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "createComponent()                       DONE");
#endif 
                    }

                if (bOK)
                    {
                    bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_INPUT, 
                                                m_InputPortHandle, 
                                                m_PortInfoGetTx_Input_A, 
                                                m_PortInfoGetRx_Input_A);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL( Input )               DONE");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_OUTPUT, 
                                                m_OutputPortHandle, 
                                                m_PortInfoGetTx_Output_A, 
                                                m_PortInfoGetRx_Output_A);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL( Output )               DONE");
#endif 
                    }          
                
                primePortFormatInputMMAL (      m_videoBlockSize,
                                                m_PortInfoGetRx_Input_A,  
                                                m_PortInfoSetTx_Input);

                primePortFormatOutputMMAL(      m_frameBlockSizeA,
                                                m_PortInfoGetRx_Output_A, 
                                                m_PortInfoSetTx_Output);                     
/*
                if (bOK)
                    {
                    bOK = setPortInfoMMAL(      m_PortInfoSetTx_Input, 
                                                    m_PortInfoSetRx_Input);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "setPortInfoMMAL( Input )                DONE");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setPortInfoMMAL(      m_PortInfoSetTx_Output, 
                                                    m_PortInfoSetRx_Output);
#ifdef __DEBUG_LOG__
                    if (bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "setPortInfoMMAL( Output )               DONE");
#endif 
                    }
*/

/*


                if (bOK)
                    {



                    if (bOK)
                        {
                        bOK = enableComponentMMAL(  m_ComponentEnableTx, 
                                                    m_ComponentEnableRx);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "enableComponentMMAL()                  FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_INPUT, 
                                                    m_InputPortHandle, 
                                                    m_PortInfoGetTx_Input_B, 
                                                    m_PortInfoGetRx_Input_B);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL( Input )               FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_OUTPUT, 
                                                    m_OutputPortHandle, 
                                                    m_PortInfoGetTx_Output_B, 
                                                    m_PortInfoGetRx_Output_B);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL( Output )              FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = setZeroCopyModeMMAL(  m_PortInfoGetRx_Input_B, 
                                                    m_PortParamTx_Input, 
                                                    m_PortParamRx_Input);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "setZeroCopyModeMMAL( Input )           FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = setZeroCopyModeMMAL(  m_PortInfoGetRx_Output_B, 
                                                    m_PortParamTx_Output, 
                                                    m_PortParamRx_Output);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "setZeroCopyModeMMAL( Output )          FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_INPUT, 
                                                    m_InputPortHandle, 
                                                    m_PortInfoGetTx_Input_C, 
                                                    m_PortInfoGetRx_Input_C);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL ( Input )              FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = getPortInfoMMAL(      MMAL_PORT_TYPE_OUTPUT, 
                                                    m_OutputPortHandle, 
                                                    m_PortInfoGetTx_Output_C, 
                                                    m_PortInfoGetRx_Output_C);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "getPortInfoMMAL ( Output )             FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = enablePortMMAL(       m_PortInfoGetRx_Input_C, 
                                                    m_PortActionTx_Input, 
                                                    m_PortActionRx_Input);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "enablePortMMAL ( Input )               FAILED");
#endif 
                        }
                    if (bOK)
                        {
                        bOK = enablePortMMAL(       m_PortInfoGetRx_Output_C, 
                                                    m_PortActionTx_Output, 
                                                    m_PortActionRx_Output);
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "enablePortMMAL ( Output )              FAILED");
#endif 
                        }
                    if (bOK)    // seemingly mandatory to "prime" and send / queue the buffers ones before decoder runtime
                        {
*/                            
                        /*    
                        queueInputBufferMMAL        (   m_BufferFromHostTx_Input,
                                                        m_BufferFromHostRx_Input );
                        queueOutputBufferMMAL       (   m_BufferFromHostTx_OutputA,
                                                        m_BufferFromHostRx_OutputA );
                        queueOutputBufferMMAL       (   m_BufferFromHostTx_OutputB,
                                                        m_BufferFromHostRx_OutputB );
#ifdef __DEBUG_LOG__
                        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenablePort Output FAILED");
#endif 
                        */
/*                        
                        }
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "MMAL Init                              FAILED");
#endif
                    }
*/

                    if (bOK)
                    {
                    initOGL(    &m_ogl);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "initOGL(&m_ogl)                        FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SPI_init();
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SPI_init()                             FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK =   SMI_Init(   LED_PIN);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "SMI_Init(LED_PIN)                      FAILED");
#endif
                    }
                if (bOK)
                    {
                    bOK =   WS2812_Init(LED_COUNT);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "WS2812_Init(LED_COUNT)                 FAILED");
#endif
                    }
                if (bOK)
                    {
                    GPIO_SetAlt(    CTRL_PIN, 
                                    1, 
                                    GPIO_PULL_OFF);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(CTRL_PIN, 1, GPIO_PULL_OFF) FAILED");
#endif
                    GPIO_SetAlt(    SW_PIN_A, 
                                    0, 
                                    GPIO_PULL_UP);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP) FAILED");
#endif
                    GPIO_SetAlt(    SW_PIN_B, 
                                    0, 
                                    GPIO_PULL_UP);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, ">:", m_Timer.GetClockTicks(), "GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP) FAILED");
#endif
                    }
                return bOK;
}


