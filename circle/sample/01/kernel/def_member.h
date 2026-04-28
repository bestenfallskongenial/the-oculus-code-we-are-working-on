//----------------------------------------------------------------------------------------------------------------------------------------------------
                CActLED			        m_ActLED;                           // * CActLED: Switch the Act LED on and off, checks the Raspberry Pi model to use the right LED pin.
                                                                            // still not sure if i want to keep it
                CKernelOptions		    m_Options;                          // * CKernelOptions: Providing kernel options from file cmdline.txt (see doc/cmdline.txt).
                                                                            // mandatory of need for my stuff?
                CMachineInfo		    m_MachineInfo;                      // * CMachineInfo: Helper class to get different information about the running computer.
                                                                            // mandatory of need for my stuff?
                CDeviceNameService	    m_DeviceNameService;                // * CDeviceNameService: Devices can be registered by name and retrieved later by this name
                                                                            // mandatory - but for what?
                CScreenDevice		    m_Screen;                           // * CScreenDevice: Writing characters to screen, some escape sequences (some are not yet implemented)
                                                                            // tried to replace it -> logging.cpp
                CSerialDevice		    m_Serial;                           // * CSerialDevice: Driver for PL011 UART, interrupt or polling mode
                                                                            // who needs this?
                CExceptionHandler	    m_ExceptionHandler;                 // * CExceptionHandler: Generates a stack-trace and a panic message if an abort exception occurs.
                                                                            // why we need it?
                CInterruptSystem	    m_Interrupt;                        // * CInterruptSystem: Connecting to interrupts, an interrupt handler will be called on interrupt.
                                                                            // really? for time? and what else?
                CTimer			        m_Timer;                            // * CTimer: Manages the system clock, supports kernel timers and a calibrated delay loop.
                                                                            // what is controlled here? is there a different simpler way to delay?
                CLogger			        m_Logger;                           // * CLogger: Writing logging messages to a target device
                                                                            // i got my own, is this mandatory?
                CEMMCDevice			    m_EMMC;                             // *The SD card device driver CEMMCDevice does not use DMA and does not need cache-aligned DMA buffers. 
                                                                            // mandatory for the sd device
                CUSBHCIDevice		    m_USBHCI;                           // * CUSBHCIDevice: Alias for CDWHCIDevice or CXHCIDevice, depending on Raspberry Pi model.
                                                                            // mandatory for the usb device
                CVCHIQDevice		    m_VCHIQ;                            // mandatory for the vc04 device ( uses high level linux emulation, correct? )

                CMemorySystem		    m_Memory;                           // * CMemorySystem: Enabling MMU if requested, switching page tables (not used here).
                                                                            // mandatory for my mem allocation

                CFATFileSystem*         m_pFileSystem;                      // * CFATFileSystem: File system driver for FAT16 and FAT32 storage partitions.
                                                                            // mandatory for file operation -> datamanagement.cpp

                CScheduler		        m_Scheduler;                        // * CScheduler: Cooperative non-preemtive scheduler which controls which task runs at a time.
                                                                            // really? who needs this? khronos stuff?

 //             CBcmWatchdog       	    m_Watchdog;                         // * CBcmWatchdog: Driver for the BCM2835 watchdog device.
 //                                                                             // Watchdog instance - tried to implement as class code!

                CSPIMaster		        m_SPIMaster;                        // * CSPIMaster: Driver for (non-AUX) SPI master device. Synchronous polling operation.
                                                                            // mandatory for the mpc control interfacing
                CMCP300X		        m_MCP300X;                          // *  Driver for MCP3004/3008 DAC with SPI interface
                                                                            // mandatory for the adc

                CWS2812OverSMI		    m_NeoPixels;                        // * Driver for multiple WS2812 controlled LED strips
                                                                            // mandatory for the ws2812 rgb led´s

                CGPIOManager		    m_GPIOManager;		                // * CGPIOManager: Interrupt multiplexer for CGPIOPin (only required if GPIO interrupt is used).
                                                                            // mandatory for the pin control BUT not needed in polling mode 
                                                                            // we included our own "write to gpiopin" but why does the constructor def uses "m_GPIOManager (&m_Interrupt),..." oh i see, the ws2812 led i assume

	            CGPIOPin 			    m_ChipSelectPin;  	                // * CGPIOPin: Encapsulates a GPIO pin, can be read, write or inverted. Supports interrupts. Simple initialization.
                                                                            // mandatory, but why again? 
                                                                            // Add this line for the chip select gpiopin - we try included our own "write to gpiopin"

// SPI

private:
    uintptr m_SPIBaseAddress = 0;
    boolean m_SPIValid = 0;

// framebuffer

public:
    CBcmFrameBuffer gE_FrameBuffer;
    CCharGenerator  gE_CharGenerator;
    CKernelOptions  m_Options;

    u32*            gE_PixelBuffer      = 0;
    unsigned        gE_PitchBytes       = 0;
    unsigned        gE_ScreenWidth      = 0;
    unsigned        gE_ScreenHeight     = 0;
    unsigned        gE_CharWidth        = 0;
    unsigned        gE_CharHeight       = 0;
    unsigned        gE_Cols             = 0;
    unsigned        gE_Rows             = 0;

// SMI / DMA / WS2812

private:
    CDMAChannel m_SMITxDMA;

    unsigned    m_SMIGpioPin   = 0;

    unsigned    m_SMISDMask    = 0;

    unsigned    m_LEDCount     = 0;
    unsigned    m_BufferLength = 0;

    TXDATA_T*   m_pBuffer      = 0;

    boolean     m_SMIValid     = FALSE;


// watchdog

static const unsigned WatchdogMaxTimeoutSeconds = 15;
//----------------------------------------------------------------------------------------------------------------------------------------------------
// local copies of my graphics related structs
//----------------------------------------------------------------------------------------------------------------------------------------------------
                olg_state               m_ogl    = {};

                vtx_state               m_vtx    = {};

                glsl_state              m_vsh    = {};
                glsl_state              m_fsh    = {};
                glsl_state              m_osh    = {};

                tex_state               m_tex    = {};
                tex_state               m_omt    = {};

                h264_state              m_vid    = {};    
//----------------------------------------------------------------------------------------------------------------------------------------------------
    volatile    boolean	                m_bStorageAttached  = false;

                char** 				    m_bufferVid         = nullptr;      // thats the pointer to my "array-like" buffer allocation
                char* 				    m_videoBlockBase    = nullptr;      // returns the aligned DMA base pointer
                char* 				    m_videoRawBlock     = nullptr;      // returns the original pointer from new[]
                size_t 				    m_videoBlockSize    = 0;            // size of each individual buffer

                char**				    m_bufferFrA         = nullptr;      // i created a struct for it but that means i must 
                char* 				    m_frameBlockBaseA   = nullptr;      // rewrite the wrappers and initialize the stucts properly
                char* 				    m_frameRawBlockA    = nullptr;      // and that is actually not really progress
                size_t 				    m_frameBlockSizeA   = 0;

                char**				    m_bufferFrB         = nullptr;
                char* 				    m_frameBlockBaseB   = nullptr;
                char* 				    m_frameRawBlockB    = nullptr;
                size_t 				    m_frameBlockSizeB   = 0;	

                char** 				    m_bufferOmt         = nullptr;
                char* 				    m_overlyBlockBase   = nullptr;
                char* 				    m_overlayRawBlock   = nullptr;
                size_t 				    m_overlyBlockSize   = 0;

                char** 				    m_bufferTex         = nullptr;
                char* 				    m_textureBlockBase  = nullptr;
                char* 				    m_textureRawBlock   = nullptr;
                size_t 				    m_textureBlockSize  = 0;

                char**				    m_bufferKnl         = nullptr;
                char**				    m_bufferLog         = nullptr;
                char** 				    m_bufferVsh         = nullptr;
                char** 				    m_bufferOmf         = nullptr;                
                char** 				    m_bufferFsh         = nullptr; 
//----------------------------------------------------------------------------------------------------------------------------------------------------
// the populated filecounter array - source and truth and hub for init and load
//----------------------------------------------------------------------------------------------------------------------------------------------------
                unsigned                filecounter[FT_COUNT][FLD_COUNT] =
{                                       // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
                                        { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
                                        { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
                                        { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
                                        { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
                                        { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
                                        { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
                                        { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
                                        { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
                                        { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
};
//----------------------------------------------------------------------------------------------------------------------------------------------------
// lists of extensions possible in my scanroot directory function per filetype 
//----------------------------------------------------------------------------------------------------------------------------------------------------
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" };    // vertex shaders
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };    // fragment shaders 
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" };    // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };    // for textures 24bit rgb
        const   char                   *g_SufVid[VID_EXT]			    = { "264" };    // video in raw h264 annex b encoded 
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };    // kernel.img for the update mechanism
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };    
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };
//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t g_modeMap[MENU_LAYERS*4][MENU_LAYERS*4] =	// the first element is the max of modes for each p_channel, than we have the order ( switch case of setChannelMode(int p_channel) )
                                                    // i just wonder if i need a dedicated function to edit the first element because other code might do it as read adc 
                                                    {  //  A    /  B    /  LFO  / Sens  / etc     
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} 	// layer a is adc in 0-3 
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer b is adc in 4-7
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}   // layer d is adc in 12-15
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    {63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    { 4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
                                                    };
//----------------------------------------------------------------------------------------------------------------------------------------------------
// Use a **member function pointer table** instead of `switch`.
// That lets you add modes by only extending the table.
typedef void (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right?

ModeFunc g_modeTable[] =
{
    &CKernel::modeADC,
    &CKernel::modeTRG,
    &CKernel::modeBPM,
    &CKernel::modeLF1,
    &CKernel::modeLF2,
    nullptr,
    nullptr,
    nullptr,
    &CKernel::modeAudioAb0,
    &CKernel::modeAudioAb1,
    &CKernel::modeAudioBb0,
    &CKernel::modeAudioBb1
};                                                    
//----------------------------------------------------------------------------------------------------------------------------------------------------
                unsigned                g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];           // the integer in/out matrix
                float                   g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];           // the float in/out matrix
                bool                    g_menuPickUpFlag[4*MENU_LAYER];                     // the flags for the pickup mechanism  
                unsigned                g_buttons_states[NR_BUTTONS][BTN_INDEX_COUNT];      // the button state machine
                unsigned                g_centralModeBuffer[SLOTS][modetablecount];         // the general user settings, storable per program 
                unsigned                g_lfoMultiplier[LFO_MULTIPLIERS];

                unsigned                g_lfoBpmMatrix[4][LFO_BPM_COUNT];

                unsigned                g_hFile;                                            // file management!
//----------------------------------------------------------------------------------------------------------------------------------------------------
// VCSM and MMAL
//----------------------------------------------------------------------------------------------------------------------------------------------------

private:
        VCHI_INSTANCE_T                 m_VCHIInstance                  = 0;
        VCHI_CONNECTION_T*              m_Connection                    = 0;
        VCOS_EVENT_T                    m_EventSMEM                     = {};
        VCOS_EVENT_T                    m_EventMMAL                     = {};
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleVCSM             = 0;
        VCHI_SERVICE_HANDLE_T           m_ServiceHandleMMAL             = 0;
        u32                             m_TransactionId                 = 0;
        u32                             m_vc_handle[MAX_BUFFER]         = {0};
        u32                             m_vc_pointer[MAX_BUFFER]        = {0};
// ---------------------------------------------------------------------------------------------------------------------------------------------------
public:
        GLuint                          m_Texture = 0;                                              // must be exposed
private:
        bool                            f_firstFrameQueued              = false;    
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
// artifacs trom the earlier mmal class code initializer - what do we really need here? -> check also the mmal_init code!!!

        u32                             m_InputBufferHandle;
        u32                             m_InputBufferPointer;

        u32                             m_InputBufferSize;

        u32                             m_OutputBufferHandleA;
        u32                             m_OutputBufferPointerA;

        u32                             m_OutputBufferHandleB;
        u32                             m_OutputBufferPointerB;

        u32                             m_OutputBufferSize;

        u32                             m_ResolutionX;
        u32                             m_ResolutionY;

        u32                             m_ComponentHandle;

        u32                             m_VCSMHandleA;
        u32                             m_VCSMHandleB;

        u32                             m_InputPortHandle;
        u32                             m_OutputPortHandle;  
        
        u32                             m_CurrentHandle;
//----------------------------------------------------------------------------------------------------------------------------------------------------        
// VCSM predefined messages as public member here - used for 
//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
        SERVICE_CREATION_T              m_ServiceCreateVCSM             = {};

        VCSM_Import_MEM_Msg             m_importTxVCSM_A                = {};           // for the video in block
        VCSM_Import_MEM_Reply           m_importRxVCSM_A                = {};           

        VCSM_Import_MEM_Msg             m_importTxVCSM_B                = {};           // for the frame out A
        VCSM_Import_MEM_Reply           m_importRxVCSM_B                = {};

        VCSM_Import_MEM_Msg             m_importTxVCSM_C                = {};           // for the frame out B
        VCSM_Import_MEM_Reply           m_importRxVCSM_C                = {};        

        VCSM_Lock_MEM_Msg               m_lockTxVCSM                    = {};           // do i really need them ever?
        VCSM_Lock_MEM_Reply             m_lockRxVCSM                    = {};

        VCSM_Free_MEM_Msg               m_freeTxVCSM                    = {};           // do i really need them ever?
        VCSM_Free_MEM_Reply             m_freeRxVCSM                    = {};
//----------------------------------------------------------------------------------------------------------------------------------------------------        
// MMAL predefined messages as public member here
//----------------------------------------------------------------------------------------------------------------------------------------------------
        //  0. Open MMAL service (transport only)
        SERVICE_CREATION_T              m_ServiceCreateMMAL             = {};   //  SEND                    - sent via `openServiceVCHI()`
                                                                                //  RECEIVE none            - open VCHI/MMAL control channel. No MMAL protocol yet.
        //  1. Create component
        MMAL_Component_Create_Msg       m_ComponentCreateTx             = {};   //  SEND
        MMAL_Component_Create_Reply     m_ComponentCreateRx             = {};   //  RECEIVE                 - create `ril.video_decode`, receive `component_handle`. This handle is stored and used everywhere later.
        //  2. Snapshot A – get initial port state (before any modification)
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_A         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_A         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_A        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_A        = {};   //  Output port RECEIVE     - Snapshot A is the **authoritative VPU-owned baseline**. You must base `PORT_INFO_SET` on this snapshot.
        //  3. Build SET messages locally (nothing sent yet)
        MMAL_Port_Info_Set_Msg          m_PortInfoSetTx_Input           = {};   //  Input port WRITE

        MMAL_Port_Info_Set_Msg          m_PortInfoSetTx_Output          = {};   //  Output port WRITE       - * This step **does not send anything**. It only prepares TX structs using Snapshot A.
        //  4. Send SET (this is where data from step 3 is sent)
        // MMAL_Port_Info_Set_Msg       m_PortInfoSetTx_Input           = {};   //  Input port SEND
        MMAL_Port_Info_Set_Reply        m_PortInfoSetRx_Input           = {};   //  Input port RECEIVE

        // MMAL_Port_Info_Set_Msg       m_PortInfoSetTx_Output          = {};   //  Output port SEND
        MMAL_Port_Info_Set_Reply        m_PortInfoSetRx_Output          = {};   //  Output port RECEIVE     - Commit format/buffer changes to the VPU.
        //  5. Enable component
        MMAL_Component_Enable_Msg       m_ComponentEnableTx             = {};   //  SEND
        MMAL_Component_Enable_Reply     m_ComponentEnableRx             = {};   //  RECEIVE                 - Component must be enabled before parameters or port actions.
        //  6. Snapshot B – get port state after enable
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_B         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_B         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_B        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_B        = {};   //  Output port RECEIVE     - Snapshot B reflects VPU state **after enable**. Used as the base for zero-copy parameters.
        //  7. Zero-copy parameter set
        MMAL_Port_Parameter_Set_Msg     m_PortParamTx_Input             = {};   //  Input port SEND         - built using `m_PortInfoGetRx_Input_B`
        MMAL_Port_Parameter_Set_Reply   m_PortParamRx_Input             = {};   //  Input port RECEIVE

        MMAL_Port_Parameter_Set_Msg     m_PortParamTx_Output            = {};   //  Output port SEND        - built using `m_PortInfoGetRx_Output_B`
        MMAL_Port_Parameter_Set_Reply   m_PortParamRx_Output            = {};   //  Output port RECEIVE     - Enable zero-copy **before** ports are enabled.
        //  8. Snapshot C – get port state after zero-copy
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_C         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_C         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_C        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_C        = {};   //  Output port RECEIVE     - Snapshot C is the **final authoritative state** before enabling ports.
        //  9. Enable ports
        MMAL_Port_Action_Msg            m_PortActionTx_Input            = {};   //  Input port SEND         - uses handle from `m_PortInfoGetRx_Input_C`
        MMAL_Port_Action_Reply_Msg      m_PortActionRx_Input            = {};   //  Input port RECEIVE

        MMAL_Port_Action_Msg            m_PortActionTx_Output           = {};   //  Output port SEND        - uses handle from `m_PortInfoGetRx_Output_C`
        MMAL_Port_Action_Reply_Msg      m_PortActionRx_Output           = {};   //  Output port RECEIVE     - Ports become live. After this, buffers may flow.
        //  10. Runtime buffer flow
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostTx_Input        = {};   //  Input buffers SEND
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostRx_Input        = {};   //  Input buffers RECEIVE   (same physical layout, semantic RX)

        MMAL_Buffer_From_Host_Msg       m_BufferFromHostTx_Output       = {};   //  Output buffers SEND     (ping/pong)
        MMAL_Buffer_From_Host_Msg       m_BufferFromHostRx_Output       = {};   //  Output buffers RECEIVE  (ping/pong) - Buffer ownership transfer and frame delivery.
        
        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Input_D         = {};   //  Input port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Input_D         = {};   //  Input port RECEIVE

        MMAL_Port_Info_Get_Msg          m_PortInfoGetTx_Output_D        = {};   //  Output port SEND
        MMAL_Port_Info_Get_Reply        m_PortInfoGetRx_Output_D        = {};   //  Output port RECEIVE
//----------------------------------------------------------------------------------------------------------------------------------------------------        
        // Core invariant (unchanged, explicit)
        // Snapshot **A** → build **SET**
        // Snapshot **B** → build **PARAM**
        // Snapshot **C** → build **ACTION**
        // Snapshot **D** → buffer runtime
        // Buffer TX/RX is independent of snapshots  
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
// instead of having life time long structs for my vcsm / mmal i declare pointer instead and provide wrappers to alloc and free the structs after use!
// group A — kernel.h (pointer conversion)

SERVICE_CREATION_T*              m_ServiceCreateVCSM             = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_A                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_A                = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_B                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_B                = nullptr;

VCSM_Import_MEM_Msg*             m_importTxVCSM_C                = nullptr;
VCSM_Import_MEM_Reply*           m_importRxVCSM_C                = nullptr;

VCSM_Lock_MEM_Msg*               m_lockTxVCSM                    = nullptr;
VCSM_Lock_MEM_Reply*             m_lockRxVCSM                    = nullptr;

VCSM_Free_MEM_Msg*               m_freeTxVCSM                    = nullptr;
VCSM_Free_MEM_Reply*             m_freeRxVCSM                    = nullptr;

// group B — kernel.h (full pointer conversion)

SERVICE_CREATION_T*              m_ServiceCreateMMAL             = nullptr;

MMAL_Component_Create_Msg*       m_ComponentCreateTx             = nullptr;
MMAL_Component_Create_Reply*     m_ComponentCreateRx             = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_A         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_A         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_A        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_A        = nullptr;

MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Input           = nullptr;
MMAL_Port_Info_Set_Msg*          m_PortInfoSetTx_Output          = nullptr;

MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Input           = nullptr;
MMAL_Port_Info_Set_Reply*        m_PortInfoSetRx_Output          = nullptr;

MMAL_Component_Enable_Msg*       m_ComponentEnableTx             = nullptr;
MMAL_Component_Enable_Reply*     m_ComponentEnableRx             = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_B         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_B         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_B        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_B        = nullptr;

MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Input             = nullptr;
MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Input             = nullptr;

MMAL_Port_Parameter_Set_Msg*     m_PortParamTx_Output            = nullptr;
MMAL_Port_Parameter_Set_Reply*   m_PortParamRx_Output            = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_C         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_C         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_C        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_C        = nullptr;

MMAL_Port_Action_Msg*            m_PortActionTx_Input            = nullptr;
MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Input            = nullptr;

MMAL_Port_Action_Msg*            m_PortActionTx_Output           = nullptr;
MMAL_Port_Action_Reply_Msg*      m_PortActionRx_Output           = nullptr;

MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Input        = nullptr;
MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Input        = nullptr;

MMAL_Buffer_From_Host_Msg*       m_BufferFromHostTx_Output       = nullptr;
MMAL_Buffer_From_Host_Msg*       m_BufferFromHostRx_Output       = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Input_D         = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Input_D         = nullptr;

MMAL_Port_Info_Get_Msg*          m_PortInfoGetTx_Output_D        = nullptr;
MMAL_Port_Info_Get_Reply*        m_PortInfoGetRx_Output_D        = nullptr;