#include "kernel.h"

//  static const char FromKernel[] = "kernel";

TShutdownMode CKernel::Run                              (   void )
{
while (1)
    {    
    saveFromBufferM         (   PARTITION_NAME_SD,
                                make83FileName("TXT"),
                                m_logBuffer,
                                m_logBufferIndex );
    bufferToScreenDrawBuffer(   m_logBuffer,
                                0,
                                m_logBufferIndex,
                                0,
                                0,
                                0xFFFFFFFF
                                );
    

    debug();
    }
return ShutdownHalt;
} 
