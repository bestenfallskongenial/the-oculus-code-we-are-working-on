/*
for kernel.h


boolean CKernel::buffer_to_screen_init                              (   void );
void    CKernel::buffer_to_screen_clear_screen                      (   u32 bgColor );
void    CKernel::buffer_to_screen_draw_buffer_segment_to_screen     (   const char *pSourceBuffer,
                                                                        u32 startIndex,
                                                                        u32 endIndex,
                                                                        unsigned startCol,
                                                                        unsigned startRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor );
unsigned CKernel::buffer_to_screen_get_grid                         (    unsigned &cols, 
                                                                        unsigned &rows )

#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

    Self-contained framebuffer setup:
    width/height are queried from firmware via mailbox when constructed with 0,0.
    This reflects the effective display mode (typically driven by config.txt/EDID firmware state).
*/

static CBcmFrameBuffer gE_FrameBuffer (0, 0, 32, 0, TRUE);
static CCharGenerator  gE_CharGenerator;

static u32      *gE_PixelBuffer  = 0;
static unsigned  gE_PitchBytes   = 0;
static unsigned  gE_ScreenWidth  = 0;
static unsigned  gE_ScreenHeight = 0;
static unsigned  gE_CharWidth    = 0;
static unsigned  gE_CharHeight   = 0;
static unsigned  gE_Cols         = 0;
static unsigned  gE_Rows         = 0;

static void CKernel::buffer_to_screen_plot                          (   unsigned x, unsigned y, u32 color )
{
    gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}

static void CKernel::buffer_to_screen_draw_char                     (   char ch,
                                                                        unsigned charCol,
                                                                        unsigned charRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor )
{
    const unsigned px = charCol * gE_CharWidth;
    const unsigned py = charRow * gE_CharHeight;

    for (unsigned y = 0; y < gE_CharHeight; y++)
    {
        for (unsigned x = 0; x < gE_CharWidth; x++)
        {
            buffer_to_screen_plot (px + x, py + y,
                                     gE_CharGenerator.GetPixel (ch, x, y) ? fgColor : bgColor );
        }
    }
}

boolean CKernel::buffer_to_screen_init                              (   void )
{
    if (!gE_FrameBuffer.Initialize ())
    {
        return FALSE;
    }
    gE_PixelBuffer  = (u32 *) gE_FrameBuffer.GetBuffer ();
    gE_PitchBytes   = gE_FrameBuffer.GetPitch ();
    gE_ScreenWidth  = gE_FrameBuffer.GetWidth ();
    gE_ScreenHeight = gE_FrameBuffer.GetHeight ();
    gE_CharWidth    = gE_CharGenerator.GetCharWidth ();
    gE_CharHeight   = gE_CharGenerator.GetCharHeight ();

    if (gE_PixelBuffer == 0 || gE_CharWidth == 0 || gE_CharHeight == 0)
    {
        return FALSE;
    }
    gE_Cols = gE_ScreenWidth / gE_CharWidth;
    gE_Rows = gE_ScreenHeight / gE_CharHeight;

    if (gE_Cols == 0 || gE_Rows == 0)
    {
        return FALSE;
    }
    return TRUE;
}

void CKernel::buffer_to_screen_clear_screen                         (   u32 bgColor)
{
    const unsigned pitch32 = gE_PitchBytes >> 2;

    for (unsigned y = 0; y < gE_ScreenHeight; y++)
    {
        for (unsigned x = 0; x < gE_ScreenWidth; x++)
        {
            gE_PixelBuffer[y * pitch32 + x] = bgColor;
        }
    }
}

void CKernel::buffer_to_screen_draw_buffer_segment_to_screen        (   const char *pSourceBuffer,
                                                                        u32 startIndex,
                                                                        u32 endIndex,
                                                                        unsigned startCol,
                                                                        unsigned startRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor)
{
    if (startCol >= gE_Cols || startRow >= gE_Rows)
    {
        return;
    }
    unsigned col = startCol;
    unsigned row = startRow;

    for (u32 i = startIndex; i < endIndex; i++)
    {
        const char ch = pSourceBuffer[i];

        if (ch == '\0')
        {
            break;
        }
        if (ch == '\n')
        {
            col = startCol;
            row++;
            if (row >= gE_Rows)
            {
                break;
            }
            continue;
        }
        if (col < gE_Cols && row < gE_Rows)
        {
            buffer_to_screen_draw_char (ch, col, row, fgColor, bgColor);
        }
        col++;
        if (col >= gE_Cols)
        {
            col = startCol;
            row++;
            if (row >= gE_Rows)
            {
                break;
            }
        }
    }
}

unsigned CKernel::buffer_to_screen_get_grid                         (   unsigned &cols, unsigned &rows)
{
    cols = gE_Cols;
    rows = gE_Rows;
}