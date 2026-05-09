
#include "CLogger.h"

#include <stdio.h>
#include <string.h>

CLogger*        CLogger::s_pThis = 0;
TLoggerRawSink* CLogger::s_pSink = 0;
void*           CLogger::s_pSinkContext = 0;

CLogger::CLogger(unsigned nLogLevel, CTimer* pTimer, boolean bOverwriteOldest)
:               m_nLogLevel(nLogLevel),
                m_pPanicHandler(0)
{
                s_pThis = this;
}

CLogger::~CLogger(void)
{
                if (s_pThis == this)
                    {
                    s_pThis = 0;
                    }
}

boolean CLogger::Initialize(CDevice* pTarget)
{
                return TRUE;
}

void CLogger::SetNewTarget(CDevice* pTarget)
{
}

CLogger* CLogger::Get(void)
{
                if (s_pThis == 0)
                    {
                    new CLogger(LogPanic);
                    }

                return s_pThis;
}

void CLogger::SetRawSink(TLoggerRawSink* pSink, void* pContext)
{
                s_pSink        = pSink;
                s_pSinkContext = pContext;
}

void CLogger::RegisterPanicHandler(TLogPanicHandler* pHandler)
{
                m_pPanicHandler = pHandler;
}

void CLogger::WriteRaw(const char* pText, unsigned nLength)
{
                if (pText == 0) return;
                if (nLength == 0) return;
                if (s_pSink == 0) return;

                s_pSink(s_pSinkContext, pText, nLength);
}

void CLogger::WriteCString(const char* pText)
{
                if (pText == 0) return;

                WriteRaw(pText, strlen(pText));
}

void CLogger::Write(const char* pSource,
                    TLogSeverity Severity,
                    const char* pMessage,
                    ...)
{
                va_list Args;
                va_start(Args, pMessage);

                WriteV(pSource, Severity, pMessage, Args);

                va_end(Args);
}
/*
void CLogger::WriteV(const char* pSource,
                     TLogSeverity Severity,
                     const char* pMessage,
                     va_list Args)
{
                if (Severity > m_nLogLevel) return;

                char Message[256];

                vsnprintf(Message, sizeof Message, pMessage, Args);
                Message[sizeof Message - 1] = '\0';

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(Message);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}
*/
void CLogger::WriteV(const char* pSource,
                     TLogSeverity Severity,
                     const char* pMessage,
                     va_list Args)
{
                (void) Args;

                if (Severity > m_nLogLevel) return;
                if (pMessage == 0) return;

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(pMessage);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}

void CLogger::WriteNoAlloc(const char* pSource,
                           TLogSeverity Severity,
                           const char* pMessage)
{
                if (Severity > m_nLogLevel) return;

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(pMessage);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}

// for CKernel!!!
/*
kernel.h relevant complete section inside class CKernel:

#define LOG_BUFFER_SIZE (1024 * 64)

class CKernel
{
public:
                char m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32  m_logBufferIndex = 0;

private:
        static  void LoggerSink          (   void*       pContext,
                                            const char* pText,
                                            unsigned    nLength );

private:
                CLogger m_Logger;
};

// CKernel log sink function:

void CKernel::LoggerSink(void* pContext, const char* pText, unsigned nLength)
{
                CKernel* pThis = (CKernel*) pContext;

                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (pThis->m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    pThis->m_logBuffer[pThis->m_logBufferIndex++] = ch;
                    }

                pThis->m_logBuffer[pThis->m_logBufferIndex] = '\0';
}

CKernel constructor relevant complete section:

CKernel::CKernel(void)
:               m_Interrupt(),
                m_Timer(&m_Interrupt),
                m_Logger(LOGLEVEL, &m_Timer),
                m_EMMC(&m_Interrupt, &m_Timer, &m_ActLED),
                m_USBHCI(&m_Interrupt, &m_Timer, TRUE)
{
}

// CKernel::Initialize() logger section:

if (bOK)
    {
    CLogger::SetRawSink(CKernel::LoggerSink, this);

    bOK = m_Logger.Initialize(0);
    }

// draw logger buffer:

screen_clear_screen(0x00000000);

screen_draw_buffer_segment  (
                            m_logBuffer,
                            0,
                            m_logBufferIndex,
                            0,
                            0,
                            0xFFFFFFFF,
                            0x00000000
                            );

Buffer declaration / definition models:

All valid buffer definitions for the logger buffer:

#define LOG_BUFFER_SIZE (1024 * 64)

### 1. `CKernel` member array

public:
                char m_logBuffer[LOG_BUFFER_SIZE] = {0};
                u32  m_logBufferIndex = 0;

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 2. `CKernel` member pointer to allocated/existing buffer

public:
                char* m_logBuffer = 0;
                u32   m_logBufferIndex = 0;

Then before logger use:

m_logBuffer = some_valid_buffer;

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 3. `CKernel` member pointer to one existing `char**` slice

public:
                char* m_logBuffer = 0;
                u32   m_logBufferIndex = 0;

Then after allocation:

m_logBuffer = m_bufferLog[0];

Use with:

pThis->m_logBuffer
pThis->m_logBufferIndex

### 4. Fixed global array

In one `.`:

char m_logBuffer[LOG_BUFFER_SIZE] = {0};
u32  m_logBufferIndex = 0;

In header if needed elsewhere:

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

Use directly:

m_logBuffer
m_logBufferIndex

### 5. Global storage + global pointer alias

In one `.`:

char  g_logBufferStorage[LOG_BUFFER_SIZE] = {0};
char* m_logBuffer = g_logBufferStorage;
u32   m_logBufferIndex = 0;

Extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Use directly:

m_logBuffer
m_logBufferIndex

### 6. Global pointer to allocated/existing buffer

In one `.cpp`:

char* m_logBuffer = 0;
u32   m_logBufferIndex = 0;

Extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Before logger use:

m_logBuffer = some_valid_buffer;

Use directly:

m_logBuffer
m_logBufferIndex

### 7. Local/static file-scope buffer inside one `.`

static char m_logBuffer[LOG_BUFFER_SIZE] = {0};
static u32  m_logBufferIndex = 0;

Only usable inside that `.`.

For your current context-pointer `CKernel::LoggerSink`, the clean matching choices are:

char m_logBuffer[LOG_BUFFER_SIZE] = {0};
u32  m_logBufferIndex = 0;

or:

char* m_logBuffer = 0;
u32   m_logBufferIndex = 0;

*/