	#include "kernel.h"
	#include <string.h>

    static const char FromKernel[] = "kernel";


				CKernel::CKernel						(	void )
				:	gE_FrameBuffer( m_Options.GetWidth(), 
                                    m_Options.GetHeight(), 
                                    32, 
                                    0, 
                                    TRUE ),
                    m_Timer(&m_Interrupt),
                    m_Logger(LOGLEVEL, &m_Timer)
{
                m_ActLED.Blink(5);
}

				CKernel::~CKernel						(	void )
{
}

boolean			CKernel::Initialize						(	void )
{
                boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = frameBufferInit();
                    }

                if (bOK)
                    {
                    bOK = m_Interrupt.Initialize();
                    }

                if (bOK)
                    {
                    bOK = m_Timer.Initialize();
                    }

                if (bOK)
                    {
                    CLogger::SetRawSink(CKernel::LoggerSink, this);

                    bOK = m_Logger.Initialize(0);
                    }

                return bOK;
}

boolean         CKernel::frameBufferInit            	(   void )
{
                if (!gE_FrameBuffer.Initialize()) return FALSE;

                gE_PixelBuffer  = (u32*) gE_FrameBuffer.GetBuffer();
                gE_PitchBytes   = gE_FrameBuffer.GetPitch();
                gE_ScreenWidth  = gE_FrameBuffer.GetWidth();
                gE_ScreenHeight = gE_FrameBuffer.GetHeight();
                gE_CharWidth    = gE_CharGenerator.GetCharWidth();
                gE_CharHeight   = gE_CharGenerator.GetCharHeight();

                if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0) return FALSE;

                gE_Cols = gE_ScreenWidth / gE_CharWidth;
                gE_Rows = gE_ScreenHeight / gE_CharHeight;

                if (gE_Cols == 0 || gE_Rows == 0) return FALSE;

                return TRUE;
}

void            CKernel::bufferToScreenPlot         	(           unsigned 		    x, 
                                                                    unsigned 		    y, 
                                                                    u32 			    color )
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}

void            CKernel::bufferToScreenDrawChar     	(           char        	    ch,
                                                                    unsigned    	    charCol,
                                                                    unsigned    	    charRow,
                                                                    u32         	    fgColor )
{
                const unsigned px = charCol * gE_CharWidth;
                const unsigned py = charRow * gE_CharHeight;

                for (unsigned y = 0; y < gE_CharHeight; y++)
                    {
                    for (unsigned x = 0; x < gE_CharWidth; x++)
                        {
                        bufferToScreenPlot (px + x, py + y,  gE_CharGenerator.GetPixel (ch, x, y) ? fgColor : 0 );
                        }
                    }
}

void            CKernel::bufferToScreenClear        	(   void )
{
                memset(gE_PixelBuffer, 0, gE_PitchBytes * gE_ScreenHeight);
}

void            CKernel::bufferToScreenDrawBuffer   	(   const   char* 	            pSourceBuffer,
                                                                    u32         	    startIndex,
                                                                    u32         	    endIndex,
                                                                    unsigned    	    startCol,
                                                                    unsigned    	    startRow,
                                                                    u32         	    fgColor )
{   
                if (startCol >= gE_Cols || startRow >= gE_Rows || pSourceBuffer == 0 || startIndex >= endIndex ) return;

                unsigned col = startCol;
                unsigned row = startRow;

                for (u32 i = startIndex; i < endIndex; i++)
                    {
                    const char ch = pSourceBuffer[i];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = startCol;
                        row++;
                        if (row >= gE_Rows) break;

                        continue;
                        }
                    if (col < gE_Cols && row < gE_Rows) bufferToScreenDrawChar (ch, col, row, fgColor );

                    col++;
                    if (col >= gE_Cols)
                        {
                        col = startCol;
                        row++;
                        if (row >= gE_Rows) break;
                        }
                    }
}

void            CKernel::bufferToScreenGetGrid      	(           unsigned& 		    cols, 
															        unsigned& 		    rows )
{
                cols = gE_Cols;
                rows = gE_Rows;
}
void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}

void 			CKernel::LoggerSink						(	        void* 			    pContext, 
															const   char* 	            pText, 
															        unsigned 		    nLength )
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch;
                    }

                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0';
}

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