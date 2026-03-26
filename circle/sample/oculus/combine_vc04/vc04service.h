//----------------------------------------------------------------------------------------------------------------------------------------------------
// this is kinda a note file, all this will get into my ckernel.h file, i only gather the needed elements here!!!
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include <circle/types.h>                                                           // for u32, size_t, etc.

#include "vc_sm_defs.h"
#include "h264_decoder_defs.h"

#include "interface/vcos/vcos.h"
#include "interface/vcos/vcos_event.h"          // ?
#include <vc4/interface/vcinclude/common.h>     // ?
#include <vc4/vchi/vchi.h>
#include <vc4/vchiq/vchiq.h>
#include <circle/bcm2835.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <GLES2/gl2.h>
#include <GLES/glext.h>
#include <GLES2/gl2ext.h>

#define EMPTYLOG 255
#define DEBUG_MAX_LENGTH (1024 * 16)

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
//----------------------------------------------------------------------------------------------------------------------------------------------------
// this are CKernel methods!
//----------------------------------------------------------------------------------------------------------------------------------------------------
{
public:
        bool initializeVCSM                                       (   );
        bool importMemoryVCSM                                     (     void*                   buffer,
                                                                        size_t                  size,
                                                                        int                     slot,
                                                                        VCSM_Import_MEM_Msg&       tx,
                                                                        VCSM_Import_MEM_Reply&     rx );

        bool lockMemoryVCSM                                       (     int                     slot,
                                                                        VCSM_Lock_MEM_Msg&         tx,
                                                                        VCSM_Lock_MEM_Reply    &       rx );

        bool freeMemoryVCSM                                       (     int                     slot,
                                                                        VCSM_Free_MEM_Msg&         tx,
                                                                        VCSM_Free_MEM_Reply    &       rx );

private:
static  void callbackVCSM                                        (      void                    *callback_param,
                                                                        VCHI_CALLBACK_REASON_T  reason,
                                                                        void                    *msg_handle );

static  void callbackMMAL                                        (      void                    *callback_param,
                                                                        VCHI_CALLBACK_REASON_T  reason,
                                                                        void                    *msg_handle );

        void getStateVCHI                                         (   );
        bool initEventsVCOS                                       (     VCOS_EVENT_T            &event,
                                                                        const char*             name );

        u32  NextTransId                                          (     u32                     &tid );

        void initHeaderVCSM                                       (     vc_sm_msg_hdr_t         &tx,
                                                                        u32                     type );

        void initHeaderMMAL                                       (     mmal_msg_header         &hdr,
                                                                        u32                     type );

        bool sendAndWaitVCHI                                      (     VCHI_SERVICE_HANDLE_T   ServiceHandle,
                                                                        VCOS_EVENT_T            &VCOSevent,
                                                                        const void              *msg,
                                                                        size_t                  msg_size,
                                                                        void                    *rx_msg,
                                                                        size_t                  max_reply_len,
                                                                        size_t                  *actual_reply_len );

        bool openServiceVCHI                                      (     SERVICE_CREATION_T      &tx,
                                                                        uint32_t                serviceVersion,
                                                                        uint32_t                serviceVersionMin,
                                                                        int32_t                 service_id,
                                                                        VCHI_CALLBACK_T         cb,
                                                                        void                    *cb_param,
                                                                        VCHI_INSTANCE_T         VCHIInstance,
                                                                        VCHI_SERVICE_HANDLE_T   &ServiceHandle );

        u32  convertAddress                                       (     void                    *buffer,
                                                                        size_t                  size );
//----------------------------------------------------------------------------------------------------------------------------------------------------                                                                        
        bool initializeMMAL                                       (     u32         InBufferHandle,         // my input buffer handle from smem
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
                                                                        EGLContext  eglContext );           // EGL rendering context
        bool createTexturesMMAL                                       (   );
        bool framePollerMMAL                                          (     u32 frame_offset, u32 frame_length);
        //----------------------------------------------------------------------------------------------------------------------------------------------------
        private:
        bool bufferReadyMMAL                                          (     u32 handle);
        bool queueOutputBufferMMAL                                    (     MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size);
        bool queueInputBufferMMAL                                     (     MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length);
        //----------------------------------------------------------------------------------------------------------------------------------------------------
        bool checkGLerrorMMAL                                         (   );
        //----------------------------------------------------------------------------------------------------------------------------------------------------
        bool createComponent                                      (     MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply    & rx);
        bool getPortInfoMMAL                                          (     u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply    & rx);
        bool setPortInfoMMAL                                          (     MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply    & rx);
        bool enableComponentMMAL                                      (     MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply    & rx);
        bool setZeroCopyModeMMAL                                      (     /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply    & src,  MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply    & rx);
        bool enablePortMMAL                                           (     /*u32 port_handle,*/ const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx);

        void PrimeOutputBufferBodyMMAL                                (     MMAL_Buffer_From_Host_Msg& tx);
        void PrimeInputBufferBodyMMAL                                 (     MMAL_Buffer_From_Host_Msg& tx);

        void primePortFormatInputMMAL                                   (     const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Info_Set_Msg& tx);
        void primePortFormatOutputMMAL                                  (     const MMAL_Port_Info_Get_Reply    & src, MMAL_Port_Info_Set_Msg& tx);
        //----------------------------------------------------------------------------------------------------------------------------------------------------
        #ifdef __H264_DECODER_DEBUG_INIT__
        void Log_openService                                      (     const SERVICE_CREATION_T &src);
        void Log_createComponent                                  (     const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply    & rx);
        void Log_getPortInfo                                      (     const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply    & rx);
        void Log_setPortInfo                                      (     const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply    & rx);
        void Log_enableComponentMMAL                                  (     const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply    & rx);
        void Log_enablePort                                       (     const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx);
        void Log_setZeroCopyModeMMAL                                  (     const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply    & rx);
        void Log_BufferFromHost                                   (     const MMAL_Buffer_From_Host_Msg& rx);
#endif
//      void Log_BufferBody                                       (     const mmal_msg_buffer_from_host_wire32& msg );
//      void Log_queueBufferFromHost                              (     const MMAL_Buffer_From_Host_Msg& tx );
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// VCSM members
private:
        VCHI_INSTANCE_T                 m_VCHIInstance                  = 0;
        VCHI_CONNECTION_T*              m_Connection                    = 0;
        VCOS_EVENT_T                    m_EventSMEM                     = {};
        VCOS_EVENT_T                    m_EventMMAL                     = {};
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleVCSM             = 0;
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleMMAL             = 0;
        u32                             m_TransactionId                 = 0;
        u32                             m_vc_handle[MAX_BUFFER]         = {0};
        u32                             m_vc_pointer[MAX_BUFFER]        = {0};
// ---------------------------------------------------------------------------------------------------------------------------------------------------
public:
        GLuint                          m_Texture = 0;                                              // must be exposed
private:
        bool                            m_FirstFrameQueued              = false;    
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
        u32                             m_InputBufferHandle;
        u32                             m_InputBufferPointer;

        u32                             m_InputBufferSize;

        u32                             m_OutputBufferHandleA;
        u32                             m_OutputBufferPointerA;

        u32                             m_OutputBufferHandleB;
        u32                             m_OutputBufferPointerB;

        u32                             m_OutputBufferSize;

        u32                             m_ResolutionX;
        u32                             m_ResolutionY;

        u32                             m_ComponentHandle;

        u32                             m_VCSMHandleA;
        u32                             m_VCSMHandleB;

        u32                             m_InputPortHandle;
        u32                             m_OutputPortHandle;  
        
        u32                             m_CurrentHandle;

        EGLDisplay                      m_eglDisplay;                                               // egl display handle 
        EGLContext                      m_eglContext;                                               // egl contex handle
        EGLImageKHR                     m_EGLimage                      = nullptr;
//----------------------------------------------------------------------------------------------------------------------------------------------------        
// VCSM predefined messages as public member here
//----------------------------------------------------------------------------------------------------------------------------------------------------
public:
        SERVICE_CREATION_T              m_ServiceCreateVCSM             = {};

        VCSM_Import_MEM_Msg             m_importTxVCSM                  = {};
        VCSM_Import_MEM_Reply           m_importRxVCSM                  = {};

        VCSM_Lock_MEM_Msg               m_lockTxVCSM                    = {};
        VCSM_Lock_MEM_Reply             m_lockRxVCSM                    = {};

        VCSM_Free_MEM_Msg               m_freeTxVCSM                    = {};
        VCSM_Free_MEM_Reply             m_freeRxVCSM                    = {};
//----------------------------------------------------------------------------------------------------------------------------------------------------        
// MMAL predefined messages as public member here
//----------------------------------------------------------------------------------------------------------------------------------------------------
        //  0. Open MMAL service (transport only)
        SERVICE_CREATION_T              m_ServiceCreateMMAL             = {};   //  SEND                    - sent via `openServiceVCHI()`
                                                                                        //  RECEIVE none            - open VCHI/MMAL control channel. No MMAL protocol yet.
        //  1. Create component
        MMAL_Component_Create_Msg       m_ComponentCreateTx             = {};   //  SEND
        MMAL_Component_Create_Reply     m_ComponentCreateRx             = {};   //  RECEIVE                 - create `ril.video_decode`, receive `component_handle`. This handle is stored and used everywhere later.
        //  2. Snapshot A – get initial port state (before any modification)
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_A         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_A         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_A        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_A        = {};   //  Output port RECEIVE     - Snapshot A is the **authoritative VPU-owned baseline**. You must base `PORT_INFO_SET` on this snapshot.
        //  3. Build SET messages locally (nothing sent yet)
        MMAL_Port_Info_Set_Msg          m_PortInfoSetTx_Input           = {};   //  Input port WRITE

        MMAL_Port_Info_Set_Msg          m_PortInfoSetTx_Output          = {};   //  Output port WRITE       - * This step **does not send anything**. It only prepares TX structs using Snapshot A.
        //  4. Send SET (this is where data from step 3 is sent)
        // MMAL_Port_Info_Set_Msg       m_PortInfoSetTx_Input           = {};   //  Input port SEND
        MMAL_Port_Info_Set_Reply        m_PortInfoSetRx_Input           = {};   //  Input port RECEIVE

        // MMAL_Port_Info_Set_Msg       m_PortInfoSetTx_Output          = {};   //  Output port SEND
        MMAL_Port_Info_Set_Reply        m_PortInfoSetRx_Output          = {};   //  Output port RECEIVE     - Commit format/buffer changes to the VPU.
        //  5. Enable component
        MMAL_Component_Enable_Msg       m_ComponentEnableTx             = {};   //  SEND
        MMAL_Component_Enable_Reply     m_ComponentEnableRx             = {};   //  RECEIVE                 - Component must be enabled before parameters or port actions.
        //  6. Snapshot B – get port state after enable
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_B         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_B         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_B        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_B        = {};   //  Output port RECEIVE     - Snapshot B reflects VPU state **after enable**. Used as the base for zero-copy parameters.
        //  7. Zero-copy parameter set
        MMAL_Port_Parameter_Set_Msg     m_PortParamTx_Input             = {};   //  Input port SEND         - built using `m_PortInfoGetRx_Input_B`
        MMAL_Port_Parameter_Set_Reply   m_PortParamRx_Input             = {};   //  Input port RECEIVE

        MMAL_Port_Parameter_Set_Msg     m_PortParamTx_Output            = {};   //  Output port SEND        - built using `m_PortInfoGetRx_Output_B`
        MMAL_Port_Parameter_Set_Reply   m_PortParamRx_Output            = {};   //  Output port RECEIVE     - Enable zero-copy **before** ports are enabled.
        //  8. Snapshot C – get port state after zero-copy
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_C         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_C         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_C        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_C        = {};   //  Output port RECEIVE     - Snapshot C is the **final authoritative state** before enabling ports.
        //  9. Enable ports
        MMAL_Port_Action_Msg            m_PortActionTx_Input            = {};   //  Input port SEND         - uses handle from `m_PortInfoGetRx_Input_C`
        MMAL_Port_Action_Reply_Msg      m_PortActionRx_Input            = {};   //  Input port RECEIVE

        MMAL_Port_Action_Msg            m_PortActionTx_Output           = {};   //  Output port SEND        - uses handle from `m_PortInfoGetRx_Output_C`
        MMAL_Port_Action_Reply_Msg      m_PortActionRx_Output           = {};   //  Output port RECEIVE     - Ports become live. After this, buffers may flow.
        //  10. Runtime buffer flow
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostTx_Input        = {};   //  Input buffers SEND
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostRx_Input        = {};   //  Input buffers RECEIVE   (same physical layout, semantic RX)

        MMAL_Buffer_From_Host_Msg       m_BufferFromHostTx_Output       = {};   //  Output buffers SEND     (ping/pong)
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostRx_Output       = {};   //  Output buffers RECEIVE  (ping/pong) - Buffer ownership transfer and frame delivery.
        
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_D         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_D         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_D        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_D        = {};   //  Output port RECEIVE
//----------------------------------------------------------------------------------------------------------------------------------------------------        
        // Core invariant (unchanged, explicit)
        // Snapshot **A** → build **SET**
        // Snapshot **B** → build **PARAM**
        // Snapshot **C** → build **ACTION**
        // Snapshot **D** → buffer runtime
        // Buffer TX/RX is independent of snapshots        
//----------------------------------------------------------------------------------------------------------------------------------------------------
};

#endif