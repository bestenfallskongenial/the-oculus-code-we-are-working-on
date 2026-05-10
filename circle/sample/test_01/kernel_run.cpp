#include "kernel.h"

    static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run                              (   void )
{
                unsigned tick = 0;
                boolean  bExceptionTimerStarted = FALSE;

                while (1)
                    {
                    m_Timer.MsDelay(1000);

                    tick++;

                    bufferToScreenClear();

                    u32 colorCycle[8] =
                        {
                        0xFFFFFFFF,
                        0xFFFF0000,
                        0xFF00FF00,
                        0xFF0000FF,
                        0xFFFFFF00,
                        0xFFFF00FF,
                        0xFF00FFFF,
                        0xFF808080
                        };

                    u32 color0 = colorCycle[(tick + 0) & 7];
                    u32 color1 = colorCycle[(tick + 1) & 7];
                    u32 color2 = colorCycle[(tick + 2) & 7];
                    u32 color3 = colorCycle[(tick + 3) & 7];
                    u32 color4 = colorCycle[(tick + 4) & 7];
                    u32 color5 = colorCycle[(tick + 5) & 7];

                    int adc0 = ReadMCP3008Raw(0);
                    int adc1 = ReadMCP3008Raw(1);
                    int adc2 = ReadMCP3008Raw(2);
                    int adc3 = ReadMCP3008Raw(3);
                    int adc4 = ReadMCP3008Raw(4);
                    int adc5 = ReadMCP3008Raw(5);
                    int adc6 = ReadMCP3008Raw(6);
                    int adc7 = ReadMCP3008Raw(7);

                    unsigned mytestCol = tick % gE_Cols;
                    unsigned mytestRow = 0;

                    bufferToScreenDrawBuffer(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            mytestCol,
                                            mytestRow,
                                            color0
                                            );

                    unsigned alphabetStart = tick % 20;
                    unsigned alphabetEnd   = alphabetStart + 8;
                    unsigned alphabetCol   = 0;
                    unsigned alphabetRow   = 2;

                    bufferToScreenDrawBuffer(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            alphabetStart,
                                            alphabetEnd,
                                            alphabetCol,
                                            alphabetRow,
                                            color1
                                            );

                    unsigned columnRowCol = (tick * 2) % gE_Cols;
                    unsigned columnRowRow = 4 + ((tick / 2) % 4);

                    bufferToScreenDrawBuffer(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            columnRowCol,
                                            columnRowRow,
                                            color2
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "tick %u mytest col %u row %u",
                                    tick,
                                    mytestCol,
                                    mytestRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            10,
                                            color3
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogWarning,
                                    "alphabet start %u end %u col %u row %u",
                                    alphabetStart,
                                    alphabetEnd,
                                    alphabetCol,
                                    alphabetRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            4,
                                            12,
                                            color4
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogError,
                                    "columnrow col %u row %u",
                                    columnRowCol,
                                    columnRowRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            8,
                                            14,
                                            color5
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "adc0 %d adc1 %d adc2 %d adc3 %d",
                                    adc0,
                                    adc1,
                                    adc2,
                                    adc3
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            16,
                                            colorCycle[(tick + 6) & 7]
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "adc4 %d adc5 %d adc6 %d adc7 %d",
                                    adc4,
                                    adc5,
                                    adc6,
                                    adc7
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            18,
                                            colorCycle[(tick + 7) & 7]
                                            );
/*
                    if (!bExceptionTimerStarted && tick == 10)
                        {
                        bExceptionTimerStarted = TRUE;

                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        m_Logger.Write(
                                        FromKernel,
                                        LogNotice,
                                        "starting exception timer"
                                        );

                        bufferToScreenDrawBuffer(
                                                m_logBuffer,
                                                0,
                                                m_logBufferIndex,
                                                0,
                                                16,
                                                0xFFFFFFFF
                                                );

                        m_Timer.StartKernelTimer(5 * HZ, TimerHandler);
                        }
*/     
                    }

} 
/*
TShutdownMode CKernel::Run                              (   void )
{
                unsigned tick = 0;
                boolean  bExceptionTimerStarted = FALSE;

                while (1)
                    {
                    m_Timer.MsDelay(1000);

                    tick++;

                    bufferToScreenClear();

                    unsigned mytestCol = tick % gE_Cols;
                    unsigned mytestRow = 0;

                    bufferToScreenDrawBuffer(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            mytestCol,
                                            mytestRow,
                                            0xFFFFFFFF
                                            );

                    unsigned alphabetStart = tick % 20;
                    unsigned alphabetEnd   = alphabetStart + 8;
                    unsigned alphabetCol   = 0;
                    unsigned alphabetRow   = 2;

                    bufferToScreenDrawBuffer(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            alphabetStart,
                                            alphabetEnd,
                                            alphabetCol,
                                            alphabetRow,
                                            0xFFFFFFFF
                                            );

                    unsigned columnRowCol = (tick * 2) % gE_Cols;
                    unsigned columnRowRow = 4 + ((tick / 2) % 4);

                    bufferToScreenDrawBuffer(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            columnRowCol,
                                            columnRowRow,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "tick %u mytest col %u row %u",
                                    tick,
                                    mytestCol,
                                    mytestRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            10,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogWarning,
                                    "alphabet start %u end %u col %u row %u",
                                    alphabetStart,
                                    alphabetEnd,
                                    alphabetCol,
                                    alphabetRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            4,
                                            12,
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogError,
                                    "columnrow col %u row %u",
                                    columnRowCol,
                                    columnRowRow
                                    );

                    bufferToScreenDrawBuffer(
                                            m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            8,
                                            14,
                                            0xFFFFFFFF
                                            );

                    if (!bExceptionTimerStarted && tick == 10)
                        {
                        bExceptionTimerStarted = TRUE;

                        m_logBufferIndex = 0;
                        m_logBuffer[0] = '\0';

                        m_Logger.Write(
                                        FromKernel,
                                        LogNotice,
                                        "starting exception timer"
                                        );

                        bufferToScreenDrawBuffer(
                                                m_logBuffer,
                                                0,
                                                m_logBufferIndex,
                                                0,
                                                16,
                                                0xFFFFFFFF
                                                );

                        m_Timer.StartKernelTimer(5 * HZ, TimerHandler);
                        }
   
                    }

}
*/
