#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_logBuffer     
    #define MY_INDEX    m_logBufferIndex    

static const char FromKernel[] = "kernel";

void CKernel::debug()
{
 
                 
}

void            CKernel::TimerHandler                   (           TKernelTimerHandle  hTimer, 
                                                                    void*               pParam, 
                                                                    void*               pContext)
{
                void (*pInvalid)(void) = (void (*)(void)) 0x500000;

                (*pInvalid)();
}

void CKernel::logButtonStatesRuntime(void)
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "BTN PRESS_START   DOUBLE        RELEASE       SINGLE",
                EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "A  ",
                (u32)g_buttons_states[0][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[0][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[0][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[0][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "A   HOLD_TICK",
                (u32)g_buttons_states[0][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "B  ",
                (u32)g_buttons_states[1][BTN_PRESS_START],
                "  ",
                (u32)g_buttons_states[1][BTN_DOUBLE],
                "  ",
                (u32)g_buttons_states[1][BTN_RELEASE],
                "  ",
                (u32)g_buttons_states[1][BTN_SINGLE] );

    storeLog(   f_logBuffer, f_logIndex,
                "B   HOLD_TICK",
                (u32)g_buttons_states[1][BTN_HOLD_TICK],
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG,
                EMPTYSTR, EMPTYLOG );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       10,
                       0xFFFFFFFF );
}

void CKernel::logInOutRuntime(void)
{
    char f_logBuffer[512];
    u32  f_logIndex = 0;

    f_logBuffer[0] = '\0';

    storeLog(   f_logBuffer, f_logIndex,
                "0 RAW",
                (u32)g_inOutMatrixInt[0][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[0][VAL],
                "RND",
                (u32)g_inOutMatrixInt[0][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "1 RAW",
                (u32)g_inOutMatrixInt[1][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[1][VAL],
                "RND",
                (u32)g_inOutMatrixInt[1][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "2 RAW",
                (u32)g_inOutMatrixInt[2][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[2][VAL],
                "RND",
                (u32)g_inOutMatrixInt[2][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "3 RAW",
                (u32)g_inOutMatrixInt[3][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[3][VAL],
                "RND",
                (u32)g_inOutMatrixInt[3][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "4 RAW",
                (u32)g_inOutMatrixInt[4][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[4][VAL],
                "RND",
                (u32)g_inOutMatrixInt[4][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "5 RAW",
                (u32)g_inOutMatrixInt[5][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[5][VAL],
                "RND",
                (u32)g_inOutMatrixInt[5][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "6 RAW",
                (u32)g_inOutMatrixInt[6][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[6][VAL],
                "RND",
                (u32)g_inOutMatrixInt[6][RND],
                EMPTYSTR, EMPTYLOG );

    storeLog(   f_logBuffer, f_logIndex,
                "7 RAW",
                (u32)g_inOutMatrixInt[7][RAW],
                "VAL",
                (u32)g_inOutMatrixInt[7][VAL],
                "RND",
                (u32)g_inOutMatrixInt[7][RND],
                EMPTYSTR, EMPTYLOG );

    bufferScreenDraw(  f_logBuffer,
                       0,
                       f_logIndex,
                       0,
                       0,
                       0xFFFFFFFF );
}
/*
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;
                if (MY_INDEX == 0) return;

                u32 drawIndex  = m_logScreenStartIndex;
                u32 scanIndex  = m_logScreenStartIndex;

                unsigned col   = 0;
                unsigned row   = 0;

                bufferScreenClear();

                while (scanIndex < MY_INDEX)
                    {
                    char ch = MY_BUFFER[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < MY_INDEX && MY_BUFFER[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < MY_INDEX)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                MY_BUFFER,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    drawIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;

                boolean bChanged = FALSE;

                u32 delta = m_logBufferIndex - m_logBufferIndexLast;

                if (delta > 0)
                    {
                    for (u32 i = m_logBufferIndexLast; i < m_logBufferIndex; i++)
                        {
                        if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                        m_screenLogBuffer[m_screenLogBufferIndex++] = m_logBuffer[i];
                        }

                    m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                    m_logBufferIndexLast = m_logBufferIndex;
                    bChanged = TRUE;
                    }

                for (unsigned n = 0; n < (LOG_SD+LOG_USB); n++)
                    {
                    if (m_bufferLog[n] == 0) continue;
                    if (*m_bufferLog[n] == 0) continue;

                    delta = m_bufferLogIndex[n] - m_bufferLogIndexLast[n];

                    if (delta > 0)
                        {
                        for (u32 i = m_bufferLogIndexLast[n]; i < m_bufferLogIndex[n]; i++)
                            {
                            if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                            m_screenLogBuffer[m_screenLogBufferIndex++] = (*m_bufferLog[n])[i];
                            }

                        m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];
                        bChanged = TRUE;
                        }
                    }

                if (!bChanged) return;
                if (m_screenLogBufferIndex == 0) return;

                u32 drawIndex = m_logScreenStartIndex;
                u32 scanIndex = m_logScreenStartIndex;

                unsigned col = 0;
                unsigned row = 0;

                bufferScreenClear();

                while (scanIndex < m_screenLogBufferIndex)
                    {
                    char ch = m_screenLogBuffer[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < m_screenLogBufferIndex && m_screenLogBuffer[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < m_screenLogBufferIndex)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    m_screenLogBuffer,
                                    drawIndex,
                                    m_screenLogBufferIndex,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;

                boolean bChanged = FALSE;

                u32 delta = m_logBufferIndex - m_logBufferIndexLast;

                if (delta > 0)
                    {
                    for (u32 i = m_logBufferIndexLast; i < m_logBufferIndex; i++)
                        {
                        if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                        m_screenLogBuffer[m_screenLogBufferIndex++] = m_logBuffer[i];
                        }

                    m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                    m_logBufferIndexLast = m_logBufferIndex;
                    bChanged = TRUE;
                    }

                for (unsigned n = 0; n < (LOG_SD+LOG_USB); n++)
                    {
                    if (m_bufferLog[n] == 0) continue;

                    delta = m_bufferLogIndex[n] - m_bufferLogIndexLast[n];

                    if (delta > 0)
                        {
                        for (u32 i = m_bufferLogIndexLast[n]; i < m_bufferLogIndex[n]; i++)
                            {
                            if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                            m_screenLogBuffer[m_screenLogBufferIndex++] = m_bufferLog[n][i];
                            }

                        m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];
                        bChanged = TRUE;
                        }
                    }

                if (!bChanged) return;
                if (m_screenLogBufferIndex == 0) return;

                u32 drawIndex = m_logScreenStartIndex;
                u32 scanIndex = m_logScreenStartIndex;

                unsigned col = 0;
                unsigned row = 0;

                bufferScreenClear();

                while (scanIndex < m_screenLogBufferIndex)
                    {
                    char ch = m_screenLogBuffer[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < m_screenLogBufferIndex && m_screenLogBuffer[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < m_screenLogBufferIndex)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    m_screenLogBuffer,
                                    drawIndex,
                                    m_screenLogBufferIndex,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}
*/
void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows <= 1) return;

                boolean bChanged = FALSE;

                if (m_logBufferIndex < m_logBufferIndexLast)
                    {
                    m_logBufferIndexLast = m_logBufferIndex;
                    }

                if (m_logBufferIndex > m_logBufferIndexLast)
                    {
                    for (u32 i = m_logBufferIndexLast; i < m_logBufferIndex; i++)
                        {
                        if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                        if (m_logBuffer[i] == '\0') break;

                        m_screenLogBuffer[m_screenLogBufferIndex++] = m_logBuffer[i];
                        }

                    m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                    m_logBufferIndexLast = m_logBufferIndex;
                    bChanged = TRUE;
                    }

                for (unsigned n = 0; n < (LOG_SD+LOG_USB); n++)
                    {
                    if (m_bufferLog[n] == 0) continue;

                    if (m_bufferLogIndex[n] < m_bufferLogIndexLast[n])
                        {
                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];
                        continue;
                        }

                    if (m_bufferLogIndex[n] > m_bufferLogIndexLast[n])
                        {
                        for (u32 i = m_bufferLogIndexLast[n]; i < m_bufferLogIndex[n]; i++)
                            {
                            if (m_screenLogBufferIndex >= (SCREEN_LOG_BUFFER_SIZE - 1)) break;
                            if (m_bufferLog[n][i] == '\0') break;

                            m_screenLogBuffer[m_screenLogBufferIndex++] = m_bufferLog[n][i];
                            }

                        m_screenLogBuffer[m_screenLogBufferIndex] = '\0';
                        m_bufferLogIndexLast[n] = m_bufferLogIndex[n];
                        bChanged = TRUE;
                        }
                    }

                if (!bChanged) return;
                if (m_screenLogBufferIndex == 0) return;

                if (m_logScreenStartIndex > m_screenLogBufferIndex)
                    {
                    m_logScreenStartIndex = 0;
                    }

                u32 drawIndex = m_logScreenStartIndex;
                u32 scanIndex = m_logScreenStartIndex;

                unsigned col = 0;
                unsigned row = 0;

                bufferScreenClear();

                while (scanIndex < m_screenLogBufferIndex)
                    {
                    char ch = m_screenLogBuffer[scanIndex];

                    if (ch == '\0') break;

                    if (ch == '\n')
                        {
                        col = 0;
                        row++;
                        scanIndex++;

                        if (row >= (gE_Rows - 1))
                            {
                            while (drawIndex < m_screenLogBufferIndex && m_screenLogBuffer[drawIndex] != '\n')
                                {
                                drawIndex++;
                                }

                            if (drawIndex < m_screenLogBufferIndex)
                                {
                                drawIndex++;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }

                        continue;
                        }

                    col++;
                    scanIndex++;

                    if (col >= gE_Cols)
                        {
                        col = 0;
                        row++;

                        if (row >= (gE_Rows - 1))
                            {
                            drawIndex += gE_Cols;

                            if (drawIndex > m_screenLogBufferIndex)
                                {
                                drawIndex = m_screenLogBufferIndex;
                                }

                            bufferScreenClear();

                            bufferScreenDraw(
                                                m_screenLogBuffer,
                                                drawIndex,
                                                scanIndex,
                                                0,
                                                0,
                                                0xFFFFFFFF
                                                );

                            msDelay(SCROLLSPEED);

                            row = gE_Rows - 2;
                            col = 0;
                            }
                        }
                    }

                bufferScreenClear();

                bufferScreenDraw(
                                    m_screenLogBuffer,
                                    drawIndex,
                                    m_screenLogBufferIndex,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );

                m_logScreenStartIndex = drawIndex;
}