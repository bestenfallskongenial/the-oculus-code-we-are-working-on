// SPI
private:
                uintptr m_SPIBaseAddress = 0;
                boolean m_SPIValid = 0;
// framebuffer
public:
                CBcmFrameBuffer gE_FrameBuffer;
                CCharGenerator  gE_CharGenerator;
                CKernelOptions  m_Options;

                u32*            gE_PixelBuffer      = nullptr;
                unsigned        gE_PitchBytes       = 0;
                unsigned        gE_ScreenWidth      = 0;
                unsigned        gE_ScreenHeight     = 0;
                unsigned        gE_CharWidth        = 0;
                unsigned        gE_CharHeight       = 0;
                unsigned        gE_Cols             = 0;
                unsigned        gE_Rows             = 0;
// SMI / DMA / WS2812
private:
                CDMAChannel m_SMITxDMA;

                unsigned    m_SMIGpioPin   = 0;

                unsigned    m_SMISDMask    = 0;

                unsigned    m_LEDCount     = 0;
                unsigned    m_BufferLength = 0;

                TXDATA_T*   m_pBuffer      = 0;

                boolean     m_SMIValid     = FALSE;
// watchdog
static const unsigned WatchdogMaxTimeoutSeconds = 15;
// debug
public:
                char                m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_logBufferIndex = 0;
                char                m_startupBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_startupBufferIndex = 0;
// circle system requirements
private:
                CActLED             m_ActLED;
                CKernelOptions      m_Options;
                CMachineInfo        m_MachineInfo;                
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
// circle system requirements
private:
                CExceptionHandler   m_ExceptionHandler;
                CInterruptSystem    m_Interrupt;
                CTimer              m_Timer;
                CLogger             m_Logger;