void            CKernel::storeLog              (   const char* label, u32 value1, u32 value2, u32 value3, u32 value4 )
{
                /* always write the label */
                for (const char* p = label; *p; ++p)
                    m_DebugCharArray[m_CharIndex++] = *p;

                /* if all values are placeholders, finish */
                if ( value1 == EMPTYLOG &&
                    value2 == EMPTYLOG &&
                    value3 == EMPTYLOG &&
                    value4 == EMPTYLOG )
                {
                    m_DebugCharArray[m_CharIndex++] = '\n';
                    m_DebugCharArray[m_CharIndex]   = '\0';
                    return;
                }
                /* write first value if valid */
                if (value1 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value1 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write second value if valid */
                if (value2 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value2 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write third value if valid */
                if (value3 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value3 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* write fourth value if valid */
                if (value4 != EMPTYLOG) {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) {
                        char hex = "0123456789ABCDEF"[(value4 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                    }
                }
                /* terminate line */
                m_DebugCharArray[m_CharIndex++] = '\n';
                m_DebugCharArray[m_CharIndex]   = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeMsg              ( const char* label, const void* tx_msg, u32 total_size)
{   
                // insert leading newline
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                // copy label
                for (const char* p = label; *p; ++p) 
                    {
                    m_DebugCharArray[m_CharIndex] = *p;
                    m_CharIndex++;
                    }
                // next line please
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                // hex dump, 16 bytes per line
                const unsigned char* b = (const unsigned char*)tx_msg;
                for (u32 i = 0; i < total_size; ++i) {
                    if (i && (i % 16) == 0) 
                        {
                        m_DebugCharArray[m_CharIndex] = '\n';
                        m_CharIndex++;
                        }
                    unsigned char v = b[i];

                    char hi = "0123456789ABCDEF"[v >> 4];
                    m_DebugCharArray[m_CharIndex] = hi;
                    m_CharIndex++;

                    char lo = "0123456789ABCDEF"[v & 0xF];
                    m_DebugCharArray[m_CharIndex] = lo;
                    m_CharIndex++;

                    m_DebugCharArray[m_CharIndex] = ' ';
                    m_CharIndex++;
                }
                // newline + terminator
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;
                m_DebugCharArray[m_CharIndex] = '\n';
                m_CharIndex++;    
                m_DebugCharArray[m_CharIndex] = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
inline void CH264Decoder::nextline()
{
    m_DebugCharArray[m_CharIndex++] = '\n';
    m_DebugCharArray[m_CharIndex]   = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::bindLogBuffer(u32 index)
{
    m_DebugCharArray = m_bufferLog[index];
    m_CharIndex = 0;
}
/*
Usage:

m_bufferLog = memory_init_buffer(2, 1024 * 64);
bindLogBuffer(0);
*/

// and other stuff:

/*
static inline char toLowerAscii(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}
// Case-insensitive version

bool CKernel::IsValidFile(const char* pFileName,
                                         const char* extension)
{
    if (!pFileName || !extension)
        return false;

    const char* dot = 0;
    const char* p = pFileName;

    // find last '.'
    while (*p)
    {
        if (*p == '.')
            dot = p;
        ++p;
    }

    if (!dot)
        return false;

    int dotPos = (int)(dot - pFileName);

    if (dotPos == 0 || dotPos > 8)
        return false;

    const char* suffix = dot + 1;

    // case-insensitive compare
    while (*suffix && *extension)
    {
        if (toLowerAscii(*suffix) != toLowerAscii(*extension))
            return false;

        ++suffix;
        ++extension;
    }

    return (*suffix == '\0' && *extension == '\0');
}
*/

void            CKernel::modeADC(int channel)
{
                switch (channel)
                {
                    case 4:                                                 /* -------- CH4 : frame / time input -------- */
                    if (g_menu_mode_new == 0)
                        {
                            inOutMatrixFlt[4][out] = inOutMatrixFlt[4][ in] // adc_float_value[4];
                            inOutMatrixInt[4][out] = inOutMatrixInt[4][ in] // adc_int_value[4];
                        }
                    break;

                    case 6:                                                 /* -------- CH6 : pickup + single_tex_mode -------- */
                    if (!menu_pickup_flag[12])
                        {
                            if (inOutMatrixInt[6][raw] > g_menu_pickup_buffer[12] - ADC_TOLERANCE && inOutMatrixInt[6][raw] < g_menu_pickup_buffer[12] + ADC_TOLERANCE)
                            {
                                menu_pickup_flag[12] = true;
                            }
                            else
                            {
                                return;
                            }
                        }
                    if (g_menu_mode_new == 0)
                        {
                        inOutMatrixFlt[6][out] = inOutMatrixFlt[6][ in] // adc_float_value[6];
                        inOutMatrixInt[6][out] = inOutMatrixInt[6][ in] // adc_int_value[6];

                        if (single_tex_mode)                    // this is an error, we integrated this in our mode_storage_buffer ! 
                            {
                            inOutMatrixInt[6][out] = inOutMatrixInt[6][raw] // adc_raw_value[6];
                            }
                        }
                    break;

                    case 7:                                                 /* -------- CH7 : pickup + shader select -------- */
                    if (!menu_pickup_flag[13])
                        {
                        if (inOutMatrixInt[7][raw] > g_menu_pickup_buffer[13] - ADC_TOLERANCE && inOutMatrixInt[7][raw] < g_menu_pickup_buffer[13] + ADC_TOLERANCE)
                            {
                            menu_pickup_flag[13] = true;
                            }
                        else
                            {
                            return;
                            }
                        }
                    if (g_menu_mode_new == 0)
                        {
                        inOutMatrixFlt[7][out] = inOutMatrixFlt[7][ in] // adc_float_value[7];
                        inOutMatrixInt[7][out] = inOutMatrixInt[7][raw] // adc_raw_value[7];
                        }
                    break;

                    default:                                                /* -------- all other channels : normal ADC -------- */
                    inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][ in] // adc_float_value[channel];
                    inOutMatrixInt[channel][out] = inOutMatrixInt[channel][ in] // adc_int_value[channel];
                    break;
                }
}


void            CKernel::modePOT ()
{
                // lets see later ... 
}