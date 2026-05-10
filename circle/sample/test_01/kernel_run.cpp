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