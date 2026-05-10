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
// datamanagement.cpp:
                bool        Mount                       (   const   char*                           p_deviceName);          // "emmc1-1" cd ( root ), "umsd1-1" usb returns success

                bool        UnMount                     ();                                                                 // returns success

                bool        openFile                    (   const   char*                           p_fileName);            // filename format "8.3"

                unsigned    loadToBuffer                (           char*                           p_bufferArray,          // destination buffer for file
                                                                    unsigned                        p_bufferSize);          // max bytes to read into the buffer returns loaded bytes - 0 is false/failed !!!
//5
                bool        saveFromBufferO             (   const   char*                           p_fileName,             // filename format "8.3" 
                                                            const   char*                           p_bufferArray,               // my allocated buffer
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        saveFromBufferM             (   const   char*                           p_deviceName,           // "emmc1-1" cd ( root ), "umsd1-1" usb
                                                            const   char*                           p_fileName,             // filename format "8.3"
                                                            const   char*                           p_bufferArray,               // my allocated buffer 
                                                                    unsigned                        p_bufferSize);          // max buffer size

                bool        closeFile                   ();                                                                 // release g_hFile handle 

                void        bulkLoad                    (           char*                           p_fileNameArray[],      // where we have stored the filenames from the root directory scan
                                                                    unsigned                        p_loadedBytes[],        // where we store the size in bytes for each file
                                                                    char**                          p_bufferArray,          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                                    unsigned                        p_maxFiles,             // how many files we are allowed to process ( os limitations )
                                                                    unsigned&                       p_validFiles,           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                                    unsigned&                       p_prevFiles,            // number of loads from the last call - we need it to init the files correctly
                                                                    unsigned                        p_fileSize);            // maximum size for each file

                bool        IsValidFile                 (   const   char*                           pFileName, 
                                                            const   char*                           extension);
//10
                bool        scanRoot                    (           char**                          p_fileNameArray,        // where we store the valid filenames we find
                                                                    const char*                     p_fileExtArray[],       // the array of valid file extensions for this type of file
                                                                    unsigned                        p_extentionCount,       // how many valid file extensions we have in the array above
                                                                    unsigned&                       p_scannedFiles,         // our counter of found files per device / call
                                                                    unsigned                        p_maxFiles);            // how many files are allowed to scan and stored in the array returns success not files found!

                bool        updateUSB                   (   const   char*                           p_deviceType);          // "umsd1" is the type, not "umsd1-1"needs volatile boolean	m_bStorageAttached ! 

        static  void        removeUSB                   (           CDevice*                        pDevice,                // USB device that was removed
                                                                    void*                           pContext);              // user context pointer; expected to be CKernel*

                char**      allocBufferMEM              (           size_t                          p_count,                // number of buffer slots
                                                                    size_t                          bufferSize);            // size of each buffer in bytes *** msleep ?!

                char**      allocBufferDMA              (           size_t                          p_count,                // number of buffer slots
                                                                    size_t                          bufferSize,             // size of each buffer in bytes
                                                                    char**                          blockBaseOut,           // receives 4K-aligned DMA block base
                                                                    char**                          rawBlockOut,            // receives original raw allocation pointer
                                                                    size_t*                         alignedSizeOut);        // receives total aligned allocation size *** msleep ?!
//15
                void        clearBufferMEM              (           char**                          buffers,                // buffer pointer table returned by allocBufferMEM()
                                                                    size_t                          p_count);               // number of buffers in the table

                void        clearBufferDMA              (           char**                          buffers,                // buffer pointer table returned by allocBufferDMA()
                                                                    char*                           rawBlock);              // original raw allocation pointer to delete
// hardware.cpp:
                void        usDelay(unsigned us);
                void        msDelay(unsigned ms);
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

                void        SMI_SetupDMA             (           size_t byteLength);
     
                boolean     WS2812_Init              (           unsigned                        ledCount);                          // WS2812
//45            
                void        WS2812_SetLED            (           unsigned                        index, 
                                                                    u8                              red, 
                                                                    u8                              green, 
                                                                    u8                              blue);
                void        WS2812_Update            (           void);    

     

                int         ReadMCP3008Raw              (           unsigned                        channel);                           // MPC 3008

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

//  UTIL
                void        adc_AcquireConvert                     ();                             // can we extract the erraticness / audio engine and the mode_index_mod into separate functions?

                void        adc_ProcessAudio            (   void    );

                void        adc_AdvanceIndex            ();

                bool        checkUpdate                 ();

                bool        Update                      ();
//85
                void        set_pot_routing             (           bool                            adc_pot_routing);


                char*       make83FileName              (   const   char*                           ext );

                void        prepParameters              ();

                void        chooseIndex                 (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount, 
                                                                    bool*                           flags);

                void        chooseIndexD                (           int                             p_channel, 
                                                                    int&                            p_activeIndex, 
                                                                    int                             p_maxCount);
                void        storeModes                  ();
//90
                void        buttonPing                  (           int                             p_btn_id, 
                                                                    int                             p_pin);

                void        button_consumer             (           int                             p_btn_id);
// 
                void        randomVec8                  (           uint32_t                        p_seed);



                void        calculate1BPM               (           int                             p_source, 
                                                                    unsigned long                   p_triggerTimeClock);

                void        predict1Beat                (           int                             p_source, 
                                                                    int                             p_lfoMult);
//95
                void        sample1WaveTable            (           int                             p_source, 
                                                                    int                             p_lfoIn, 
                                                                    int                             p_lfoOut );

                boolean         startupScreen               (   void );                                                                

private:
        static  void            TimerHandler               (    TKernelTimerHandle hTimer, 
                                                                void *pParam, 
                                                                void *pContext);
                                                                
        static  void            LoggerSink                 (    void*       pContext,
                                                                const char* pText,
                                                                unsigned    nLength );

                void            debug();

#include "defs_member.h" // <- should go to the bottom, right?                
};

#endif