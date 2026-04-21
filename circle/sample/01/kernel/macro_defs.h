//----------------------------------------------------------------------------------------------------------------------------------------------------
//  MACROS
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define         EMPTYSTR ""
#define         EMPTYLOG 255

#define			TIMEOUT					10
#define 		LOGLEVEL				4	// 0: only panic 1: also errors 2: also warnings 3: also notices 4: also debug output (default))

#define 		FILENAME_KNL			"kernel.img"

#define         PARTITIONSD	            "emmc1-1"	// partition sd
#define         PARTITIONUSB	        "umsd1-1"	// partition usb

#define 		FILENAME_GL_LOG			"LOG-GLES.txt"
#define 		FILENAME_GLERROR		"LOG-GLER.txt"
#define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
#define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"

#define         ADC_CHANNELS         	8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
#define         VREF			 		5.0f	                            // Reference voltage 5 Volt     ( dependency <sensor/mcp300x.h>	)

#define 		ADC_BUFFER			 	4	// number of buffer cells for smoothing

#define         CHANNEL			    	7		                            // im confused - is it needed to init the mcp3008?

#define         SPI_MASTER_DEVICE	 	0		                            // 0    m_SPIMaster ( also for the mcp3008 )
#define         SPI_CHIP_SELECT		 	0		                            // 0
#define         SPI_CLOCK_SPEED	   		1000000		                        // Hz

#define         CS_PIN                 	12                                  // for menu input selector!!!

#define         NR_BUTTONS              2                                   // was addon/sensors/buttons.h is now buttonPing()  no class required 
#define         SW_PIN_A	           	13                                  // button A ( lower )
#define         SW_PIN_B	          	6                                   // button B ( upper )

#define         LED_PIN		           	19		                            // led ( must be between 8 .. 23 )
#define         LED_COUNT		       	4		                            // number of LEDs on the stripes

#define         GPIO_TO_SD_LINE(pin)	((pin) - 8)							// dont touch!
#define         SD_LINE1		      	GPIO_TO_SD_LINE (LED_PIN)			// dont touch!
#define         SD_LINE_TO_MASK(line)	(1 << (line))						// dont touch!
#define         SD_LINES_MASK		 	(  SD_LINE_TO_MASK (SD_LINE1) )		// dont touch! all needed for <WS28XX/ws2812oversmi.h>

#define 		CHUNK_SIZE				1024

#define         SLOTS                   34 // for the g_centralModeBuffer[SLOTS][modetablecount] array - 1 firmware / 32 user / 1 default slot
#define         DEFAULT_SLOT            34 // or 0 ?? 

#define         MAX_TEXTURE_SIZE        (1024*1024*4) // same as TEX_SIZ !!!

#define         MAX_VIDEO_WIDTH         640
#define         MAX_VIDEO_HEIGHT        480
#define         MAX_VIDEO_PROFILE        66		// Baseline
#define         MAX_VIDEO_LEVEL

#define         TARGET_FPS              24      // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		// min bpm for adc selector 
#define			MAX_BPM					240 	// max bpm for adc selector

#define 		NUMBER_OF_MODES			6       // is now defined in the g_modeMap array

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves
#define 		LFO_Waves			    4
#define 		LFO_INSTANCES			2
#define			LFO_MULTIPLIERS			7

//----------------------------------------------------------------------------------------------------------------------------------------------------
// the array for the loader constance - i think its better than scatter the values / constants everywhere around instead unsigned filecounter[FT_COUNT][FLD_COUNT]
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs. examples:

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

#define         ADC_SELECT_PRG          7
#define         ADC_SELECT_TEX          6
#define         ADC_SELECT_VID          5
#define			ADC_INPUT_CLK			5	// adc p_channel use as clock ! BIGGER THAN 3 !

#define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------