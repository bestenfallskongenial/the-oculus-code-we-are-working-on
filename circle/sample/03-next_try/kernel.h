// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _kernel_h
#define _kernel_h

#include <circle/string.h> 

#include <linux/kernel.h> // here or in the kernel.h? 

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

#include <circle/sched/scheduler.h>                         // multitasking ?!

#include <vc4/vchiq/vchiqdevice.h>
#include "vc4/vchiq/vchiq_arm.h"

#include <interface/vcos/vcos.h>  // For VCHI_INSTANCE_T
#include <vc4/vchi/vchi.h>       // For VCHI_CONNECTION_T

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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#define         LED_PIN		           	19		                            // pin led ( 8 .. 23 )
#define         CS_PIN                 	12                                  // pin for menu input selector
#define         SW_PIN_A	          	6                                   // button A ( upper )
#define         SW_PIN_B	           	13                                  // button B ( lower )
#define         LED_COUNT		       	4		                            // number of LEDs on the stripes

#define         GPIO_TO_SD_LINE(pin)	((pin) - 8)							// dont touch!
#define         SD_LINE1		      	GPIO_TO_SD_LINE (LED_PIN)			// dont touch!
#define         SD_LINE_TO_MASK(line)	(1 << (line))						// dont touch!
#define         SD_LINES_MASK		 	(  SD_LINE_TO_MASK (SD_LINE1) )		// dont touch!

#define         ADC_CHANNEL			    7		                    // 7 ( for mcp3008 ) - is never nowhere used?!
#define         ADC_VOLT_REF		    5.0f	                    // Reference voltage 5 Volt
#define         SPI_MASTER_DEVICE	 	0		                    // 0
#define         SPI_CHIP_SELECT		 	0		                    // 0
#define         SPI_CLOCK_SPEED	   		1000000		                // Hz

#define 		VIDEO_WIDTH				640
#define			VIDEO_HEIGHT			480
#define 		MAX_FRAMES 				2048
#define 		BASELINE_PROFILE		66		// Baseline

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves

#define 		CHUNK_SIZE				1024

#define 		ADC_TOLERANCE 		    8

#define			KNL_FILE_SIZE		    (1024*1024*2) 	// 2mb must be sufficient here ( i confused the kernel size by the factor 10 )
#define         VSH_FILE_SIZE           (1024*32) 		// 32768 size of u_vertex shader files
#define         FSH_FILE_SIZE           (1024*32) 		// 32768 size of fragment shader files
#define         TEX_FILE_SIZE           (1024*1024*4)  	// 4194304	// size of texture ( .bpm ) files
#define         VID_FILE_SIZE           (1024*1024*8) 	// 16777216// size of video ( .264 ) files

#define 		FRAME_SIZE 				(1024*1024) 	// 1MB, which comfortably fits a 640x480x3 frame

#define         PARTITION_NAME_SD	    "emmc1-1"	// partition sd
#define         PARTITION_NAME_USB	    "umsd1-1"	// partition usb

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

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

enum modetable
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
	FRM_MODE,
	TEX_MODE,
	CLK_MODE,
	modetablecount
}

enum io_types
{
    raw = 0,        //  the position the dampened adc values per channels are stored *
     in,            //  either the calculated int or flt value *
    out,            //  here lands the processed ( after mode ) for the glsl uniforms      
    rnd,            //  either the per-channel random int or flt value *
    lf1,            //  either the lfo one int or flt value
    lf2,            //  either the lfo two int or flt value
    au0,            //  the audio band 0 flt value
    au1,            //  the audio band 1 flt value
    au2,            //  the audio band 2 flt value
    au3,            //  the audio band 3 flt value
                    // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
    trL,            //  per channel threshold low !!! dont forget to copy the values in here    128
    trH,            //  per channel threshold high                                              320
    trF,            //  per channel threshold "flag"
    io_type_count
}

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
	oldcount, // ? i mean the function now & the counterm no old-new thing neccesary
	count,
	scanned,
	fileindexcount
}

enum ButtonTSIndex
{
    BTN_STATUS      = 0,
    BTN_PRESS_START = 1,
    BTN_DOUBLE      = 2,
    BTN_LONG        = 3,
    BTN_RELEASE     = 4,
    BTN_SINGLE      = 5,
    BTN_HOLD_TICK   = 6,   // COUNTER: increases while held
    BTN_INDEX_COUNT 
};

enum colorindex
{
	M_1 = 0,		// menu a
	M_2,			// menu b
	M_L,			// menu flo
	A25,			// attenuator 2.5v
	A33,			// attenuator 3.3v
	A50,  			// attenuator 5.0v
	TRG,			// trigger mode
	BPM,			// bmp mode
	LF1,			// lfo mode 1
	LF2,			// lfo mode 2
	CLK,			// clock mode? do we have one ?!
	A_0,		
	A_1,
	A_2,
	A_3,
	colorindexcount // to define the array size, must be last
};

// list of extensions used in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[SUFFIX_VSH]			= { "vsh" }; 
        const   char                   *g_SufOmf[SUFFIX_FSH]			= { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[SUFFIX_FSH]			= { "fsh" };
        const   char                   *g_SufOmt[SUFFIX_TEX]			= { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[SUFFIX_TEX]			= { "bmp" };
        const   char                   *g_SufVid[SUFFIX_VID]			= { "264" }; // i guess i will remove the whole parse code for anything but h264

// array to store the scanned filenames?
                char                   *g_ScnVsh[VSH_FILES_ON_SD + VSH_FILES_ON_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_FILES_ON_SD + OMF_FILES_ON_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_FILES_ON_SD + FSH_FILES_ON_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_FILES_ON_SD + OMT_FILES_ON_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_FILES_ON_SD + TEX_FILES_ON_USB]     	= { 0 };
                char                   *g_ScnVid[VID_FILES_ON_SD + VID_FILES_ON_USB]     	= { 0 };
// array to store the length of the loased files
                unsigned                g_bytVsh[VSH_FILES_ON_SD + VSH_FILES_ON_USB]          = { 0 };
                unsigned                g_bytOmf[OMF_FILES_ON_SD + OMF_FILES_ON_USB]          = { 0 };
                unsigned                g_bytFsh[FSH_FILES_ON_SD + FSH_FILES_ON_USB]          = { 0 };
                unsigned                g_bytOmt[OMT_FILES_ON_SD + OMT_FILES_ON_USB]          = { 0 };
                unsigned                g_bytTex[TEX_FILES_ON_SD + TEX_FILES_ON_USB]        	= { 0 };
                unsigned                g_bytVid[VID_FILES_ON_SD + VID_FILES_ON_USB]          = { 0 };

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // _kernel_h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------