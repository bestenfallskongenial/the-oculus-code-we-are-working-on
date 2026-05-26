#include "kernel.h"

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

u32             CKernel::bufferScreenFindStartIndex (   const char* p_buffer,
                                                        u32         endIndex,
                                                        unsigned    cols,
                                                        unsigned    rows)
{
                if (p_buffer == 0) return 0;
                if (endIndex == 0) return 0;
                if (cols == 0) return 0;
                if (rows <= 1) return 0;

                u32 visible_chars = cols * (rows - 1);

                if (endIndex <= visible_chars) return 0;

                u32 startIndex = endIndex - visible_chars;

                while (startIndex < endIndex && p_buffer[startIndex] != '\n')
                    {
                    startIndex++;
                    }

                if (startIndex < endIndex)
                    {
                    startIndex++;
                    }

                return startIndex;
}

void            CKernel::logScreenUpdate            (   void )
{
                if (gE_PixelBuffer == 0) return;
                if (gE_Cols == 0) return;
                if (gE_Rows == 0) return;
                if (MY_INDEX == 0) return;

                u32 startIndex = bufferScreenFindStartIndex(
                                                            MY_BUFFER,
                                                            MY_INDEX,
                                                            gE_Cols,
                                                            gE_Rows
                                                            );

                bufferScreenClear();

                bufferScreenDraw(
                                    MY_BUFFER,
                                    startIndex,
                                    MY_INDEX,
                                    0,
                                    0,
                                    0xFFFFFFFF
                                    );
}

