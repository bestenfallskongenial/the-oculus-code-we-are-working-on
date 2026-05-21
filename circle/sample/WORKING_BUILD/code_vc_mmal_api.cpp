#include "kernel.h"

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
                getStateVCHI                ( );  // redundant right?
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

#include "kernel.h"

bool            CKernel::framePollerMMAL            (   u32 frame_offset, u32 frame_length)
{
#ifdef __DEBUG_LOG__
                if (!f_firstFrameQueued)
                    {
                    if (!queueInputBufferMMAL(m_BufferFromHostTx_Input, frame_offset, frame_length))
                        {
                    //  nextline( MY_BUFFER, MY_INDEX );
                        storeLog( MY_BUFFER, MY_INDEX, "very first frame queue ERROR!", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                        return false;
                        }
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "very first frame queue SUCCESS", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);

                    getPortInfoMMAL(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  m_PortInfoGetTx_Input_D, m_PortInfoGetRx_Input_D);
                    getPortInfoMMAL(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_D, m_PortInfoGetRx_Output_D);

                    f_firstFrameQueued = true;
                    return true;
                    }
#endif 
                const char*  message = "";
                uint32_t msg_len = 0;


    // Drain RX queue unconditionally (non-blocking)

                while (vchi_msg_dequeue(m_ServiceHandleMMAL, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
                    {
                    switch (m_BufferFromHostTx_Output.hdr.type) // FIRST AXIS: message type (semantic meaning)
                        {
                        case MMAL_MSG_TYPE_BUFFER_TO_HOST:
                            {
                            switch (m_BufferFromHostTx_Output.hdr.status)   // SECOND AXIS: status (outcome)
                                {
                                case MMAL_MSG_STATUS_SUCCESS:
                                    {
                                    u32 m_CurrentHandle = m_BufferFromHostTx_Output.msg.buffer_header.data;  // Payload layout reused: buffer_from_host
#ifdef __DEBUG_LOG__   
                                //  nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, "frame offset", frame_offset, "length", frame_length, "status", m_BufferFromHostTx_Output.hdr.status, "handle", m_CurrentHandle);
#endif 
                                    if (m_CurrentHandle != m_OutputBufferHandleA && m_CurrentHandle != m_OutputBufferHandleB)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (m_CurrentHandle == m_OutputBufferHandleA) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBufferMMAL(m_BufferFromHostTx_Output, m_OutputBufferHandleB, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (m_CurrentHandle == m_OutputBufferHandleB) 
                                        {
                                        if (!queueOutputBufferMMAL(m_BufferFromHostTx_Output, m_OutputBufferHandleA, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (!bufferReadyMMAL(m_CurrentHandle))
                                        return false;
                                    if (!queueInputBufferMMAL(m_BufferFromHostTx_Input, frame_offset, frame_length))
                                        return false;
#ifdef __DEBUG_LOG__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                //  nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "frame offset", frame_offset, "length", frame_length);
#endif                       
                                    return true;
                                    }
#ifdef __DEBUG_LOG__   
                                case MMAL_MSG_STATUS_ENOMEM:     message = "MMAL_MSG_STATUS_ENOMEM       - Out of memory                      "; break;
                                case MMAL_MSG_STATUS_ENOSPC:     message = "MMAL_MSG_STATUS_ENOSPC       - Out of resources other than memory "; break;
                                case MMAL_MSG_STATUS_EINVAL:     message = "MMAL_MSG_STATUS_EINVAL       - Argument is invalid                "; break;
                                case MMAL_MSG_STATUS_ENOSYS:     message = "MMAL_MSG_STATUS_ENOSYS       - Function not implemented           "; break;
                                case MMAL_MSG_STATUS_ENOENT:     message = "MMAL_MSG_STATUS_ENOENT       - No such file or directory          "; break;
                                case MMAL_MSG_STATUS_ENXIO:      message = "MMAL_MSG_STATUS_ENXIO        - No such device or address          "; break;
                                case MMAL_MSG_STATUS_EIO:        message = "MMAL_MSG_STATUS_EIO          - I/O error                          "; break;
                                case MMAL_MSG_STATUS_ESPIPE:     message = "MMAL_MSG_STATUS_ESPIPE       - Illegal seek                       "; break;
                                case MMAL_MSG_STATUS_ECORRUPT:   message = "MMAL_MSG_STATUS_ECORRUPT     - Data is corrupt                    "; break;
                                case MMAL_MSG_STATUS_ENOTREADY:  message = "MMAL_MSG_STATUS_ENOTREADY    - Component is not ready             "; break;
                                case MMAL_MSG_STATUS_ECONFIG:    message = "MMAL_MSG_STATUS_ECONFIG      - Component is not configured        "; break;
                                case MMAL_MSG_STATUS_EISCONN:    message = "MMAL_MSG_STATUS_EISCONN      - Port is already connected          "; break;
                                case MMAL_MSG_STATUS_ENOTCONN:   message = "MMAL_MSG_STATUS_ENOTCONN     - Port is disconnected               "; break;
                                case MMAL_MSG_STATUS_EAGAIN:     message = "MMAL_MSG_STATUS_EAGAIN       - Resource temporarily unavailable   "; break;
                                case MMAL_MSG_STATUS_EFAULT:     message = "MMAL_MSG_STATUS_EFAULT       - Bad address                        "; break;
#endif                   
                                default:                         message = "Unknown MMAL status          - WTF!!!                             "; break;
                                }
#ifdef __DEBUG_LOG__   
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message, EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (BUFFER_TO_HOST)", &m_BufferFromHostTx_Output, msg_len);
#endif               
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __DEBUG_LOG__    
                        //  nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message,  EMPTYLOG, "Frame offset", frame_offset, "Type",  m_BufferFromHostTx_Output.hdr.type, "Status", m_BufferFromHostTx_Output.hdr.status);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (UNEXPECTED MESSAGE)", &m_BufferFromHostTx_Output, msg_len);
#endif 
                            break;
                            }
                        }
#ifdef __DEBUG_LOG__ 
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "Unexpected Reply", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);
                    storeMsg( MY_BUFFER, MY_INDEX, "Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
#endif       
                    }
#ifdef __DEBUG_LOG__   
            //  nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Nothing in the Pipeline", EMPTYLOG, "Frame offset", frame_offset, "length", frame_length);    // Nothing relevant received
#endif 
                return true;
}


// a-sync VCHI runtime messages ?

bool            CKernel::bufferReadyMMAL            (   u32 handle)
{
        //      if (handle != m_VCSMHandleA && handle != m_VCSMHandleB)         // Only react to our two output buffers
        //          return true;                                                // why???? we have filtered before ad infiniti 
                if (m_EGLimage != EGL_NO_IMAGE_KHR)                             // Destroy previous EGLImage (if any)
                    {
                    eglDestroyImageKHR(m_eglDisplay, m_EGLimage);
                    m_EGLimage = EGL_NO_IMAGE_KHR;
                    }
                egl_image_brcm_vcsm_info info =                                 // Describe the finished VCSM buffer
                    {
                    .width       = m_ResolutionX,
                    .height      = m_ResolutionY,
                    .vcsm_handle = handle
                    };
                m_EGLimage = eglCreateImageKHR( m_eglDisplay, m_eglContext, EGL_IMAGE_BRCM_VCSM, (EGLClientBuffer)&info, nullptr ); // Create new EGLImage viewing this buffer
                if (m_EGLimage == EGL_NO_IMAGE_KHR)
                    {
#ifdef __DEBUG_LOG__        
                    storeLog( MY_BUFFER, MY_INDEX, "EGLImage creation FAILED", handle);
#endif 
                    return false;
                    }
                glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
                glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
                glBindTexture(GL_TEXTURE_2D, 0);

                return true;
}

bool            CKernel::queueOutputBufferMMAL      (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 vc_handle, 
                                                        u32 alloc_size)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.data                          = vc_handle;
                tx.msg.buffer_header.alloc_size                    = alloc_size;  // why again?
                tx.msg.buffer_header.length                        = 0;
                tx.msg.buffer_header.offset                        = 0;
                tx.msg.buffer_header.flags                         = 0;

#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueOutputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

bool            CKernel::queueInputBufferMMAL       (   MMAL_Buffer_From_Host_Msg& tx, 
                                                        u32 frame_offset, 
                                                        u32 frame_length)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.offset                        = frame_offset;
                tx.msg.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.msg.buffer_header.flags                         = flags;
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "BUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif 
                return (vchi_msg_queue(m_ServiceHandleMMAL, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}

