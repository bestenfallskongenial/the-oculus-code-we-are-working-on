#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run(void)
{
                unsigned    start_time_fps_calculation = m_Timer.GetClockTicks();     
                randomVec8              ( start_time_fps_calculation );
                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );

                bufferToScreenDrawBuffer(   m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            0,
                                            0xFFFFFFFF );

                wrapper_from_sd();
                            start_time_fps_calculation = m_Timer.GetClockTicks();     
                randomVec8              ( start_time_fps_calculation );                
                saveFromBufferM         (   PARTITION_NAME_SD,
                                            make83FileName("TXT"),
                                            m_logBuffer,
                                            m_logBufferIndex );

                bufferToScreenDrawBuffer(   m_logBuffer,
                                            0,
                                            m_logBufferIndex,
                                            0,
                                            0,
                                            0xFFFFFFFF );
                randomVec8              ( )
                while (1)
                    {
                    }

                return ShutdownHalt;
}
