//----------------------------------------------------------------------------------------------------------------------------------------------------
// deterministic log and print
//----------------------------------------------------------------------------------------------------------------------------------------------------
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

void CKernel::storeLogLong  (   char*       buffer,
                                u32&        index,
                                const char* l1, u32 v1,
                                const char* l2, u32 v2,
                                const char* l3, u32 v3,
                                const char* l4, u32 v4)
{
    for (const char* p = l1; *p; ++p)   // always write first label
    {
        buffer[index] = *p;
        index++;
    }
    if (v1 != EMPTYLOG)
    {
        buffer[index] = ' ';
        index++;
        buffer[index] = '0';
        index++;
        buffer[index] = 'x';
        index++;
        for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
        {
            char hex = "0123456789ABCDEF"[(v1 >> (i * 4)) & 0xF];
            buffer[index] = hex;
            index++;
        }
    }
    if (l2 != EMPTYSTR) // label 2
    {
        buffer[index] = ' ';
        index++;
        for (const char* p = l2; *p; ++p)
        {
            buffer[index] = *p;
            index++;
        }
    }
    if (v2 != EMPTYLOG)
    {
        buffer[index] = ' ';
        index++;
        buffer[index] = '0';
        index++;
        buffer[index] = 'x';
        index++;
        for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
        {
            char hex = "0123456789ABCDEF"[(v2 >> (i * 4)) & 0xF];
            buffer[index] = hex;
            index++;
        }
    }
    if (l3 != EMPTYSTR) // label 3
    {
        buffer[index] = ' ';
        index++;
        for (const char* p = l3; *p; ++p)
        {
            buffer[index] = *p;
            index++;
        }
    }
    if (v3 != EMPTYLOG)
    {
        buffer[index] = ' ';
        index++;
        buffer[index] = '0';
        index++;
        buffer[index] = 'x';
        index++;
        for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
        {
            char hex = "0123456789ABCDEF"[(v3 >> (i * 4)) & 0xF];
            buffer[index] = hex;
            index++;
        }
    }
    if (l4 != EMPTYSTR) // label 4
    {
        buffer[index] = ' ';
        index++;
        for (const char* p = l4; *p; ++p)
        {
            buffer[index] = *p;
            index++;
        }
    }
    if (v4 != EMPTYLOG)
    {
        buffer[index] = ' ';
        index++;
        buffer[index] = '0';
        index++;
        buffer[index] = 'x';
        index++;
        for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i)
        {
            char hex = "0123456789ABCDEF"[(v4 >> (i * 4)) & 0xF];
            buffer[index] = hex;
            index++;
        }
    }

    buffer[index] = '\n';
    index++;
    buffer[index] = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

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
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::nextline(char* buffer,
                                   u32& index)
{
                buffer[index] = '\n';
                index++;
                buffer[index] = '\0';
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              we try to avoid CString, CScreen etc
//----------------------------------------------------------------------------------------------------------------------------------------------------
static void     CKernel::bufferToScreenPlot                          (   unsigned x, unsigned y, u32 color )   // why the fuck static?
{
                gE_PixelBuffer[y * (gE_PitchBytes >> 2) + x] = color;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

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
//----------------------------------------------------------------------------------------------------------------------------------------------------
unsigned        CKernel::bufferToScreenGetGrid                         (   unsigned &cols, unsigned &rows)
{
                cols = gE_Cols;
                rows = gE_Rows;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__
                return success == GL_TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
            //  int internal_index = 0;                     // i wonder, if and why we need it, was the indexing in the buffer incorrect? !!! DOUBLECHECK !!!
            //  if (program_index > 0)
            //      internal_index = program_index - 1;
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif // __DEBUG_LOG__
            //  char name[27];
            //  strncpy(name, &m_bufferFsh[program_index][2], 26);
            //  name[26] = '\0';
#ifdef __DEBUG_LOG__ 
            //  storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
            //  storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
#endif // __DEBUG_LOG__
                char log[1024];
                glGetProgramInfoLog(program, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif // __DEBUG_LOG__
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numUniforms; ++i)
                    {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(program, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(program, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Uniform name", uname, length);
#endif // __DEBUG_LOG__                    
                    }

                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numAttributes; ++i)
                    {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(program, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(program, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif // __DEBUG_LOG__                    
                    }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif // __DEBUG_LOG__

                return success == GL_TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
// #define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation
//----------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_check                  (   const char* file, 
                                                        unsigned    line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (resetFlag && !summary_written)
                    {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
#ifdef __DEBUG_LOG__  
                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);
#endif // __DEBUG_LOG__ 
                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Program", (u32)value);
#endif // __DEBUG_LOG__ 
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Active Texture Unit", (u32)value);
#endif // __DEBUG_LOG__ 
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport x/y", (u32)viewport[0], (u32)viewport[1]);
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport w/h", (u32)viewport[2], (u32)viewport[3]);
#endif // __DEBUG_LOG__ 
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Framebuffer", (u32)fb);
                    storeLog( MY_BUFFER, MY_INDEX, "=== End Status Report ===");
#endif // __DEBUG_LOG__                     
                    return;
                    }

                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    const char* error_str;
                    const char* severity;

                    switch (error)
                        {
                        case GL_INVALID_ENUM:
                            error_str = "GL_INVALID_ENUM";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_VALUE:
                            error_str = "GL_INVALID_VALUE";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_OPERATION:
                            error_str = "GL_INVALID_OPERATION";
                            severity  = "WARNING";
                            break;

                        case GL_OUT_OF_MEMORY:
                            error_str = "GL_OUT_OF_MEMORY";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        default:
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
                            break;
                        }
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "OpenGL Error err/ticks/line", (u32)error, (u32)ticks, (u32)line);
                    storeLog( MY_BUFFER, MY_INDEX, severity);
                    storeLog( MY_BUFFER, MY_INDEX, error_str);
                    storeLog( MY_BUFFER, MY_INDEX, file);
#endif // __DEBUG_LOG__ 
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        resetFlag = true;
                    }
}
