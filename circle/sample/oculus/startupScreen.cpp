// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::startupScreen(char* buffer, u32& index)
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

    storeLog(buffer, index, "Machine Model");
    storeLog(buffer, index, machineName);
    nextline(buffer, index);
    storeLog(buffer, index, "SoC Name");
    storeLog(buffer, index, socName);
    nextline(buffer, index);

    /* numeric values */

    storeLog(buffer, index, "Model Major    ", modelMajor);
    storeLog(buffer, index, "Model Revision ", modelRevision);
    nextline(buffer, index);
    storeLog(buffer, index, "RAM Size     MB", ramSize);
    storeLog(buffer, index, "CPU Speed Mode ", cpuSpeedMode);
    nextline(buffer, index);
    storeLog(buffer, index, "SoC Max Temp   ", socMaxTemp);
    nextline(buffer, index);
    storeLog(buffer, index, "Clock CORE  MHz", coreClock);
    storeLog(buffer, index, "Clock ARM   MHz", armClock);    
    storeLog(buffer, index, "Clock EMMC  MHz", emmcClock, emmc2Clock, uartClock);
    storeLog(buffer, index, "Clock EMMC2 MHz", emmcClock, emmc2Clock, uartClock);
    storeLog(buffer, index, "Clock UART  MHz", emmcClock, emmc2Clock, uartClock);    
    nextline(buffer, index);
    storeLog(buffer, index, "DMA Channel    ", dmaChannel);
    nextline(buffer, index);
    storeLog(buffer, index, "USB Delay      ", usbDelay);
    storeLog(buffer, index, "USB FullSpeed  ", usbSpeed);
    
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