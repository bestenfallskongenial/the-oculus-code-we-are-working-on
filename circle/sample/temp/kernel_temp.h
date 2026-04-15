// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MENU
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <GLES2/gl2.h>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include "vc04_service.h"
#include "vc04_defs.h"

#define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  MACROS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define         EMPTYSTR ""
#define         EMPTYLOG 255

#define         PARTITIONSD	            "emmc1-1"	// partition sd
#define         PARTITIONUSB	        "umsd1-1"	// partition usb

#define 		FILENAME_GL_LOG			"LOG-GLES.txt"
#define 		FILENAME_GLERROR		"LOG-GLER.txt"
#define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
#define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"

#define         ADC_CHANNELS         	8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
#define         VREF			 		5.0f	                            // Reference voltage 5 Volt     ( dependency <sensor/mcp300x.h>	)

#define         CHANNEL			    	7		                            // im confused - is it needed to init the mcp3008?

#define         SPI_MASTER_DEVICE	 	0		                            // 0    m_SPIMaster ( also for the mcp3008 )
#define         SPI_CHIP_SELECT		 	0		                            // 0
#define         SPI_CLOCK_SPEED	   		1000000		                        // Hz

#define         CS_PIN                 	12                                  // pin for menu input selector!!!

#define         NR_BUTTONS              2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
#define         SW_PIN_A	           	13                                  // button A ( lower )
#define         SW_PIN_B	          	6                                   // button B ( upper )

#define         LED_PIN		           	19		                            // pin led ( must be between pin 8 .. 23 )
#define         LED_COUNT		       	4		                            // number of LEDs on the stripes

#define         GPIO_TO_SD_LINE(pin)	((pin) - 8)							// dont touch!
#define         SD_LINE1		      	GPIO_TO_SD_LINE (LED_PIN)			// dont touch!
#define         SD_LINE_TO_MASK(line)	(1 << (line))						// dont touch!
#define         SD_LINES_MASK		 	(  SD_LINE_TO_MASK (SD_LINE1) )		// dont touch! all needed for <WS28XX/ws2812oversmi.h>



#define         SLOTS                   34 // for the g_centralModeBuffer[SLOTS][modetablecount] array - 1 firmware / 32 user / 1 default slot
#define         DEFAULT_SLOT            34 // or 0 ?? 

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

enum modetable		                        // for the g_centralModeBuffer[SLOTS][modetablecount] array
{
	int CH0_MODE = 0,
	int CH1_MODE,
	int CH2_MODE,
	int CH3_MODE,

	int CH4_MODE,
	int CH5_MODE,
	int CH6_MODE,
	int CH7_MODE,

	int LF1_WAVE,
	int LF2_WAVE,
	int LF1_MULT,
	int LF2_MULT,

	int SENS_A,
	int SENS_B,
	int SENS_C,
	int SENS_D,

	int FRM_MODE,
	int TEX_MODE,
	int CLK_MODE,
	int VID_MODE,

	int IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

	modetablecount = 21                     // right?
}

enum io_types                               // for the array g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT] & g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT]
{
    int RAW = 0,                            //  the position the dampened adc values per channels are stored *
    int VAL,                                //  either the calculated/normalized int or flt value *
    int OUT,                                //  here lands the processed ( after mode ) for the glsl uniforms      
    int RND,                                //  either the per-channel random int or flt value *
    int LF1,                                //  either the lfo one int or flt value
    int LF2,                                //  either the lfo two int or flt value
    int AU0,                                //  the audio band 0 flt value
    int AU1,                                //  the audio band 1 flt value
    int AU2,                                //  the audio band 2 flt value
    int AU3,                                //  the audio band 3 flt value
                                            // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
    int TRL,                                //  per channel threshold low !!! dont forget to copy the values in here    128
    int TRH,                                //  per channel threshold high                                              320
    int TRF,                                //  per channel threshold "flag" - whre do i need you again?

    int IO_TYPE_COUNT = 13
}

enum ButtonTSIndex  // for the button state machine - unsigned int g_buttons_states[NR_BUTTONS][5] = {0} !!!
{
    BTN_PRESS_START = 0,
    BTN_DOUBLE      = 1,
    BTN_RELEASE     = 2,
    BTN_SINGLE      = 3,
    BTN_HOLD_TICK   = 4,   // COUNTER: increases while held
    BTN_INDEX_COUNT = 5
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  PUBLIC / PRIVATE
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean     Initialize (void);

	TShutdownMode Run (void);

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
                                    GLenum      type);
    void        initProgram     (   vtx_state* v,
                                    glsl_state* vsh,
                                    glsl_state* fsh,
                                    tex_state* t,
                                    int fromFile,
                                    int toFile,
                                    unsigned& valid_count);
    void        initTexture     (   vtx_state* v,
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  RUNTIME RENDER
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    void        updateOvlState  (   olg_state* o, 
                                    glsl_state* s, 
                                    tex_state* t);
    void        setUniOvl       (   olg_state* o, 
                                    glsl_state* s, 
                                    tex_state* t);
    void        setTexOvl       (   olg_state* o, 
                                    glsl_state* s, 
                                    tex_state* t);
    void        drawGLsOvl      ();
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

    bool        checkUpdate(); // wrapper right?

    bool        Update();       // not ready

    void        set_pot_routing(    bool adc_pot_routing);

    void        prepParameters();

    void        chooseIndex     (   int p_channel, 
                                    int& p_activeIndex, 
                                    int p_maxCount, 
                                    bool* flags);
    void        chooseIndexD    (   int p_channel, 
                                    int& p_activeIndex, 
                                    int p_maxCount);
    void        storeModes();

    void        buttonPing      (   int p_btn_id, 
                                    int pin);
    void        button_consumer (   int p_btn_id);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void        randomVec8      (   uint32_t p_seed);

    void        calculate1BPM   (   int p_source, 
                                    unsigned long p_triggerTimeClock);

    void        predict1Beat    (   int p_source, int p_lfoMult);

    void        sample1WaveTable(   int p_source, int p_lfoOut, int p_lfoIn );


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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  WRAPPERS - HERE THE JOY BEGINS
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    bool wrapperInitDMA();
    bool wrapperInitMEM();

    void wrapperDMAcleanUp();
    void wrapperMEMcleanUp();

    void wrapper_from_sd();
    void wrapper_load_usb();

    void wrapper_init_gl_sd();
    void wrapper_init_gl_usb();

    void wrapper_io();
    void wrapper_modes();
};