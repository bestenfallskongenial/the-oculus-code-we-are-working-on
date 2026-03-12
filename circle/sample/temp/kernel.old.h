

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




#define AUD_MODE 5

#define 		ADC_TOLERANCE 				8


#define			KNL_FILE_SIZE				(1024*1024*2) 	// 2mb must be sufficient here ( i confused the kernel size by the factor 10 )
#define         VSH_FILE_SIZE                (1024*32) 		// 32768 size of u_vertex shader files
#define         FSH_FILE_SIZE                (1024*32) 		// 32768 size of fragment shader files
#define         TEX_FILE_SIZE                (1024*1024*4)  	// 4194304	// size of texture ( .bpm ) files
#define         VID_FILE_SIZE                (1024*1024*8) 	// 16777216// size of video ( .264 ) files

#define 		FRAME_SIZE 				(1024*1024) 	// 1MB, which comfortably fits a 640x480x3 frame

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves

#define         PARTITION_NAME_SD	            "emmc1-1"	// partition sd
#define         PARTITION_NAME_USB	        "umsd1-1"	// partition usb

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

#define         ADC_CHANNEL			    	7		                    // 7 ( for mcp3008 ) - is never nowhere used?!
#define         ADC_VOLT_REF			 		5.0f	                    // Reference voltage 5 Volt
#define         SPI_MASTER_DEVICE	 	0		                    // 0
#define         SPI_CHIP_SELECT		 	0		                    // 0
#define         SPI_CLOCK_SPEED	   		1000000		                // Hz

#define         glslCheck() 				gfx_check(__FILE__, __LINE__) 	// my own assertiion implimentation

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
   	GLuint gl_vsh_id[VSH_FILES_ON_SD+VSH_FILES_ON_USB];
   	GLuint gl_fsh_id[FSH_FILES_ON_SD+FSH_FILES_ON_USB];

   	GLuint gl_prg_id[FSH_FILES_ON_SD+FSH_FILES_ON_USB];

	GLuint gl_tex_id[TEX_FILES_ON_SD+TEX_FILES_ON_USB];
	GLint  u_tex_id[FSH_FILES_ON_SD+FSH_FILES_ON_USB][TEX_FILES_ON_SD+TEX_FILES_ON_USB];

   	GLuint gl_buf; // buffer
	GLuint gl_vtx; // vertex

	GLint u_time[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
	GLint u_tres[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
	GLint u_seed[FSH_FILES_ON_SD+FSH_FILES_ON_USB];	
	GLint u_aud[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
	GLint u_col[FSH_FILES_ON_SD+FSH_FILES_ON_USB];

	GLint u_par_a[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
	GLint u_par_b[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
    GLint u_tex_l[FSH_FILES_ON_SD+FSH_FILES_ON_USB];
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

	bool 			filesystem_open_file		(	const char *p_fileName	);
	unsigned 		filesystem_load_file		(	char *buffer, unsigned bufferSize, int mode);		
	bool 			filesystem_close_file		();
	int 			filesystem_process_files	(  	char* p_fileNameArray[], 
													unsigned p_loadedBytes[], 
													char** p_bufferArray, 
                            						int p_maxFiles, int p_validFiles, 
													unsigned TEX_FILE_SIZE, int mode);
	bool 			filesystem_save_log_file	(	const char* p_deviceName, const char* p_fileName, const CString& p_str_to_save);
	bool 			filesystem_update_USB		(	const char* deviceType);
	bool 			filesystem_mount			(	const char* p_deviceName, 
                              						char* fileNamesVsh[], 
													unsigned vStotalLoadedBytes[], int maxVshaderFiles,
                              						char* fileNamesFsh[], 
													unsigned fStotalLoadedBytes[], int maxFshaderFiles,
                              						char* fileNamesTex[], 
													unsigned tXtotalLoadedBytes[], int maxTextureFiles,
                                            		char* fileNamesVid[]  , 
													unsigned vItotalLoadedBytes[], int maxVideoFiles);
	bool			filesystem_IsValidFileType	(const char* p_fileName, const char* p_fileExtension);
	unsigned 		filesystem_ScanRootDir     	( char** fileArray, const char* p_fileExtension[], int p_extentionCount, unsigned p_maxFiles );
	bool			filesystem_load_kernel		(const char* p_deviceName, const char* p_fileName, unsigned p_fileIndex);
	bool			filesystem_save_kernel		(const char* p_deviceName, const char* p_fileName, unsigned p_fileIndex);
private:
	static void 	filesystem_remove_USB 		(	CDevice *pDevice, void *p_pContext);	
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
	void			util_random_vec8			(uint32_t p_seed); 
	void 			util_calculate_BPM			(	unsigned long p_triggerTimeClockA, unsigned long p_triggerTimeClockB);
	void 			util_determine_bpm_source	();
	void			util_update_predicted_beat	();
  	void 			util_LFO					();
	void 			util_audio_energy			(	float p_adcvalue	);		

	float			util_get_adc_bpm			(	int adcValue	);
	void			util_calculate_filesize		();
	bool 			util_check_for_update		();
                 
	void 			util_save_modes_file		();

	void   			gfx_check					(	const char *file, unsigned line);		// what do i need to refactor here? and what needs a string in the caller?
	void			gfx_shader_log				(	GLint shader, int shaderIndex);			// what can i replace with my other logger?
	void 			gfx_program_log				(	GLint shader, int program_index) ;				// is to change / remove

	void 			gfx_init_OGL				(	CUBE_STATE_T *state);  		// ****
	void 			gfx_init_vshaders			(	CUBE_STATE_T *state, int p_fromFile, int p_toFile);	// ****

	void 			gfx_init_overlay_fshader    (   CUBE_STATE_T *state ); // NEW OVERLAY MENU

	void 			gfx_init_fshaders			(	CUBE_STATE_T *state, int p_fromFile, int p_toFile);	// ****
	
	void			gfx_init_overlay_program    (   CUBE_STATE_T *state);  // NEW OVERLAY MENU

	void 			gfx_init_programs			(	CUBE_STATE_T *state, int p_fromFile, int p_toFile);	// ****

	void			gfx_init_overlay_uniforms   (   CUBE_STATE_T *state);  // NEW OVERLAY MENU

	void 			gfx_init_uniforms			(	CUBE_STATE_T *state, int p_fromFile, int p_toFile);	// ****

	void			gfx_init_overlay_texture    (   CUBE_STATE_T *state);  // NEW OVERLAY MENU  

	void 			gfx_init_textures			(	CUBE_STATE_T *state, int p_fromFile, int p_toFile);	// possible change 
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

	void 			parser_h264					(int p_fromFile, int p_toFile);
	void			parser_bmp					(int p_fromFile, int p_toFile);
	void 			GenerateH264ParserInfo		(	int     p_fileIndex );
	void 			GenerateBmpParserInfo		(	int     p_fileIndex );

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
	char** 				m_bufferVid;
	char* 				m_videoBlockBase;
	char* 				m_videoRawBlock;
    size_t 				m_videoBlockSize;

	char**				m_bufferFrA;
	char* 				m_frameBlockBaseA;
	char* 				m_frameRawBlockA;
    size_t 				m_frameBlockSizeA;

	char**				m_bufferFrB;
	char* 				m_frameBlockBaseB;
	char* 				m_frameRawBlockB;
    size_t 				m_frameBlockSizeB;	

    char** 				m_BufferOmt;
	char* 				m_overlyBlockBase;
	char* 				m_overlayRawBlock;
    size_t 				m_overlyBlockSize;	

    char** 				m_bufferTex;
	char* 				m_textureBlockBase;
	char* 				m_textureRawBlock;
    size_t 				m_textureBlockSize;
// buffers
	char**				m_bufferKnl;

	char** 				m_bufferVsh;

	char**				m_bufferOmf;

    char** 				m_bufferFsh;


                CString                 g_log_string;
                CString                 g_error_string;
                CString                 g_modes;

                unsigned                g_hFile                                                               =     0;  // ?? empty ??

                float                   current_fps                                                         =     0.0; // for our display / menu for the fps break mechanism                
                
                float                   /* u_audioSmoothBand */                  [4]                         =  {0.0f};		// buffer to smooth out the audio detection 

                float                   GLtime                                                              =  0.0f;
                float                   g_opaque                                                              =  1.0f;	// we dont need this anymore later on

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
	bool                  	m_shaderStatusFlags[FSH_FILES_ON_SD+FSH_FILES_ON_USB]; // we need this here to ensure that we not try to link/compile damaged glsl code, right? 

	bool                  	menu_pickup_flag[ADC_CHANNELS+ADC_CHANNELS]              			= { 0 };                 // Array tracking if pot value has been "picked up"
enum filecount 
{
	vsm = 0,
	omf,
	fsh,
	omt,
	tex,
	vid,
	filetypescount
}
enum fileindex
{
	maxSD = 0,
	maxUSB,
	scanned,
	scanned,
	count,

	fileindexcount
}

int filecounter[fileindexcount][filetypescount] = { {0} };	// 2 for sd and usb, filetypescount for the different file types ( vsh, omf, fsh, omt, tex, vid ) 
// 															// [0] is g_loaded_*_old and [1] is g_loaded_*_new, i love to have also g_scanned_vsh here
	int                     g_current_gl_program                                                  =     0; 		// GLOBAL INDEX FOR THE SHADER PROGRAM IN USE
	int                     g_last_gl_program 													  =    -1;  		// GLOBAL INDEX FOR THE LAST SHADER PROGRAM IN USE - needed to Store last active shader program parameters!

	int                     g_currentProgramBuffer 												  = 	0;		// CENTRAL GLOBAL VARIABLE

	int                     gl_current_tex                                                        =     0;
	int                     g_linked_programs_counter                                             =     0;		// used for display to display the number 
	int                     g_audio_source_channel                                                =    -1;      	// Tracks which channel is audio source (-1 if none) OBSOLETE!?

	int                     g_activeBpmChannel                                                    =     0;		// AS LONG WE HAVE TAB AND CLOCK IN BPM RIGHT?
	int                     g_menu_mode_new                                                       =     0;        // GLOBAL INDEX FOR THE MENU LAYER -WE WILL EXPAND HERE FOR SYSTEM SETINGS ETC - Current menu mode (0, 1, or 2)
	int                     g_menu_mode_old                                                       =     0;
	int                     g_current_texture                                                     =     0;
	int                     g_attenuation                                                         =     2;		// nifty little mechanism - what shall we do about you?!?

	int                     g_lfoMultiplier[7]                         							  = {   64, 32, 16, 8, 4, 2, 1 };

	int 					g_sampleIndex[LFO_INSTANCES];

	int 					LFO_mode[LFO_PARAMETERS] 											  = { 0,3,3,3 };

	int 					g_sensitivityNew 													 = 0;
	int 					g_sensitivityOld 													 = 0;	

	int 					g_centralModeBuffer[16][33] 										 = {0};               // array to store the modes and specials for all loaded fshaders?
	int 					shader_has_stored_params[33] 										 = { 0 };
	int 					audio_sample[2] 													 = { 0,0};

	int 					g_validTextureCount													 =	0;

	int                   	g_menu_pickup_buffer[ADC_CHANNELS+ADC_CHANNELS]              		 = { 0 };               // Array storing previous pot values for pickup detection	

	unsigned long           last_swap_duration                                                   =     0;

	unsigned long           start_time_fps_calculation                                          =     0;				// shall tis be system main time??
	unsigned long           end_time_fps_calculation                                            =     0;
	unsigned long           total_time_fps_calculation                                          =     0;

	unsigned long           currentTime = 0;																			// shall tis be system main time??
	unsigned long           targetTime = 0;
	unsigned long           delayTime = 0;

	unsigned long           g_lastCircleBuffer[LFO_INSTANCES]             = {   0    };
	unsigned long           g_nextCircleBuffer[LFO_INSTANCES]             = {   0    };
	unsigned long 			g_elapsedMicroseconds[LFO_INSTANCES];	
	unsigned long 			g_cycleLength[LFO_INSTANCES];

	unsigned long         	current_time_for_bpm                                                  = 0;                      // Array storing timing information per channel

	unsigned long         	g_nextBeatTime[2]                         = { 0 };                     // Next beat timestamp for BPM mode
	unsigned long         	g_intervalCalculated[2]                         = { 0 };
	unsigned long         	g_lastBpmCalculation[2]                         = { 0 };
	unsigned long         	g_lastValidCalculationBuffer[2]                         = { 0 };
	unsigned long         	g_resultBPM[2]                         = { 0 };
	unsigned long         	g_intervalBuffer[2][3]                      = { 0 };	// used for the BPM calculation

	unsigned char          	led_col_new[20][3]                     = { 0 }; // is now a single cpp file


	unsigned long     		loaded_bytes_kernel[2] = { 0 };	// little confusing aye? 

// for the log files of my classes
        const   char                    *g_texLogNames                          [TEX_FILES_ON_SD+TEX_FILES_ON_USB]                   = { "BMPLOG00.TXT",   "BMPLOG01.TXT",  "BMPLOG02.TXT",   "BMPLOG03.TXT", "BMPLOG04.TXT",   "BMPLOG05.TXT",  "BMPLOG06.TXT",   "BMPLOG07.TXT",  "BMPLOG08.TXT",   "BMPLOG09.TXT" };
        const   char                    *g_vidLogNames                          [VID_FILES_ON_SD+VID_FILES_ON_USB]                   = { "VIDLOG00.TXT",   "VIDLOG01.TXT",  "VIDLOG02.TXT",   "VIDLOG03.TXT", "VIDLOG04.TXT",   "VIDLOG05.TXT",  "VIDLOG06.TXT",   "VIDLOG07.TXT",  "VIDLOG08.TXT",   "VIDLOG09.TXT" };


                int                     g_lfoMltTmp[LFO_INSTANCES]             = {   0    };


extern  const   unsigned char           g_rgb_color_table                                 [49][3]; 

extern          unsigned long           g_waveTable                               [WAVEFORMS][WAVESAMPLES];
};

#endif