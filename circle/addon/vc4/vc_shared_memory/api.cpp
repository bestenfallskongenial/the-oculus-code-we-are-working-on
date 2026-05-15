//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "vc_sm.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CVCSharedMemory::CVCSharedMemory     (   )
                : m_ServiceHandle(0)
                , m_TransactionId(0)
{
}
                CVCSharedMemory::~CVCSharedMemory    (   )
{
                if (m_ServiceHandle)
                    {
                    vchi_service_close(m_ServiceHandle);
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              USER API
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::initialize     (   )
{
#ifdef __VCSM_DEBUG_INIT__ 
                storeLog ( SERVICENAMESTRING, SERVICEVERSIONSTRING);    
#endif // __VCSM_DEBUG_INIT__
                getVCHI();

                if(!initEvents())
                    {
#ifdef __VCSM_DEBUG_INIT__ 
                    storeLog ( "\nVCOS Init FAILED!");      
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
                if(!openService( m_ServiceCreate ))
                    {
#ifdef __VCSM_DEBUG_INIT__            
                    storeLog ( "VCHI Init FAILED!");      
#endif // __VCSM_DEBUG_INIT__                    
                    return false;
                    }
#ifdef __VCSM_DEBUG_INIT__  
                storeLog ( "\nVCSM Successful Initialized");      
#endif // __VCSM_DEBUG_INIT__
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::importMemory   ( void* buffer, size_t size, int slot, vc_sm_import_msg& tx, vc_sm_import_result& rx)
{
                initHeader(tx.hdr, VC_SM_MSG_TYPE_IMPORT);

                tx.msg = {};
                tx.msg.type       = VC_SM_ALLOC_NON_CACHED;
                tx.msg.addr       = convertAddress(buffer, size);
                tx.msg.size       = static_cast<u32>(size);
                tx.msg.kernel_id  = 0;
                tx.msg.allocator  = 0;
                strncpy(tx.msg.name, "SMEM", sizeof(tx.msg.name));

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __VCSM_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
#ifdef __VCSM_DEBUG_INIT__
                Log_importMemory(tx,rx);
#endif // __VCSM_DEBUG_INIT__                            
                if (rx.res_handle != 0)
                    {
                    m_vc_handle[slot] = rx.res_handle;
                    return true;
                    }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::lockMemory     ( int slot, vc_sm_lock_msg& tx, vc_sm_lock_result_t& rx)
{
                initHeader(tx.hdr, VC_SM_MSG_TYPE_LOCK);

                tx.msg = {};
                tx.msg.res_handle = m_vc_handle[slot];
                tx.msg.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __VCSM_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
#ifdef __VCSM_DEBUG_INIT__
                Log_lockMemory(tx,rx);
#endif // __VCSM_DEBUG_INIT__                            
                if (rx.res_mem != 0)
                    {
                    m_vc_pointer[slot] = rx.res_mem;
                    return true;
                    }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CVCSharedMemory::freeMemory     ( int slot, vc_sm_free_msg& tx, vc_sm_result_t& rx)
{
                initHeader(tx.hdr, VC_SM_MSG_TYPE_FREE);

                tx.msg = {};
                tx.msg.res_handle = m_vc_handle[slot];
                tx.msg.res_mem    = 0;

                size_t rx_len = 0;

                if (!sendAndWait(&tx, sizeof(tx), &rx, sizeof(rx), &rx_len))
                    {
#ifdef __VCSM_DEBUG_INIT__
                    storeLog("\nMMALsendAndWait FAILED - MSG #", tx.hdr.context );
#endif // __VCSM_DEBUG_INIT__
                    return false;
                    }
#ifdef __VCSM_DEBUG_INIT__
                Log_freeMemory(tx,rx);
#endif // __VCSM_DEBUG_INIT__                            
                if (rx.success == 0)
                    {
                    m_vc_handle[slot]  = 0;
                    m_vc_pointer[slot] = 0;
                    return true;
                    }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------