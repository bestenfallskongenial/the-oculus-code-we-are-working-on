	#include "kernel.h"
	#include <string.h>

				CKernel::CKernel						(	void )
				:	gE_FrameBuffer(m_Options.GetWidth(), m_Options.GetHeight(), 32, 0, TRUE ),
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

void            CKernel::bufferToScreenPlot         	(   unsigned 		x, 
															unsigned 		y, 
															u32 			color )
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}

void            CKernel::bufferToScreenDrawChar     	(   char        	ch,
															unsigned    	charCol,
															unsigned    	charRow,
															u32         	fgColor )
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

void            CKernel::bufferToScreenDrawBuffer   	(   const char* 	pSourceBuffer,
															u32         	startIndex,
															u32         	endIndex,
															unsigned    	startCol,
															unsigned    	startRow,
															u32         	fgColor )
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

void            CKernel::bufferToScreenGetGrid      	(   unsigned& 		cols, 
															unsigned& 		rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}

void            CKernel::storeLogLong               (   char*       p_buffer,
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
}

boolean CKernel::startupScreen(void)
{
                m_startupBufferIndex = 0;
                m_startupBuffer[0] = '\0';

                const char* machineName = m_MachineInfo.GetMachineName();
                const char* socName     = m_MachineInfo.GetSoCName();

                unsigned modelMajor     = m_MachineInfo.GetModelMajor();
                unsigned modelRevision  = m_MachineInfo.GetModelRevision();
                unsigned ramSize        = m_MachineInfo.GetRAMSize();

                unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;
                unsigned socMaxTemp     = m_Options.GetSoCMaxTemp();

                unsigned coreClock      = m_MachineInfo.GetClockRate(CLOCK_ID_CORE)  / 1000000;
                unsigned armClock       = m_MachineInfo.GetClockRate(CLOCK_ID_ARM)   / 1000000;
                unsigned emmcClock      = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC)  / 1000000;
                unsigned emmc2Clock     = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC2) / 1000000;
                unsigned uartClock      = m_MachineInfo.GetClockRate(CLOCK_ID_UART)  / 1000000;

                unsigned dmaChannel     = m_MachineInfo.AllocateDMAChannel(DMA_CHANNEL_NORMAL);
                m_MachineInfo.FreeDMAChannel(dmaChannel);

                unsigned usbDelay       = m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       = m_Options.GetUSBFullSpeed();

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "Machine Model", EMPTYLOG, machineName);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "SoC Name", EMPTYLOG, socName,
                                                                  	EMPTYLOG, "Model Major    ", modelMajor,
                                                                  	"Model Revision ", modelRevision);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "RAM Size     MB", ramSize);
                storeLogLong(m_startupBuffer, m_startupBufferIndex, "CPU Speed Mode", cpuSpeedMode);
                storeLogLong(m_startupBuffer, m_startupBufferIndex, "SoC Max Temperature", socMaxTemp);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "Clock CORE MHz", coreClock,
                                                                  	"Clock ARM MHz", armClock);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "Clock EMMC  MHz", emmcClock,
                                                                  	"EMMC2 MHz", emmc2Clock);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "Clock UART  MHz", uartClock);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "DMA Channel", dmaChannel);

                storeLogLong(m_startupBuffer, m_startupBufferIndex, "USB Delay", usbDelay,
                                                                  	"USB FullSpeed", usbSpeed);

                bufferToScreenClear();

                bufferToScreenDrawBuffer(
                                        m_startupBuffer,
                                        0,
                                        m_startupBufferIndex,
                                        0,
                                        0,
                                        0xFFFFFFFF
                                        );

                return TRUE;
}

void 			CKernel::LoggerSink						(	void* 			pContext, 
															const char* 	pText, 
															unsigned 		nLength )
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
