#define         EMPTYSTR                0 // ""                                                 // for the logger
#define         EMPTYLOG                255                                                // for the logger


#define LOG_BUFFER_SIZE (1024 * 64)
#define LOGLEVEL        LogDebug

#define 		CHUNK_SIZE				1024                                // for the file load 

// SPI
    #define SPI0_BASE                   (ARM_IO_BASE + 0x204000)

    #define         SPI_MASTER_DEVICE	 	0		                            // 0    m_SPIMaster ( also for the mcp3008 )
    #define         SPI_CHIP_SELECT		 	0		                            // 0
    #define         SPI_CLOCK_SPEED	   		1000000		                        // Hz

    #define ARM_SPI_CS                     (m_SPIBaseAddress + 0x00)
    #define ARM_SPI_FIFO                   (m_SPIBaseAddress + 0x04)
    #define ARM_SPI_CLK                    (m_SPIBaseAddress + 0x08)
    #define ARM_SPI_DLEN                   (m_SPIBaseAddress + 0x0C)

    #define CS_TXD                         (1 << 18)
    #define CS_RXD                         (1 << 17)
    #define CS_DONE                        (1 << 16)
    #define CS_TA                          (1 << 7)
    #define CS_CLEAR_RX                    (1 << 5)
    #define CS_CLEAR_TX                    (1 << 4)

    #define CS_CPOL_SHIFT                   3
    #define CS_CPHA_SHIFT                   2

    #define CS_CS                          (3 << 0)
    #define CS_CS_SHIFT                     0
    #define CS_NONE                         3
// GPIO PINS FOR THE SPI
    #define MISO                            9
    #define MOSI                            10
    #define SCLK                            11

    #define CE0                             8
    #define CE1                             7

    #define GPIO_ALT0                       4
    #define GPIO_ALT1                       5
// SMI
    #define MY_SMI_BASE                    (ARM_IO_BASE + 0x600000)

    #define ARM_SMI_CS                     (MY_SMI_BASE + 0x00)
    #define ARM_SMI_L                      (MY_SMI_BASE + 0x04)
    #define ARM_SMI_A                      (MY_SMI_BASE + 0x08)
    #define ARM_SMI_D                      (MY_SMI_BASE + 0x0C)
    #define ARM_SMI_DSR0                   (MY_SMI_BASE + 0x10)
    #define ARM_SMI_DSW0                   (MY_SMI_BASE + 0x14)
    #define ARM_SMI_DMC                    (MY_SMI_BASE + 0x30)
    #define ARM_SMI_DCS                    (MY_SMI_BASE + 0x34)
    #define ARM_SMI_DCA                    (MY_SMI_BASE + 0x38)

    #define SMI_CS_ENABLE                  (1 << 0)
    #define SMI_CS_START                   (1 << 3)
    #define SMI_CS_CLEAR                   (1 << 4)
    #define SMI_CS_WRITE                   (1 << 5)
    #define SMI_CS_SETERR                  (1 << 13)
    #define SMI_CS_PXLDAT                  (1 << 14)

    #define SMI_WSTROBE_SHIFT               0
    #define SMI_WPACE_SHIFT                 8
    #define SMI_WHOLD_SHIFT                 16
    #define SMI_WSETUP_SHIFT                24
    #define SMI_WWIDTH_SHIFT                30

    #define SMI_DMC_REQW_SHIFT              0
    #define SMI_DMC_REQR_SHIFT              6
    #define SMI_DMC_PANICW_SHIFT            12
    #define SMI_DMC_PANICR_SHIFT            18
    #define SMI_DMC_DMAEN                  (1 << 28)

    #define SMI_DMA_REQUEST_THRESH          2
    #define SMI_DMA_PANIC_LEVEL             8

    #define CM_SMICTL_BUSY                 (1 << 7)
    #define CM_SMICTL_KILL                 (1 << 5)
    #define CM_SMICTL_ENAB                 (1 << 4)

    #define CM_SMIDIV_DIVI_SHIFT            12

    #define SMI_WIDTH_16                    1
// WS2812 pin
    #define         LED_PIN		           	19		                            // led ( must be between 8 .. 23 )
    #define         LED_COUNT		       	4		                            // number of LEDs on the stripes    
// WS2812 timing
    #define NEOPIXEL_SMI_NS                 10
    #define NEOPIXEL_SMI_SETUP              10
    #define NEOPIXEL_SMI_STROBE             20
    #define NEOPIXEL_SMI_HOLD               10

    #define NEOPIXEL_SMI_WIDTH		       (LED_NCHANS > 8 ? SMI16Bits : SMI8Bits) // ?
    #define NEOPIXEL_SMI_PACE               0
// WS2812 buffer layout
    #define LED_NBITS                       24
    #define BIT_NPULSES                     3
    #define LED_PREBITS                     4
    #define LED_POSTBITS                    4

    #define LED_DLEN                       (LED_NBITS * BIT_NPULSES)
    #define LED_TX_OSET(n)                 (LED_PREBITS + (LED_DLEN * (n)))
    #define TX_BUFF_LEN(n)                 (LED_TX_OSET(n) + LED_POSTBITS)

    #define TXDATA_T                        u16
// MPC 3008 
    #define SPI_CHIP_SELECT                 0
// GPIO
    #define GPIO_PULL_OFF                   0
    #define GPIO_PULL_DOWN                  1
    #define GPIO_PULL_UP                    2

    #define LOW                             0
    #define HIGH                            1
// macros.h - watchdog
    #define PACKED                          __attribute__ ((packed))
    #define MAXALIGN                        __attribute__ ((aligned))
    #define ALIGN(n)                        __attribute__ ((aligned (n)))
    #define NORETURN                        __attribute__ ((noreturn))

    #ifndef __clang__
    #define NOOPT                           __attribute__ ((optimize (0)))
    #define STDOPT                          __attribute__ ((optimize (2)))
    #define MAXOPT                          __attribute__ ((optimize (3)))
    #else
    #define NOOPT
    #define STDOPT
    #define MAXOPT
    #endif

    #define WEAK                            __attribute__ ((weak))

    #define likely(exp)                     __builtin_expect (!!(exp), 1)
    #define unlikely(exp)                   __builtin_expect (!!(exp), 0)

    #define BIT(n)                         (1U << (n))

    #define IS_POWEROF_2(num)              ((num) != 0 && (((num) & ((num) - 1)) == 0))

    #define BE(value)                      ((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))