#include "buffer_to_screen_b.h"

#include <string.h>

CBufferToScreenB::CBufferToScreenB (unsigned nWidth, unsigned nHeight)
: m_FrameBuffer (nWidth, nHeight, Depth),
  m_pBuffer (0),
  m_nPitchPixels (0),
  m_nWidth (0),
  m_nHeight (0),
  m_nCols (0),
  m_nRows (0),
  m_pChars (0)
{
}

CBufferToScreenB::~CBufferToScreenB (void)
{
	delete [] m_pChars;
	m_pChars = 0;
}

boolean CBufferToScreenB::Initialize (void)
{
	if (!m_FrameBuffer.Initialize ())
	{
		return FALSE;
	}

	m_pBuffer = (TPixel *) (uintptr) m_FrameBuffer.GetBuffer ();
	m_nWidth = m_FrameBuffer.GetWidth ();
	m_nHeight = m_FrameBuffer.GetHeight ();
	m_nPitchPixels = m_FrameBuffer.GetPitch () / sizeof (TPixel);

	m_nCols = m_nWidth / m_CharGen.GetCharWidth ();
	m_nRows = m_nHeight / m_CharGen.GetCharHeight ();
	if (m_nCols == 0 || m_nRows == 0)
	{
		return FALSE;
	}

	m_pChars = new char[m_nCols * m_nRows];
	if (!m_pChars)
	{
		return FALSE;
	}

	memset (m_pChars, ' ', m_nCols * m_nRows);
	Clear ();
	return TRUE;
}

void CBufferToScreenB::Clear (TPixel nBackground)
{
	for (unsigned y = 0; y < m_nHeight; y++)
	{
		TPixel *pRow = m_pBuffer + y * m_nPitchPixels;
		for (unsigned x = 0; x < m_nWidth; x++)
		{
			pRow[x] = nBackground;
		}
	}

	if (m_pChars)
	{
		memset (m_pChars, ' ', m_nCols * m_nRows);
	}
}

void CBufferToScreenB::PutLine (unsigned nRow, const char *pText)
{
	if (!pText || nRow >= m_nRows || !m_pChars)
	{
		return;
	}

	const unsigned nOffset = nRow * m_nCols;
	unsigned nCol = 0;

	while (pText[nCol] && nCol < m_nCols)
	{
		m_pChars[nOffset + nCol] = pText[nCol];
		nCol++;
	}

	while (nCol < m_nCols)
	{
		m_pChars[nOffset + nCol] = ' ';
		nCol++;
	}
}

void CBufferToScreenB::PutTextBuffer (const char *pText)
{
	if (!pText || !m_pChars)
	{
		return;
	}

	for (unsigned i = 0; i < m_nCols * m_nRows; i++)
	{
		m_pChars[i] = ' ';
	}

	unsigned nRow = 0;
	unsigned nCol = 0;

	for (const char *p = pText; *p; p++)
	{
		if (*p == '\n')
		{
			nRow++;
			nCol = 0;
		}
		else
		{
			if (nRow < m_nRows)
			{
				if (nCol < m_nCols)
				{
					m_pChars[nRow * m_nCols + nCol] = *p;
					nCol++;
				}
				else
				{
					nRow++;
					nCol = 0;
					if (nRow < m_nRows)
					{
						m_pChars[nRow * m_nCols + nCol] = *p;
						nCol++;
					}
				}
			}
		}

		if (nRow >= m_nRows)
		{
			break;
		}
	}
}

void CBufferToScreenB::Render (TPixel nForeground, TPixel nBackground)
{
	if (!m_pChars)
	{
		return;
	}

	const unsigned nCharWidth = m_CharGen.GetCharWidth ();
	const unsigned nCharHeight = m_CharGen.GetCharHeight ();

	for (unsigned row = 0; row < m_nRows; row++)
	{
		for (unsigned col = 0; col < m_nCols; col++)
		{
			const char ch = m_pChars[row * m_nCols + col];
			RenderChar (ch, col * nCharWidth, row * nCharHeight, nForeground, nBackground);
		}
	}
}

unsigned CBufferToScreenB::GetRows (void) const
{
	return m_nRows;
}

unsigned CBufferToScreenB::GetCols (void) const
{
	return m_nCols;
}

unsigned CBufferToScreenB::GetWidth (void) const
{
	return m_nWidth;
}

unsigned CBufferToScreenB::GetHeight (void) const
{
	return m_nHeight;
}

void CBufferToScreenB::RenderChar (char ch, unsigned nPosX, unsigned nPosY,
					   TPixel nForeground, TPixel nBackground)
{
	const unsigned nCharWidth = m_CharGen.GetCharWidth ();
	const unsigned nCharHeight = m_CharGen.GetCharHeight ();

	for (unsigned y = 0; y < nCharHeight; y++)
	{
		for (unsigned x = 0; x < nCharWidth; x++)
		{
			const boolean bSet = m_CharGen.GetPixel (ch, x, y);
			Plot (nPosX + x, nPosY + y, bSet ? nForeground : nBackground);
		}
	}
}

void CBufferToScreenB::Plot (unsigned nPosX, unsigned nPosY, TPixel nColor)
{
	if (nPosX >= m_nWidth || nPosY >= m_nHeight)
	{
		return;
	}

	m_pBuffer[nPosY * m_nPitchPixels + nPosX] = nColor;
}

// how to use

bool CKernel::display_startup_screen(CUBE_STATE_T *state)
{
    (void) state;

    // 1) Build one continuous text stream: '\n' separated, '\0' terminated
    static char startupText[4096];
    u32 idx = 0;
    startupText[0] = '\0';

    // if you already have CKernel::nextline() for this buffer style, use that.
    // otherwise do explicit newline + terminator:
    auto push_newline = [&](void)
    {
        startupText[idx++] = '\n';
        startupText[idx]   = '\0';
    };

    // Header spacing
    push_newline();
    push_newline();

    // 2) Reuse your storeLog/storeMsg style
    // Label-only lines:
    storeLog(startupText, idx, "SYSTEM STARTUP", EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    push_newline();

    // Numeric lines (hex formatting like your existing storeLog)
    storeLog(startupText, idx, "RAM Size (MB)", m_MachineInfo.GetRAMSize(), EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog(startupText, idx, "Core MHz", m_MachineInfo.GetClockRate(CLOCK_ID_CORE) / 1000000, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog(startupText, idx, "ARM MHz",  m_MachineInfo.GetClockRate(CLOCK_ID_ARM)  / 1000000, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog(startupText, idx, "EMMC MHz", m_MachineInfo.GetClockRate(CLOCK_ID_EMMC) / 1000000, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog(startupText, idx, "UART MHz", m_MachineInfo.GetClockRate(CLOCK_ID_UART) / 1000000, EMPTYLOG, EMPTYLOG, EMPTYLOG);

    // Text fields via storeMsg (hex-dump style in your current util implementation)
    // If you later add a plain-text storeLabelValue(), use that instead.
    storeMsg(startupText, idx, "Machine Model", m_MachineInfo.GetMachineName(),
             strlen(m_MachineInfo.GetMachineName()));

    const char *cpuSpeed = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? "Maximum" : "Low";
    storeMsg(startupText, idx, "CPU Speed", cpuSpeed, strlen(cpuSpeed));

    // 3) Render stream buffer to framebuffer text surface
    // Assume you added member: CBufferToScreenB m_BufferToScreen;
    m_BufferToScreen.Clear(CBufferToScreenB::DefaultBG);
    m_BufferToScreen.PutTextBuffer(startupText);
    m_BufferToScreen.Render(CBufferToScreenB::DefaultFG, CBufferToScreenB::DefaultBG);

    return true;
}
