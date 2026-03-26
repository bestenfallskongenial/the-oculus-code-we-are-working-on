// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// deterministic log and print
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeLog       (   char*       buffer, 
                                            u32&        index,
                                            const char* label,
                                            u32         value1, 
                                            u32         value2,
                                            u32         value3, 
                                            u32         value4)
{
                for (const char* p = label; *p; ++p)                    // always write the label
                    {
                    buffer[index] = *p;
                    index++;
                    }
                if (value1 == EMPTYLOG &&                              // if all values are placeholders, finish
                    value2 == EMPTYLOG &&
                    value3 == EMPTYLOG &&
                    value4 == EMPTYLOG )
                        {
                        buffer[index] = '\n';
                        index++;
                        buffer[index]   = '\0';
                        return;
                        }
                if (value1 != EMPTYLOG)                                 // write first value if valid
                    {
                    buffer[index] = ' ';
                    index++;
                    buffer[index] = '0';
                    index++;
                    buffer[index] = 'x';
                    index++;
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value1 >> (i * 4)) & 0xF];
                        buffer[index] = hex;
                        index++;
                        }
                    }
                if (value2 != EMPTYLOG)                                 // write second value if valid
                    {
                    buffer[index] = ' ';
                    index++;
                    buffer[index] = '0';
                    index++;
                    buffer[index] = 'x';
                    index++;
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value2 >> (i * 4)) & 0xF];
                        buffer[index] = hex;
                        index++;
                        }
                    }
                if (value3 != EMPTYLOG)                                 // write third value if valid
                    {
                    buffer[index] = ' ';
                    index++;
                    buffer[index] = '0';
                    index++;
                    buffer[index] = 'x';
                    index++;
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value3 >> (i * 4)) & 0xF];
                        buffer[index] = hex;
                        index++;
                        }
                    }
                if (value4 != EMPTYLOG)                                 // write fourth value if valid
                    {
                    buffer[index] = ' ';
                    index++;
                    buffer[index] = '0';
                    index++;
                    buffer[index] = 'x';
                    index++;
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value4 >> (i * 4)) & 0xF];
                        buffer[index] = hex;
                        index++;
                        }
                }
                buffer[index] = '\n';                                 // terminate line
                index++;
                buffer[index]   = '\0';
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeMsg       (   char*       buffer,
                                            u32&        index,
                                            const char* label,
                                            const void* tx_msg,
                                            u32         total_size)
{
                buffer[index] = '\n';                                   // insert leading newline
                index++;

                for (const char* p = label; *p; ++p)                    // copy label
                    {
                    buffer[index] = *p;
                    index++;
                    }
                buffer[index] = '\n';                                   // next line please
                index++;

                const unsigned char* b = (const unsigned char*)tx_msg;  // hex dump, 16 bytes per line

                for (u32 i = 0; i < total_size; ++i) 
                    {
                    if (i && (i % 16) == 0)
                        {
                        buffer[index] = '\n';
                        index++;
                        }
                    unsigned char v = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    buffer[index] = hi;
                    index++;
                    char lo = "0123456789ABCDEF"[v & 0xF];
                    buffer[index] = lo;
                    index++;
                    buffer[index] = ' ';
                    index++;
                    }
                buffer[index] = '\n';                                   // newline + terminator
                index++;
                buffer[index] = '\n';
                index++;
                buffer[index] = '\0';
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::nextline(char* buffer,
                                   u32& index)
{
                buffer[index] = '\n';
                index++;
                buffer[index] = '\0';
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//              we try to avoid CString, CScreen etc
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void     CKernel::bufferToScreenPlot                          (   unsigned x, unsigned y, u32 color )   // why the fuck static?
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void     CKernel::bufferToScreenDrawChar                     (   char ch,
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
                        bufferToScreenPlot (px + x, py + y,
                                                gE_CharGenerator.GetPixel (ch, x, y) ? fgColor : bgColor );
                        }
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean         CKernel::bufferToScreenInit                              (   void )
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::bufferToScreenClear                         (   u32 bgColor)
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::bufferToScreenDrawBufferSegment        (   const char *pSourceBuffer,
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
                        bufferToScreenDrawChar (ch, col, row, fgColor, bgColor);
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned        CKernel::bufferToScreenGetGrid                         (   unsigned &cols, unsigned &rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------