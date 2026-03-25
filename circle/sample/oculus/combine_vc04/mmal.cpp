//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "h264_decoder.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::initialize                  ( u32         InBufferHandle,         // my input buffer handle from smem
                                                            u32         InBufferPointer,        // i got the feeling i rather need this
                                                            u32         InBufferSize,           // my allocated input buffer size 
                                                            u32         OutBufferHandleA,       // my output buffer handle a from smem
                                                            u32         OutBufferPointerA,      // i got the feeling i rather need this
                                                            u32         OutBufferHandleB,       // my output buffer handle b from smem
                                                            u32         OutBufferPointerB,      // i got the feeling i rather need this
                                                            u32         OutBufferSize,          // my allocated output buffer size
                                                            u32         ResolutionX,            // obvious, right?
                                                            u32         ResolutionY,            // obvious, right?
                                                            EGLDisplay  eglDisplay,             // EGL display connection
                                                            EGLContext  eglContext )            // EGL rendering context
{
                /* store parameters */                                  // from the 
                m_InputBufferHandle         = InBufferHandle;                 //
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
#ifdef __H264_DECODER_DEBUG_INIT__                
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif
                getVCHIstate                ( );
#ifdef __H264_DECODER_DEBUG_INIT__
                nextline( MY_BUFFER, MY_INDEX );
#endif                
                initEvents                  ( );
#ifdef __H264_DECODER_DEBUG_INIT__
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");   
                storeLog ( MY_BUFFER, MY_INDEX, "Input Port      Handle / Size ",m_InputBufferHandle, m_InputBufferPointer, m_InputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output A Port   Handle / Size ",m_OutputBufferHandleA, m_OutputBufferPointerA, m_OutputBufferSize);
                storeLog ( MY_BUFFER, MY_INDEX, "Output B Port   Handle / Size ",m_OutputBufferHandleB, m_OutputBufferPointerB, m_OutputBufferSize); 
                nextline( MY_BUFFER, MY_INDEX );       
                storeLog ( MY_BUFFER, MY_INDEX, "Resolution      Height / Width",m_ResolutionX, m_ResolutionY);
                storeLog ( MY_BUFFER, MY_INDEX, "EGL Display / Contex          ", (u32)m_eglDisplay, (u32)m_eglContext);
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif

bool bOK = true;

    if (bOK)
        {
    bOK = openService(m_ServiceCreate);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMAL openService FAILED");
#endif
        }
    if (bOK)
        {
    bOK = createComponent(m_ComponentCreateTx, m_ComponentCreateRx);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALcreateComponent FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_A, m_PortInfoGetRx_Input_A);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Input A FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_A, m_PortInfoGetRx_Output_A);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Output A FAILED");
#endif
        }
    if (bOK)
        {
        setPortFormatInput (m_PortInfoGetRx_Input_A,  m_PortInfoSetTx_Input);
        setPortFormatOutput(m_PortInfoGetRx_Output_A, m_PortInfoSetTx_Output);
        }
    if (bOK)
        {
        bOK = setPortInfo(m_PortInfoSetTx_Input, m_PortInfoSetRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALsetPortInfo Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = setPortInfo(m_PortInfoSetTx_Output, m_PortInfoSetRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALsetPortInfo Output FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enableComponent(m_ComponentEnableTx, m_ComponentEnableRx);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALenableComponent FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_B, m_PortInfoGetRx_Input_B);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Input B FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_B, m_PortInfoGetRx_Output_B);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Output B BFAILED");
#endif
        }
    if (bOK)
        {
        bOK = setZeroCopyMode(m_PortInfoGetRx_Input_B, m_PortParamTx_Input, m_PortParamRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALsetZeroCopyMode Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = setZeroCopyMode(m_PortInfoGetRx_Output_B, m_PortParamTx_Output, m_PortParamRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALsetZeroCopyMode Output FAILED");
#endif
        }
    if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_INPUT, m_InputPortHandle, m_PortInfoGetTx_Input_C, m_PortInfoGetRx_Input_C);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Input C FAILED");
#endif
        }
if (bOK)
        {
        bOK = getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_C, m_PortInfoGetRx_Output_C);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALgetPortInfo Output C FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enablePort(m_PortInfoGetRx_Input_C, m_PortActionTx_Input, m_PortActionRx_Input);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALenablePort Input FAILED");
#endif
        }
    if (bOK)
        {
        bOK = enablePort(m_PortInfoGetRx_Output_C, m_PortActionTx_Output, m_PortActionRx_Output);
#ifdef __H264_DECODER_DEBUG_INIT__
        if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "\nMMALenablePort Output FAILED");
#endif
        }
    return bOK;

            /*
                queueInputBuffer        ( m_BufferFromHostTx_Input,         // ---------- BUFFERS ---------- 
                                            m_BufferFromHostRx_Input );

                queueOutputBuffer       ( m_BufferFromHostTx_OutputA,
                                            m_BufferFromHostRx_OutputA );

                queueOutputBuffer       ( m_BufferFromHostTx_OutputB,
                                            m_BufferFromHostRx_OutputB );
            */
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::createTextures       (   )
{
                int count = 0;

                glGenTextures(1, &m_Texture);
                if(!checkGLerror()) count++;
                glBindTexture(GL_TEXTURE_2D, m_Texture);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!checkGLerror()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!checkGLerror()) count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!checkGLerror()) count++;

                if( count != 0)
                    { 
#ifdef __H264_DECODER_DEBUG_INIT__ 
                    nextline( MY_BUFFER, MY_INDEX );                                 
                    storeLog( MY_BUFFER, MY_INDEX, "\nTexture A Creation FAILED");
#endif                    
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__             
                nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "\nTexture Creation SUCCESS");
#endif                
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::framePoller(u32 frame_offset, u32 frame_length)
{
#ifdef __H264_DECODER_DEBUG_RUNTIME__           // Bootstrap: prime first input buffer and snapshot port state (debug)
                if (!m_FirstFrameQueued)
                    {
                    if (!queueInputBuffer(m_BufferFromHostTx_Input, frame_offset, frame_length))
                        {
                        nextline( MY_BUFFER, MY_INDEX );
                        storeLog( MY_BUFFER, MY_INDEX, "very first frame queue error!", frame_offset, frame_length);
                        return false;
                        }
                    nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "very first frame queue SUCCESS", frame_offset, frame_length);

                    getPortInfo(MMAL_PORT_TYPE_INPUT,  m_InputPortHandle,  m_PortInfoGetTx_Input_D, m_PortInfoGetRx_Input_D);
                    getPortInfo(MMAL_PORT_TYPE_OUTPUT, m_OutputPortHandle, m_PortInfoGetTx_Output_D, m_PortInfoGetRx_Output_D);

                    m_FirstFrameQueued = true;
                    return true;
                    }
#endif
                const char*  message = "";
                uint32_t msg_len = 0;

    // ---------------------------------------------------------------------
    // Drain RX queue unconditionally (non-blocking)
    // ---------------------------------------------------------------------
                while (vchi_msg_dequeue(m_ServiceHandle, &m_BufferFromHostTx_Output, sizeof(m_BufferFromHostTx_Output), &msg_len, VCHI_FLAGS_NONE) == 0)
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, "offset / length / status / data", frame_offset, frame_length, m_BufferFromHostTx_Output.hdr.status, m_CurrentHandle);
#endif
                                    if (m_CurrentHandle != m_OutputBufferHandleA && m_CurrentHandle != m_OutputBufferHandleB)
                                        {
                                        continue; // as long as we really want to drain, break; will only proceed the switch (m_RxMsg.h.status) // unrelated buffer, keep draining
                                        }

                                    if (m_CurrentHandle == m_OutputBufferHandleA) // Ping-pong: requeue the other output buffer
                                        {
                                        if (!queueOutputBuffer(m_BufferFromHostTx_Output, m_OutputBufferHandleB, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (m_CurrentHandle == m_OutputBufferHandleB) 
                                        {
                                        if (!queueOutputBuffer(m_BufferFromHostTx_Output, m_OutputBufferHandleA, m_OutputBufferSize))
                                            return false;
                                        }
                                    if (!bufferReady(m_CurrentHandle))
                                        return false;
                                    if (!queueInputBuffer(m_BufferFromHostTx_Input, frame_offset, frame_length))
                                        return false;
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                                    message = "MMAL_MSG_STATUS_SUCCESS      - All is Fine";
                                    nextline( MY_BUFFER, MY_INDEX );
                                    storeLog( MY_BUFFER, MY_INDEX, message, frame_offset, frame_length);
#endif                        
                                    return true;
                                    }
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                            nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message, frame_offset, frame_length);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (BUFFER_TO_HOST)", &m_BufferFromHostTx_Output, msg_len);
#endif                
                            return false;
                            }

                        default: 
                            {
                            message = "UNEXPECTED MESSAGE";
#ifdef __H264_DECODER_DEBUG_RUNTIME__    
                            nextline( MY_BUFFER, MY_INDEX );
                            storeLog( MY_BUFFER, MY_INDEX, message, frame_offset, frame_length,m_BufferFromHostTx_Output.hdr.type, m_BufferFromHostTx_Output.hdr.status);
                            storeMsg( MY_BUFFER, MY_INDEX, "Poller ERROR (UNEXPECTED MESSAGE)", &m_BufferFromHostTx_Output, msg_len);
#endif
                            break;
                            }
                        }
#ifdef __H264_DECODER_DEBUG_RUNTIME__ 
                    nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "Unexpected Reply", frame_offset, frame_length);
                    storeMsg( MY_BUFFER, MY_INDEX, "Unexpected Reply", &m_BufferFromHostTx_Output, msg_len);
#endif        
                    }
#ifdef __H264_DECODER_DEBUG_RUNTIME__   
                nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "Nothing in the Pipeline", frame_offset, frame_length);    // Nothing relevant received
#endif
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::bufferReady(u32 handle)
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
#ifdef __H264_DECODER_DEBUG_RUNTIME__        
                    storeLog( MY_BUFFER, MY_INDEX, "\nEGLImage creation FAILED", handle);
#endif
                    return false;
                    }
                glBindTexture(GL_TEXTURE_2D, m_Texture);                    // Bind the EGLImage to the single public texture
                glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_EGLimage);
                glBindTexture(GL_TEXTURE_2D, 0);

                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::queueOutputBuffer     (   MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;  /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.data                          = vc_handle;
                tx.msg.buffer_header.alloc_size                    = alloc_size;
                tx.msg.buffer_header.length                        = 0;
                tx.msg.buffer_header.offset                        = 0;
                tx.msg.buffer_header.flags                         = 0;

#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog( MY_BUFFER, MY_INDEX, "\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueOutputBuffer", &tx, (u32)sizeof(tx));
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::queueInputBuffer      (   MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_BUFFER_FROM_HOST );

                tx.msg.drvbuf.client_context                       = tx.hdr.context;                /* patch dynamic fields into already-primed msg */

                tx.msg.buffer_header.offset                        = frame_offset;
                tx.msg.buffer_header.length                        = frame_length;

                u32 flags                                           = MMAL_BUFFER_HEADER_FLAG_FRAME | MMAL_BUFFER_HEADER_FLAG_KEYFRAME;
                tx.msg.buffer_header.flags                         = flags;
#ifdef __H264_DECODER_DEBUG_RUNTIME__
                storeLog( MY_BUFFER, MY_INDEX, "\nBUFFER FROM HOST MSG", (u32)sizeof(tx));     /* expected: sizeof(hdr)+268 */
                storeMsg( MY_BUFFER, MY_INDEX, "QueueInputBuffer", &tx, (u32)sizeof(tx));
#endif
                return (vchi_msg_queue(m_ServiceHandle, &tx, (u32)sizeof(tx), VCHI_FLAGS_BLOCK_UNTIL_QUEUED, nullptr) == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::createComponent       (   MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply    & rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_CREATE );

                tx.msg                                             = {};   /* prime TX msg */

                memset(tx.msg.name, 0, sizeof(tx.msg.name));
                strncpy(tx.msg.name, "ril.video_decode", sizeof(tx.msg.name) - 1);
                        
                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_createComponent(tx,rx);
#endif        
                m_ComponentHandle = rx.msg.component_handle;

                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::getPortInfo           (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply    & rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_GET );

                tx.msg                                             = {};       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_type                                   = port_type;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
                port_handle = rx.msg.port_handle;
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_getPortInfo(tx,rx);
#endif
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::setPortInfo           (   MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply    & rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_INFO_SET );

                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_setPortInfo(tx,rx); 
#endif                
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::enableComponent       (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply    & rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_COMPONENT_ENABLE );

                tx.msg                                             = {};                                                   /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enableComponent(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::setZeroCopyMode       (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply    & src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply    & rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_PARAMETER_SET );

                tx.msg = {};                                                       /* prime TX msg */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle; // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.msg.id                                          = MMAL_PARAMETER_ZERO_COPY;
                tx.msg.size                                        = sizeof(u32);

                memset(tx.msg.value, 0, sizeof(tx.msg.value));
                tx.msg.value[0] = 1;   /* enable zero-copy */

                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_setZeroCopyMode(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::enablePort            (   /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx)
{
                initHeader( tx.hdr, MMAL_MSG_TYPE_PORT_ACTION );

                tx.msg                                             = {};                                                   /* prime TX msg from GET snapshot */
                tx.msg.component_handle                            = m_ComponentHandle;
                tx.msg.port_handle                                 = src.msg.port_handle;                // port_handle - my original code takes it as parameter! i assume this is chosen because source has the correct handle
                tx.msg.action                                      = MMAL_MSG_PORT_ACTION_TYPE_ENABLE;
                tx.msg.port                                        = src.msg.port;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __H264_DECODER_DEBUG_INIT__
                    storeLog( MY_BUFFER, MY_INDEX, "\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif
                    return false;
                    }
#ifdef __H264_DECODER_DEBUG_INIT__
                Log_enablePort(tx,rx);
#endif        
                return (rx.msg.status == MMAL_MSG_STATUS_SUCCESS);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::PrimeOutputBufferBody     (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.msg = {};
                tx.msg.drvbuf.magic                                = MMAL_MAGIC;
                tx.msg.drvbuf.component_handle                     = m_ComponentHandle;
                tx.msg.drvbuf.port_handle                          = m_OutputPortHandle;

                tx.msg.buffer_header.pts_lo                        = 0;
                tx.msg.buffer_header.pts_hi                        = 0x80000000;
                tx.msg.buffer_header.dts_lo                        = 0;
                tx.msg.buffer_header.dts_hi                        = 0x80000000;

                tx.msg.is_zero_copy                                = 1;
                tx.msg.has_reference                               = 0;
                tx.msg.payload_in_message                          = 0;

#ifdef __H264_DECODER_DEBUG_INIT__
//             Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::PrimeInputBufferBody      (   MMAL_Buffer_From_Host_Msg& tx)
{
                tx.msg = {};
                tx.msg.drvbuf.magic                                = MMAL_MAGIC;
                tx.msg.drvbuf.component_handle                     = m_ComponentHandle;
                tx.msg.drvbuf.port_handle                          = m_InputPortHandle;

                tx.msg.buffer_header.data                          = m_InputBufferHandle;
                tx.msg.buffer_header.alloc_size                    = m_InputBufferSize;

                tx.msg.buffer_header.pts_lo                        = 0;
                tx.msg.buffer_header.pts_hi                        = 0x80000000;
                tx.msg.buffer_header.dts_lo                        = 0;
                tx.msg.buffer_header.dts_hi                        = 0x80000000;

                tx.msg.is_zero_copy                                = 1;
                tx.msg.has_reference                               = 0;
                tx.msg.payload_in_message                          = 0;

#ifdef __H264_DECODER_DEBUG_INIT__
//               Log_createComponent(tx);
#endif            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::setPortFormatInput( const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Info_Set_Msg& tx)
{
    // SET-prefix fields (must be explicit)
    tx.msg.component_handle = src.msg.component_handle;
    tx.msg.port_type        = src.msg.port_type;
    tx.msg.port_index       = src.msg.port_index;

    // Layout-compatible sub-structs
    tx.msg.port   = src.msg.port;
    tx.msg.format = src.msg.format;
    tx.msg.es     = src.msg.es;

    // Modifications
    tx.msg.port.buffer_num  = NUMBER_INPUTBUFFER;
    tx.msg.port.buffer_size = m_InputBufferSize;

    tx.msg.format.encoding         = MMAL_ENCODING_H264;
    tx.msg.format.encoding_variant = MMAL_ENCODING_VARIANT_H264_DEFAULT;

    tx.msg.es.video.width       = m_ResolutionX;
    tx.msg.es.video.height      = m_ResolutionY;
    tx.msg.es.video.crop.x      = 0;
    tx.msg.es.video.crop.y      = 0;
    tx.msg.es.video.crop.width  = m_ResolutionX;
    tx.msg.es.video.crop.height = m_ResolutionY;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::setPortFormatOutput( const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Info_Set_Msg& tx)
{
    // SET-prefix fields (must be explicit)
    tx.msg.component_handle = src.msg.component_handle;
    tx.msg.port_type        = src.msg.port_type;
    tx.msg.port_index       = src.msg.port_index;

    // Layout-compatible sub-structs
    tx.msg.port   = src.msg.port;
    tx.msg.format = src.msg.format;
    tx.msg.es     = src.msg.es;

    // Modifications
    tx.msg.port.buffer_num  = NUMBER_OUTPUTBUFFER;
    tx.msg.port.buffer_size = m_OutputBufferSize;

    tx.msg.format.encoding = MMAL_ENCODING_I420;

    tx.msg.es.video.width       = m_ResolutionX;
    tx.msg.es.video.height      = m_ResolutionY;
    tx.msg.es.video.crop.x      = 0;
    tx.msg.es.video.crop.y      = 0;
    tx.msg.es.video.crop.width  = m_ResolutionX;
    tx.msg.es.video.crop.height = m_ResolutionY;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
