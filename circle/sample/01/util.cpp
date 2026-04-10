// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::startupScreen(char* buffer, u32& index)
{
                const char* machineName = m_MachineInfo.GetMachineName();
                const char* socName     = m_MachineInfo.GetSoCName();

                unsigned modelMajor     = m_MachineInfo.GetModelMajor();
                unsigned modelRevision  = m_MachineInfo.GetModelRevision();
                unsigned ramSize        = m_MachineInfo.GetRAMSize();

                unsigned cpuSpeedMode   = (m_Options.GetCPUSpeed() == CPUSpeedMaximum) ? 1 : 0;
                unsigned socMaxTemp     = m_Options.GetSoCMaxTemp();

                unsigned coreClock      = m_MachineInfo.GetClockRate(CLOCK_ID_CORE)  / 1000000;
                unsigned armClock       = m_MachineInfo.GetClockRate(CLOCK_ID_ARM)   / 1000000;
                unsigned emmcClock      = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC)  / 1000000;
                unsigned emmc2Clock     = m_MachineInfo.GetClockRate(CLOCK_ID_EMMC2) / 1000000;
                unsigned uartClock      = m_MachineInfo.GetClockRate(CLOCK_ID_UART)  / 1000000;

                unsigned dmaChannel     = m_MachineInfo.AllocateDMAChannel(DMA_CHANNEL_NORMAL);

                                        m_MachineInfo.FreeDMAChannel(dmaChannel);

                unsigned usbDelay       = m_Options.GetUSBPowerDelay();
                unsigned usbSpeed       = m_Options.GetUSBFullSpeed();

                /* text labels */

                storeLog( MY_BUFFER, MY_INDEX, "Machine Model");
                storeLog( MY_BUFFER, MY_INDEX, machineName);
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "SoC Name");
                storeLog( MY_BUFFER, MY_INDEX, socName);
                nextline(buffer, index);

                /* numeric values */

                storeLog( MY_BUFFER, MY_INDEX, "Model Major    ", modelMajor);
                storeLog( MY_BUFFER, MY_INDEX, "Model Revision ", modelRevision);
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "RAM Size     MB", ramSize);
                storeLog( MY_BUFFER, MY_INDEX, "CPU Speed Mode ", cpuSpeedMode);
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "SoC Max Temp   ", socMaxTemp);
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "Clock CORE  MHz", coreClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock ARM   MHz", armClock);    
                storeLog( MY_BUFFER, MY_INDEX, "Clock EMMC  MHz", emmcClock, emmc2Clock, uartClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock EMMC2 MHz", emmcClock, emmc2Clock, uartClock);
                storeLog( MY_BUFFER, MY_INDEX, "Clock UART  MHz", emmcClock, emmc2Clock, uartClock);    
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "DMA Channel    ", dmaChannel);
                nextline(buffer, index);
                storeLog( MY_BUFFER, MY_INDEX, "USB Delay      ", usbDelay);
                storeLog( MY_BUFFER, MY_INDEX, "USB FullSpeed  ", usbSpeed);
                
                screen_clear_screen(0x00000000);

                screen_draw_buffer_segment  (
                                            buffer,
                                            0,
                                            index,
                                            0,
                                            0,
                                            0xFFFFFFFF,
                                            0x00000000
                                            );

                return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my adc read and convert function plus audio detection!
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::readADC() 
{
                const float maxA = 1023.0;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                static int f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                static int f_index_ring_buffer;

                const int f_scale = scaleFactors[attenuation];
#ifdef __AUDIO_DETECTION__
                const int AUDIO_THRESHOLD = 160;  // Threshold for audio detection

                const int f_maxBuffer = 33;

                static float f_band0[f_maxBuffer] = {0};
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};

                static float sum0 = 0;
                static float sum1 = 0;
                static float sum2 = 0;
                static float sum3 = 0;

                static unsigned char idx0 = 0;
                static unsigned char idx1 = 0;
                static unsigned char idx2 = 0;
                static unsigned char idx3 = 0;

                static uint32_t audio_hold_A = 0;
                static uint32_t audio_hold_B = 0;

                int w0 = g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63; // dont need the modulo! its a retardo saveguard!
                int w1 = g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63;
                int w2 = g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63;
                int w3 = g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63;

                int i0 =  f_index_ring_buffer;
                int i1 = (f_index_ring_buffer - 1) & 3;
                int i2 = (f_index_ring_buffer - 2) & 3;
                int i3 = (f_index_ring_buffer - 3) & 3;

                    g_modeMap[0][0] = 5;
                    g_modeMap[1][0] = 5;
                    g_modeMap[2][0] = 5;
                    g_modeMap[3][0] = 5;
#endif // __AUDIO_DETECTION__
                f_ring_buffer[0][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(0);    // Channel 0 - First of pair for audio_sample[0]
#ifdef __AUDIO_DETECTION__
                g_irregularity[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(g_irregularity[0] > AUDIO_THRESHOLD || g_irregularity[0] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 0;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[0][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][AU0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][AU1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[0][RAW] =  (f_ring_buffer[0][0] + f_ring_buffer[0][1] + f_ring_buffer[0][2] + f_ring_buffer[0][3]) >>2 ; 

                g_inOutMatrixInt[0][VAL] = (g_inOutMatrixInt[0][RAW] * f_scale) >> 10; //  -> / 1023;                                
                
                g_inOutMatrixFlt[0][VAL] = (g_inOutMatrixInt[0][VAL]) * 0.0009765625f;

                f_ring_buffer[1][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(1);    // Channel 1 - First of pair for audio_sample[1]
#ifdef __AUDIO_DETECTION__
                g_irregularity[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(g_irregularity[1] > AUDIO_THRESHOLD || g_irregularity[1] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 1;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[1][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][AU2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][AU3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[1][RAW] =  (f_ring_buffer[1][0] + f_ring_buffer[1][1] + f_ring_buffer[1][2] + f_ring_buffer[1][3]) >>2 ; 

                g_inOutMatrixInt[1][VAL] = (g_inOutMatrixInt[1][RAW] * f_scale) >> 10;                               
                
                g_inOutMatrixFlt[1][VAL] = (g_inOutMatrixInt[1][VAL]) * 0.0009765625f;

                f_ring_buffer[2][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(2);    // Channel 2 - Second of pair for audio_sample[0]
#ifdef __AUDIO_DETECTION__
                g_irregularity[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(g_irregularity[2] > AUDIO_THRESHOLD || g_irregularity[2] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 2;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[2][f_index_ring_buffer] * 0.0009765625f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][AU0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][AU1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[2][RAW] =  (f_ring_buffer[2][0] + f_ring_buffer[2][1] + f_ring_buffer[2][2] + f_ring_buffer[2][3]) >>2 ; 

                g_inOutMatrixInt[2][VAL] = (g_inOutMatrixInt[2][RAW] * f_scale) >> 10;                 
                
                g_inOutMatrixFlt[2][VAL] = (g_inOutMatrixInt[2][VAL]) * 0.0009765625f;

                f_ring_buffer[3][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(3);    // Channel 3 - Second of pair for audio_sample[1]
#ifdef __AUDIO_DETECTION__
                g_irregularity[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(g_irregularity[3] > AUDIO_THRESHOLD || g_irregularity[3] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 3;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[3][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][AU2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][AU3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }
#endif // __AUDIO_DETECTION__
                g_inOutMatrixInt[3][RAW] = (f_ring_buffer[3][0] + f_ring_buffer[3][1] + f_ring_buffer[3][2] + f_ring_buffer[3][3]) >>2 ; 

                g_inOutMatrixInt[3][VAL] = (g_inOutMatrixInt[3][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[3][VAL] = (g_inOutMatrixInt[3][VAL]) * 0.0009765625f;

                f_ring_buffer[4][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(4);    // Channel 4 (no audio detection)

                g_inOutMatrixInt[4][RAW] =  (f_ring_buffer[4][0] + f_ring_buffer[4][1] + f_ring_buffer[4][2] + f_ring_buffer[4][3]) >>2 ; 

                g_inOutMatrixInt[4][VAL] = (g_inOutMatrixInt[4][RAW] * f_scale) >> 10;               
                
                g_inOutMatrixFlt[4][VAL] = (g_inOutMatrixInt[4][VAL]) * 0.0009765625f;

                f_ring_buffer[5][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(5);

                g_inOutMatrixInt[5][RAW] =  (f_ring_buffer[5][0] + f_ring_buffer[5][1] + f_ring_buffer[5][2] + f_ring_buffer[5][3]) >>2 ; 

                g_inOutMatrixInt[5][VAL] = (g_inOutMatrixInt[5][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[5][VAL] = (g_inOutMatrixInt[5][VAL]) * 0.0009765625f;

                f_ring_buffer[6][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(6);

                g_inOutMatrixInt[6][RAW] = (f_ring_buffer[6][0] + f_ring_buffer[6][1] + f_ring_buffer[6][2] + f_ring_buffer[6][3]) >>2 ; 

                g_inOutMatrixInt[6][VAL] = (g_inOutMatrixInt[6][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[6][VAL] = (g_inOutMatrixInt[6][VAL]) * 0.0009765625f;

                f_ring_buffer[7][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(7);

                g_inOutMatrixInt[7][RAW] = (f_ring_buffer[7][0] + f_ring_buffer[7][1] + f_ring_buffer[7][2] + f_ring_buffer[7][3]) >>2 ; 

                g_inOutMatrixInt[7][VAL] = (g_inOutMatrixInt[7][RAW] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[7][VAL] = (g_inOutMatrixInt[7][VAL]) * 0.0009765625f;
#ifdef __AUDIO_DETECTION__                
                if(audio_hold_A > 0) --audio_hold_A;
                if(audio_hold_B > 0) --audio_hold_B;
#endif // __AUDIO_DETECTION__
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my little helpers 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::checkUpdate    () // aka is a new firmware present?!
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;    // there is the second ( usb ) firmware
                    }
                else
                    {
                    return false;    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::Update         ()
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel loaded from sd ( the running kernel )
                // - filesystem is already mounted by caller

                if (saveFromBuffer(PARTITION_NAME_SD, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                // fallback attempt (allowed to fail)
                saveFromBuffer(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]);

                return false;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            set_pot_routing         (   bool        adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}
// can we use this instead of the CGPIOPin class????
/*
#include <circle/bcm2835.h>
#include <circle/types.h>
#include <circle/timer.h>

#define LOW  0
#define HIGH 1

#define PULL_OFF  0
#define PULL_DOWN 1
#define PULL_UP   2

static inline u32 mmio_read32(uintptr addr)
{
    return *(volatile u32 *)addr;
}

static inline void mmio_write32(uintptr addr, u32 value)
{
    *(volatile u32 *)addr = value;
}

void gpio_write(unsigned pin, unsigned state, int pull)
{
    unsigned shift = (pin % 10) * 3;
    uintptr sel = ARM_GPIO_GPFSEL0 + (pin / 10) * 4;

    // set OUTPUT
    u32 v = mmio_read32(sel);
    v &= ~(7 << shift);
    v |=  (1 << shift);
    mmio_write32(sel, v);

    // optional pull
    if (pull >= 0)
    {
        u32 mask = 1 << (pin % 32);

        mmio_write32(ARM_GPIO_GPPUD, pull);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (pin / 32) * 4, mask);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUD, 0);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (pin / 32) * 4, 0);
    }

    // write HIGH / LOW (explicit)
    u32 mask = 1 << (pin % 32);

    if (state == HIGH)
    {
        mmio_write32(ARM_GPIO_GPSET0 + (pin / 32) * 4, mask);
    }

    if (state == LOW)
    {
        mmio_write32(ARM_GPIO_GPCLR0 + (pin / 32) * 4, mask);
    }
}


// from marcos.h

#define PACKED		__attribute__ ((packed))
#define	MAXALIGN	__attribute__ ((aligned))
#define	ALIGN(n)	__attribute__ ((aligned (n)))
#define NORETURN	__attribute__ ((noreturn))
#ifndef __clang__
#define NOOPT		__attribute__ ((optimize (0)))
#define STDOPT		__attribute__ ((optimize (2)))
#define MAXOPT		__attribute__ ((optimize (3)))
#else
#define NOOPT
#define STDOPT
#define MAXOPT
#endif
#define WEAK		__attribute__ ((weak))

#define likely(exp)	__builtin_expect (!!(exp), 1)
#define unlikely(exp)	__builtin_expect (!!(exp), 0)

#define BIT(n)		(1U << (n))

#define IS_POWEROF_2(num) ((num) != 0 && (((num) & ((num) - 1)) == 0))

// big endian (to be used for constants only)
#define BE(value)	((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

// from memio.h

#include <circle/types.h>

#ifdef __cplusplus      // why? 
extern "C" {
#endif

/// \brief Read 32-bit value from MMIO address
static inline u32 read32 (uintptr nAddress)
{
	return *(u32 volatile *) nAddress;
}
/// \brief Write 32-bit value to MMIO address
static inline void write32 (uintptr nAddress, u32 nValue)
{
	*(u32 volatile *) nAddress = nValue;
}
#ifdef __cplusplus
}
#endif

// from bcmwatchdog.h 
#include <circle/spinlock.h>


	static const unsigned MaxTimeoutSeconds = 15;

    CSpinLock m_SpinLock; // really ?!?!


void CKernel::watchDogStart (unsigned nTimeoutSeconds)
{
	if (nTimeoutSeconds > MaxTimeoutSeconds)
	{
		nTimeoutSeconds = MaxTimeoutSeconds;
	}
	m_SpinLock.Acquire ();

	write32 (ARM_PM_WDOG, ARM_PM_PASSWD | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME));

	write32 (ARM_PM_RSTC,   ARM_PM_PASSWD | ARM_PM_RSTC_REBOOT
			      | (read32 (ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR));

	m_SpinLock.Release ();
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::prepParameters       ()        // f_buffer guess here we need much more to do!
{
                for ( int f_buffer=0; f_buffer <= DEFAULT_SLOT; f_buffer++)
                    {
                    g_centralModeBuffer[f_buffer][LF1_WAVE] = 0;
                    g_centralModeBuffer[f_buffer][LF2_WAVE] = 1;
                    g_centralModeBuffer[f_buffer][LF1_MULT] = 3;
                    g_centralModeBuffer[f_buffer][LF2_MULT] = 3;
                    g_centralModeBuffer[f_buffer][SENS_A] = 15;
                    g_centralModeBuffer[f_buffer][SENS_B] = 47;
                    g_centralModeBuffer[f_buffer][SENS_C] = 15;
                    g_centralModeBuffer[f_buffer][SENS_D] = 47;                    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void            CKernel::chooseProgram        ( int p_channel, &p_activeShader )
{
                static int p_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][RAW] * g_loaded_fsh_new >> 10; // <- why no bracelets here too? g_loaded_fsh_new doesnt exis anymore !!!

                if (m_shaderStatusFlags[f_calculated])  // comes from gfx.cpp ->
                    {
                    p_activeShader = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void             CKernel::chooseTexture        ( int p_channel, &p_activeTexture, &p_validTextureCount) // f_buffer have three possible ways here! f_buffer can a) invent a mechanism to get the is valid table for the vids - f_buffer can also draw from parser.is_valid[x] 
{                                                                                                       // wait, we have a parser and this parser is giving like for program a valid status array!!!
                static int p_activeTexture = 0;
                if (p_validTextureCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][RAW] * (p_validTextureCount) >> 10;
                    p_activeTexture = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int             CKernel::chooseVideo        ( int p_channel, &p_activeVideo, &p_validVideoCount )
{
                static int p_activeVideo = 0;
                if (p_validVideoCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][RAW] * (p_validVideoCount) >> 10;
                    p_activeVideo = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int             CKernel::chooseFrame        ( int p_channel, &p_activeFrame, &p_validFrameCount ) // or direct m_H264Parser.m_frame_count[p_activeVideo]? <- why is this different?
{
                static int p_activeFrame = 0;
                if (p_validFrameCount != 0) // <- !!! this is the point where i decided to include the h264 / vc_sm / parser class into the CKernel code ( again ) !!!
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][RAW] *  (p_validFrameCount) >> 10;
                    p_activeFrame = f_calculated;
                    }
}
*/
// NEW generic not condensed valid arrays, max number of files ( macros for example!)
void CKernel::chooseIndexSparse(int p_channel, int& p_activeIndex, int p_maxCount, bool* flags)
{
    static int p_activeIndex = 0;

    int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

    if (flags[f_calculated])
    {
        p_activeIndex = f_calculated;
    }
}
// NEW generic condensed valid arrays, max number of files ( macros for example!)
void CKernel::chooseIndexDense(int p_channel, int& p_activeIndex, int p_maxCount)
{
    int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

    p_activeIndex = f_calculated;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void            CKernel::storeModesV1         ()  // faster
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               
                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true )
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program]));
                    
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else  if (g_centralModeBuffer[g_current_gl_program][is_stored] == false )
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeModesV2         ()    // "saver"
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               

                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true /* && g_currentProgramBuffer != g_current_gl_program */)
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else if (g_centralModeBuffer[g_current_gl_program][is_stored] == false /* && g_currentProgramBuffer != DEFAULT_SLOT */)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::buttonPing(int p_btn_id, int pin)
{
                g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
                g_buttons_states[p_btn_id][BTN_DOUBLE] = 0;

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() == BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
                    {
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_SINGLE] = 1;

                    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id][BTN_DOUBLE] = 1;
                        }
                    g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
                    }

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() != BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] != 0)
                    {
                    g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::button_consumer(int p_btn_id) // this is where the magic happens: we need to set the states of menu layer, menu, we need to use one button for bpm input and so on 
{
                if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
                if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

                // long-entry edge (once)
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1)
                    counter += 5;

                // periodic hold event (once per threshold)
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10)
                    longhold += 1;
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20)
                    longhold += 2;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my abstract stuff like random, lfo, bpm needed for glsl shader uniform control
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::randomVec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;
                uint32_t        f_x         = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[0][RND] = / f_x * f_scale;
                g_inOutMatrixInt[0][RND] = ( g_inOutMatrixFlt[0][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[1][RND] = f_x * f_scale;
                g_inOutMatrixInt[1][RND] = ( g_inOutMatrixFlt[1][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[2][RND] = f_x * f_scale;
                g_inOutMatrixInt[2][RND] = ( g_inOutMatrixFlt[2][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[3][RND] = f_x * f_scale;
                g_inOutMatrixInt[3][RND] = ( g_inOutMatrixFlt[3][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[4][RND] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[4][RND] = ( g_inOutMatrixFlt[4][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[5][RND] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[5][RND] = ( g_inOutMatrixFlt[5][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[6][RND] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[6][RND] = ( g_inOutMatrixFlt[6][RND] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[7][RND] = f_x * f_scale;
                g_inOutMatrixInt[7][RND] = ( g_inOutMatrixFlt[7][RND] * f_max_int);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// question here - should i rather have one function for both channels or should i separate the functions and call per channel?
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::calculate2BPM   (   unsigned long   p_triggerTimeClockA,       // love to split it but i will need additional parameters right?
                                                unsigned long   p_triggerTimeClockB) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static unsigned long f_deltaBuffer[2][3]= { 0 };

                unsigned long f_intervalAverage = 0;
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClockA != f_lastTime[0])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[0][f_timeIndex[0]] = p_triggerTimeClockA;
        
                    f_deltaBuffer[0][0]          =   f_timeBuffer[0][1] - f_timeBuffer[0][0];   
                    f_deltaBuffer[0][1]          =   f_timeBuffer[0][2] - f_timeBuffer[0][1];
                    f_deltaBuffer[0][2]          =   f_timeBuffer[0][3] - f_timeBuffer[0][2];

                    if(     f_deltaBuffer[0][1]  <   f_deltaBuffer[0][0] * 1.25f &&  f_deltaBuffer[0][2]  <   f_deltaBuffer[0][0] * 1.25f &&  f_deltaBuffer[0][0]  <   f_deltaBuffer[0][2] * 1.25f ) // calculates an average and allows 25% play ( quite high right ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[0][0] + f_deltaBuffer[0][1] + f_deltaBuffer[0][2]) / 3;
            
                        g_resultBPM[0]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[0]     =   f_intervalAverage;
                        g_lastBpmCalculation[0]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[0]                   =   p_triggerTimeClockA;

                    f_timeIndex[0]                  = ( f_timeIndex[0] + 1) % 4;    
                    }
                if (p_triggerTimeClockB != f_lastTime[1])                                                           // Process clock u_time (instance 1)
                    {
                    f_timeBuffer[1][f_timeIndex[1]] = p_triggerTimeClockB;
        
                    f_deltaBuffer[1][0]          =   f_timeBuffer[1][1] - f_timeBuffer[1][0];
                    f_deltaBuffer[1][1]          =   f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    f_deltaBuffer[1][2]          =   f_timeBuffer[1][3] - f_timeBuffer[1][2];

                    if(     f_deltaBuffer[1][1]  <   f_deltaBuffer[1][0] * 1.25f &&  f_deltaBuffer[1][2]  <   f_deltaBuffer[1][0] * 1.25f &&  f_deltaBuffer[1][0]  <   f_deltaBuffer[1][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[1][0] + f_deltaBuffer[1][1] + f_deltaBuffer[1][2]) / 3;
            
                        g_resultBPM[1]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[1]     =   f_intervalAverage;
                        g_lastBpmCalculation[1]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[1]                   =   p_triggerTimeClockB;

                    f_timeIndex[1]                  = ( f_timeIndex[1] + 1) % 4;    
                    }
                g_activeBpmChannel                  = ( g_lastBpmCalculation[0] > g_lastBpmCalculation[1]) ? 0 : 1; // what was the last bpm input? 
}

void            CKernel::calculate1BPM   (   int p_source, unsigned long   p_triggerTimeClock)       // love to split it but i will need additional parameters right?
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static unsigned long f_deltaBuffer[2][3]= { 0 };

                unsigned long f_intervalAverage = 0;
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClock != f_lastTime[p_source])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[p_source][f_timeIndex[p_source]] = p_triggerTimeClock;
        
                    f_deltaBuffer[p_source][0]          =   f_timeBuffer[p_source][1] - f_timeBuffer[p_source][0];   
                    f_deltaBuffer[p_source][1]          =   f_timeBuffer[p_source][2] - f_timeBuffer[p_source][1];
                    f_deltaBuffer[p_source][2]          =   f_timeBuffer[p_source][3] - f_timeBuffer[p_source][2];

                    if(     f_deltaBuffer[p_source][1]  <   f_deltaBuffer[p_source][0] * 1.25f &&  f_deltaBuffer[p_source][2]  <   f_deltaBuffer[p_source][0] * 1.25f &&  f_deltaBuffer[p_source][0]  <   f_deltaBuffer[p_source][2] * 1.25f ) // calculates an average and allows 25% play ( quite high right ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[p_source][0] + f_deltaBuffer[p_source][1] + f_deltaBuffer[p_source][2]) / 3;
            
                        g_resultBPM[p_source]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[p_source]     =   f_intervalAverage;
                        g_lastBpmCalculation[p_source]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[p_source]                   =   p_triggerTimeClock;

                    f_timeIndex[p_source]                  = ( f_timeIndex[p_source] + 1) % 4;    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::predictedNextBeat2 ()  // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                if (currentTime >= g_nextBeatTime[0])
                    {
                    g_nextBeatTime[0]               +=  g_intervalCalculated[0];                                                                        // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[0]) 
                    {
                    g_lastCircleBuffer[0]           =   g_nextCircleBuffer[0];
                    g_nextCircleBuffer[0]           =   g_nextCircleBuffer[0] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[0]); // why again g_lfoMultiplierTMP? isnt it stored already, do we need to back it up?
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
                if ((g_lastBpmCalculationTMP[0]     !=  g_lastBpmCalculation[0]))
                    {
                    g_nextBeatTime[0]               =   g_lastBpmCalculation[0];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[0]      =   g_lastBpmCalculation[0];
                    }
                if (g_lfoMultiplierTMP[0]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]])
                    {
                    g_lastCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[0]);
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
                if (currentTime >= g_nextBeatTime[1])
                    {
                    g_nextBeatTime[1]               +=  g_intervalCalculated[1];                                                                                   // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[1]) 
                    {
                    g_lastCircleBuffer[1]           =   g_nextCircleBuffer[1];
                    g_nextCircleBuffer[1]           =   g_nextCircleBuffer[1] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[1] );                                  // g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]];
                    }
                if ((g_lastBpmCalculationTMP[1]     !=  g_lastBpmCalculation[1]))                                                                   // Handle BPM changes for instance 1
                    {
                    g_nextBeatTime[1]               =   g_lastBpmCalculation[1];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[1]      =   g_lastBpmCalculation[1];
                    }
                if (g_lfoMultiplierTMP[1]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]])
                    {
                    g_lastCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[1]);                                   // g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]];
                    }
}

void            CKernel::predictedNextBeat2 ( int p_source, int p_lfoMult )  // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                if (currentTime >= g_nextBeatTime[p_source])
                    {
                    g_nextBeatTime[p_source]               +=  g_intervalCalculated[p_source];                                                                        // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[p_source]) 
                    {
                    g_lastCircleBuffer[p_source]           =   g_nextCircleBuffer[p_source];
                    g_nextCircleBuffer[p_source]           =   g_nextCircleBuffer[p_source] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[p_source]); // why again g_lfoMultiplierTMP? isnt it stored already, do we need to back it up?
                    g_lfoMultiplierTMP[source0]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMult]];
                    }
                if ((g_lastBpmCalculationTMP[p_source]     !=  g_lastBpmCalculation[p_source]))
                    {
                    g_nextBeatTime[p_source]               =   g_lastBpmCalculation[p_source];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[p_source]      =   g_lastBpmCalculation[p_source];
                    }
                if (g_lfoMultiplierTMP[p_source]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMult]])
                    {
                    g_lastCircleBuffer[p_source]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[p_source]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[p_source]);
                    g_lfoMultiplierTMP[p_source]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMult]];
                    }

}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::sampleWaveTable                   () // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                          // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_elapsedMicroseconds[0]            =   currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0]                    =   g_nextCircleBuffer[0] - g_lastCircleBuffer[0];                                                    // Total length of the current cycle
                int f_indexA                        =  (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];                                               // 255 is not the amplitude! its the number of samples
                g_sampleIndex[0]                    =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255!
                g_inOutMatrixFlt[0][LF1]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][g_sampleIndex[0]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][LF1]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1]            =   currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1]                    =   g_nextCircleBuffer[1] - g_lastCircleBuffer[1];                                                   // Total length of the current cycle
                int f_indexB                        =  (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1]                    =   f_indexB > 255 ? 255 : f_indexB;                                                                  // ! i like to get rid of this saveguard !
                g_inOutMatrixFlt[0][LF2]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][g_sampleIndex[1]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][LF2]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][g_sampleIndex[1]];
}   

void            CKernel::sampleWaveTable                   ( int p_source, int p_lfoOut, int p_waveTable ) // love to split it but i will need additional parameters right? like LF1_MULT
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                          // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_elapsedMicroseconds[p_source]     =   currentTime - g_lastCircleBuffer[p_source];
                g_cycleLength[p_source]             =   g_nextCircleBuffer[p_source] - g_lastCircleBuffer[p_source];                                                    // Total length of the current cycle
                int f_indexA                        =  (g_elapsedMicroseconds[p_source] * 255) / g_cycleLength[p_source];                                               // 255 is not the amplitude! its the number of samples
                g_sampleIndex[p_source]             =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255!
                g_inOutMatrixFlt[0][p_lfoOut]       =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_waveTable]][g_sampleIndex[p_source]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][p_lfoOut]       =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_waveTable]][g_sampleIndex[p_source]];
}   
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------