#include <GLES2/gl2.h>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include "vc04_service.h"
#include "vc04_defs.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  DATAMANAGEMENT
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CKernel
{
public:
    bool        Mount           (   const char* p_deviceName);
    bool        UnMount         ();

    bool        openFile        (   const char* p_fileName);
    unsigned    loadToBuffer    (   char*       p_buffer, 
                                    unsigned    p_bufferSize);

    bool        saveFromBufferO (  const char* p_fileName, 
                                    const char* p_buffer, 
                                    unsigned    p_bufferSize);
    bool        saveFromBuffer  (   const char* p_deviceName, 
                                    const char* p_fileName, 
                                    const char* p_buffer, 
                                    unsigned    p_bufferSize);

    bool        closeFile       ();

    void        bulkLoad        (   char*       p_fileNameArray[],
                                    unsigned    p_loadedBytes[],
                                    char**      p_bufferArray,
                                    unsigned    p_maxFiles,
                                    unsigned&   p_validFiles,
                                    unsigned&   p_prevFiles,
                                    unsigned    p_fileSize);

    bool        IsValidFile     (   const char* pFileName, 
                                    const char* extension);

    bool        scanRoot        (   char**      p_fileNameArray,
                                    const char* p_fileExtension[],
                                    unsigned    p_extentionCount,
                                    unsigned&   p_scannedFiles,
                                    unsigned    p_maxFiles);

    bool        updateUSB       (   const char* p_deviceName);
    static void removeUSB       (   CDevice*    f_partitionName, 
                                    void*       p_pContext);

    char**      allocBufferMEM  (   size_t      count, 
                                    size_t      bufferSize);
    char**      allocBufferDMA  (   size_t      count,
                                    size_t      bufferSize,
                                    char**      blockBaseOut,
                                    char**      rawBlockOut,
                                    size_t*     alignedSizeOut);

    void        clearBufferMEM  (   char**      buffers, 
                                    size_t      count);
    void        clearBufferDMA  (   char**      buffers, 
                                    char*       rawBlock);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  GRAPHICS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
    struct vtx_state;       // whats wrong with you piece of shit! the structs in question are included in gfx_new_new.cpp so why you dont include them in this header you piece of shit?! 
    struct glsl_state;
    struct tex_state;
    struct olg_state;

    bool        shaderLog       (   GLint       shader, 
                                    int         shaderIndex);
    bool        programLog      (   GLint       program, 
                                    int         program_index);

    void        gfx_check       (   const char* file, 
                                    unsigned    line);

    void        initOGL         (   olg_state*  o);
    void        initVbuffer     (   olg_state*  o, 
                                    vtx_state*  v);

    void        initShader      (   vtx_state*  v,
                                    glsl_state* s,
                                    tex_state*  t,
                                    char**      srcBuffer,
                                    int         fromFile,
                                    int         toFile,
                                    GLenum      type,
                                    bool*       flags);

    void        initProgram     (   vtx_state* v,
                                    glsl_state* vsh,
                                    glsl_state* fsh,
                                    tex_state* t,
                                    int fromFile,
                                    int toFile,
                                    unsigned& valid_count,
                                    bool* flags_vsh,
                                    bool* flags_fsh);

    void        initTexture     (   vtx_state* v,
                                    glsl_state* s,
                                    tex_state* t,
                                    char** buffer,
                                    int fromFile,
                                    int toFile,
                                    unsigned& valid_count,
                                    bool* flags,
                                    GLint wrap_s,
                                    GLint wrap_t);

    void        initTextureNEW  (   vtx_state* v,
                                    glsl_state* s,
                                    tex_state* t,
                                    int fromFile,
                                    int toFile,
                                    unsigned& valid_count,
                                    GLint wrap_s,
                                    GLint wrap_t);

    void        initUniform     (   vtx_state* v,
                                    glsl_state* s,
                                    tex_state* t,
                                    int fromFile,
                                    int toFile);

    void        frmBufferSet    (   vtx_state* v);
    void        frmBufferSwap   (   olg_state* o);

    void        setUniPrg       (   olg_state* o,
                                    glsl_state* s,
                                    tex_state* t,
                                    int gl_current_tex,
                                    unsigned p_validTextureCount);

    void        setTexPrg       (   olg_state* o,
                                    glsl_state* s,
                                    tex_state* t,
                                    int gl_current_tex,
                                    unsigned p_validTextureCount);

    void        drawGLsPrg      ();
    void        frmRateBreak    (   bool* noTargetFPS);

    void        updateOvlState  (   olg_state* o, glsl_state* s, tex_state* t);
    void        setUniOvl       (   olg_state* o, glsl_state* s, tex_state* t);
    void        setTexOvl       (   olg_state* o, glsl_state* s, tex_state* t);
    void        drawGLsOvl      ();

    void        chooseIndex     (   int p_channel, 
                                    int& p_activeIndex, 
                                    int p_maxCount, 
                                    bool* flags);
    void        chooseIndexDense(   int p_channel, 
                                    int& p_activeIndex, 
                                    int p_maxCount);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MENU
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
    void        resetPickUpFlags();

    void        mapMenuGroup    (   uint8_t menu_id, 
                                    uint8_t base);

    void        setChannelMode  (   int channel);

    void        modeADC         (   int channel);
    void        modeTRG         (   int channel);
    void        modeBPM         (   int channel);
    void        modeLF1         (   int channel);
    void        modeLF2         (   int channel);

    void        modeAudioAb0    (   int channel);
    void        modeAudioAb1    (   int channel);
    void        modeAudioBb0    (   int channel);
    void        modeAudioBb1    (   int channel);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PARSER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct tex_state;
struct h264_state;  // same question! why!

public:
    bool        initBMPparser   (   tex_state* t,
                                    char* buffer_array[],
                                    size_t size_array[],
                                    u32 max_tex_size,
                                    int fromFile,
                                    int toFile);

    bool        initH264parser  (   h264_state* h,
                                    char* blockBase,
                                    char* buffer_array[],
                                    size_t size_array[],
                                    int fromFile,
                                    int toFile,
                                    u16 max_width,
                                    u16 max_height,
                                    u8 max_profile,
                                    u8 max_level);

    void        ParseBPM        (   tex_state* t,
                                    char* filename_array[],
                                    int fromFile,
                                    int toFile);

    void        ParseAnnexB     (   h264_state* h,
                                    char* filename_array[],
                                    int fromFile,
                                    int toFile);

    bool        ParseSPS        (   u8* sps_data,
                                    size_t sps_size,
                                    size_t sps_sc_len,
                                    u16* width,
                                    u16* height,
                                    u8* profile,
                                    u8* level) const;

    size_t      findNext000001  (   u8* data, 
                                    size_t pos, 
                                    size_t size) const;
    u32         ReadExpGolomb   (   u8* data, 
                                    size_t* bit_offset) const;
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  UTIL
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
    bool        startupScreen(char* buffer, u32& index);

    void        readADC();

    bool        checkUpdate();
    bool        Update();

    void        prepParameters();

    void        chooseIndex(int p_channel, int& p_activeIndex, int p_maxCount, bool* flags);
    void        chooseIndexDense(int p_channel, int& p_activeIndex, int p_maxCount);

    void        storeModesV2();

    void        buttonPing(int p_btn_id, int pin);
    void        button_consumer(int p_btn_id);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void        randomVec8(uint32_t p_seed);

    void        calculate2BPM(unsigned long p_triggerTimeClockA,
                       unsigned long p_triggerTimeClockB);
    void        calculate1BPM(int p_source, unsigned long p_triggerTimeClock);

    void        predictedBeat1();
    void        predictedBeat2(int p_source, int p_lfoMult);

    void        sampleWaveTable1();
    void        sampleWaveTable2(int p_source, int p_lfoOut, int p_waveTable);

    void        set_pot_routing(bool adc_pot_routing);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  VC04_SERVICE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
    void callbackVCSM           (   void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);
    void callbackMMAL           (   void* callback_param, VCHI_CALLBACK_REASON_T reason, void* msg_handle);

    void getStateVCHI();
    bool initEventsVCOS         (   VCOS_EVENT_T& event, const char* name);

    u32 convertAddress          (   void* buffer, size_t size);
    bool checkGLerrorMMAL       ();

    u32 NextTransId             (   u32& tid);

    void initHeaderVCSM         (   vc_sm_msg_hdr_t& hdr, u32 type);
    void initHeaderMMAL         (   mmal_msg_header& hdr, u32 type);

    bool sendAndWaitVCHI        (   VCHI_SERVICE_HANDLE_T ServiceHandle,
                                    VCOS_EVENT_T& VCOSevent,
                                    const void* msg,
                                    size_t msg_size,
                                    void* rx_msg,
                                    size_t max_reply_len,
                                    size_t* actual_reply_len);

    bool openServiceVCHI        (   SERVICE_CREATION_T& tx,
                                    uint32_t serviceVersion,
                                    uint32_t serviceVersionMin,
                                    int32_t service_id,
                                    VCHI_CALLBACK_T cb,
                                    void* cb_param,
                                    VCHI_INSTANCE_T VCHIInstance,
                                    VCHI_SERVICE_HANDLE_T& ServiceHandle);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    bool initializeVCSM         ();
    bool importMemoryVCSM       (   void* buffer, size_t size, int slot, VCSM_Import_MEM_Msg& tx, VCSM_Import_MEM_Reply& rx);
    bool lockMemoryVCSM         (   int slot, VCSM_Lock_MEM_Msg& tx, VCSM_Lock_MEM_Reply& rx);
    bool freeMemoryVCSM         (   int slot, VCSM_Free_MEM_Msg& tx, VCSM_Free_MEM_Reply& rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    bool initializeMMAL         (   u32 InBufferHandle,
                                    u32 InBufferPointer,
                                    u32 InBufferSize,
                                    u32 OutBufferHandleA,
                                    u32 OutBufferPointerA,
                                    u32 OutBufferHandleB,
                                    u32 OutBufferPointerB,
                                    u32 OutBufferSize,
                                    u32 ResolutionX,
                                    u32 ResolutionY,
                                    EGLDisplay eglDisplay,
                                    EGLContext eglContext);
    bool createTexturesMMAL     ();
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    bool framePollerMMAL        (   u32 frame_offset, u32 frame_length);
    bool bufferReadyMMAL        (   u32 handle);

    bool queueOutputBufferMMAL  (   MMAL_Buffer_From_Host_Msg& tx, u32 vc_handle, u32 alloc_size);
    bool queueInputBufferMMAL   (   MMAL_Buffer_From_Host_Msg& tx, u32 frame_offset, u32 frame_length);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    bool createComponent        (   MMAL_Component_Create_Msg& tx, MMAL_Component_Create_Reply& rx);
    bool getPortInfoMMAL        (   u32 port_type, u32& port_handle, MMAL_Port_Info_Get_Msg& tx, MMAL_Port_Info_Get_Reply& rx);
    bool setPortInfoMMAL        (   MMAL_Port_Info_Set_Msg& tx, MMAL_Port_Info_Set_Reply& rx);
    bool enableComponentMMAL    (   MMAL_Component_Enable_Msg& tx, MMAL_Component_Enable_Reply& rx);
    bool setZeroCopyModeMMAL    (   const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Parameter_Set_Msg& tx, MMAL_Port_Parameter_Set_Reply& rx);
    bool enablePortMMAL         (   const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Action_Msg& tx, MMAL_Port_Action_Reply_Msg& rx);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void PrimeOutputBufferBodyMMAL( MMAL_Buffer_From_Host_Msg& tx);
    void PrimeInputBufferBodyMMAL(  MMAL_Buffer_From_Host_Msg& tx);
    void primePortFormatInputMMAL(  const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);
    void primePortFormatOutputMMAL( const MMAL_Port_Info_Get_Reply& src, MMAL_Port_Info_Set_Msg& tx);
};
#endif // VC04_SERVICE_API_H