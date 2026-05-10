#include "kernel.h"

TShutdownMode CKernel::Run                              (   void )
{
                unsigned tick = 0;
                boolean  bExceptionTimerStarted = FALSE;

                while (1)
                    {
                    m_Timer.MsDelay(1000);

                    tick++;

                    bufferToScreenClear();

                    bufferToScreenDrawBuffer(
                                            "mytest",
                                            0,
                                            sizeof("mytest"),
                                            tick % gE_Cols,
                                            0,
                                            0xFFFFFFFF
                                            );

                    bufferToScreenDrawBuffer(
                                            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                            tick % 20,
                                            (tick % 20) + 8,
                                            0,
                                            2,
                                            0xFFFFFFFF
                                            );

                    bufferToScreenDrawBuffer(
                                            "column and row test",
                                            0,
                                            sizeof("column and row test"),
                                            (tick * 2) % gE_Cols,
                                            4 + ((tick / 2) % 4),
                                            0xFFFFFFFF
                                            );

                    m_logBufferIndex = 0;
                    m_logBuffer[0] = '\0';

                    m_Logger.Write(
                                    FromKernel,
                                    LogNotice,
                                    "individual log tick %u",
                                    tick
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
                                    "warning level test"
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
                                    "error level test"
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