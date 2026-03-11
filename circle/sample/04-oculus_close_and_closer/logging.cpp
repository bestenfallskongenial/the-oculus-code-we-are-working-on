// ---------------------------------------------------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::nextline(char* buffer,
                                   u32& index)
{
                buffer[index] = '\n';
                index++;
                buffer[index] = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------