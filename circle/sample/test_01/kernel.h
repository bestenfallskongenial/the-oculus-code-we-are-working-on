#ifndef _kernel_h
#define _kernel_h

#include "defs_include.h"
#include "defs_macro.h"

enum TShutdownMode
{
                ShutdownNone,
                ShutdownHalt,
                ShutdownReboot
};

class CKernel
{
public:
                                CKernel                    (    void );
                               ~CKernel                    (    void );

#include "defs_enum.h"
#include "defs_struct.h"

                boolean         Initialize                 (    void );

                TShutdownMode   Run                        (    void );

public:
inline          void        usDelay(unsigned us);
inline          void        msDelay(unsigned ms);
                u32         read32                   (           uintptr                         nAddress);                          // MMIO
                void        write32                  (           uintptr                         nAddress, 
                                                                    u32                             nValue);
                void        GPIO_SetPull             (           unsigned                        nPin,                               // GPIO
                                                                    unsigned                        nPullMode);
//35
                void        GPIO_SetAlt              (           unsigned                        nPin, 
                                                                    unsigned                        nAltMode, 
                                                                    unsigned                        nPullMode);

                void        GPIO_Write               (           unsigned                        nPin, 
                                                                    unsigned                        nValue);
                unsigned    GPIO_Read                (           unsigned                        nPin);

                void        watchdog_Start           (           unsigned                        nTimeoutSeconds);                   // watchdog

                boolean     SPI_init                 (           void);                                                              // SPI
//40
                int         WriteRead                (           unsigned                        nChipSelect,
                                                            const   void*                           pWriteBuffer,
                                                                    void*                           pReadBuffer,
                                                                    unsigned                        nCount);
    

                boolean     SMI_Init                 (           unsigned                        gpioPin);                           // SMI
                                                                   
                void        SMI_SetupTiming          (           unsigned                        width,
                                                                    unsigned                        cycle_ns,
                                                                    unsigned                        setup,
                                                                    unsigned                        strobe,
                                                                    unsigned                        hold,
                                                                    unsigned                        pace);

                void        SMI_SetupDMA             (           void);
     
                boolean     WS2812_Init              (           unsigned                        ledCount);                          // WS2812
//45            
                void        WS2812_SetLED            (           unsigned                        index, 
                                                                    u8                              red, 
                                                                    u8                              green, 
                                                                    u8                              blue);
                void        WS2812_Update            (           void);    

     

        inline  int         ReadMCP3008Raw              (           unsigned                        channel);                           // MPC 3008

                boolean         frameBufferInit            (    void );

                void            bufferToScreenPlot         (    unsigned x,
                                                                unsigned y,
                                                                u32 color );

                void            bufferToScreenDrawChar     (    char        ch,
                                                                unsigned    charCol,
                                                                unsigned    charRow,
                                                                u32         fgColor );

                void            bufferToScreenClear        (    void );

                void            bufferToScreenDrawBuffer   (    const char* pSourceBuffer,
                                                                u32         startIndex,
                                                                u32         endIndex,
                                                                unsigned    startCol,
                                                                unsigned    startRow,
                                                                u32         fgColor );

                void            bufferToScreenGetGrid      (    unsigned& cols,
                                                                unsigned& rows );

                void            storeLog                   (            char*                           p_bufferArray,
                                                                        u32&                            index,
                                                                const   char*                           p_string0, 
                                                                        u32                             p_value0    = EMPTYLOG,
                                                                const   char*                           p_string1   = EMPTYSTR, 
                                                                        u32                             p_value1    = EMPTYLOG,
                                                                const   char*                           p_string2   = EMPTYSTR, 
                                                                        u32                             p_value2    = EMPTYLOG,
                                                                const   char*                           p_string3   = EMPTYSTR, 
                                                                        u32                             p_value3    = EMPTYLOG);                                                              

                void            storeMsg                   (            char*                           p_buffer,
                                                                        u32&                            index,
                                                                        const char*                     label,
                                                                        const void*                     tx_msg,
                                                                        u32                             total_size);
                                                                        
                void            nextline                   (            char*                           p_buffer,
                                                                        u32&                            index);

                boolean         startupScreen               (   void );                                                                

private:
        static  void            TimerHandler               (    TKernelTimerHandle hTimer, 
                                                                void *pParam, 
                                                                void *pContext);
                                                                
        static  void            LoggerSink                 (    void*       pContext,
                                                                const char* pText,
                                                                unsigned    nLength );

#include "defs_member.h" // <- should go to the bottom, right?                
};

#endif