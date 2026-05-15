//----------------------------------------------------------------------------------------------------------------------------------------------------
//      vc_sm_class.h
//----------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _VC_SM_CLASS_H
#define _VC_SM_CLASS_H

#include "vc_sm_defs.h"

#include <circle/types.h>                                                           // for u32, size_t, etc.

#include "interface/vcos/vcos.h"
#include <vc4/interface/vcinclude/common.h>
#include <vc4/vchi/vchi.h>                                                          // for VCHI_INSTANCE_T, VCHI_CONNECTION_T, VCHI_SERVICE_HANDLE_T, VCHI_CALLBACK_REASON_T
#include <vc4/vchiq/vchiq.h>

// #include <circle/logger.h>

// #include "interface/vcos/vcos_event.h"


#include <circle/bcm2835.h>

#define VCSMLOG

#define DEBUG_MAX_LENGTH (1024 * 16)

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);

struct vc_sm_import_msg
{
    vc_sm_msg_hdr_t hdr;
    vc_sm_import body;
};

struct vc_sm_import_reply
{
    vc_sm_import_result body;
};

struct vc_sm_lock_msg
{
    vc_sm_msg_hdr_t hdr;
    vc_sm_lock_unlock_t body;
};

struct vc_sm_lock_reply
{
    vc_sm_lock_result_t body;
};

struct vc_sm_free_msg
{
    vc_sm_msg_hdr_t hdr;
    vc_sm_free_t body;
};

struct vc_sm_free_reply
{
    vc_sm_result_t body;
};

class CVCSharedMemory
{
public:
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              CONSTRUCTOR / DECONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------------------------------------
                CVCSharedMemory     (   );
               ~CVCSharedMemory     (   );
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              USER API
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool initialize    ( );
bool importMemory  ( void* buffer, size_t size, int slot, vc_sm_import_msg& tx, vc_sm_import_result& rx);
bool lockMemory    ( int slot, vc_sm_lock_msg& tx, vc_sm_lock_result_t& rx);
bool freeMemory    ( int slot, vc_sm_free_msg& tx, vc_sm_result_t& rx);

private:
static void callback       ( void *callback_param, VCHI_CALLBACK_REASON_T reason, void *msg_handle );
u32         nextId         ( u32 &tid );
void        getVCHI        ( );
bool        initEvents     ( );
u32         convertAddress ( void* buffer, size_t size );
void        initHeader     ( vc_sm_msg_hdr_t& tx, u16 type );
bool        sendAndWait    ( const void *msg, size_t msg_size, void *rx_msg, size_t max_reply_len, size_t *actual_reply_len );
bool        openService    ( SERVICE_CREATION_T &tx );

#ifdef VCSMLOG
void storeLog              ( const char* label, u32 value1 = EMPTYLOG, u32 value2 = EMPTYLOG, u32 value3 = EMPTYLOG, u32 value4 = EMPTYLOG );
void storeMsg              ( const char* label, const void* tx_msg, u32 total_size );
void nextline              ( );
void Log_openService       ( const SERVICE_CREATION_T &src );
void Log_importMemory      ( const vc_sm_import_msg& tx, const vc_sm_import_result& rx );
void Log_lockMemory        ( const vc_sm_lock_msg& tx, const vc_sm_lock_result_t& rx );
void Log_freeMemory        ( const vc_sm_free_msg& tx, const vc_sm_result_t& rx );
#endif

private:
        VCHI_INSTANCE_T                                     m_VCHIInstance;
        VCHI_CONNECTION_T*                                  m_Connection;
        VCOS_EVENT_T                                        m_VCOSevent;
        VCHI_SERVICE_HANDLE_T                               m_ServiceHandle;
        u32                                                 m_TransactionId;
        SERVICE_CREATION_T                                 m_ServiceCreate = {};
        vc_sm_import_msg                                    m_ImportTx = {};
        vc_sm_import_reply                                  m_ImportRx = {};
        vc_sm_lock_msg                                      m_LockTx = {};
        vc_sm_lock_reply                                    m_LockRx = {};
        vc_sm_free_msg                                      m_FreeTx = {};
        vc_sm_free_reply                                    m_FreeRx = {};

public:
        u32                                                 m_vc_handle[MAX_BUFFER] = {0};
        u32                                                 m_vc_pointer[MAX_BUFFER] = {0};

        u32                                                 m_CharIndex = 0;
        char                                                m_DebugCharArray[MAX_DEBUG_FILE_LENGTH] = { 0 };
};
#endif