
#include <circle/logger.h>
#include <circle/util.h>
#include <stdio.h>
#include <string.h>

CLogger*        CLogger::s_pThis = 0;
TLoggerRawSink* CLogger::s_pSink = 0;

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

void CLogger::SetRawSink(TLoggerRawSink* pSink)
{
                s_pSink = pSink;
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

                s_pSink(pText, nLength);
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
void CKernel::KernelLoggerSink(const char* pText, unsigned nLength)
{
                for (unsigned i = 0; i < nLength; i++)
                    {
                    if (m_logBufferIndex >= LOG_BUFFER_SIZE - 1)
                        {
                        break;
                        }

                    const char ch = pText[i];

                    if (ch == '\r')
                        {
                        continue;
                        }

                    m_logBuffer[m_logBufferIndex++] = ch;
                    }

                m_logBuffer[m_logBufferIndex] = '\0';
}

Rest around your fixed function:



// kernel.h relevant member
CLogger m_Logger;

// constructor relevant part
CKernel::CKernel(void)
:               m_Interrupt(),
                m_Timer(&m_Interrupt),
                m_Logger(LOGLEVEL, &m_Timer),
                m_EMMC(&m_Interrupt, &m_Timer, &m_ActLED),
                m_USBHCI(&m_Interrupt, &m_Timer, TRUE),

{
}

// Initialize relevant logger part
if (bOK)
    {
    m_logBufferIndex = 0;

    CLogger::SetRawSink(KernelLoggerSink);

    bOK = m_Logger.Initialize(0);
    }

// draw logger buffer
bufferToScreenClear();
bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Buffer declaration / definition models:

Model 1: fixed global array

#define LOG_BUFFER_SIZE 1024*64

char m_logBuffer[LOG_BUFFER_SIZE];
u32  m_logBufferIndex = 0;

Matching extern:

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

Model 2: fixed global storage plus pointer alias

#define LOG_BUFFER_SIZE 1024*64

char  g_logBufferStorage[LOG_BUFFER_SIZE];
char* m_logBuffer = g_logBufferStorage;
u32   m_logBufferIndex = 0;

Matching extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Model 3: pointer to existing allocated buffer

#define LOG_BUFFER_SIZE 1024*64

char* m_logBuffer = nullptr;
u32   m_logBufferIndex = 0;

Later, before logger use:

m_logBuffer = your_existing_buffer;
m_logBufferIndex = 0;

Matching extern:

extern char* m_logBuffer;
extern u32   m_logBufferIndex;

Model 4: CKernel member array

// kernel.h
#define LOG_BUFFER_SIZE 1024*64

class CKernel
{
private:
    char m_logBuffer[LOG_BUFFER_SIZE];
    u32  m_logBufferIndex;
};

Then `KernelLoggerSink()` cannot be a plain free function using `m_logBuffer` unless it has access to the active `CKernel` object. For your current free-function sink, use Model 1, 2, or 3.

`bufferToScreenDrawBuffer()` accepts anything that can be passed as:

const char* pSourceBuffer

So these are valid buffer/source models.

Model 1: string literal

bufferToScreenDrawBuffer("hello world", 0, 11, 0, 0, 0xFFFFFFFF);

With length:

const char* text = "hello world";

bufferToScreenDrawBuffer(text, 0, strlen(text), 0, 0, 0xFFFFFFFF);

Model 2: fixed char array

char logBuffer[1024];

bufferToScreenDrawBuffer(logBuffer, 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 3: global fixed char array

extern char m_logBuffer[LOG_BUFFER_SIZE];
extern u32  m_logBufferIndex;

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 4: pointer to allocated memory

char* logBuffer = (char*)malloc(LOG_BUFFER_SIZE);

bufferToScreenDrawBuffer(logBuffer, 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 5: pointer to one slice from your `char**`

char** buffers = allocBufferMEM(4, LOG_BUFFER_SIZE);

bufferToScreenDrawBuffer(buffers[0], 0, logIndex, 0, 0, 0xFFFFFFFF);

Model 6: CKernel member array

// kernel.h
char m_logBuffer[LOG_BUFFER_SIZE];
u32  m_logBufferIndex;

Call:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 7: CKernel member pointer

// kernel.h
char* m_logBuffer;
u32   m_logBufferIndex;

Call:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Model 8: partial range of a buffer

bufferToScreenDrawBuffer(m_logBuffer, 128, 256, 0, 0, 0xFFFFFFFF);

Model 9: last N chars

u32 start = (m_logBufferIndex > 512) ? (m_logBufferIndex - 512) : 0;

bufferToScreenDrawBuffer(m_logBuffer, start, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

Required condition for all models:

pSourceBuffer != 0
startIndex < endIndex
endIndex <= real buffer size

Your logger case is simply:

bufferToScreenDrawBuffer(m_logBuffer, 0, m_logBufferIndex, 0, 0, 0xFFFFFFFF);

*/