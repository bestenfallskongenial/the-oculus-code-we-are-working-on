public:
                char                m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_logBufferIndex = 0;
                char                m_startupBuffer[LOG_BUFFER_SIZE] = {0};
                u32                 m_startupBufferIndex = 0;
                
private:
                CActLED             m_ActLED;
                CKernelOptions      m_Options;
                CMachineInfo        m_MachineInfo;                

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

private:
                CExceptionHandler   m_ExceptionHandler;
                CInterruptSystem    m_Interrupt;
                CTimer              m_Timer;
                CLogger             m_Logger;