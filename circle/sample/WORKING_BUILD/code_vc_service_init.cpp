#include "kernel.h"

bool            CKernel::initializeVCSM             (   )
{
bool bOK = true;

#ifdef __DEBUG_LOG__
            //  storeLog ( MY_BUFFER, MY_INDEX, SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif 

                getStateVCHI                ();
                if (bOK)
                    {
                    bOK = initEventsVCOS(m_EventSMEM, "SMEM");
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "VCSM initEventsVCOS FAILED");
#endif 
                    if (!bOK) return false;
                    }

                if (bOK)
                    {
                    bOK = openServiceVCHI ( m_ServiceCreateVCSM,
                                            VC_SM_VER,
                                            VC_SM_MIN_VER,
                                            VCHIQ_MAKE_FOURCC('S','M','E','M'),
                                            callbackVCSM,
                                            &m_EventSMEM,
                                            m_VCHIInstance,
                                            m_ServiceHandleVCSM
                                            );
#ifdef __DEBUG_LOG__            
                    if (!bOK) storeLog ( MY_BUFFER, MY_INDEX, "VCHI openService FAILED!");      
#endif                     
                    if (!bOK) return false;
                    }
#ifdef __DEBUG_LOG__ 
            //  nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "VCSM Successful Initialized");
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");                
#endif 
                return bOK;
}

bool            CKernel::initializeMMAL             (   u32                     InBufferHandle,
                                                        u32                     InBufferPointer,
                                                        u32                     InBufferSize, 
                                                        u32                     OutBufferHandleA,
                                                        u32                     OutBufferPointerA,
                                                        u32                     OutBufferHandleB,
                                                        u32                     OutBufferPointerB,
                                                        u32                     OutBufferSize,
                                                        u32                     ResolutionX,
                                                        u32                     ResolutionY,
                                                        EGLDisplay              eglDisplay,
                                                        EGLContext              eglContext )
{
                m_InputBufferHandle         = InBufferHandle;
                m_InputBufferPointer        = InBufferPointer;

                m_OutputBufferHandleA       = OutBufferHandleA;
                m_OutputBufferPointerA      = OutBufferPointerA;

                m_OutputBufferHandleB       = OutBufferHandleB;
                m_OutputBufferPointerB      = OutBufferPointerB;

                m_InputBufferSize           = InBufferSize;
                m_OutputBufferSize          = OutBufferSize;

                m_ResolutionX               = ResolutionX;
                m_ResolutionY               = ResolutionY;

                m_eglDisplay                = eglDisplay;
                m_eglContext                = eglContext;

bool bOK = true;

#ifdef __DEBUG_LOG__                
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif
                getStateVCHI                ( );
                if (bOK)
                    {
                    bOK = initEventsVCOS( m_EventMMAL, "MMAL" );
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMAL initEventsVCOS FAILED");
#endif                   
                    if (!bOK) return false;
                    }

#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");   
                storeLog ( MY_BUFFER, MY_INDEX, "Input    Buffer Handle",m_InputBufferHandle,   "Pointer", m_InputBufferPointer,    "Size", m_InputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output A Buffer Handle",m_OutputBufferHandleA, "Pointer", m_OutputBufferPointerA,  "Size", m_OutputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output B Buffer Handle",m_OutputBufferHandleB, "Pointer", m_OutputBufferPointerB,  "Size", m_OutputBufferSize); 
            //  nextline ( MY_BUFFER, MY_INDEX );       
                storeLog ( MY_BUFFER, MY_INDEX, "Resolution      Height", m_ResolutionX, "Width", m_ResolutionY, "EGL Display", (u32)m_eglDisplay, "EGL Context", (u32)m_eglContext);
            //  storeLog ( MY_BUFFER, MY_INDEX, "EGL Display", (u32)m_eglDisplay, "EGL Context", (u32)m_eglContext);
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif 

                if (bOK)
                    {
                    bOK = openServiceVCHI(  m_ServiceCreateMMAL,
                                            VC_MMAL_VER,
                                            VC_MMAL_MIN_VER,
                                            VCHIQ_MAKE_FOURCC('m','m','a','l'),
                                            callbackMMAL,
                                            &m_EventMMAL,
                                            m_VCHIInstance,
                                            m_ServiceHandleMMAL);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMAL openService FAILED");
#endif 
                    if (!bOK) return false;
                    }
                if (bOK)
                    {
                    bOK = createComponent(m_ComponentCreateTx, m_ComponentCreateRx);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALcreateComponent FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_A, m_PortInfoGetRx_Input_A);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input A FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_A, m_PortInfoGetRx_Output_A);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output A FAILED");
#endif 
                    }

                primePortFormatInputMMAL (m_PortInfoGetRx_Input_A,  m_PortInfoSetTx_Input);
                primePortFormatOutputMMAL(m_PortInfoGetRx_Output_A, m_PortInfoSetTx_Output); 

                if (bOK)
                    {
                    bOK = setPortInfoMMAL(m_PortInfoSetTx_Input, m_PortInfoSetRx_Input);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetPortInfo Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setPortInfoMMAL(m_PortInfoSetTx_Output, m_PortInfoSetRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetPortInfo Output FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enableComponentMMAL(m_ComponentEnableTx, m_ComponentEnableRx);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenableComponentMMAL FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_B, m_PortInfoGetRx_Input_B);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input B FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_B, m_PortInfoGetRx_Output_B);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output B BFAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setZeroCopyModeMMAL(m_PortInfoGetRx_Input_B, m_PortParamTx_Input, m_PortParamRx_Input);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetZeroCopyModeMMAL Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = setZeroCopyModeMMAL(m_PortInfoGetRx_Output_B, m_PortParamTx_Output, m_PortParamRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALsetZeroCopyModeMMAL Output FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_C, m_PortInfoGetRx_Input_C);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Input C FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_C, m_PortInfoGetRx_Output_C);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALgetPortInfo Output C FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enablePortMMAL(m_PortInfoGetRx_Input_C, m_PortActionTx_Input, m_PortActionRx_Input);
#ifdef __DEBUG_LOG__
                if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenablePort Input FAILED");
#endif 
                    }
                if (bOK)
                    {
                    bOK = enablePortMMAL(m_PortInfoGetRx_Output_C, m_PortActionTx_Output, m_PortActionRx_Output);
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "MMALenablePort Output FAILED");
#endif 
                    }
            /*
                queueInputBufferMMAL        ( m_BufferFromHostTx_Input,
                                            m_BufferFromHostRx_Input );
                queueOutputBufferMMAL       ( m_BufferFromHostTx_OutputA,
                                            m_BufferFromHostRx_OutputA );
                queueOutputBufferMMAL       ( m_BufferFromHostTx_OutputB,
                                            m_BufferFromHostRx_OutputB );
            */
#ifdef __DEBUG_LOG__ 
            //  nextline ( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "MMAL Successful Initialized");
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");                
#endif 
                return bOK;
}

bool            CKernel::initTexturesMMAL         (   )
{
                int f_count = 0;

                glGenTextures(1, &m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!checkGLerrorMMAL()) f_count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!checkGLerrorMMAL()) f_count++;

                if( f_count != 0)
                    { 
#ifdef __DEBUG_LOG__ 
                //  nextline( MY_BUFFER, MY_INDEX );                                 
                    storeLog( MY_BUFFER, MY_INDEX, "Texture Creation FAILED");
#endif                   
                    return false;
                    }
#ifdef __DEBUG_LOG__             
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Texture Creation SUCCESS");
#endif               
                return true;
}

