#include "vc_sm.h"
// ---------------------------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __H264_DECODER_DEBUG_INIT__
void            CVCSharedMemory::storeLog              (   const char* label, u32 value1, u32 value2, u32 value3, u32 value4 )
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
                if (value1 != EMPTYLOG) 
                    {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value1 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                        }
                    }
                /* write second value if valid */
                if (value2 != EMPTYLOG) 
                    {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value2 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                        }
                    }
                /* write third value if valid */
                if (value3 != EMPTYLOG) 
                    {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value3 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                        }
                    }
                /* write fourth value if valid */
                if (value4 != EMPTYLOG) 
                    {
                    m_DebugCharArray[m_CharIndex++] = ' ';
                    m_DebugCharArray[m_CharIndex++] = '0';
                    m_DebugCharArray[m_CharIndex++] = 'x';
                    for (int i = (sizeof(u32) * 2) - 1; i >= 0; --i) 
                        {
                        char hex = "0123456789ABCDEF"[(value4 >> (i * 4)) & 0xF];
                        m_DebugCharArray[m_CharIndex++] = hex;
                        }
                    }
                /* terminate line */
                m_DebugCharArray[m_CharIndex++] = '\n';
                m_DebugCharArray[m_CharIndex]   = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void            CVCSharedMemory::storeMsg              ( const char* label, const void* tx_msg, u32 total_size)
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
                for (u32 i = 0; i < total_size; ++i) 
                    {
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
void CVCSharedMemory::nextline()
{
    m_DebugCharArray[m_CharIndex++] = '\n';
    m_DebugCharArray[m_CharIndex]   = '\0';
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CVCSharedMemory::Log_openService(const SERVICE_CREATION_T &src)
{
    nextline();
    storeLog("version                           ", src.version.version);
    storeLog("version_min                       ", src.version.version_min);
    storeLog("service_id                        ", src.service_id);
    storeLog("connection                        ", (u32)(uintptr_t)src.connection);
    storeLog("rx_fifo_size                      ", src.rx_fifo_size);
    storeLog("tx_fifo_size                      ", src.tx_fifo_size);
    storeLog("callback                          ", (u32)(uintptr_t)src.callback);
    storeLog("callback_param                    ", (u32)(uintptr_t)src.callback_param);
    storeLog("want_unaligned_bulk_rx            ", src.want_unaligned_bulk_rx);
    storeLog("want_unaligned_bulk_tx            ", src.want_unaligned_bulk_tx);
    storeLog("want_crc                          ", src.want_crc);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CVCSharedMemory::Log_importMemory(const vc_sm_import_msg& tx, const vc_sm_import_result& rx)
{
    nextline();
    storeLog("IMPORT tx.hdr.type                   ", tx.hdr.type);
    storeLog("IMPORT tx.hdr.trans_id               ", tx.hdr.trans_id);
    storeLog("IMPORT tx.msg.type                   ", tx.msg.type);
    storeLog("IMPORT tx.msg.addr                   ", tx.msg.addr);
    storeLog("IMPORT tx.msg.size                   ", tx.msg.size);
    storeLog("IMPORT tx.msg.kernel_id              ", tx.msg.kernel_id);
    storeLog("IMPORT tx.msg.allocator              ", tx.msg.allocator);
    storeMsg("IMPORT tx.msg.name                   ", tx.msg.name, sizeof(tx.msg.name));    
    nextline();
    storeLog("IMPORT rx.trans_id                ", rx.trans_id);
    storeLog("IMPORT rx.res_handle              ", rx.res_handle);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CVCSharedMemory::Log_lockMemory(const vc_sm_lock_msg& tx, const vc_sm_lock_result_t& rx)
{
    nextline();
    storeLog("LOCK tx.hdr.type                     ", tx.hdr.type);
    storeLog("LOCK tx.hdr.trans_id                 ", tx.hdr.trans_id);
    storeLog("LOCK tx.msg.res_handle               ", tx.msg.res_handle);
    storeLog("LOCK tx.msg.res_mem                  ", tx.msg.res_mem);
    nextline();
    storeLog("LOCK rx.trans_id                  ", rx.trans_id);
    storeLog("LOCK rx.res_handle                ", rx.res_handle);
    storeLog("LOCK rx.res_mem                   ", rx.res_mem);
    storeLog("LOCK rx.res_old_mem               ", rx.res_old_mem);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
void CVCSharedMemory::Log_freeMemory(const vc_sm_free_msg& tx, const vc_sm_result_t& rx)
{
    nextline();
    storeLog("FREE tx.hdr.type                     ", tx.hdr.type);
    storeLog("FREE tx.hdr.trans_id                 ", tx.hdr.trans_id);
    storeLog("FREE tx.msg.res_handle               ", tx.msg.res_handle);
    storeLog("FREE tx.msg.res_mem                  ", tx.msg.res_mem);
    nextline();
    storeLog("FREE rx.trans_id                  ", rx.trans_id);
    storeLog("FREE rx.success                   ", (u32)rx.success);
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif