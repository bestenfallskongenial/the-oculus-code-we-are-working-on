//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::initializeVCSM     (   )
{
bool bOK = true;

#ifdef __DEBUG_LOG__
                storeLog ( MY_BUFFER, MY_INDEX, SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif // __DEBUG_LOG__

                getStateVCHI                ();
                if (bOK)
                    {
                    bOK = initEventsVCOS(m_EventSMEM, "SMEM");
#ifdef __DEBUG_LOG__
                    if (!bOK) storeLog( MY_BUFFER, MY_INDEX, "VCSM initEventsVCOS FAILED");
#endif // __DEBUG_LOG__
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
#endif // __DEBUG_LOG__                    
                    if (!bOK) return false;
                    }
#ifdef __DEBUG_LOG__ 
                nextline ( MY_BUFFER, MY_INDEX );
                storeLog ( MY_BUFFER, MY_INDEX, "VCSM Successful Initialized");
                storeLog ( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");                
#endif // __DEBUG_LOG__
                return bOK;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::importMemoryVCSM   ( void* buffer, size_t size, int slot, VCSM_Import_MEM_Msg& tx, VCSM_Import_MEM_Reply& rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_IMPORT);

                tx.body = {};
                tx.body.type       = VC_SM_ALLOC_NON_CACHED;
                tx.body.addr       = convertAddress(buffer, size);
                tx.body.size       = static_cast<u32>(size);
                tx.body.kernel_id  = 0;
                tx.body.allocator  = 0;
                strncpy(tx.body.name, "SMEM", sizeof(tx.body.name));

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len ))
                    return false;

                if (rx.body.res_handle != 0)
                {
                    m_vc_handle[slot] = rx.body.res_handle;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Import VCSM Memory to Slot ", slot); 
                        storeLog( MY_BUFFER, MY_INDEX, "ARM Address / GPU Address / Size / VCSM Handle ", buffer, tx.body.addr, size, rx.body.res_handle);
#endif // __DEBUG_LOG__                    
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::lockMemoryVCSM     ( int slot, VCSM_Lock_MEM_Msg& tx, VCSM_Lock_MEM_Reply& rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_LOCK);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];     // 
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.body.res_mem != 0)
                {
                    m_vc_pointer[slot] = rx.body.res_mem;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Lock VCSM Memory in Slot   ", slot);  
                        storeLog( MY_BUFFER, MY_INDEX, "VCSM Handle / VCSM Pointer ", rx.body.res_handle, rx.body.res_mem);
#endif // __DEBUG_LOG__        
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::freeMemoryVCSM     ( int slot, VCSM_Free_MEM_Msg& tx, VCSM_Free_MEM_Reply& rx)
{
                initHeaderVCSM(tx.hdr, VC_SM_MSG_TYPE_FREE);

                tx.body = {};
                tx.body.res_handle = m_vc_handle[slot];
                tx.body.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWaitVCHI( m_ServiceHandleVCSM, m_EventSMEM, &tx, sizeof(tx), &rx, sizeof(rx), &rx_len) )
                    return false;

                if (rx.body.success == 0)
                {
                    m_vc_handle[slot]  = 0;
                    m_vc_pointer[slot] = 0;
#ifdef __DEBUG_LOG__
                        nextline( MY_BUFFER, MY_INDEX );  
                        storeLog( MY_BUFFER, MY_INDEX, "Free VCSM Memory in Slot ", slot);  
#endif // __DEBUG_LOG__       
                    return true;
                }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
#undef __DEBUG_LOG__