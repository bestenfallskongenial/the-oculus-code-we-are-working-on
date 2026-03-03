#ifndef _buffer_to_screen_b_h
#define _buffer_to_screen_b_h

#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>
#include <circle/types.h>

class CBufferToScreenB
{
public:
	typedef u32 TPixel;

	static const unsigned Depth = 32;
	static const TPixel DefaultFG = 0xFFFFFFFF; // BGRA white
	static const TPixel DefaultBG = 0xFF000000; // BGRA black

public:
	CBufferToScreenB (unsigned nWidth = 0, unsigned nHeight = 0);
	~CBufferToScreenB (void);

	boolean Initialize (void);
	void Clear (TPixel nBackground = DefaultBG);

	void PutLine (unsigned nRow, const char *pText);
	void PutTextBuffer (const char *pText);
	void Render (TPixel nForeground = DefaultFG, TPixel nBackground = DefaultBG);

	unsigned GetRows (void) const;
	unsigned GetCols (void) const;
	unsigned GetWidth (void) const;
	unsigned GetHeight (void) const;

private:
	void RenderChar (char ch, unsigned nPosX, unsigned nPosY, TPixel nForeground, TPixel nBackground);
	void Plot (unsigned nPosX, unsigned nPosY, TPixel nColor);

private:
	CBcmFrameBuffer m_FrameBuffer;
	CCharGenerator m_CharGen;

	TPixel *m_pBuffer;
	unsigned m_nPitchPixels;
	unsigned m_nWidth;
	unsigned m_nHeight;
	unsigned m_nCols;
	unsigned m_nRows;
	char *m_pChars;
};

#endif