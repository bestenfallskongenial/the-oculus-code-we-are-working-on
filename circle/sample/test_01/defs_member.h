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
                CMemorySystem		        m_Memory;
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
                char                m_startupBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_startupBufferIndex = 0;
// global
                unsigned                g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];           // the integer in/out matrix
                float                   g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];           // the float in/out matrix
                bool                    g_menuPickUpFlag[4*MENU_LAYER_COUNT];                     // the flags for the pickup mechanism  
                unsigned                g_buttons_states[NR_BUTTONS][BTN_INDEX_COUNT];      // the button state machine
                unsigned                g_centralModeBuffer[SLOTS][MODETABLE_COUNT];         // the general user settings, storable per program 
                unsigned                g_lfoMultiplier[LFO_MULTIPLIERS];

                unsigned                g_lfoBpmMatrix[4][LFO_BPM_COUNT]        = {   64, 32, 16, 8, 4, 2, 1 };
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

                uint8_t                 g_modeLengthAdd[MODELEN_FLAG_COUNT] =
                                                                {
                                                                MAX_MODES,                  // 5 for now    
                                                                2,                          // MODELEN_AUDIO_A  // this enum than is used here to get the actual numbers for the  [i][0] in g_modeMap
                                                                2,                          // MODELEN_AUDIO_B
                                                                };
                                                            