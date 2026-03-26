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