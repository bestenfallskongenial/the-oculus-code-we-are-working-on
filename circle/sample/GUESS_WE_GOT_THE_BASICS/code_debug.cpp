#include "kernel.h"

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

void            CKernel::logButtonStatesRuntime(void)
{
                char f_logBuffer[2048];
                u32  f_logIndex = 0;

                f_logBuffer[0] = '\0';

                storeLogHex(   f_logBuffer, f_logIndex,
                            "BTN PRESS_START   DOUBLE        RELEASE       SINGLE", EMPTYLOG );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "A  ",
                            (u32)g_buttons_states[0][BTN_PRESS_START],
                            EMPTYSTR,
                            (u32)g_buttons_states[0][BTN_DOUBLE],
                            EMPTYSTR,
                            (u32)g_buttons_states[0][BTN_RELEASE],
                            EMPTYSTR,
                            (u32)g_buttons_states[0][BTN_SINGLE] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "A   HOLD_TICK",
                            (u32)g_buttons_states[0][BTN_HOLD_TICK] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "B  ",
                            (u32)g_buttons_states[1][BTN_PRESS_START],
                            EMPTYSTR,
                            (u32)g_buttons_states[1][BTN_DOUBLE],
                            EMPTYSTR,
                            (u32)g_buttons_states[1][BTN_RELEASE],
                            EMPTYSTR,
                            (u32)g_buttons_states[1][BTN_SINGLE] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "B   HOLD_TICK",
                            (u32)g_buttons_states[1][BTN_HOLD_TICK],
                            EMPTYSTR, EMPTYLOG,
                            EMPTYSTR, EMPTYLOG,
                            EMPTYSTR, EMPTYLOG );
            /*
                storeLogHex(   f_logBuffer, f_logIndex,
                            "g_menuLayer",
                            g_menuLayer,
                            "CTRL_PIN",
                            GPIO_Read(CTRL_PIN),
                            "bpm 0", 
                            g_lfoBpmMatrix[0][BPM],
                            "bpm 1",
                            g_lfoBpmMatrix[1][BPM]);
                            
                storeLogHex(   f_logBuffer, f_logIndex,
                            "g_gl_program_current",
                            g_gl_program_current,
                            "g_currentProgramBuffer",
                            g_currentProgramBuffer);

                storeLogHex(   f_logBuffer, f_logIndex,                
                            "filecounter[FT_FSH][FLD_LOADED]",
                            filecounter[FT_FSH][FLD_LOADED],
                            "filecounter[FT_FSH][FLD_VALID]",
                            filecounter[FT_FSH][FLD_VALID]);

                storeLogHex(   f_logBuffer, f_logIndex,
                            "mapping",
                            ((g_inOutMatrixInt[ADC_SELECT_PRG][RAW] * (filecounter[FT_FSH][FLD_VALID]  ) ) >> 10) // (filecounter[FT_FSH][FLD_VALID] -1 )
                            );
            */
            /*
                storeLogHex(   f_logBuffer, f_logIndex,
                            "out0",
                            g_inOutMatrixInt[0][OUT],
                            "out1",
                            g_inOutMatrixInt[1][OUT],
                            "out2",
                            g_inOutMatrixInt[2][OUT],
                            "out3",
                            g_inOutMatrixInt[3][OUT]);                                                            

                storeLogHex(   f_logBuffer, f_logIndex,
                            "out4",
                            g_inOutMatrixInt[4][OUT],
                            "out5",
                            g_inOutMatrixInt[5][OUT],
                            "out6",
                            g_inOutMatrixInt[6][OUT],
                            "out7",
                            g_inOutMatrixInt[7][OUT]);
            */
                storeLogHex(   f_logBuffer, f_logIndex,
                            "MODES      00-03",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][0],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][1],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][2],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][3] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "MODES      04-07",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][4],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][5],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][6],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][7] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "LFOs       08-11",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][8],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][9],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][10],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][11] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "AUDIO      12-15",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][12],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][13],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][14],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][15] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "SELECTOR   16-19",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][16],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][17],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][18],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][19] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "FLAGS      20-23",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][20],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][21],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][22],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][23] );

                storeLogHex(   f_logBuffer, f_logIndex,
                            "SYSTEM     28-31",
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][28],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][29],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][30],
                            EMPTYSTR,
                            (u32)g_centralModeBuffer[g_currentProgramBuffer][31] );                
            /*
                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 00-03",
                            g_menuPickUpFlag[0],
                            EMPTYSTR,
                            g_menuPickUpFlag[1],
                            EMPTYSTR,
                            g_menuPickUpFlag[2],
                            EMPTYSTR,
                            g_menuPickUpFlag[3]);    
                            
                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 04-07",
                            g_menuPickUpFlag[4],
                            EMPTYSTR,
                            g_menuPickUpFlag[5],
                            EMPTYSTR,
                            g_menuPickUpFlag[6],
                            EMPTYSTR,
                            g_menuPickUpFlag[7]); 
                            
                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 08-11",
                            g_menuPickUpFlag[8],
                            EMPTYSTR,
                            g_menuPickUpFlag[9],
                            EMPTYSTR,
                            g_menuPickUpFlag[10],
                            EMPTYSTR,
                            g_menuPickUpFlag[11]); 
                            
                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 12-15",
                            g_menuPickUpFlag[12],
                            EMPTYSTR,
                            g_menuPickUpFlag[13],
                            EMPTYSTR,
                            g_menuPickUpFlag[14],
                            EMPTYSTR,
                            g_menuPickUpFlag[15]);   

                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 16-19",
                            g_menuPickUpFlag[16],
                            EMPTYSTR,
                            g_menuPickUpFlag[17],
                            EMPTYSTR,
                            g_menuPickUpFlag[18],
                            EMPTYSTR,
                            g_menuPickUpFlag[19]); 

                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 20-23",
                            g_menuPickUpFlag[20],
                            EMPTYSTR,
                            g_menuPickUpFlag[21],
                            EMPTYSTR,
                            g_menuPickUpFlag[22],
                            EMPTYSTR,
                            g_menuPickUpFlag[23]);                                           

                storeLogHex(   f_logBuffer, f_logIndex,
                            "Flags 28-31",
                            g_menuPickUpFlag[28],
                            EMPTYSTR,
                            g_menuPickUpFlag[29],
                            EMPTYSTR,
                            g_menuPickUpFlag[30],
                            EMPTYSTR,
                            g_menuPickUpFlag[31]); 
            */
                bufferScreenDraw(  f_logBuffer,
                                0,
                                f_logIndex,
                                0,
                                3,
                                0xFFFFFFFF );
}

void CKernel::ADCDebug()
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    storeLogHex(   f_logBuffer, f_logIndex,
                "0 RAW", (u32)g_inOutMatrixInt[0][RAW],
                "1 RAW", (u32)g_inOutMatrixInt[1][RAW],
                "2 RAW", (u32)g_inOutMatrixInt[2][RAW],
                "3 RAW", (u32)g_inOutMatrixInt[3][RAW] );
    storeLogHex(   f_logBuffer, f_logIndex,
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

    storeLogHex(   f_logBuffer, f_logIndex,
                "0 RAW",
                (u32)g_inOutMatrixInt[0][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[0][VAL],
                "RND",
                (u32)g_inOutMatrixInt[0][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "1 RAW",
                (u32)g_inOutMatrixInt[1][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[1][VAL],
                "RND",
                (u32)g_inOutMatrixInt[1][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "2 RAW",
                (u32)g_inOutMatrixInt[2][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[2][VAL],
                "RND",
                (u32)g_inOutMatrixInt[2][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "3 RAW",
                (u32)g_inOutMatrixInt[3][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[3][VAL],
                "RND",
                (u32)g_inOutMatrixInt[3][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "4 RAW",
                (u32)g_inOutMatrixInt[4][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[4][VAL],
                "RND",
                (u32)g_inOutMatrixInt[4][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "5 RAW",
                (u32)g_inOutMatrixInt[5][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[5][VAL],
                "RND",
                (u32)g_inOutMatrixInt[5][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "6 RAW",
                (u32)g_inOutMatrixInt[6][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[6][VAL],
                "RND",
                (u32)g_inOutMatrixInt[6][RND] );

    storeLogHex(   f_logBuffer, f_logIndex,
                "7 RAW",
                (u32)g_inOutMatrixInt[7][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[7][VAL],
                "RND",
                (u32)g_inOutMatrixInt[7][RND] );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       0,
                       0xFFFFFFFF );
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

                unsigned fbWidth        =  gE_FrameBuffer.GetWidth();
                unsigned fbHeight       =  gE_FrameBuffer.GetHeight();
                
                nextline(   MY_BUFFER, MY_INDEX);

                storeLogU32(   MY_BUFFER, MY_INDEX, 
                            machineName, EMPTYLOG );

                nextline(   MY_BUFFER, MY_INDEX);    

                storeLogU32(   MY_BUFFER, MY_INDEX, 
                            socName          , EMPTYLOG, 
                            "V", modelMajor,
                            ".", modelRevision);

                nextline(   MY_BUFFER, MY_INDEX);

                storeLogU32(   MY_BUFFER, MY_INDEX, 
                            "RAM Size     MB", ramSize,
                            "CPU Speed Mode ", cpuSpeedMode,
                            "SoC Max Temp   ", socMaxTemp);    

                nextline(   MY_BUFFER, MY_INDEX);

                storeLogU32(   MY_BUFFER, MY_INDEX, 
                            "Clock CORE MHz", coreClock,
                            "ARM  ", armClock,
                            "EMMC ", emmcClock,
                            "EMMC2", emmc2Clock);  

                nextline(   MY_BUFFER, MY_INDEX);

                storeLogU32(   MY_BUFFER, MY_INDEX,
                            "Clock UART  MHz", uartClock,
                            "DMA Channels    ", dmaChannel);

                storeLogU32(   MY_BUFFER, MY_INDEX,                            
                            "USB Delay      ", usbDelay,
                            "USB FullSpeed  ", usbSpeed);      

                nextline(   MY_BUFFER, MY_INDEX);

                storeLogU32(   MY_BUFFER, MY_INDEX, 
                            "FB Screen X    ", fbWidth,
                            "/ Y "           , fbHeight,
                            "gE Screen X    ", gE_ScreenWidth,
                            " / Y "          , gE_ScreenHeight);                            

                nextline(   MY_BUFFER, MY_INDEX);                            

                return TRUE;
}
