#ifndef _kernel_h
#define _kernel_h

#include <circle/string.h> 
#include <stdint.h>

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

#include "bcm_host.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
// declutter declutter declutter
/*
#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/EGL/egl.h"
#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/GLES/gl.h"
#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/GLES2/gl2.h"

#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/EGL/eglext.h"
#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/GLES/glext.h"
#include "/home/bestenfalls/circle/addon/vc4/interface/khronos/include/GLES2/gl2ext.h"
*/
#include <circle/sched/scheduler.h>                         // multitasking ?!
#include <vc4/vchiq/vchiqdevice.h>
#include "vc4/vchiq/vchiq_arm.h"
// my video player
#include <interface/vcos/vcos.h>  // For VCHI_INSTANCE_T
#include <vc4/vchi/vchi.h>       // For VCHI_CONNECTION_T
/*
// Add the external function declaration

*/
#include <vc4/vc_sm/vc_sm_class.h>							// VideoCore Shared Memory							
#include <vc4/h264_decoder/VersionA/vc_h264_decoder.h>					// VideoCore H.264 Decoder for fiexd Resolution Annex B Raw H.264 Videos
#include <vc4/h264_parser/VersionA/vc_h264_parser.h>					// Parser for the H.264 Files

#include <circle/types.h>                                   // 

#include <circle/bcmwatchdog.h> // new watchdog

#include <circle/spimaster.h>

#include <sensor/mcp300x.h>									// adc

#include <WS28XX/ws2812oversmi.h> 

#include <circle/gpiomanager.h>
#include <circle/gpiopin.h>
#include <sensor/buttons.h>  // integrated now !!

// #define 		DEBUGMODE              false    // enables verbose 

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

#define 		DEFAULT_SLOT			32

#define 		LFO_PARAMETERS			4
#define 		LFO_INSTANCES			2
#define			LFO_MULTIPLIERS

#define 		R     					0
#define 		G   					1  
#define 		B    					2

// positions in my ccolor table also better as enums?
#define			M_1						1		// menu a
#define			M_2						2		// menu b
#define			M_L						3		// menu flo
#define			A25						4		// attenuator 2.5v
#define			A33						5		// attenuator 3.3v
#define			A50  					6		// attenuator 5.0v
#define			TRG						7		// trigger mode
#define			BPM						8		// bmp mode
#define			LF1						9		// lfo mode 1
#define			LF2						10		// lfo mode 2
#define			CLK						11		// clock mode? do we have one ?!
#define			A_0						12		
#define			A_1						13
#define			A_2						14
#define			A_3						15

// maybe better of as enums ?! table positions in the global mode storage
#define CH0_MODE 0
#define CH1_MODE 1
#define CH2_MODE 2
#define CH3_MODE 3
#define CH4_MODE 4
#define CH5_MODE 5
#define CH6_MODE 6
#define CH7_MODE 7

#define LF1_WAVE 8
#define LF2_WAVE 9
#define LF1_MULT 10
#define LF2_MULT 11
#define FRM_MODE 12
#define TEX_MODE 13 
#define CLK_MODE 14
#define DMY_MODE 15

#define AUD_MODE 5

#define 		TOLERANCE 				8

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMS_SD             		1	// overlay shader program!
#define         FSH_SD             		2	// max number of fragment shader on sd
#define         OMT_SD             		1	// overlay texture atlas!
#define         TEX_SD             		2	// max number of textures on sd should be one or zero? can be zero?!
#define         VID_SD             		2	// max number of videos on sd should be zero? can be zero?!

#define         VSH_USB            		0	// max number of u_vertex shader on usb
#define         FSH_USB           		32	// max number of fragment shader on usb
#define         TEX_USB            		8	// max number of textures on usb
#define         VID_USB            		8	// max number of videos on usb

#define			KERNEL_SIZE				(1024*1024*2) 	// 2mb must be sufficient here ( i confused the kernel size by the factor 10 )
#define         VSH_SIZE                (1024*32) 		// 32768 size of u_vertex shader files
#define         FSH_SIZE                (1024*32) 		// 32768 size of fragment shader files
#define         TEX_SIZE                (1024*1024*4)  	// 4194304	// size of texture ( .bpm ) files
#define         VID_SIZE                (1024*1024*8) 	// 16777216// size of video ( .264 ) files
#define 		FRAME_SIZE 				(1024*1024) 	// 1MB, which comfortably fits a 640x480x3 frame

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves
#define         PARTITIONSD	            "emmc1-1"	// partition sd
#define         PARTITIONUSB	        "umsd1-1"	// partition usb


// valid fileneme suffixes

#define 		VSH_VALID_SUFFIX_COUNT 1		// .vsh for vertex shader ( one for all this far )
#define 		FSH_VALID_SUFFIX_COUNT 1		// .fsh for my fragment shaders ( 32 + 1 default )
#define 		TEX_VALID_SUFFIX_COUNT 1		// .bmp for textures ( 2+ 8 i guess ) 
#define 		VID_VALID_SUFFIX_COUNT 1		// .264 / .mov / .mp4 ( what we can parse )

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

#define 		CLEAR_SCREEN			"\x1b[H\x1b[J"
#define 		CURSOR_OFF				"\x1b[?25l"

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

#define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertiion implimentation

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean Initialize (void);

	TShutdownMode Run (void);

struct CUBE_STATE_T									// we will  rework this here! seperation of the dispmax/egl - the user shader / overlay shader, vertex, buffer?? means 5 instead of one?
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
// like this?!?
struct OGL_STATE
{
   	uint32_t screen_width;
   	uint32_t screen_height;
 
   	DISPMANX_ELEMENT_HANDLE_T dispman_element;
   	DISPMANX_DISPLAY_HANDLE_T dispman_display;

   	EGLDisplay display;
   	EGLSurface surface;
   	EGLContext context;
};
struct BUF_STATE
{
   	GLuint gl_buf; // buffer
};

struct GL_STATE
{
   	GLuint gl_vsh_id[VSH_SD+VSH_USB];
   	GLuint gl_fsh_id[FSH_SD+FSH_USB];

   	GLuint gl_prg_id[FSH_SD+FSH_USB];

	GLuint gl_tex_id[TEX_SD+TEX_USB];
	GLint  u_tex_id[FSH_SD+FSH_USB][TEX_SD+TEX_USB];
};
struct USER_UNIFORM_STATE
{
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

	bool 			memory_allocate				();											// 
    void 			memory_clean_up				();
	char** 			memory_init_buffer			(	size_t count, size_t bufferSize);
	char** 			memory_init_dma_buffer		(	size_t count, size_t bufferSize,
                                       				char** blockBaseOut,
                                       				char** rawBlockOut,
                                       				size_t* alignedSizeOut);

	void 			memory_clear_buffer			(	char** buffers, size_t count);
	void			memory_clear_dma_buffer		(	char** buffers, char* rawBlock);

	bool 			filesystem_open_file		(	const char *pTitle	);
	unsigned 		filesystem_load_file		(	char *buffer, unsigned bufferSize, int mode);		
	bool 			filesystem_close_file		();
	int 			filesystem_process_files	(  	char* fileNameArray[], 
													unsigned totalLoadedBytes[], 
													char** bufferArray, 
                            						int maxFiles, int successfulLoaded, 
													unsigned TEX_FILE_SIZE, int mode);
	bool 			filesystem_save_log_file	(	const char* deviceName, const char* filename, const CString& str_to_save);
	bool 			filesystem_update_USB		(	const char* deviceType);
	bool 			filesystem_mount			(	const char* deviceName, 
                              						char* vshaderFileNames[], 
													unsigned vStotalLoadedBytes[], int maxVshaderFiles,
                              						char* fshaderFileNames[], 
													unsigned fStotalLoadedBytes[], int maxFshaderFiles,
                              						char* textureFileNames[], 
													unsigned tXtotalLoadedBytes[], int maxTextureFiles,
                                            		char* videoFileNames[]  , 
													unsigned vItotalLoadedBytes[], int maxVideoFiles);
	bool			filesystem_IsValidFileType	(const char* pFileName, const char* extension);
	unsigned 		filesystem_ScanRootDir     	( char** fileArray, const char* exts[], int extCount, unsigned maxFiles );
	bool			filesystem_load_kernel		(const char* deviceName, const char* filename, unsigned bufferIndex);
	bool			filesystem_save_kernel		(const char* deviceName, const char* filename, unsigned bufferIndex);
private:
	static void 	filesystem_remove_USB 		(	CDevice *pDevice, void *pContext);	
public:
	void 			io_read_ADC					();
	void			LED_circle_color			();
	void 			LED_update				();
private:
	static void 	io_event_button_A 			(	BUTTONS::TEvent Event, void *pParam);			// to simplify?
	static void 	io_event_button_B 			(	BUTTONS::TEvent Event, void *pParam);			// to simplify?
public:
	void			util_prep_parameters		();
	int 			util_choose_program			();
	int 			util_choose_texture			();
	void			util_store_program			(); 
	void			util_random_vec8			(uint32_t seed); 
	void 			util_calculate_BPM			(	unsigned long buttonTime, unsigned long clockTime);
	void 			util_determine_bpm_source	();
	void			util_update_predicted_beat	();
  	void 			util_LFO					();
	void 			util_audio_energy			(	float adcvalue	);		

	float			util_get_adc_bpm			(	int adcValue	);
	void			util_calculate_filesize		();
	bool 			util_check_for_update		();
                 
	void 			util_save_modes_file		();

	void   			gfx_check					(	const char *file, unsigned line);		// what do i need to refactor here? and what needs a string in the caller?
	void			gfx_shader_log				(	GLint shader, int shaderIndex);			// what can i replace with my other logger?
	void 			gfx_program_log				(	GLint shader, int program_index) ;				// is to change / remove

	void 			gfx_init_OGL				(	CUBE_STATE_T *state);  		// ****
	void 			gfx_init_vshaders			(	CUBE_STATE_T *state, int fromFile, int toFile);	// ****

	void 			gfx_init_overlay_fshader    (   CUBE_STATE_T *state ); // NEW OVERLAY MENU

	void 			gfx_init_fshaders			(	CUBE_STATE_T *state, int fromFile, int toFile);	// ****
	
	void			gfx_init_overlay_program    (   CUBE_STATE_T *state);  // NEW OVERLAY MENU

	void 			gfx_init_programs			(	CUBE_STATE_T *state, int fromFile, int toFile);	// ****

	void			gfx_init_overlay_uniforms   (   CUBE_STATE_T *state);  // NEW OVERLAY MENU

	void 			gfx_init_uniforms			(	CUBE_STATE_T *state, int fromFile, int toFile);	// ****

	void			gfx_init_overlay_texture    (   CUBE_STATE_T *state);  // NEW OVERLAY MENU  

	void 			gfx_init_textures			(	CUBE_STATE_T *state, int fromFile, int toFile);	// possible change 
	void 			gfx_init_v_buffer			(	CUBE_STATE_T *state);							// possible change
	void 			gfx_render_shader_a			(	CUBE_STATE_T *state);							// possible change

	void			gfx_render_overlay_a		(   CUBE_STATE_T *state);  // NEW OVERLAY, I ASSUME WE NEED TO CALL THE OVERLAY DRAW AFTER THE EFX CALL?!

	void 			gfx_render_shader_b			(	CUBE_STATE_T *state);	

	bool			display_startup_screen		(	CUBE_STATE_T *state); // where do we use state here? for the xy dimensions? impossible!
	void			display_LoadScreenTexVidShd ( int mode );
	void 			display_debug				( 	CUBE_STATE_T *state, int level );					// display_debug menu 
	void 			display_append_modes		(); // display_debug output
	void 			display_print_log_file		();
    void			display_print_File_names 	(const char* prefix, char** array_to_print, int max_files);

	void            menu_general				();
    void            menu_mode_default_A			();
    void            menu_mode_default_B			();
    void            menu_mode_assign_A			();
    void            menu_mode_assign_B			();
	void			menu_mode_assign_LFO		();

	void 			parser_h264					(int fromFile, int toFile);
	void			parser_bmp					(int fromFile, int toFile);
	void 			GenerateH264ParserInfo		(	int     file_index );
	void 			GenerateBmpParserInfo		(	int     file_index );

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
// my Video Player
    CVCSharedMemory 	m_SharedMemory;
    CH264Decoder 		m_H264Decoder;

	CH264Parser 		m_H264SystemParser; // <-- new and only for the overlay... 

    CH264Parser 		m_H264Parser;

		volatile boolean	m_bStorageAttached;
	CFATFileSystem		*m_pFileSystem;

  	CScheduler		    m_Scheduler;

	CBcmWatchdog       	m_Watchdog; // Watchdog instance

	CSPIMaster		    m_SPIMaster;
	CMCP300X		    m_MCP300X;

	CWS2812OverSMI		m_NeoPixels;

	CGPIOManager		m_GPIOManager;		// not needed in polling mode

	CGPIOPin 			m_ChipSelectPin;  	// Add this line for the chip select pin

	BUTTONS				m_Button_A;
	BUTTONS				m_Button_B;

	CUBE_STATE_T  		state;
	
// buffers in order of allocation
// DMA buffers
	char** 				m_bufferVideo;
	char* 				m_videoBlockBase;
	char* 				m_videoRawBlock;
    size_t 				m_videoBlockSize;

	char**				m_bufferFrameBufferA;
	char* 				m_frameBlockBaseA;
	char* 				m_frameRawBlockA;
    size_t 				m_frameBlockSizeA;

	char**				m_bufferFrameBufferB;
	char* 				m_frameBlockBaseB;
	char* 				m_frameRawBlockB;
    size_t 				m_frameBlockSizeB;	

    char** 				m_BufferOverlayTexture;
	char* 				m_overlyBlockBase;
	char* 				m_overlayRawBlock;
    size_t 				m_overlyBlockSize;	

    char** 				m_bufferTexture;
	char* 				m_textureBlockBase;
	char* 				m_textureRawBlock;
    size_t 				m_textureBlockSize;
// buffers
	char**				m_bufferKernel;

	char** 				m_bufferVshader;

	char**				m_bufferFoverlay;

    char** 				m_bufferFshader;


                CString                 g_log_string;
                CString                 g_error_string;
                CString                 g_modes;

                unsigned                hFile                                                               =     0;  // ?? empty ??

                float                   current_fps                                                         =     0.0; // for our display / menu for the fps break mechanism                
                
                float                   util_audio_smooth_band                  [4]                         =  {0.0f};		// buffer to smooth out the audio detection 

                float                   GLtime                                                              =  0.0f;
                float                   opaque                                                              =  1.0f;	

	bool 					m_USBhasLoadOnes;
	bool 					m_resetFlag; // SAME AS resetFlag
	bool 					m_runtimelog = false;												// failsave for the assumed buffer overflaw that cause an crash at ~22 minutes

	bool                    noTargetFPS                                                         =  true;		// is it still in use? the fps break?
	bool                    adc_pot_routing                                                     = false;		// IMPORTANT - SETS THE HARWARE PIN THAT ROUTs THE POTS TO THE ADC!
	bool                    resetFlag                                                           = false;		// IMPORTANT - AS THE NAME SAYS
	bool                    g_show_bank                                                         = false;		// for the led, ment to show the A/B bank depending on the last button pressed

	bool                    is_hold_for_2_sec_a                                                 = false;		// do we really need such stuff after reworking the whole display layer approach???
	bool                    is_hold_for_2_sec_b                                                 = false;
	bool                    is_hold_for_4_sec_a                                                 = false;
	bool                    is_hold_for_4_sec_b                                                 = false;

	bool           			a_is_hold 															= false;		// will be integrated in button´s buttons_pressed_timestamps[2][7] array, right?
	bool           			b_is_hold 															= false;
/*
    long buttons_pressed_timestamps[2][7]; <- why not include the buttons_states[2] there too???? 
    unsigned buttons_states[2]; // 0 for SW_PIN_A, 1 for SW_PIN_B 
*/
	bool                  	m_shaderStatusFlags[FSH_SD+FSH_USB]; // we need this here to ensure that we not try to link/compile damaged glsl code, right? 

	bool                  	menu_pickup_flag[ADC_CHANNELS+ADC_CHANNELS]              			= { 0 };                 // Array tracking if pot value has been "picked up"
	bool                  	input_threshold_flag[ADC_CHANNELS]              					= { 0 };             // Array tracking threshold state per channel

	int                     VSH_LOADED_OLD                                                      =     0;		// counter to ensure that we can load shaders and textures etc from sd and usb
	// we need none for the new overlay shader because we never load a second
	int                     FSH_LOADED_OLD                                                      =     0;
	// we need none for the new overlay shader because we never load a second
	int                     TEX_LOADED_OLD                                                      =     0;
	int                     VID_LOADED_OLD                                                      =     0;
	int                     VSH_LOADED_NEW                                                      =     0;
	int                     FSH_LOADED_NEW                                                      =     0;
	int                     TEX_LOADED_NEW                                                      =     0;
	int                     VID_LOADED_NEW                                                      =     0;

	int                     scanned_vsh                                                      	=     0;		// keep track of the loaded files from sd or usb!
		
	int                     scanned_fsh                                                      	=     0;
	int                     scanned_tex                                                      	=     0;
	int                     scanned_vid                                                      	=     0;

	int                     gl_current_prg                                                      =     0; 		// GLOBAL INDEX FOR THE SHADER PROGRAM IN USE
	int                     last_gl_current_prg 												= 	 -1;  		// GLOBAL INDEX FOR THE LAST SHADER PROGRAM IN USE - needed to Store last active shader program parameters!

	int                     current_buffer 														= 	  0;		// CENTRAL GLOBAL VARIABLE


	int                     gl_current_tex                                                      =     0;
	int                     linked_programs                                                     =     0;		// used for display to display the number 
	int                     audio_source_channel                                                =    -1;      	// Tracks which channel is audio source (-1 if none) OBSOLETE!?

	int                     active_bpm_channel                                                  =     0;		// AS LONG WE HAVE TAB AND CLOCK IN BPM RIGHT?
	int                     g_menu_mode_new                                                     =     0;        // GLOBAL INDEX FOR THE MENU LAYER -WE WILL EXPAND HERE FOR SYSTEM SETINGS ETC - Current menu mode (0, 1, or 2)
	int                     g_menu_mode_old                                                     =     0;
	int                     g_current_texture                                                   =     0;
	int                     attenuation                                                         =     2;		// nifty little mechanism - what shall we do about you?!?

	int                     multiplier[7]                         								= {   64, 32, 16, 8, 4, 2, 1 };

	int                   	threshold_high[ADC_CHANNELS]              							= {   320,  320,  320,  320,  320,  320,  320,  320 };                    // Array of upper threshold values per channel - i guess this should become a menu layer where i can set them up
	int                   	threshold_low[ADC_CHANNELS]              							= {   128,  128,  128,  128,  128,  128,  128,  128 };                     // Array of lower threshold values per channel	

	int 					sampleIndex[LFO_INSTANCES];

	int 					LFO_mode[LFO_PARAMETERS] 											= { 0,3,3,3 };

	int 					sensitivity_new 													= 0;
	int 					sensitivity_old 													= 0;	

	int 					mode_storage_buffers[16][33] 										= {0};               // array to store the modes and specials for all loaded fshaders?
	int 					shader_has_stored_params[33] 										= { 0 };
	int 					audio_sample[2] 													= { 0,0};

	int 					m_validTextureCount=0;

    int                   	g_input_mode[ADC_CHANNELS]              							= { 0 };             // Array storing operation mode for each channel - ITS A RELICT, RIGHT? NOT IN USE ANYWHERE

	int                   	menu_pickup_buffer[ADC_CHANNELS+ADC_CHANNELS]              			= { 0 };               // Array storing previous pot values for pickup detection	

	unsigned long           last_swap_duration                                                  =     0;

	unsigned long           start_time_fps_calculation                                          =     0;				// shall tis be system main time??
	unsigned long           end_time_fps_calculation                                            =     0;
	unsigned long           total_time_fps_calculation                                          =     0;

	unsigned long           currentTime = 0;																			// shall tis be system main time??
	unsigned long           targetTime = 0;
	unsigned long           delayTime = 0;

	unsigned long           last_circle_buffer[LFO_INSTANCES]             = {   0    };
	unsigned long           next_circle_buffer[LFO_INSTANCES]             = {   0    };

	unsigned long         	current_time_for_bpm                                                  = 0;                      // Array storing timing information per channel

	unsigned long         	next_beat_time[2]                         = { 0 };                     // Next beat timestamp for BPM mode
	unsigned long         	buffer_interval[2]                         = { 0 };
	unsigned long         	last_valid_bpm_calculation[2]                         = { 0 };
	unsigned long         	last_valid_bpm_buffer[2]                         = { 0 };
	unsigned long         	resultBPM[2]                         = { 0 };
	unsigned long         	intervalBuffer[2][3]                      = { 0 };	// used for the BPM calculation

	unsigned char          	led_col_new[20][3]                     = { 0 }; // is now a single cpp file


	unsigned long     		loaded_bytes_kernel[2] = { 0 };	// little confusing aye? 

	unsigned long 			elapsedMicroseconds[LFO_INSTANCES];	
	unsigned long 			cycleLength[LFO_INSTANCES];

// for the log files of my classes
        const   char                    *VID__LOG_NAMES                          [VID_SD+VID_USB+4]                   = { "VIDLOG00.TXT",   "VIDLOG01.TXT",  "VIDLOG02.TXT",   "VIDLOG03.TXT", "VIDLOG04.TXT",   "VIDLOG05.TXT",  "VIDLOG06.TXT",   "VIDLOG07.TXT",  "VIDLOG08.TXT",   "VIDLOG09.TXT",   "VIDLOG10.TXT",   "VIDLOG11.TXT",   "VIDLOG12.TXT",   "VIDLOG13.TXT"    };
        const   char                    *BMP__LOG_NAMES                          [TEX_SD+TEX_USB+4]                   = { "BMPLOG00.TXT",   "BMPLOG01.TXT",  "BMPLOG02.TXT",   "BMPLOG03.TXT", "BMPLOG04.TXT",   "BMPLOG05.TXT",  "BMPLOG06.TXT",   "BMPLOG07.TXT",  "BMPLOG08.TXT",   "BMPLOG09.TXT",   "BMPLOG10.TXT",   "BMPLOG11.TXT",   "BMPLOG12.TXT",   "BMPLOG14.TXT"    };
// list of extensions used in my scanroot directory function
        const   char                    *vhsExtensions[VSH_VALID_SUFFIX_COUNT]={ "vsh" }; 
        // we dont need this for the overlay shader because wie dont scan for it        
        const   char                    *fhsExtensions[FSH_VALID_SUFFIX_COUNT]={ "fsh" };
        // also not needed for the overlay texture
        const   char                    *texExtensions[TEX_VALID_SUFFIX_COUNT]={ "bmp" };
        const   char                    *vidExtensions[VID_VALID_SUFFIX_COUNT]={ "mp4"/*, "264", "mov"*/ };

// array to store the scanned filenames?
                char                   *SCANED_FILES_VSH                         [40]                       = {   0    };
        // we dont need this for the overlay shader because wie dont scan for it  	char	*SCANED_FILES_OMS[1] = {"overlay.fsh"};               
                char                   *SCANED_FILES_FSH                         [40]                       = {   0    };
        // we dont need this for the overlay shader because wie dont scan for it 	char	*SCANED_FILES_OMT[1] = {"overlay.bmp"};                 
                char                   *SCANED_FILES_TEX                         [40]                       = {   0    };
                char                   *SCANED_FILES_VID                         [40]                       = {   0    };
// array to store the length of the loased files
                unsigned                VSH_LOADED_BYTES                        [VSH_SD + VSH_USB]          = {   0    };
                unsigned                OMS_LOADED_BYTES                        [1 ]                        = {   0    };                
                unsigned                FSH_LOADED_BYTES                        [FSH_SD + FSH_USB]          = {   0    };
                unsigned                OMT_LOADED_BYTES                        [1 ]                        = {   0    };                
                unsigned                TEX_LOADED_BYTES                        [TEX_SD + TEX_USB+4]        = {   0    }; // <- why again +4?!? artefact?!
                unsigned                VID_LOADED_BYTES                        [VID_SD + VID_USB]          = {   0    }; 	

                float                   LFO_float_output                        [LFO_INSTANCES]             = {   0    };
                int                     LFO_int_output                          [LFO_INSTANCES]             = {   0    };
                int                     last_multiplier                         [LFO_INSTANCES]             = {   0    };

                int                   	adc_raw_value                             [ADC_CHANNELS]              = { 0 };
                int                   	adc_int_value                             [ADC_CHANNELS]              = { 0 };
                float                 	adc_float_value                           [ADC_CHANNELS]              = { 0 };
                int                   	output_int_value                          [ADC_CHANNELS]              = { 0 };
                float                 	output_float_value                        [ADC_CHANNELS]              = { 0 };                    // Array of output values per channel
                int                   	random_int_value                          [ADC_CHANNELS]              = { 0 };
                float                 	random_float_value                        [ADC_CHANNELS]              = { 0 };                    // Array of u_seed_a values per channel

extern  const   unsigned char           rgb_tab                                 [49][3]; 

extern          unsigned long           waveTable                               [WAVEFORMS][WAVESAMPLES];
};

#endif