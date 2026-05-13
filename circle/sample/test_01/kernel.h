#ifndef _kernel_h
#define _kernel_h

#include "defs_include.h"

#include "o_c_setup.h"

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
// DATAMANAGEMENT

                bool        Mount                       (   const   char*                           p_deviceName);          // "emmc1-1" cd ( root ), "umsd1-1" usb returns success

                bool        UnMount                     ();                                                                 // returns success

                bool        openFile                    (   const   char*                           p_fileName);            // filename format "8.3"

                unsigned    loadToBuffer                (           char*                           p_bufferArray,          // destination buffer for file
                                                                    unsigned                        p_bufferSize);          // max bytes to read into the buffer returns loaded bytes - 0 is false/failed !!!

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

                void        clearBufferMEM              (           char**                          buffers,                // buffer pointer table returned by allocBufferMEM()
                                                                    size_t                          p_count);               // number of buffers in the table

                void        clearBufferDMA              (           char**                          buffers,                // buffer pointer table returned by allocBufferDMA()
                                                                    char*                           rawBlock);              // original raw allocation pointer to delete
//  GRAPHICS
                void        initOGL                     (           olg_state*                      o);

                void        initVbuffer                 (           olg_state*                      o, 
                                                                    vtx_state*                      v);

                void        initShader                  (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    char**                          p_bufferArray,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    GLenum                          type);
//20
                void        initProgram                 (           vtx_state*                      v,
                                                                    glsl_state*                     vsh,
                                                                    glsl_state*                     fsh,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count);
                void        initTexture                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile,
                                                                    unsigned&                       valid_count,
                                                                    GLint                           wrap_s,
                                                                    GLint                           wrap_t);

                void        initUniform                 (           vtx_state*                      v,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             p_fromFile,
                                                                    int                             p_toFile);
//  RUNTIME RENDER
                void        frmBufferSet                (           vtx_state*                      v);

                void        frmBufferSwap               (           olg_state*                      o);
//25
                void        setUniPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                /*  int                             gl_current_tex, */
                                                                    unsigned                        p_validTextureCount);

                void        setTexPrg                   (           olg_state*                      o,
                                                                    glsl_state*                     s,
                                                                    tex_state*                      t,
                                                                    int                             gl_current_tex,
                                                                    unsigned                        p_validTextureCount);
                void        drawGLsPrg                  ();

                void        frmRateBreak                (           bool*                           noTargetFPS);

//30
                void        setUniOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);

                void        setTexOvl                   (           olg_state*                      o, 
                                                                    glsl_state*                     s, 
                                                                    tex_state*                      t);
                void        drawGLsOvl                  ();
                                
// HARDWARE
                void        usDelay(unsigned us);
                void        msDelay(unsigned ms);
                u32         read32                   (           uintptr                         nAddress);                          // MMIO
                void        write32                  (           uintptr                         nAddress, 
                                                                    u32                             nValue);
                void        GPIO_SetPull             (           unsigned                        nPin,                               // GPIO
                                                                    unsigned                        nPullMode);

                void        GPIO_SetAlt              (           unsigned                        nPin, 
                                                                    unsigned                        nAltMode, 
                                                                    unsigned                        nPullMode);

                void        GPIO_Write               (           unsigned                        nPin, 
                                                                    unsigned                        nValue);
                unsigned    GPIO_Read                (           unsigned                        nPin);

                void        watchdog_Start           (           unsigned                        nTimeoutSeconds);                   // watchdog

                boolean     SPI_init                 (           void);                                                              // SPI

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

                void        storeMsg                   (            char*                           p_buffer,
                                                                        u32&                            index,
                                                                        const char*                     label,
                                                                        const void*                     tx_msg,
                                                                        u32                             total_size);
                                                                        
                void        nextline                   (            char*                           p_buffer,
                                                                        u32&                            index);
                bool        shaderLog                   (           GLint                           shader, 
                                                                    int                             shaderIndex);

                bool        programLog                  (           GLint                           program, 
                                                                    int                             program_index);

                void        gfx_check                   (   const   char*                           file, 
                                                                    unsigned                        line);
//  UTIL
                void        adc_AcquireConvert                     ();                             // can we extract the erraticness / audio engine and the mode_index_mod into separate functions?

                void        adc_ProcessAudio            (   void    );

                void        adc_AdvanceIndex            ();

                bool        checkUpdate                 ();

                bool        Update                      ();

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
//  WRAPPERS - HERE THE JOY BEGINS
                bool        wrapperInitDMA              ();             // init/alloc the dma buffers 

                bool        wrapperInitMEM              ();             // init/alloc the mem buffers

                void        wrapperDMAcleanUp           ();             // clean/delete the dma buffers
//130                
                void        wrapperMEMcleanUp           ();             // clean delete the mem buffers          
                
                void        wrapper_from_sd             ();             // mount/scan/load and validate memory for sd

                void        wrapper_load_usb            ();             // mount/scan/load and validate memory for usb          
                
                void        wrapper_init_gl_sd          ();             // parse/init vertex/default shader and overlay texture                 

                boolean     startupScreen               (   void );                                                                

private:
        static  void        TimerHandler               (    TKernelTimerHandle hTimer, 
                                                                void *pParam, 
                                                                void *pContext);
                                                                
        static  void        LoggerSink                 (    void*       pContext,
                                                                const char* pText,
                                                                unsigned    nLength );

                void        debug();

#include "defs_member.h" // <- should go to the bottom, right?    

#include "table_lfo.cpp"

#include "table_col.cpp"
};

#endif