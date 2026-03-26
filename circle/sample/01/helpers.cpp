//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::callbackVCSM       ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )   // new for mmal and vcsm and what the hell is msg_handle?? i need sleep!
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;   // how do ii ensure that this callback takes as reason only the initEventsVCOS(m_EventSMEM, "SMEM"); and not initEventsVCOS(m_EventMMAL, "MMAL");? explain!
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::callbackMMAL       ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )   // new for mmal and vcsm
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::getStateVCHI        ( )    // new for mmal and vcsm
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                         //1. get the VCHI instance and the connection handle from bcm_host.h
#ifdef __DEBUG_LOG__
                nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "\nVCHI State Instance     ", (u32)m_VCHIInstance);   
                storeLog ( MY_BUFFER, MY_INDEX, "VCHI State Connection   ", (u32)m_Connection);   
#endif // __DEBUG_LOG__            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::initEventsVCOS(VCOS_EVENT_T &event, const char* name)
{
                if (vcos_event_create(&event, name) != VCOS_SUCCESS)
                    {
#ifdef __DEBUG_LOG__
                    nextline ( MY_BUFFER, MY_INDEX );
                    storeLog ( MY_BUFFER, MY_INDEX, "\nVCOS Event Init Failed!", (u32)&event);  
#endif // __DEBUG_LOG__              
                    return false;
                    }
                return true;
}
/*
                VCOS_EVENT_T m_EventSMEM;
                VCOS_EVENT_T m_EventMMAL;

                initEventsVCOS(m_EventSMEM, "SMEM");
                initEventsVCOS(m_EventMMAL, "MMAL");
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CKernel::convertAddress ( void* buffer, size_t size )
{
                u32 bus_addr = BUS_ADDRESS(reinterpret_cast<uintptr_t>(buffer));
                u32 vcsm_addr = (bus_addr & ~0xC0000000) | 0xC0000000;
#ifdef __DEBUG_LOG__
                nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "\nBuffer Address USR", (u32)buffer); 
                storeLog ( MY_BUFFER, MY_INDEX, "Buffer Address ARM", (u32)bus_addr); 
                storeLog ( MY_BUFFER, MY_INDEX, "Buffer Address VPU", (u32)vcsm_addr); 
#endif // __DEBUG_LOG__
                CleanAndInvalidateDataCacheRange((uintptr_t)(buffer), size);

                return vcsm_addr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::checkGLerrorMMAL                  (   )
{
                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    const char* error_str;
                    
                    switch(error) 
                        {
                        case        GL_INVALID_ENUM:                    error_str = "GL_INVALID_ENUM"; break;
                        case        GL_INVALID_VALUE:                   error_str = "GL_INVALID_VALUE"; break;
                        case        GL_INVALID_OPERATION:               error_str = "GL_INVALID_OPERATION"; break;
                        case        GL_OUT_OF_MEMORY:                   error_str = "GL_OUT_OF_MEMORY"; break;
                        case        GL_INVALID_FRAMEBUFFER_OPERATION:   error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                        default:                                        error_str = "UNKNOWN_ERROR"; break;
                        }
#ifdef __DEBUG_LOG__  
                    nextline ( MY_BUFFER, MY_INDEX );
                    storeLog ( MY_BUFFER, MY_INDEX, error_str);
#endif // __DEBUG_LOG__
                    return false;
                    }
                return true;    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CKernel::NextTransId         ( u32 &tid )   // new for mmal and vcsm
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initHeaderVCSM             (   vc_sm_msg_hdr_t& hdr, u32 type)
{␊
                hdr                 = {};
                hdr.type            = type;
                hdr.trans_id        = NextTransId(m_TransactionId);
}␊
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initHeaderMMAL            (   mmal_msg_header& hdr, u32 type)
{
                hdr                 = {};
                hdr.magic           = MMAL_MAGIC;
                hdr.type            = type;
                hdr.control_service = 0;
                hdr.context         = NextTransId(m_TransactionId);
                hdr.status          = 0;
                hdr.padding         = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::sendAndWaitVCHI                (   VCHI_SERVICE_HANDLE_T   ServiceHandle, 
                                                            VCOS_EVENT_T             &VCOSevent, 
                                                            const void              *msg, 
                                                            size_t                  msg_size, 
                                                            void                    *rx_msg, 
                                                            size_t                  max_reply_len, 
                                                            size_t                  *actual_reply_len ) // new for mmal and vcsm
{
#ifdef __DEBUG_LOG__
                nextline( MY_BUFFER, MY_INDEX );
                storeLog( MY_BUFFER, MY_INDEX, "TX MSG", (u32)msg_size);
                storeMsg( MY_BUFFER, MY_INDEX, "Raw TX", msg, msg_size);
#endif // __DEBUG_LOG__

                if (vchi_msg_queue(ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );    
                        storeLog( MY_BUFFER, MY_INDEX, "RX MSG", ReplyLength);
                        storeMsg( MY_BUFFER, MY_INDEX, "Raw RX", rx_msg, ReplyLength);
#endif // __DEBUG_LOG__
                        break;
                        }
                    } 
                while (vcos_event_wait(&VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                if (ReplyLength != max_reply_len)               /* enforce completeness HERE */
                    {
#ifdef __DEBUG_LOG__
                //  const mmal_msg_header* h = (const mmal_msg_header*)msg; 
                //  nextline( MY_BUFFER, MY_INDEX );
                    storeLog( MY_BUFFER, MY_INDEX, "ANSWER TO SHORT - MSG #" /*, h->context*/ ); // is only available with mmal, not for vcsm
#endif // __DEBUG_LOG__
                    return false;
                    }
                return true;                                    //  return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::openServiceVCHI(   SERVICE_CREATION_T      &tx, 
                                        uint32_t                serviceVersion, 
                                        uint32_t                serviceVersionMin, 
                                        int32_t                 service_id, 
                                        VCHI_CALLBACK_T         cb, 
                                        void                    *cb_param, 
                                        VCHI_INSTANCE_T         VCHIInstance, 
                                        VCHI_SERVICE_HANDLE_T&  ServiceHandle ) // sure about uint32_t service_id for VCHIQ_MAKE_FOURCC??

{

            //  SERVICE_CREATION_T tx = {}; // why not here?!
                tx.version.version          = serviceVersion;
                tx.version.version_min      = serviceVersionMin;
                tx.service_id               = service_id;
                tx.connection               = m_Connection;

                tx.rx_fifo_size             = 0;
                tx.tx_fifo_size             = 0;

                tx.callback                 = cb;
                tx.callback_param           = cb_param;

                tx.want_unaligned_bulk_rx   = 0;
                tx.want_unaligned_bulk_tx   = 0;
                tx.want_crc                 = 0;

                int rc = vchi_service_open(VCHIInstance, &tx, &ServiceHandle);
                return (rc == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#undef __DEBUG_LOG__