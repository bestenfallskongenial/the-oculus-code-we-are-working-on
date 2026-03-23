//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::initializeVCSM     (   )
{
#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif // __DEBUG_LOG__
                getVCHIstate();

                if(!initEvents(m_EventSMEM, "SMEM"))
                    {
#ifdef __DEBUG_LOG__
                    nextline ( MY_BUFFER, MY_INDEX );
                    storeLog ( MY_BUFFER, MY_INDEX, "VCOS Init FAILED!");      
#endif // __DEBUG_LOG__
                    return false;
                    }
                if(!openService (
                                tx,
                                VC_SM_VER,
                                VC_SM_MIN_VER,
                                VCHIQ_MAKE_FOURCC('S','M','E','M'),
                                callbackVCSM,
                                &m_EventSMEM,
                                m_VCHIInstance,
                                m_ServiceHandleVCSM
                                ))
                    {
#ifdef __DEBUG_LOG__            
                    storeLog ( MY_BUFFER, MY_INDEX, "VCHI Init FAILED!");      
#endif // __DEBUG_LOG__                    
                    return false;
                    }
#ifdef __DEBUG_LOG__ 
                nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "VCSM Successful Initialized");      
#endif // __DEBUG_LOG__
                return true;
}
/*
                VCOS_EVENT_T m_EventSMEM;
                VCOS_EVENT_T m_EventMMAL;

                initEvents(m_EventSMEM, "SMEM");
                initEvents(m_EventMMAL, "MMAL");

                openService(
                    tx,
                    VC_SM_VER,
                    VC_SM_MIN_VER,
                    VCHIQ_MAKE_FOURCC('S','M','E','M'),
                    callbackCSM,
                    &m_EventSMEM,
                    m_VCHIInstance,
                    m_ServiceHandleVCSM
                );                
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::importMemoryVCSM   ( void* buffer, size_t size, int slot, vc_sm_import_msg& tx, vc_sm_import_result& rx)
{
                initHeaderVCSM(tx, VC_SM_MSG_TYPE_IMPORT);

                tx.body = {};
                tx.body.type       = VC_SM_ALLOC_NON_CACHED;
                tx.body.addr       = convertAddress(buffer, size);
                tx.body.size       = static_cast<u32>(size);
                tx.body.kernel_id  = 0;
                tx.body.allocator  = 0;
                strncpy(tx.body.name, "SMEM", sizeof(tx.body.name));

                size_t rx_len = 0;
/*
bool            CH264Decoder::sendAndWait           (   VCHI_SERVICE_HANDLE_T   ServiceHandle, V
                                                        COS_EVENT_T             &VCOSevent, 
                                                        const void              *msg, 
                                                        size_t                  msg_size, 
                                                        void                    *rx_msg, 
                                                        size_t                  max_reply_len, 
                                                        size_t                  *actual_reply_len ) // new for mmal and vcsm
*/
                if (!sendAndWait( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len ))
                    return false;

                if (rx.res_handle != 0)
                {
                    m_vc_handle[slot] = rx.res_handle;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Import VCSM Memory to Slot ", slot)  
                        storeLog( MY_BUFFER, MY_INDEX, "ARM Address / GPU Address / Size / VCSM Handle ", buffer, tx.body.addr, size, rx.res_handle);
#endif // __DEBUG_LOG__                    
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::lockMemoryVCSM     ( int slot, vc_sm_lock_msg& tx, vc_sm_lock_result_t& rx)
{
                initHeaderVCSM(tx, VC_SM_MSG_TYPE_LOCK);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];     // 
                tx.body.res_mem    = 0;

                size_t rx_len = 0;
/*
bool            CH264Decoder::sendAndWait           (   VCHI_SERVICE_HANDLE_T   ServiceHandle, V
                                                        COS_EVENT_T             &VCOSevent, 
                                                        const void              *msg, 
                                                        size_t                  msg_size, 
                                                        void                    *rx_msg, 
                                                        size_t                  max_reply_len, 
                                                        size_t                  *actual_reply_len ) // new for mmal and vcsm
*/
                if (!sendAndWait( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.res_mem != 0)
                {
                    m_vc_pointer[slot] = rx.res_mem;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Lock VCSM Memory in Slot ", slot)  
                        storeLog( MY_BUFFER, MY_INDEX, "VCSM Handle / VCSM Pointer ", rx.res_handle, rx.res_mem);
#endif // __DEBUG_LOG__        
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::freeMemoryVCSM     ( int slot, vc_sm_free_msg& tx, vc_sm_result_t& rx)
{
                initHeaderVCSM(tx, VC_SM_MSG_TYPE_FREE);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];
                tx.body.res_mem    = 0;

                size_t rx_len = 0;
/*
bool            CH264Decoder::sendAndWait           (   VCHI_SERVICE_HANDLE_T   ServiceHandle, V
                                                        COS_EVENT_T             &VCOSevent, 
                                                        const void              *msg, 
                                                        size_t                  msg_size, 
                                                        void                    *rx_msg, 
                                                        size_t                  max_reply_len, 
                                                        size_t                  *actual_reply_len ) // new for mmal and vcsm
*/
                if (!sendAndWait( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.success == 0)
                {
                    m_vc_handle[slot]  = 0;
                    m_vc_pointer[slot] = 0;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Free VCSM Memory in Slot ", slot)  
#endif // __DEBUG_LOG__       
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#undef __DEBUG_LOG__