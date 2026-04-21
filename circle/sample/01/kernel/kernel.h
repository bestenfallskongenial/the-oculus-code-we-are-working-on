// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PUBLIC / PRIVATE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "include_defs.h"
#include "macro_defs.h"

class CKernel
{
public:
	            CKernel (void);
	           ~CKernel (void);

#include "enums.h"
#include "structs.h"
#include "members.h" // <- should go to the bottom, right?

	        boolean     Initialize (void);

	        TShutdownMode Run (void);

                bool        Mount                       (   const   char*                           p_deviceName);          // "emmc1-1" cd ( root ), "umsd1-1" usb

                bool        UnMount                     ();

                bool        openFile                    (   const   char*                           p_fileName);            // filename format "8.3"

                unsigned    loadToBuffer                (           char*                           p_bufferArray,               // destination buffer for the file data
                                                                    unsigned                        p_bufferSize);          // maximum number of bytes to read into the buffer

                bool        saveFromBufferO             (   const   char*                           p_fileName,             // filename format "8.3" 
                                                            const   char*                           p_bufferArray,               // my allocated buffer
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        saveFromBufferM             (   const   char*                           p_deviceName,           // "emmc1-1" cd ( root ), "umsd1-1" usb
                                                            const   char*                           p_fileName,             // filename format "8.3"
                                                            const   char*                           p_bufferArray,               // my allocated buffer 
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        closeFile                   ();

                void        bulkLoad                    (           char*                           p_fileNameArray[],      // where we have stored the filenames from the root directory scan
                                                                    unsigned                        p_loadedBytes[],        // where we store the size in bytes for each file
                                                                    char**                          p_bufferArray,          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                    unsigned                        p_maxFiles,             // how many files we are allowed to process ( os limitations )
                                                                    unsigned&                       p_validFiles,           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                                    unsigned&                       p_prevFiles,            // number of loads from the last call - we need it to init the files correctly
                                                                    unsigned                        p_fileSize);            // maximum size for each file

                bool        IsValidFile                 (   const   char*                           pFileName, 
                                                            const   char*                           extension);

                bool        scanRoot                    (           char**                          p_fileNameArray,        // where we store the valid filenames we find
                                                                    const char*                     p_fileExtArray[],       // the array of valid file extensions for this type of file
                                                                    unsigned                        p_extentionCount,       // how many valid file extensions we have in the array above
                                                                    unsigned&                       p_scannedFiles,         // our counter of found files
                                                                    unsigned                        p_maxFiles);            // how many files are allowed to scan and stored in the array

                bool        updateUSB                   (   const   char*                           p_deviceName);

        static  void        removeUSB                   (           CDevice*                        pDevice, 
                                                                    void*                           pContext);

                char**      allocBufferMEM              (           size_t                          p_count, 
                                                                    size_t                          bufferSize);

                char**      allocBufferDMA              (           size_t                          p_count,
                                                                    size_t                          bufferSize,
                                                                    char**                          blockBaseOut,
                                                                    char**                          rawBlockOut,
                                                                    size_t*                         alignedSizeOut);

                void        clearBufferMEM              (           char**                          buffers, 
                                                                    size_t                          p_count);

                void        clearBufferDMA              (           char**                          buffers, 
                                                                    char*                           rawBlock);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  GRAPHICS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        initOGL                     (           olg_state*                      o);

                void        initVbuffer                 (           olg_state*                      o, 
                                                                    vtx_state*                      v);

                void        initShader                  (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    char**                          p_bufferArray,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    GLenum                          type);

                void        initProgram                 (           vtx_state*                      v,
                                                                    glsl_state*                     vsh,
                                                                    glsl_state*                     fsh,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count);
                void        initTexture                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count,
                                                                    GLint                           wrap_s,
                                                                    GLint                           wrap_t);

                void        initUniform                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  RUNTIME RENDER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        frmBufferSet                (           vtx_state*                      v);

                void        frmBufferSwap               (           olg_state*                      o);

                void        setUniPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                /*  int                             gl_current_tex, */
                                                                    unsigned                        p_validTextureCount);

                void        setTexPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             gl_current_tex,
                                                                    unsigned                        p_validTextureCount);
                void        drawGLsPrg                  ();

                void        frmRateBreak                (           bool*                           noTargetFPS);

                void        updateOvlState              (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setUniOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setTexOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);
                void        drawGLsOvl                  ();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  LOGGING
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        storeLog                    (           char*                           p_bufferArray, 
                                                                    u32&                            index,
                                                            const   char*                           label,
                                                                    u32                             p_value0, 
                                                                    u32                             p_value1,
                                                                    u32                             p_value2, 
                                                                    u32                             p_value3);

                void        storeLogLong                (           char*                           p_bufferArray,
                                                                    u32&                            index,
                                                            const   char*                           p_string0, 
                                                                    u32                             p_value0,
                                                            const   char*                           p_string1, 
                                                                    u32                             p_value1,
                                                            const   char*                           p_string2, 
                                                                    u32                             p_value2,
                                                            const   char*                           p_string3, 
                                                                    u32                             p_value3);

                void        storeMsg                    (           char*                           p_bufferArray,
                                                                    u32&                            index,
                                                                    const char*                     label,
                                                                    const void*                     tx_msg,
                                                                    u32                             total_size);

                void        nextline                    (           char*                           p_bufferArray,
                                                                    u32&                            index);

        static  void        bufferToScreenPlot          (           unsigned                        x, 
                                                                    unsigned                        y, 
                                                                    u32                             color );       

        static  void        bufferToScreenDrawChar      (           char                            ch,
                                                                    unsigned                        charCol,
                                                                    unsigned                        charRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor );

                boolean     bufferToScreenInit          (           void )

                void        bufferToScreenClear         (           u32                             bgColor);

                void        bufferToScreenDrawBuffer    (   const   char*                           pSourceBuffer,
                                                                    u32                             startIndex,
                                                                    u32                             endIndex,
                                                                    unsigned                        startCol,
                                                                    unsigned                        startRow,
                                                                    u32                             fgColor,
                                                                    u32                             bgColor);

                unsigned    bufferToScreenGetGrid       (           unsigned&                       cols, 
                                                                    unsigned&                       rows);
// also "only" log / debug or really runtime requirements?
                bool        shaderLog                   (           GLint                           shader, 
                                                                    int                             shaderIndex);

                bool        programLog                  (           GLint                           program, 
                                                                    int                             program_index);

                void        gfx_check                   (   const   char*                           file, 
                                                                    unsigned                        line);

                bool        startupScreen               (           char*                           p_bufferArray,
                                                                    u32&                            index);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MENU
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        resetPickUpFlags            ();

                void        mapMenuGroup                (           uint8_t                         menu_id, 
                                                                    uint8_t                         base);

                void        setChannelMode              (           int                             p_channel);

                void        modeADC                     (           int                             p_channel);

                void        modeTRG                     (           int                             p_channel);

                void        modeBPM                     (           int                             p_channel);

                void        modeLF1                     (           int                             p_channel);

                void        modeLF2                     (           int                             p_channel);

                void        modeAudioAb0                (           int                             p_channel);

                void        modeAudioAb1                (           int                             p_channel);

                void        modeAudioBb0                (           int                             p_channel);

                void        modeAudioBb1                (           int                             p_channel);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PARSER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initBMPparser               (           tex_state*                      t,
                                                                    char*                           p_buffer_array[],
                                                                    size_t                          size_array[],
                                                                    u32                             max_tex_size,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);

                bool        initH264parser              (           h264_state*                     h,
                                                                    char*                           blockBase,
                                                                    char*                           p_buffer_array[],
                                                                    size_t                          size_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    u16                             max_width,
                                                                    u16                             max_height,
                                                                    u8                              max_profile,
                                                                    u8                              max_level);

                void        ParseBPM                    (           tex_state*                      t,
                                                                    char*                           filename_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);

                void        ParseAnnexB                 (           h264_state*                     h,
                                                                    char*                           filename_array[],
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
                                 
                bool        ParseSPS                    (           u8*                             sps_data,
                                                                    size_t                          sps_size,
                                                                    size_t                          sps_sc_len,
                                                                    u16*                            width,
                                                                    u16*                            height,
                                                                    u8*                             profile,
                                                                    u8*                             level) const;

                size_t      findNext000001              (           u8*                             data, 
                                                                    size_t                          pos, 
                                                                    size_t                          size) const;

                u32         ReadExpGolomb               (           u8*                             data, 
                                                                    size_t*                         bit_offset) const;
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  UTIL
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        readADC                     ();

                bool        checkUpdate                 ();

                bool        Update                      ();

                void        set_pot_routing             (           bool                            adc_pot_routing);

                void        prepParameters              ();

                void        chooseIndex                 (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount, 
                                                                    bool*                           flags);

                void        chooseIndexD                (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount);
                void        storeModes                  ();

                void        buttonPing                  (           int                             p_btn_id, 
                                                                    int                             p_pin);

                void        button_consumer             (           int                             p_btn_id);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        randomVec8                  (           uint32_t                        p_seed);

                void        calculate1BPM               (           int                             p_source, 
                                                                    unsigned long                   p_triggerTimeClock);

                void        predict1Beat                (           int                             p_source, 
                                                                    int                             p_lfoMult);

                void        sample1WaveTable            (           int                             p_source, 
                                                                    int                             p_lfoIn, 
                                                                    int                             p_lfoOut );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  VC04_SERVICE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
        static  void        callbackVCSM                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle); // NOT STATIC REQUIRED?!

        static  void        callbackMMAL                (           void*                           callback_param, 
                                                                    VCHI_CALLBACK_REASON_T          reason, 
                                                                    void*                           msg_handle);

                void        getStateVCHI();
                bool        initEventsVCOS              (           VCOS_EVENT_T&                   event, 
                                                            const   char*                           name);

                u32         convertAddress              (           void*                           p_busAddress, 
                                                                    size_t                          p_size);
                bool        checkGLerrorMMAL            ();

                u32         NextTransId                 (           u32&                            tid);

                void        initHeaderVCSM              (           vc_sm_msg_hdr_t&                hdr, 
                                                                    u32                             type);

                void        initHeaderMMAL              (           mmal_msg_header&                hdr, 
                                                                    u32                             type);

                bool        sendAndWaitVCHI             (           VCHI_SERVICE_HANDLE_T           ServiceHandle,
                                                                    VCOS_EVENT_T&                   VCOSevent,
                                                                    const void*                     msg,
                                                                    size_t                          msg_size,
                                                                    void*                           rx_msg,
                                                                    size_t                          max_reply_len,
                                                                    size_t*                         actual_reply_len);

                bool        openServiceVCHI             (           SERVICE_CREATION_T&             tx,
                                                                    uint32_t                        serviceVersion,
                                                                    uint32_t                        serviceVersionMin,
                                                                    int32_t                         service_id,
                                                                    VCHI_CALLBACK_T                 cb,
                                                                    void*                           cb_param,
                                                                    VCHI_INSTANCE_T                 VCHIInstance,
                                                                    VCHI_SERVICE_HANDLE_T&          ServiceHandle);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initializeVCSM              ();
                bool        importMemoryVCSM            (           void*                           p_bufferBlockbase, 
                                                                    size_t                          size, 
                                                                    int                             slot, 
                                                                    VCSM_Import_MEM_Msg&            tx, 
                                                                    VCSM_Import_MEM_Reply&          rx);

                bool        lockMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Lock_MEM_Msg&              tx, 
                                                                    VCSM_Lock_MEM_Reply&            rx);

                bool        freeMemoryVCSM              (           int                             slot, 
                                                                    VCSM_Free_MEM_Msg&              tx, 
                                                                    VCSM_Free_MEM_Reply&            rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        initializeMMAL              (           u32                             InBufferHandle,
                                                                    u32                             InBufferPointer,
                                                                    u32                             InBufferSize,
                                                                    u32                             OutBufferHandleA,
                                                                    u32                             OutBufferPointerA,
                                                                    u32                             OutBufferHandleB,
                                                                    u32                             OutBufferPointerB,
                                                                    u32                             OutBufferSize,
                                                                    u32                             ResolutionX,
                                                                    u32                             ResolutionY,
                                                                    EGLDisplay                      eglDisplay,
                                                                    EGLContext                      eglContext);
                bool        createTexturesMMAL          ();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        framePollerMMAL             (           u32                             frame_offset, 
                                                                    u32                             frame_length);

                bool        bufferReadyMMAL             (           u32                             handle);

                bool        queueOutputBufferMMAL       (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             vc_handle, 
                                                                    u32                             alloc_size);

                bool        queueInputBufferMMAL        (           MMAL_Buffer_From_Host_Msg&      tx, 
                                                                    u32                             frame_offset, 
                                                                    u32                             frame_length);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        createComponent             (           MMAL_Component_Create_Msg&      tx, 
                                                                    MMAL_Component_Create_Reply&    rx);

                bool        getPortInfoMMAL             (           u32                             port_type, 
                                                                    u32&                            port_handle, 
                                                                    MMAL_Port_Info_Get_Msg&         tx, 
                                                                    MMAL_Port_Info_Get_Reply&       rx);

                bool        setPortInfoMMAL             (           MMAL_Port_Info_Set_Msg&         tx, 
                                                                    MMAL_Port_Info_Set_Reply&       rx);

                bool        enableComponentMMAL         (           MMAL_Component_Enable_Msg&      tx, 
                                                                    MMAL_Component_Enable_Reply&    rx);

                bool        setZeroCopyModeMMAL         (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Parameter_Set_Msg&    tx, 
                                                                    MMAL_Port_Parameter_Set_Reply&  rx);
                                                                    
                bool        enablePortMMAL              (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Action_Msg&           tx, 
                                                                    MMAL_Port_Action_Reply_Msg&     rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                void        PrimeOutputBufferBodyMMAL   (           MMAL_Buffer_From_Host_Msg&      tx);

                void        PrimeInputBufferBodyMMAL    (           MMAL_Buffer_From_Host_Msg&      tx);

                void        primePortFormatInputMMAL    (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);

                void        primePortFormatOutputMMAL   (   const   MMAL_Port_Info_Get_Reply&       src, 
                                                                    MMAL_Port_Info_Set_Msg&         tx);
/*
// NOT IMPLEMENTED YET!!!
#ifdef __DEBUG_LOG__
        void Log_openService                                      (     const SERVICE_CREATION_T &src);
        void Log_createComponent                                  (     const MMAL_Component_Create_Msg& tx, const MMAL_Component_Create_Reply    & rx);
        void Log_getPortInfo                                      (     const MMAL_Port_Info_Get_Msg& tx, const MMAL_Port_Info_Get_Reply    & rx);
        void Log_setPortInfo                                      (     const MMAL_Port_Info_Set_Msg& tx, const MMAL_Port_Info_Set_Reply    & rx);
        void Log_enableComponentMMAL                                  (     const MMAL_Component_Enable_Msg& tx, const MMAL_Component_Enable_Reply    & rx);
        void Log_enablePort                                       (     const MMAL_Port_Action_Msg& tx, const MMAL_Port_Action_Reply_Msg&  rx);
        void Log_setZeroCopyModeMMAL                                  (     const MMAL_Port_Parameter_Set_Msg& tx, const MMAL_Port_Parameter_Set_Reply    & rx);
        void Log_BufferFromHost                                   (     const MMAL_Buffer_From_Host_Msg& rx);
//      void Log_BufferBody                                       (     const mmal_msg_buffer_from_host_wire32& msg );
//      void Log_queueBufferFromHost                              (     const MMAL_Buffer_From_Host_Msg& tx );        
#endif // __DEBUG_LOG__
*/                                                                    
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  WRAPPERS - HERE THE JOY BEGINS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                bool        wrapperInitDMA              ();
                bool        wrapperInitMEM              ();

                void        wrapperDMAcleanUp           ();
                void        wrapperMEMcleanUp           ();

                void        wrapper_from_sd             ();
                void        wrapper_load_usb            ();

                void        wrapper_init_gl_sd          ();
                void        wrapper_init_gl_usb         ();

                void        wrapper_io                  ();
                void        wrapper_modes               ();
};