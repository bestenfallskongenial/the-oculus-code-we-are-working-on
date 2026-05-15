#include "vc_sm.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::callBack       ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle )
{
                VCOS_EVENT_T *event = (VCOS_EVENT_T *)callback_param;
                if (reason == VCHI_CALLBACK_MSG_AVAILABLE && event)
                    {
                    vcos_event_signal(event);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::initHeader     (vc_sm_msg_hdr_t& hdr, u32 type)
{
                hdr                     = {};
                hdr.type                = type;
                hdr.trans_id            = nextId(m_TransactionId);
            //  hdr.body[0]             = what do we do about you?!
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CVCSharedMemory::NextTransId         ( u32 &tid )
{
                tid = ( tid+1 ) & ~0x80000000u;                                             // mask for async messages really needed ?!                        
                return tid;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::getVCHIstate   ( )
{
                vc_host_get_vchi_state(&m_VCHIInstance, &m_Connection);                     //1. get the VCHI instance and the connection handle from bcm_host.h
#ifdef __VCSM_DEBUG_INIT__
                storeLog ( "\nVCHI State Instance & Connection  ", (u32)m_VCHIInstance, (u32)m_Connection);                 
##endif // __VCSM_DEBUG_INIT__
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::initEvents     ( )
{
                if (vcos_event_create(&m_VCOSevent, "SMEM") != VCOS_SUCCESS)
                    {
#ifdef __VCSM_DEBUG_INIT__ 
                    storeLog ( "\nVCOS Event Init FAILED!   ");                        
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
#ifdef __VCSM_DEBUG_INIT__ 
                storeLog ( "\nVCOS Event Init SUCCESS!  ", (u32)&m_VCOSevent);
#endif // __VCSM_DEBUG_INIT__                
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
u32             CVCSharedMemory::convertAddress ( void* buffer, size_t size )
{
                u32 bus_addr = BUS_ADDRESS(reinterpret_cast<uintptr_t>(buffer));
                u32 vcsm_addr = (bus_addr & ~0xC0000000) | 0xC0000000;
#ifdef __VCSM_DEBUG_INIT__
                nextline();
                storeLog ( "\nBuffer Address User / ARM / VPU", (u32)buffer, (u32)bus_addr, (u32)vcsm_addr);
#endif // __VCSM_DEBUG_INIT__
                CleanAndInvalidateDataCacheRange((uintptr_t)(buffer), size);

                return vcsm_addr;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::sendAndWait    (   const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len )
{
#ifdef __VCSM_DEBUG_INIT__
                nextline();
                storeLog("TX MSG", (u32)msg_size);
                storeMsg( "Raw TX", msg, msg_size);
#endif // __VCSM_DEBUG_INIT__

                if (vchi_msg_queue(m_ServiceHandle, msg, msg_size, VCHI_FLAGS_BLOCK_UNTIL_QUEUED, NULL) != 0)
                    {
                    return false;
                    }
                uint32_t ReplyLength = 0;
                do {
                    if (vchi_msg_dequeue(m_ServiceHandle, rx_msg, max_reply_len, &ReplyLength, VCHI_FLAGS_NONE) == 0)
                        {
#ifdef __VCSM_DEBUG_INIT__
                        nextline();
                        storeLog("RX MSG", ReplyLength);
                        storeMsg("Raw RX", rx_msg, ReplyLength);
#endif // __VCSM_DEBUG_INIT__
                        break;
                        }
                    }
                while (vcos_event_wait(&m_VCOSevent) == VCOS_SUCCESS);

                if (actual_reply_len)
                    {
                    *actual_reply_len = ReplyLength;
                    }
                if (ReplyLength != max_reply_len)               /* enforce completeness HERE */
                    {
#ifdef __VCSM_DEBUG_INIT__
//                  const mmal_msg_header* h = (const mmal_msg_header*)msg;
                    nextline();
                    storeLog("MMALsendAndWait ANSWER TO SHORT - MSG #" /*, h->context*/ );
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
                return true;                                    //  return (ReplyLength != 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::openService    ( SERVICE_CREATION_T &tx)
{
                tx.version.version                                  = VC_MMAL_VER;
                tx.version.version_min                              = VC_MMAL_MIN_VER;
                tx.service_id                                       = VCHIQ_MAKE_FOURCC('S','M','E','M');
                tx.connection                                       = m_Connection;
                tx.rx_fifo_size                                     = 0;
                tx.tx_fifo_size                                     = 0;
                tx.callback                                         = callBack;
                tx.callback_param                                   = &m_VCOSevent;
                tx.want_unaligned_bulk_rx                           = 0;
                tx.want_unaligned_bulk_tx                           = 0;
                tx.want_crc                                         = 0;

                int rc = vchi_service_open( m_VCHIInstance, &tx, &m_ServiceHandle );
#ifdef __VCSM_DEBUG_INIT__
                Log_openService(tx);
#endif
                return (rc == 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------