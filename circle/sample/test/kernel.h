#ifndef _kernel_h
#define _kernel_h

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>

#include "CLogger.h"

#include <circle/types.h>

#define LOG_BUFFER_SIZE (1024 * 64)
#define LOGLEVEL        LogDebug

enum TShutdownMode
{
                ShutdownNone,
                ShutdownHalt,
                ShutdownReboot
};

class CKernel
{
public:
                CKernel                 (   void );
                ~CKernel                (   void );

                boolean Initialize      (   void );

                TShutdownMode Run       (   void );

public:
                char            m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32             m_logBufferIndex = 0;
                char            m_startupBuffer[LOG_BUFFER_SIZE] = {0};
                u32             m_startupBufferIndex = 0;

public:
                boolean         frameBufferInit            (    void );

                void            bufferToScreenPlot         (    unsigned x,
                                                                unsigned y,
                                                                u32 color );

                void            bufferToScreenDrawChar     (    char        ch,
                                                                unsigned    charCol,
                                                                unsigned    charRow,
                                                                u32         fgColor );

                void            bufferToScreenClear        (    void );

                void            bufferToScreenDrawBuffer   (    const char* pSourceBuffer,
                                                                u32         startIndex,
                                                                u32         endIndex,
                                                                unsigned    startCol,
                                                                unsigned    startRow,
                                                                u32         fgColor );

                void            bufferToScreenGetGrid      (    unsigned& cols,
                                                                unsigned& rows );

                boolean         startupScreen               (   void );                                                                

private:
        static  void            LoggerSink                 (    void*       pContext,
                                                                const char* pText,
                                                                unsigned    nLength );

private:
                CActLED             m_ActLED;
                CKernelOptions      m_Options;

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
};

#endif