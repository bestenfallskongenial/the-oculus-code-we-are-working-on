#ifndef _kernel_h
#define _kernel_h

#include <circle/types.h>   

#include <circle/actled.h>                                  // onboard LED
#include <circle/koptions.h>                                // rasppi hardware options
#include <circle/machineinfo.h>
#include <circle/devicenameservice.h>						
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
//#include <circle/interrupt.h>								// ????????????????????????????????
#include <circle/timer.h>
// #include <circle/logger.h>

#include <circle/memory.h>										// new for dma allocated buffers!
#include <circle/new.h>												// new for dma allocated buffers!
// ironically i could remove header files without problems but im still concerned about the correct order here and in kernel.cpp ( for example the memory system must be initialized before the file system, because of the dma buffers )
#include "/home/bestenfalls/circle/addon/SDCard/emmc.h"

#include <circle/usb/usbhcidevice.h>
#include <circle/fs/fat/fatfs.h>

#include <interface/vcos/vcos.h>  // For VCHI_INSTANCE_T
#include <vc4/vchi/vchi.h>       // For VCHI_CONNECTION_T
#include <vc4/vchiq/vchiqdevice.h>
#include "vc4/vchiq/vchiq_arm.h"

#include "bcm_host.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>

#include <EGL/eglext.h>
#include <GLES/glext.h>
#include <GLES2/gl2ext.h>

#include <circle/sched/scheduler.h>                         // multitasking ?!


#include <circle/bcmwatchdog.h> // new watchdog

#include <circle/spimaster.h>

#include <sensor/mcp300x.h>									// adc

#include <WS28XX/ws2812oversmi.h> 

#include <circle/gpiomanager.h>
#include <circle/gpiopin.h>



#define 		VIDEO_WIDTH				640
#define			VIDEO_HEIGHT			480
#define 		MAX_FRAMES 				2048
#define 		BASELINE_PROFILE		66		// Baseline

#define			TIMEOUT					10
#define 		LOGLEVEL				4	// 0: only panic 1: also errors 2: also warnings 3: also notices 4: also debug output (default))
#define         ADC_CHANNELS         	8   // number of adc input channels
#define 		ADC_BUFFER			 	4	// number of buffer cells for smoothing
#define 		NUMBER_OF_MODES			6	// adc, trigger, bpm, LFO 1, LFO 2, audio ( for now )
#define 		ADC_SELECT_PRG			7	// adc channel to choose the glsl-u_program_handle
#define			ADC_SELECT_TEX			6	// adc channel to choose the texture
#define			ADC_INPUT_CLK			5	// adc channel use as clock ! BIGGER THAN 3 !

#define 		DEFAULT_SLOT			33

#define 		LFO_PARAMETERS			4
#define 		LFO_INSTANCES			2
#define			LFO_MULTIPLIERS			7

// the array for the loader constance - i think its better than scatter the values / constants everywhere around

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs.
                                            // m_bufferLog[0]       - startup - memory - init log
                                            // m_bufferLog[1]       - vc_sm
                                            // m_bufferLog[2]       - mmal
                                            // m_bufferLog[3]       - check glsl
                                            // m_bufferLog[4]       - glsl compile
                                            // m_bufferLog[5  - 15] - texture parser
                                            // m_bufferLog[16 - 23] - video parser

#define         VSH_USB                 0	// max number of u_vertex shader on sd
#define         OMF_USB            		0	// max number of fragment shader on sd
#define         FSH_USB            		32	// max number of fragment shader on sd
#define         OMT_USB            		0	// max number of fragment shader on sd
#define         TEX_USB            		8	// max number of textures on sd
#define         VID_USB            		8	// max number of videos on sd
#define         KLN_USB                 1

#define         FRM_USB                 1   // i put them here because if my mem/dma allocation
#define         LOG_USB                 1

#define         VSH_EXT                 1
#define         OMF_EXT                 1
#define         FSH_EXT                 1
#define         OMT_EXT                 1
#define         TEX_EXT                 1
#define         VID_EXT                 1
#define         KLN_EXT                 1

#define         VSH_SIZ                 (1024*32)
#define         OMF_SIZ                 (1024*32)
#define         FSH_SIZ                 (1024*32)
#define         OMT_SIZ                 (1024*1024*4)
#define         TEX_SIZ                 (1024*1024*4)
#define         VID_SIZ                 (1024*1024*8)
#define         KLN_SIZ                 (1024*1024*2)

#define         FRM_SIZ                 (1024*1024)

#define         LOG_SIZ                 (1024*64)

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves
#define         PARTITIONSD	            "emmc1-1"	// partition sd
#define         PARTITIONUSB	        "umsd1-1"	// partition usb
// LOGFILE NAMES 
#define 		FILENAME_GL_LOG			"LOG-GLES.txt"
#define 		FILENAME_GLERROR		"LOG-GLER.txt"
#define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
#define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"

#define			FILENAME_MODES			"MODES.txt"
#define 		FILENAME_PARSER0		"FRMT-0.txt"
#define 		FILENAME_PARSER1		"FRMT-1.txt"
#define 		FILENAME_PARSER2		"FRMT-2.txt"
#define 		FILENAME_PARSER3		"FRMT-3.txt"
#define 		FILENAME_KERNEL			"kernel.img"
#define 		CHUNK_SIZE				1024
// hardware 
#define         LED_PIN		           	19		// pin led ( 8 .. 23 )
#define         CS_PIN                 	12      // pin for menu input selector
#define         SW_PIN_A	           	13      // button A ( lower )
#define         SW_PIN_B	          	6       // button B ( upper )
#define         LED_COUNT		       	4		// number of LEDs on the stripes
#define         GPIO_TO_SD_LINE(pin)	((pin) - 8)							// dont touch!
#define         SD_LINE1		      	GPIO_TO_SD_LINE (LED_PIN)			// dont touch!
#define         SD_LINE_TO_MASK(line)	(1 << (line))						// dont touch!
#define         SD_LINES_MASK		 	(  SD_LINE_TO_MASK (SD_LINE1) )		// dont touch!

#define         CHANNEL			    	7		                    // 7 ( for mcp3008 )
#define         VREF			 		5.0f	                    // Reference voltage 5 Volt
#define         SPI_MASTER_DEVICE	 	0		                    // 0
#define         SPI_CHIP_SELECT		 	0		                    // 0
#define         SPI_CLOCK_SPEED	   		1000000		                // Hz
// logger/assertion
#define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertiion implimentation

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

enum modetable		// for the g_centralModeBuffer array
{
	CH0_MODE = 0,
	CH1_MODE,
	CH2_MODE,
	CH3_MODE,

	CH4_MODE,
	CH5_MODE,
	CH6_MODE,
	CH7_MODE,

	LF1_WAVE,
	LF2_WAVE,
	LF1_MULT,
	LF2_MULT,

	SENS_A,
	SENS_B,
	SENS_C,
	SENS_D,

	FRM_MODE,
	TEX_MODE,
	CLK_MODE,
	VID_MODE,

	IS_STORED,
	modetablecount = 21 // right?
}
// we need to check this, also the file menu.cpp code. 
uint8_t g_modeMap[MENU_LAYERS*4][MENU_LAYERS*4] =	// the first element is the max of modes for each channel, than we have the order ( switch case of applyModeToChannel(int channel) )
{ //  A    /  B    /  LFO  / Sens  / etc     
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} 	// layer a is adc in 0-3 
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer b is adc in 4-7
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}   // layer d is adc in 12-15
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}    // layer c is adc in 8-11
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
};


enum FileType       // for the array filecounter[FT_COUNT][FLD_COUNT]
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,         // i decided to add the output-frames A & B
    LOGGER,         // and logger buffer information here for convenience in the wrapper
    FT_COUNT
};

enum FileField      // for the array filecounter[FT_COUNT][FLD_COUNT]
{
    FLD_MAXSD = 0,
    FLD_MAXUSB,
    FLD_EXTCNT,
    FLD_SCANNED,
    FLD_LOADED,
    FLD_PREV,
    FLD_SIZE,
    FLD_COUNT
};

enum io_types       // for the array g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT] & g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT]
{
int RAW = 0,        //  the position the dampened adc values per channels are stored *
int VAL,            //  either the calculated/normalised int or flt value *
int OUT,            //  here lands the processed ( after mode ) for the glsl uniforms      
int RND,            //  either the per-channel random int or flt value *
int LF1,            //  either the lfo one int or flt value
int LF2,            //  either the lfo two int or flt value
int AU0,            //  the audio band 0 flt value
int AU1,            //  the audio band 1 flt value
int AU2,            //  the audio band 2 flt value
int AU3,            //  the audio band 3 flt value
                    // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
int TRL,            //  per channel threshold low !!! dont forget to copy the values in here    128
int TRH,            //  per channel threshold high                                              320
int TRF,            //  per channel threshold "flag"
int IO_TYPE_COUNT
}

g_inOutMatrixInt[CHANNEL][IO_TYPE_COUNT];
g_inOutMatrixFlt[CHANNEL][IO_TYPE_COUNT];
g_menuPickUpFlag[4*MENU_LAYER];


enum ButtonTSIndex  // for the button state machine - unsigned int g_buttons_states[2][5] = {0} !!!
{
    BTN_STATUS      = 0,    // omitted - doublecheck !!!
    BTN_PRESS_START = 1,
    BTN_DOUBLE      = 2,
    BTN_LONG        = 3,    // omitted ?
    BTN_RELEASE     = 4,
    BTN_SINGLE      = 5,
    BTN_HOLD_TICK   = 6,   // COUNTER: increases while held
    BTN_INDEX_COUNT 
};

class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean Initialize (void);

	TShutdownMode Run (void);

/* OLD !!!
struct glsl_state
	{
	// EGL Window
   	uint32_t screen_width;
   	uint32_t screen_height;
 
   	DISPMANX_ELEMENT_HANDLE_T dispman_element;
   	DISPMANX_DISPLAY_HANDLE_T dispman_display;

   	EGLDisplay display;
   	EGLSurface surface;
   	EGLContext context;
	// GL Handles
   	GLuint gl_vsh_id[VSH_SD+VSH_USB];
   	GLuint gl_fsh_id[FSH_SD+FSH_USB];

   	GLuint gl_prg_id[FSH_SD+FSH_USB];

	GLuint gl_tex_id[TEX_SD+TEX_USB];
	GLint  u_tex_id[FSH_SD+FSH_USB][TEX_SD+TEX_USB];

   	GLuint gl_buf; // buffer
	GLuint gl_vtx; // vertex

	GLint u_time[FSH_SD+FSH_USB];
	GLint u_tres[FSH_SD+FSH_USB];
	GLint u_seed[FSH_SD+FSH_USB];	
	GLint u_aud[FSH_SD+FSH_USB];
	GLint u_col[FSH_SD+FSH_USB];

	GLint u_par_a[FSH_SD+FSH_USB];
	GLint u_par_b[FSH_SD+FSH_USB];
    GLint u_tex_l[FSH_SD+FSH_USB];
	};

static const int MENU_GPU_TILE_COUNT = 16;

struct menu_glsl_state
{
    GLuint      gl_omp_id[OMF_SD+OMF_USB];
    GLuint      gl_omt_id[OMT_SD+OMT_USB];

    GLint       u_atlas[OMF_SD+OMF_USB];
    GLint       u_tile_count[OMF_SD+OMF_USB];
    GLint       u_tile_rect[OMF_SD+OMF_USB];
    GLint       u_tile_index[OMF_SD+OMF_USB];

    float       kMenuOrigin[2];
    float       kMenuTileSize[2];
    float       kMenuBackgroundScale[2];

    float       kMenuRelPos[MENU_GPU_TILE_COUNT][2];
    float       kMenuRelSize[MENU_GPU_TILE_COUNT][2];

    float       tile_rect_x[MENU_GPU_TILE_COUNT];
    float       tile_rect_y[MENU_GPU_TILE_COUNT];
    float       tile_rect_w[MENU_GPU_TILE_COUNT];
    float       tile_rect_h[MENU_GPU_TILE_COUNT];

    GLfloat     tile_rect[MENU_GPU_TILE_COUNT * 4];
    GLfloat     tile_index[MENU_GPU_TILE_COUNT];
};
*/

// NEW

    struct vtx_state
{
    // shared attrib/buffer
    GLuint                      gl_buf;                         // this is also an extra struct we need to pass too
    GLint                       gl_vtx[MAX_SHADER];    
};

struct olg_state
{
    // EGL Window
    uint32_t                    screen_width;
    uint32_t                    screen_height;

    DISPMANX_ELEMENT_HANDLE_T   dispman_element;
    DISPMANX_DISPLAY_HANDLE_T   dispman_display;

    EGLDisplay                  display;
    EGLSurface                  surface;
    EGLContext                  context;
};
struct tex_state
{

    unsigned                    width[MAX_TEXTURE];
    unsigned                    height[MAX_TEXTURE];
    unsigned                    offset[MAX_TEXTURE];
	GLuint                      gl_tex_id[MAX_TEXTURE];
	GLint                       u_tex_id[MAX_SHADER][MAX_TEXTURE];
};

struct glsl_state
{
    GLuint                      gl_shader_id[MAX_SHADER];
    GLuint                      gl_program_id[MAX_SHADER];
    // user uniforms                                            // this is the actual common shader struct we define for 
    GLint                       u_time[MAX_SHADER];
    GLint                       u_tres[MAX_SHADER];
    GLint                       u_seed[MAX_SHADER];
    GLint                       u_aud[MAX_SHADER];
    GLint                       u_col[MAX_SHADER];
    GLint                       u_par_a[MAX_SHADER];
    GLint                       u_par_b[MAX_SHADER];
    GLint                       u_tex_l[MAX_SHADER];
    // overlay uniforms
    GLint                       u_atlas[MAX_OMF];
    GLint                       u_tile_count[MAX_OMF];
    GLint                       u_tile_rect[MAX_OMF];
    GLint                       u_tile_index[MAX_OMF];

    // overlay data
    float                       kMenuOrigin[2];
    float                       kMenuTileSize[2];
    float                       kMenuBackgroundScale[2];

    float                       kMenuRelPos[MAX_TILES][2];
    float                       kMenuRelSize[MAX_TILES][2];

    float                       tile_rect_x[MAX_TILES];
    float                       tile_rect_y[MAX_TILES];
    float                       tile_rect_w[MAX_TILES];
    float                       tile_rect_h[MAX_TILES];

    GLfloat                     tile_rect[MAX_TILES * 4];
    GLfloat                     tile_index[MAX_TILES];
};

// we need a struct for the videos right?

                char** 				    m_bufferVid;
                char* 				    m_videoBlockBase;
                char* 				    m_videoRawBlock;
                size_t 				    m_videoBlockSize;

                char**				    m_bufferFrA;
                char* 				    m_frameBlockBaseA;
                char* 				    m_frameRawBlockA;
                size_t 				    m_frameBlockSizeA;

                char**				    m_bufferFrB;
                char* 				    m_frameBlockBaseB;
                char* 				    m_frameRawBlockB;
                size_t 				    m_frameBlockSizeB;	

                char** 				    m_bufferOmt;
                char* 				    m_overlyBlockBase;
                char* 				    m_overlayRawBlock;
                size_t 				    m_overlyBlockSize;

                char** 				    m_bufferTex;
                char* 				    m_textureBlockBase;
                char* 				    m_textureRawBlock;
                size_t 				    m_textureBlockSize;

                char**				    m_bufferKnl;
                char**				    m_bufferLog;
                char** 				    m_bufferVsh;
                char** 				    m_bufferOmf;                
                char** 				    m_bufferFsh;

int filecounter[FT_COUNT][FLD_COUNT] =
{   //          MAXSD   MAXUSB      EXTCNT      SCANNED   LOADED  PREV    SIZE  
    /* VSH */ { VSH_SD, VSH_USB,    VSH_EXT,    0,        0,      0,      VSH_SIZ },
    /* OMF */ { OMF_SD, OMF_USB,    OMF_EXT,    0,        0,      0,      OMF_SIZ },
    /* FSH */ { FSH_SD, FSH_USB,    FSH_EXT,    0,        0,      0,      FSH_SIZ },
    /* OMT */ { OMT_SD, OMT_USB,    OMT_EXT,    0,        0,      0,      OMT_SIZ },
    /* TEX */ { TEX_SD, TEX_USB,    TEX_EXT,    0,        0,      0,      TEX_SIZ },
    /* VID */ { VID_SD, VID_USB,    VID_EXT,    0,        0,      0,      VID_SIZ },
    /* KLN */ { KLN_SD, KLN_USB,    KLN_EXT,    0,        0,      0,      KLN_SIZ },
    /* FRM */ { FRM_SD, FRM_USB,          0,    0,        0,      0,      FRM_SIZ },     // i decided to add the output-frames A & B
    /* LOG */ { LOG_SD, LOG_USB,          0,    0,        0,      0,      LOG_SIZ }      // and logger buffer information here      
};
// lists of extensions possible in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLM_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLM_SD + KLN_USB]      = { 0 };
// our buffers members for the allocation

private:
	CActLED			    m_ActLED;
	CKernelOptions		m_Options;
	CMachineInfo		m_MachineInfo;
	CDeviceNameService	m_DeviceNameService;
	CScreenDevice		m_Screen;
	CSerialDevice		m_Serial;
	CExceptionHandler	m_ExceptionHandler;
	CInterruptSystem	m_Interrupt;
	CTimer			    m_Timer;
	CLogger			    m_Logger;
	CEMMCDevice			m_EMMC;
	CUSBHCIDevice		m_USBHCI;
	CVCHIQDevice		m_VCHIQ;

	CMemorySystem		m_Memory;

	volatile boolean	m_bStorageAttached;
	CFATFileSystem		*m_pFileSystem;

  	CScheduler		    m_Scheduler; // really? is it needed for the khronos stuff?

	CBcmWatchdog       	m_Watchdog; // Watchdog instance - we included our own!

	CSPIMaster		    m_SPIMaster;
	CMCP300X		    m_MCP300X;

	CWS2812OverSMI		m_NeoPixels;

	CGPIOManager		m_GPIOManager;		// not needed in polling mode - we included our own "write to pin" but why does the constructor def uses "m_GPIOManager (&m_Interrupt),..." oh i see, the ws2812 led i assume

	CGPIOPin 			m_ChipSelectPin;  	// Add this line for the chip select pin - we included our own "write to pin"
public:

    bool                        vsh_flags[MAX_VERTEX];
    bool                        fsh_flags[MAX_SHADER];
    bool                        omf_flags[MAX_SHADER];

    bool                        tex_flags[MAX_TEXTURE];
    bool                        omt_flags[MAX_TEXTURE];        

    olg_state                   m_ogl;

    vtx_state                m_vtx;

    glsl_state                  m_vsh;
    glsl_state                  m_fsh;
    glsl_state                  m_osh;

    tex_state               m_tex;
    tex_state               m_omt;

// we need also a struct for the videos, just to keep the frame hear    
}