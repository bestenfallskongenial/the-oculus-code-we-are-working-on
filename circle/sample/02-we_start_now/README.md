datamanagement.cpp
373
graphics.cpp
489
hardware.cpp
458
    /*
    `m_BufferLength` is set here:

    m_BufferLength = TX_BUFF_LEN(ledCount);

    inside:

    my_WS2812_Init(unsigned ledCount)

    So the real order is:

    my_SMI_Init(...)
    → my_WS2812_Init(...)
        → m_BufferLength gets set
        → my_SMI_SetupTiming(...)
        → my_SMI_SetupDMA()
    → my_WS2812_SetLED(...)
    → my_WS2812_Update()

    That is why `my_SMI_SetupDMA()` works there:

    because `m_BufferLength` is already valid from `my_WS2812_Init()`.
    */
logging.cpp
354
menu.cpp
225
parser.cpp
361
stubs.cpp
25
util.cpp
42
vc04_service,cpp
988

/*  NEW AUDIO AND ADC SEPERATED !

    adc_AcquireConvert()
    adc_ProcessAudio()
    advance_adc_index()

    set_mode_length(0)
    modeMenuAssignGroup(...)
*/

/*
    make83FileName             (   const char* ext )

    wil create a random filename from my random value generator, the idea 
    is to have some unique and "quick" sollution to store the logs 
    in case of issues / reboot events...

    uses:

    public:

        char m_83FileName[13];

    example calls :

    saveFromBufferM(p_deviceName, make83FileName("LOG"), MY_BUFFER, MY_INDEX);

    saveFromBufferM(p_deviceName, make83FileName("TXT"), MY_BUFFER, MY_INDEX);

    saveFromBufferM("umsd1", make83FileName("TXT"), p_bufferArray, p_bufferSize);

*/

