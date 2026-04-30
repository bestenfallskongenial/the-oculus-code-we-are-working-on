#define MISO        9
#define MOSI        10
#define SCLK        11
#define CE0         8

#define  LED_CTRL_PIN ??
#define  MY_CTRL_PIN 17
#define  SW_PIN_A 22
#define  SW_PIN_B 23

#define LED_COUNT 4


#define LED_NCHANS		16  // Number of LED channels (8 or 16) - has to be 16 if we're using SD8 or above
#define LED_NBITS		24  // Number of data bits per LED
#define LED_PREBITS		4   // Number of zero bits before LED data
#define LED_POSTBITS	4   // Number of zero bits after LED data
#define BIT_NPULSES		3   // Number of O/P pulses per LED bit


// Length of data for 1 row (1 LED on each channel)
#define LED_DLEN		(LED_NBITS * BIT_NPULSES)

// Offset into Tx data buffer, given LED number in chan
#define LED_TX_OSET(n)	(LED_PREBITS + (LED_DLEN * (n)))
#define TX_BUFF_LEN(n)	(LED_TX_OSET(n) + LED_POSTBITS)

// Transmit data type, 8 or 16 bits
#if LED_NCHANS > 8
#define TXDATA_T		u16
#else
#define TXDATA_T		u8
#endif

// SMI params for a 400 ns cycle time
// NEOPIXEL_SMI_NS is in nanoseconds: even numbers, 2 to 30
#define NEOPIXEL_SMI_WIDTH		(LED_NCHANS > 8 ? SMI16Bits : SMI8Bits)
#define NEOPIXEL_SMI_PACE		0
#if RASPI > 3	// Timings for RPi v4 (1.5 GHz)
#define NEOPIXEL_SMI_NS			10
#define NEOPIXEL_SMI_SETUP		15
#define NEOPIXEL_SMI_STROBE		30
#define NEOPIXEL_SMI_HOLD		15
#else	// Timings for RPi v0-3 (1 GHz)
#define NEOPIXEL_SMI_NS			10
#define NEOPIXEL_SMI_SETUP		10
#define NEOPIXEL_SMI_STROBE		20
#define NEOPIXEL_SMI_HOLD		10
#endif


class CKernel
{
private:
    CActLED			        m_ActLED;
    CKernelOptions          m_Options;   
//  CMachineInfo		    m_MachineInfo; // ?
    CDeviceNameService	    m_DeviceNameService;  // i assume i need it !
    CExceptionHandler	    m_ExceptionHandler;
    CInterruptSystem	    m_Interrupt;
    CTimer			        m_Timer; 
    CEMMCDevice			    m_EMMC; 
    CUSBHCIDevice		    m_USBHCI;
    CVCHIQDevice		    m_VCHIQ;
//  CMemorySystem		    m_Memory; // ??? seemingly i never use m_Memory, but CMemorySystem::Get ()
    CFATFileSystem*         m_pFileSystem = nullptr;
    CScheduler		        m_Scheduler; // really really?!?
    CBcmFrameBuffer gE_FrameBuffer;
    CCharGenerator  gE_CharGenerator;


 
    u32*     gE_PixelBuffer     = nullptr; // or 0?
    unsigned gE_PitchBytes      = 0;
    unsigned gE_ScreenWidth     = 0;
    unsigned gE_ScreenHeight    = 0;
    unsigned gE_CharWidth       = 0;
    unsigned gE_CharHeight      = 0;
    unsigned gE_Cols            = 0;
    unsigned gE_Rows            = 0;

    // SPI / MCP3008
    u32      m_SPIBaseAddress   = 0;
    boolean  m_SPIValid         = false;
    // SMI

    CDMAChannel m_SMITxDMA;
    unsigned m_SMIGpioPin = 0;
    unsigned m_LEDCount = 0;
    u32      m_SMISDMask        = 0;
    boolean  m_SMIValid         = false;
    // WS2812
    unsigned  m_BufferLength    = 0;
    TXDATA_T* m_pBuffer         = nullptr;

};
CKernel::CKernel()
                  : gE_FrameBuffer( m_Options.GetWidth(),
                                    m_Options.GetHeight(),
                                    32, 0, TRUE),

                    m_Interrupt (),                                                                     // i assume someone needs interupts here, not me!
	                m_Timer (&m_Interrupt),                                                             // this i use frequently for timeouts, delays           
                    m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),                                         // my CEMMCDevice device
	                m_USBHCI (&m_Interrupt, &m_Timer, TRUE),                                            // my CUSBHCI device, i assume the usb device
	                m_VCHIQ (CMemorySystem::Get (), &m_Interrupt),                                      // my VCHIQ device, for video and gpu communication - does it need m_Screen?
                //  m_VCHIQ (&m_Memory, &m_Interrupt),
                    m_SMITxDMA(DMA_CHANNEL_LITE)                                                        // new                 
                    {
                    m_ActLED.Blink (5);	
                    }

boolean         CKernel::Initialize (void)
{
                bool bOK = true;

                if (bOK)
                    {
                    bOK = bufferToScreenInit();                     // framebuffer
                    }
                if (bOK)
                {
                    bOK = m_Interrupt.Initialize();
                    }
                if (bOK)
                    {
                    bOK = m_Timer.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize(); 
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); 
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); 
                    m_Timer.MsDelay(1000);                          // ?
                    }
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize();
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                    {
                    bcm_host_init();
                    m_Timer.MsDelay(200);
                    }               
                if (bOK)
                    {
                    initOGL   (&m_ogl);
                    m_Timer.MsDelay(200);
                    }                     
               if (bOK)
                    {
                    bOK = my_SPI_init(); 
                    }
               if (bOK)
                    {
                    bOK = my_SMI_Init(LED_CTRL_PIN); 
                    }
               if (bOK)
                    {
                    bOK = my_WS2812_Init(LED_COUNT); 
                    }                                        
                if (bOK)
                    {
                    my_GPIO_SetAlt(MY_CTRL_PIN, 1, GPIO_PULL_OFF);   // 001 = output
                    my_GPIO_Write(MY_CTRL_PIN, LOW); // LOW OR HIGH?
                    }

                if (bOK)
                    {
                    my_GPIO_SetAlt(SW_PIN_A, 0, GPIO_PULL_UP);       // 000 = input
                    }

                if (bOK)
                    {
                    my_GPIO_SetAlt(SW_PIN_B, 0, GPIO_PULL_UP);       // 000 = input
                    }
                return bOK;
}