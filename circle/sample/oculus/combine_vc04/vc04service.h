#ifndef _VC04SERVICE_H
#define _VC04SERVICE_H

#include "../vc_sharedMemory_defs.h"
#include "../vc_wireMMAL_defs.h"

#include <circle/types.h>

#include "interface/vcos/vcos.h"
#include <vc4/vchi/vchi.h>

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);

// VCSM wrapper messages, matching the "header + body" pattern already used by MMAL.
struct vc_sm_import_msg
{
        vc_sm_msg_hdr_t                 hdr;
        vc_sm_import                    body;
};

struct vc_sm_import_reply
{
        vc_sm_import_result             body;
};

struct vc_sm_lock_msg
{
        vc_sm_msg_hdr_t                 hdr;
        vc_sm_lock_unlock_t             body;
};

struct vc_sm_lock_reply
{
        vc_sm_lock_result_t             body;
};

struct vc_sm_free_msg
{
        vc_sm_msg_hdr_t                 hdr;
        vc_sm_free_t                    body;
};

struct vc_sm_free_reply
{
        vc_sm_result_t                  body;
};

// Draft extraction target for the shared VC04 helpers currently living in CKernel.
class CVC04Service
{
public:
        bool initializeVCSM                                       (   );
        bool importMemoryVCSM                                     (   void* buffer,
                                                                    size_t size,
                                                                    int slot,
                                                                    vc_sm_import_msg& tx,
                                                                    vc_sm_import_reply& rx );
        bool lockMemoryVCSM                                       (   int slot,
                                                                    vc_sm_lock_msg& tx,
                                                                    vc_sm_lock_reply& rx );
        bool freeMemoryVCSM                                       (   int slot,
                                                                    vc_sm_free_msg& tx,
                                                                    vc_sm_free_reply& rx );

private:
static  void callbackVCSM                                        (   void *callback_param,
                                                                    VCHI_CALLBACK_REASON_T reason,
                                                                    void *msg_handle );
static  void callbackMMAL                                        (   void *callback_param,
                                                                    VCHI_CALLBACK_REASON_T reason,
                                                                    void *msg_handle );
        void getVCHIstate                                         (   );
        bool initEvents                                           (   VCOS_EVENT_T &event,
                                                                    const char* name );
        u32  NextTransId                                          (   u32 &tid );
        void initHeaderVCSM                                       (   vc_sm_msg_hdr_t& tx,
                                                                    u16 type );
        void initHeaderMMAL                                       (   mmal_msg_header& hdr,
                                                                    u32 type );
        bool sendAndWait                                          (   VCHI_SERVICE_HANDLE_T ServiceHandle,
                                                                    VCOS_EVENT_T &VCOSevent,
                                                                    const void *msg,
                                                                    size_t msg_size,
                                                                    void *rx_msg,
                                                                    size_t max_reply_len,
                                                                    size_t *actual_reply_len );
        bool openService                                          (   SERVICE_CREATION_T &tx,
                                                                    uint32_t serviceVersion,
                                                                    uint32_t serviceVersionMin,
                                                                    int32_t service_id,
                                                                    VCHI_CALLBACK_T cb,
                                                                    void *cb_param,
                                                                    VCHI_INSTANCE_T VCHIInstance,
                                                                    VCHI_SERVICE_HANDLE_T& ServiceHandle );
        u32  convertAddress                                       (   void* buffer,
                                                                    size_t size );

private:
        VCHI_INSTANCE_T                 m_VCHIInstance   = 0;
        VCHI_CONNECTION_T*              m_Connection     = 0;
        VCOS_EVENT_T                    m_EventSMEM      = {};
        VCOS_EVENT_T                    m_EventMMAL      = {};
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleVCSM = 0;
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleMMAL = 0;
        u32                             m_TransactionId  = 0;
        u32                             m_vc_handle[MAX_BUFFER]  = {0};
        u32                             m_vc_pointer[MAX_BUFFER] = {0};
};

#endif