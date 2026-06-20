#include "kernel.h"

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

static const char FromKernel[] = "kernel";

void            CKernel::storeLog                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* p_string0, u32 p_value0,
                                                        const char* p_string1, u32 p_value1,
                                                        const char* p_string2, u32 p_value2,
                                                        const char* p_string3, u32 p_value3)
{
                for (const char* p = p_string0; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                if (p_value0 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value0 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string1 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string1; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value1 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value1 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string2 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string2; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value2 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value2 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                if (p_string3 != EMPTYSTR)
                    {
                    p_buffer[index++] = ' ';
                    for (const char* p = p_string3; *p; ++p)
                        {
                        p_buffer[index++] = *p;
                        }
                    }
                if (p_value3 != EMPTYLOG)
                    {
                    p_buffer[index++] = ' ';
                    p_buffer[index++] = '0';
                    p_buffer[index++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
                        {
                        char hex = "0123456789ABCDEF"[(p_value3 >> (i * 4)) & 0xF];
                        p_buffer[index++] = hex;
                        }
                    }
                p_buffer[index++] = '\n';
                p_buffer[index] = '\0';

                logScreenUpdate();
}

void            CKernel::storeMsg                   (   char*       p_buffer,
                                                        u32&        index,
                                                        const char* label,
                                                        const void* tx_msg,
                                                        u32         total_size)
{
                p_buffer[index++]           = '\n';

                for (const char* p = label; *p; ++p)
                    {
                    p_buffer[index++] = *p;
                    }
                p_buffer[index++]           = '\n';

                const unsigned char* b = (const unsigned char*)tx_msg;

                for (u32 i = 0; i < total_size; ++i) 
                    {
                    if (i && (i % 16) == 0)
                        {
                        p_buffer[index++]   = '\n';
                        }
                    unsigned char v         = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    p_buffer[index++]       = hi;
                    char lo = "0123456789ABCDEF"[v & 0xF];
                    p_buffer[index++]       = lo;
                    p_buffer[index++]       = ' ';
                    }
                p_buffer[index++]           = '\n';
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';

                logScreenUpdate();
}

void            CKernel::nextline                   (   char*       p_buffer,
                                                        u32&        index)
{
                p_buffer[index++]           = '\n';
                p_buffer[index]             = '\0';

                logScreenUpdate();
}

void CKernel::logButtonStatesRuntime(void)
{
    char f_logBuffer[2048];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "BTN PRESS_START   DOUBLE        RELEASE       SINGLE",
                EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "A  ",
                (u32)g_buttons_states[0][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[0][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[0][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[0][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "A   HOLD_TICK",
                (u32)g_buttons_states[0][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "B  ",
                (u32)g_buttons_states[1][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[1][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[1][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[1][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "B   HOLD_TICK",
                (u32)g_buttons_states[1][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "Menu Layer",
                g_menuLayer,
                "Control Pin",
                GPIO_Read(CTRL_PIN),
                "bpm 0", 
                g_lfoBpmMatrix[0][BPM],
                "bpm 1",
                g_lfoBpmMatrix[1][BPM]   );
                
                
    storeLog(   f_logBuffer, f_logIndex,
                "g_gl_program_current",
                g_gl_program_current,
                "g_currentProgramBuffer",
                g_currentProgramBuffer,
                "filecounter[FT_FSH][FLD_LOADED]",
                filecounter[FT_FSH][FLD_LOADED],
                "mapping",
                ((g_inOutMatrixInt[ADC_SELECT_PRG][OUT] * filecounter[FT_FSH][FLD_VALID]) >> 10));



    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 00-03",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][0],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][1],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][2],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][3] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 04-07",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][4],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][5],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][6],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][7] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 08-11",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][8],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][9],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][10],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][11] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 12-15",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][12],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][13],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][14],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][15] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 16-19",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][16],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][17],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][18],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][19] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 20-23",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][20],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][21],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][22],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][23] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 24-27",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][24],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][25],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][26],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][27] );

    storeLog(   f_logBuffer, f_logIndex,
                "ModeBuf 28-31",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][28],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][29],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][30],
                " ",
                (u32)g_centralModeBuffer[g_currentProgramBuffer][31] );                
                
    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       10,
                       0xFFFFFFFF );
}

void CKernel::ADCDebug()
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    storeLog(   f_logBuffer, f_logIndex,
                "0 RAW", (u32)g_inOutMatrixInt[0][RAW],
                "1 RAW", (u32)g_inOutMatrixInt[1][RAW],
                "2 RAW", (u32)g_inOutMatrixInt[2][RAW],
                "3 RAW", (u32)g_inOutMatrixInt[3][RAW] );
    storeLog(   f_logBuffer, f_logIndex,
                "4 RAW", (u32)g_inOutMatrixInt[4][RAW],
                "5 RAW", (u32)g_inOutMatrixInt[5][RAW],
                "6 RAW", (u32)g_inOutMatrixInt[6][RAW],
                "7 RAW", (u32)g_inOutMatrixInt[7][RAW] );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       0,
                       0xFFFFFFFF );                
}
void CKernel::logInOutRuntime(void)
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "0 RAW",
                (u32)g_inOutMatrixInt[0][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[0][VAL],
                "RND",
                (u32)g_inOutMatrixInt[0][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "1 RAW",
                (u32)g_inOutMatrixInt[1][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[1][VAL],
                "RND",
                (u32)g_inOutMatrixInt[1][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "2 RAW",
                (u32)g_inOutMatrixInt[2][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[2][VAL],
                "RND",
                (u32)g_inOutMatrixInt[2][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "3 RAW",
                (u32)g_inOutMatrixInt[3][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[3][VAL],
                "RND",
                (u32)g_inOutMatrixInt[3][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "4 RAW",
                (u32)g_inOutMatrixInt[4][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[4][VAL],
                "RND",
                (u32)g_inOutMatrixInt[4][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "5 RAW",
                (u32)g_inOutMatrixInt[5][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[5][VAL],
                "RND",
                (u32)g_inOutMatrixInt[5][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "6 RAW",
                (u32)g_inOutMatrixInt[6][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[6][VAL],
                "RND",
                (u32)g_inOutMatrixInt[6][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "7 RAW",
                (u32)g_inOutMatrixInt[7][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[7][VAL],
                "RND",
                (u32)g_inOutMatrixInt[7][RND],
                EMPTYSTR, EMPTYLOG );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       0,
                       0xFFFFFFFF );
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
                if (MY_INDEX == 0) return;

                u32 drawIndex  = m_logScreenStartIndex;
                u32 scanIndex  = m_logScreenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;
#ifdef __SCROLLING__
                bufferScreenClear();
#endif
                while (scanIndex < MY_INDEX)
                    {
                    char ch = MY_BUFFER[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < MY_INDEX && MY_BUFFER[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < MY_INDEX)
                                {
                                drawIndex++;
                                }
#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(   MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );
                            msDelay(SCROLLSPEED);
#endif
                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;
#ifdef __SCROLLING__
                            bufferScreenClear();

                            bufferScreenDraw(   MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );
                            msDelay(SCROLLSPEED);
#endif
                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }
                bufferScreenClear();

                bufferScreenDraw(   MY_BUFFER,
                                    drawIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );
                m_logScreenStartIndex = drawIndex;
}

bool            CKernel::memoryDebugCheckpoint      (   const char* p_Label,
                                                        bool        p_DumpStatus )
{
                CMemorySystem* pMem = CMemorySystem::Get();

                if (pMem == nullptr)
                    {
                    return FALSE;
                    }

                size_t total = pMem->GetMemSize();
                size_t low   = pMem->GetHeapFreeSpace(HEAP_LOW);
                size_t high  = pMem->GetHeapFreeSpace(HEAP_HIGH);
                size_t any   = pMem->GetHeapFreeSpace(HEAP_ANY);

#ifdef __DEBUG_LOG__

                nextline( MY_BUFFER, MY_INDEX );

                storeLog(   MY_BUFFER,
                            MY_INDEX,
                            "MEM",
                            EMPTYLOG,
                            p_Label,
                            EMPTYLOG,
                            "TOTAL",
                            (u32)total,
                            "LOW",
                            (u32)low );

                storeLog(   MY_BUFFER,
                            MY_INDEX,
                            "MEM",
                            EMPTYLOG,
                            p_Label,
                            EMPTYLOG,
                            "HIGH",
                            (u32)high,
                            "ANY",
                            (u32)any );

#endif

#ifdef HEAP_DEBUG
                if (p_DumpStatus)
                    {
                    CMemorySystem::DumpStatus();
                    }
#endif
                nextline( MY_BUFFER, MY_INDEX );
                
                return TRUE;
}

bool            CKernel::startupScreen          (   void )
{
                const char* machineName =  m_MachineInfo.GetMachineName();
                const char* socName     =  m_MachineInfo.GetSoCName();

                unsigned modelMajor     =  m_MachineInfo.GetModelMajor();
                unsigned modelRevision  =  m_MachineInfo.GetModelRevision();
                unsigned ramSize        =  m_MachineInfo.GetRAMSize();

                unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;
                unsigned socMaxTemp     =  m_Options.GetSoCMaxTemp();

                unsigned coreClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_CORE)  / 1000000;
                unsigned armClock       =  m_MachineInfo.GetClockRate(CLOCK_ID_ARM)   / 1000000;
                unsigned emmcClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_EMMC)  / 1000000;
                unsigned emmc2Clock     =  m_MachineInfo.GetClockRate(CLOCK_ID_EMMC2) / 1000000;
                unsigned uartClock      =  m_MachineInfo.GetClockRate(CLOCK_ID_UART)  / 1000000;

                unsigned dmaChannel     =  m_MachineInfo.AllocateDMAChannel(DMA_CHANNEL_NORMAL);

                                           m_MachineInfo.FreeDMAChannel(dmaChannel);
            
                unsigned usbDelay       =  m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       =  m_Options.GetUSBFullSpeed();

                unsigned conWidth       =  m_Options.GetWidth();
                unsigned conHeight      =  m_Options.GetHeight();

                unsigned fbWidth        =  gE_FrameBuffer.GetWidth();
                unsigned fbHeight       =  gE_FrameBuffer.GetHeight();
                
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Machine Model  ", EMPTYLOG, 
                            machineName, EMPTYLOG );
                nextline(   MY_BUFFER, MY_INDEX);                            
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "SoC Name       ", EMPTYLOG, 
                            socName          , EMPTYLOG, 
                            "Model Major    ", modelMajor,
                            "Model Revision ", modelRevision);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "RAM Size     MB", ramSize);
                nextline(   MY_BUFFER, MY_INDEX);                
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "CPU Speed Mode ", cpuSpeedMode);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "SoC Max Temp   ", socMaxTemp);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock CORE  MHz", coreClock,
                            "Clock ARM   MHz", armClock);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock EMMC  MHz", emmcClock,
                            "Clock EMMC2 MHz", emmc2Clock);                            
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Clock UART  MHz", uartClock);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "DMA Channel    ", dmaChannel);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "USB Delay      ", usbDelay,
                            "USB FullSpeed  ", usbSpeed);
                nextline(   MY_BUFFER, MY_INDEX);
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "CF Screen X    ", conWidth,
                            "CF Screen Y    ", conHeight);  

                storeLog(   MY_BUFFER, MY_INDEX, 
                            "FB Screen X    ", fbWidth,
                            "FB Screen Y    ", fbHeight);
         
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "gE Screen X    ", gE_ScreenWidth,
                            "gE Screen Y    ", gE_ScreenHeight);
                nextline(   MY_BUFFER, MY_INDEX);                            

                return TRUE;
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_SIZ - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch; // means the log goes into the pre-init buffer 
                    }
            //  pThis->m_logBuffer[pThis->m_logBufferIndex++] = '\n';
                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0'; // means the log goes into the pre-init buffer 

                pThis->logScreenUpdate();
}