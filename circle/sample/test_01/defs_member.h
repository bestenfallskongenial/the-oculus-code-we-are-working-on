// circle system requirements
private:
                CActLED             m_ActLED;
                CKernelOptions      m_Options;
                CMachineInfo        m_MachineInfo;
                CDeviceNameService	m_DeviceNameService;
                CExceptionHandler   m_ExceptionHandler;
                CInterruptSystem    m_Interrupt;
                CTimer              m_Timer;
                CLogger             m_Logger;  
                CEMMCDevice		    m_EMMC;
                CUSBHCIDevice		m_USBHCI; 
                CMemorySystem		m_Memory;
                CFATFileSystem*     m_pFileSystem; 

// framebuffer
public:
                CBcmFrameBuffer     gE_FrameBuffer;
                CCharGenerator      gE_CharGenerator;

                u32*                gE_PixelBuffer      = nullptr;
                unsigned            gE_PitchBytes       = 0;
                unsigned            gE_ScreenWidth      = 0;
                unsigned            gE_ScreenHeight     = 0;
                unsigned            gE_CharWidth        = 0;
                unsigned            gE_CharHeight       = 0;
                unsigned            gE_Cols             = 0;
                unsigned            gE_Rows             = 0;
// SPI
private:
                uintptr             m_SPIBaseAddress    = 0;
                boolean             m_SPIValid          = 0;
// SMI / DMA / WS2812
private:
                CDMAChannel         m_SMITxDMA;

                unsigned            m_SMIGpioPin        = 0;
                unsigned            m_SMISDMask         = 0;
                unsigned            m_LEDCount          = 0;
                unsigned            m_BufferLength      = 0;
                TXDATA_T*           m_pBuffer           = 0;
                boolean             m_SMIValid          = FALSE;
// watchdog
static const    unsigned WatchdogMaxTimeoutSeconds      = 15;                
// debug
public:
    volatile    boolean	            m_bStorageAttached  = false;
    
                char                m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_logBufferIndex = 0;

// global

// missing globals / shared state / dummies for now
                int             g_currentProgramBuffer;
                int             g_current_gl_program;
                int             g_last_gl_program;
                int             g_activeBpmChannel;

                unsigned long   g_double_click_time = 50;
                unsigned long   g_long_click_time   = 250;
                unsigned        g_currentTime;

                int             attenuation = 0;
                bool            m_audio_mode_activated = true;

                bool is_audio[2];

                unsigned                g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];           // the integer in/out matrix
                float                   g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];           // the float in/out matrix
                bool                    g_menuPickUpFlag[4*MENU_LAYER_COUNT];                     // the flags for the pickup mechanism  
                unsigned                g_buttons_states[BUTTON_COUNT][BTN_INDEX_COUNT];      // the button state machine
                unsigned                g_centralModeBuffer[SLOTS][MODETABLE_COUNT];         // the general user settings, storable per program 
                unsigned                g_lfoMultiplier[LFO_MULTIPLIERS]        = {   64, 32, 16, 8, 4, 2, 1 };

                long long               g_lfoBpmMatrix[4][LFO_BPM_COUNT]; // was unsigned !
// datamanagement.cpp
                unsigned                g_hFile;

                char                    m_83FileName[MAX_FILE_NAME_LENGTH];
// util
        const   int                     m_scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                                1023    };  // 5.0V max     

                int                     m_adc_ring[ADC_CHANNELS][ADC_BUFFER_COUNT];
                int                     m_adc_index;

                float                   m_band[4][AUDIO_BUFFER_COUNT];

                float                   m_sum[4];  

                uint8_t                 m_idx0;
                uint8_t                 m_idx1;
                uint8_t                 m_idx2;
                uint8_t                 m_idx3;

                uint32_t                m_audio_hold_A;
                uint32_t                m_audio_hold_B;

                bool                    m_audio_flag_A = false;          
                bool                    m_audio_flag_B = false;

                uint8_t                 g_modeLengthAdd[MODELEN_FLAG_COUNT] =
                                                                {
                                                                MAX_MODES,                  // 5 for now    
                                                                2,                          // MODELEN_AUDIO_A  // this enum than is used here to get the actual numbers for the  [i][0] in g_modeMap
                                                                2,                          // MODELEN_AUDIO_B
                                                                };
                                                            
                char** 				    m_bufferVid         = nullptr;      // thats the pointer to my "array-like" buffer allocation
                char* 				    m_videoBlockBase    = nullptr;      // returns the aligned DMA base pointer
                char* 				    m_videoRawBlock     = nullptr;      // returns the original pointer from new[]
                size_t 				    m_videoBlockSize    = 0;            // size of each individual buffer - complete size, not only blocks?

                char**				    m_bufferFrA         = nullptr;      // i created a struct for it but that means i must 
                char* 				    m_frameBlockBaseA   = nullptr;      // rewrite the wrappers and initialize the stucts properly
                char* 				    m_frameRawBlockA    = nullptr;      // and that is actually not really progress
                size_t 				    m_frameBlockSizeA   = 0;

                char**				    m_bufferFrB         = nullptr;
                char* 				    m_frameBlockBaseB   = nullptr;
                char* 				    m_frameRawBlockB    = nullptr;
                size_t 				    m_frameBlockSizeB   = 0;	

                char** 				    m_bufferOmt         = nullptr;
                char* 				    m_overlyBlockBase   = nullptr;
                char* 				    m_overlayRawBlock   = nullptr;
                size_t 				    m_overlyBlockSize   = 0;

                char** 				    m_bufferTex         = nullptr;
                char* 				    m_textureBlockBase  = nullptr;
                char* 				    m_textureRawBlock   = nullptr;
                size_t 				    m_textureBlockSize  = 0;

                char**				    m_bufferKnl         = nullptr;
                char**				    m_bufferLog         = nullptr;
                char** 				    m_bufferVsh         = nullptr;
                char** 				    m_bufferOmf         = nullptr;                
                char** 				    m_bufferFsh         = nullptr; 
                
//----------------------------------------------------------------------------------------------------------------------------------------------------
// the populated filecounter array - source and truth and hub for init and load
//----------------------------------------------------------------------------------------------------------------------------------------------------
                unsigned                filecounter[FT_COUNT][FLD_COUNT] =
{                                       // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
                                        { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                        { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                        { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
                                        { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
                                        { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
                                        { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
                                        { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
                                        { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
                                        { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
// lists of extensions possible in my scanroot directory function per filetype 
//----------------------------------------------------------------------------------------------------------------------------------------------------
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" };    // vertex shaders
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };    // fragment shaders 
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" };    // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };    // for textures 24bit rgb
        const   char                   *g_SufVid[VID_EXT]			    = { "264" };    // video in raw h264 annex b encoded 
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };    // kernel.img for the update mechanism
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };    
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };
//----------------------------------------------------------------------------------------------------------------------------------------------------                