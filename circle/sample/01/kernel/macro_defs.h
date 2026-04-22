//----------------------------------------------------------------------------------------------------------------------------------------------------
//  MACROS for CKernel
TEX_SIZ
#define         EMPTYSTR ""                                                 // for the logger
#define         EMPTYLOG 255                                                // for the logger

#define			TIMEOUT					10                                  // for my watchdog
#define 		LOGLEVEL				4	                                // 0: only panic 
                                                                            // 1: also errors 
                                                                            // 2: also warnings 
                                                                            // 3: also notices 
                                                                            // 4: also debug output (default))

#define 		FILENAME_KNL			"kernel.img"                         // for logger

#define         PARTITIONSD	            "emmc1-1"	                         // name partition sd dont confuse with device type    
#define         PARTITIONUSB	        "umsd1-1"	                         // name partition usb dont confuse with device type

#define 		FILENAME_GL_LOG			"LOG-GLES.txt"                       // names for my log files 
#define 		FILENAME_GLERROR		"LOG-GLER.txt"
#define 		FILENAME_VCSM_LOG		"LOG-VCSM.txt"
#define 		FILENAME_MMAL_LOG		"LOG-MMAL.txt"

#define         ADC_CHANNELS         	8                                   // number of adc input channels ( dependency <sensor/mcp300x.h>	)
#define         VREF			 		5.0f	                            // Reference voltage 5 Volt     ( dependency <sensor/mcp300x.h>	)

#define 		ADC_BUFFER			 	4	                                // smoothing-buffer

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

#define 		CHUNK_SIZE				1024                                // for the file load 

#define         SLOTS                   34                                  // for the g_centralModeBuffer[SLOTS][modetablecount] array -  
                                                                            // 1 firmware / 32 user / 1 default slot
#define         DEFAULT_SLOT            34                                  // or 0 ?? 

#define         TEX_SIZ        (1024*1024*4)                                // same as TEX_SIZ !!!

#define         MAX_VIDEO_WIDTH         640                                 // want to a) get this from config.txt b) defoverride this the config.txt 
#define         MAX_VIDEO_HEIGHT        480                                     
#define         MAX_VIDEO_PROFILE        66		                            // Baseline
#define         MAX_VIDEO_LEVEL          41                                 // ? taken from 

#define         TARGET_FPS              24                                  // desired frame rate !!! becomes menu !!!
#define 		MIN_BPM					10		                            // min bpm for adc selector 
#define			MAX_BPM					240 	                            // max bpm for adc selector

#define 		NUMBER_OF_MODES			6       // is now defined in the g_modeMap array

#define         WAVEFORMS             	4		// number of lfo waves
#define         WAVESAMPLES            	256  	// number of samples per lfo waves

#define 		LFO_INSTANCES			2
#define			LFO_MULTIPLIERS			7

// vcsm and mmal 
#define VC_SM_VER  1    // Version to be reported to the VPU VPU assumes 0 (aka 1) which does not require the released callback
#define VC_SM_MIN_VER 0 // nor expect the client to handle VC_MEM_REQUESTS. Version 2 requires the released callback, and must support VC_MEM_REQUESTS.

#define VC_SM_MAX_MSG_LEN (sizeof(union vc_sm_msg_union_t) + \
	sizeof(struct vc_sm_msg_hdr_t))
#define VC_SM_MAX_RSP_LEN (sizeof(union vc_sm_msg_union_t))

#define VC_SM_RESOURCE_NAME 32				// Resource name maximum size //

#define VC_MMAL_VER 15
#define VC_MMAL_MIN_VER 10

#define MMAL_FOURCC(a, b, c, d) ((a) | (b << 8) | (c << 16) | (d << 24)) // two times!
#define MMAL_MAGIC 						MMAL_FOURCC('m', 'm', 'a', 'l')


#define MMAL_EVENT_ERROR				MMAL_FOURCC('E', 'R', 'R', 'O')	// really????
#define MMAL_EVENT_EOS					MMAL_FOURCC('E', 'E', 'O', 'S')
#define MMAL_EVENT_FORMAT_CHANGED		MMAL_FOURCC('E', 'F', 'C', 'H')
#define MMAL_EVENT_PARAMETER_CHANGED	MMAL_FOURCC('E', 'P', 'C', 'H')



// max total message size is 512 bytes //
#define MMAL_MSG_MAX_SIZE 512
// with six 32bit header elements max payload is therefore 488 bytes //
#define MMAL_MSG_MAX_PAYLOAD 488


// #define MMAL_TIME_UNKNOWN BIT_ULL(63)                               // Special value signalling that time is not known //
// #define MMAL_TIME_UNKNOWN (1ULL << 63)

#define BIT(n)          (1U << (n))
#define BIT_ULL(n)      (1ULL << (n))
#define MMAL_TIME_UNKNOWN BIT_ULL(63)
#define NUMBER_INPUTBUFFER 1
#define NUMBER_OUTPUTBUFFER 2


#define MIN_BUFFERS 2                                                 // from CKernel ??
#define MAX_BUFFER 8 // ???

#define VC_SM_PROTOCOL_VERSION	2 // ???
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-VCHIQ.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_PORT_COUNT 4

// Maximum size of the format extradata. //
#define MMAL_FORMAT_EXTRADATA_MAX_SIZE 128
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-VCHIQ.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_PORT_COUNT 4

// Maximum size of the format extradata. //
#define MMAL_FORMAT_EXTRADATA_MAX_SIZE 128
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-PARAMETERS.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define MMAL_PARAMETER_GROUP_COMMON		(0 << 16)
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-MSG-PORT.H what do we really need here?
//----------------------------------------------------------------------------------------------------------------------------------------------------
// The port is pass-through and doesn't need buffer headers allocated //
#define MMAL_PORT_CAPABILITY_PASSTHROUGH                       0x01

 //The port wants to allocate the buffer payloads.
 // This signals a preference that payload allocation should be done
 // on this port for efficiency reasons.

#define MMAL_PORT_CAPABILITY_ALLOCATION                        0x02

 // The port supports format change events.
 // This applies to input ports and is used to let the client know
 // whether the port supports being reconfigured via a format
 // change event (i.e. without having to disable the port).

#define MMAL_PORT_CAPABILITY_SUPPORTS_EVENT_FORMAT_CHANGE      0x04

 // mmal port structure (MMAL_PORT_T)
 //
 // most elements are informational only, the pointer values for
 // interogation messages are generally provided as additional
 // structures within the message. When used to set values only the
 // buffer_num, buffer_size and userdata parameters are writable.
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-MSG.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
// MMAL buffer transfer //

// Size of space reserved in a buffer message for short messages. //
#define MMAL_VC_SHORT_DATA 128
// Signals that the current payload is the end of the stream of data //
#define MMAL_BUFFER_HEADER_FLAG_EOS                    BIT(0)
// Signals that the start of the current payload starts a frame //
#define MMAL_BUFFER_HEADER_FLAG_FRAME_START            BIT(1)
// Signals that the end of the current payload ends a frame //
#define MMAL_BUFFER_HEADER_FLAG_FRAME_END              BIT(2)
// Signals that the current payload contains only complete frames (>1) //
#define MMAL_BUFFER_HEADER_FLAG_FRAME                  \
	(MMAL_BUFFER_HEADER_FLAG_FRAME_START | \
	 MMAL_BUFFER_HEADER_FLAG_FRAME_END)
// Signals that the current payload is a keyframe (i.e. self decodable) //
#define MMAL_BUFFER_HEADER_FLAG_KEYFRAME               BIT(3)
// Signals a discontinuity in the stream of data (e.g. after a seek). Can be used for instance by a decoder to reset its state
#define MMAL_BUFFER_HEADER_FLAG_DISCONTINUITY          BIT(4)
// Signals a buffer containing some kind of config data for the component (e.g. codec config data)
#define MMAL_BUFFER_HEADER_FLAG_CONFIG                 BIT(5)
// Signals an encrypted payload //
#define MMAL_BUFFER_HEADER_FLAG_ENCRYPTED              BIT(6)
// Signals a buffer containing side information //
#define MMAL_BUFFER_HEADER_FLAG_CODECSIDEINFO          BIT(7)
// Signals a buffer which is the snapshot/postview image from a stills capture
#define MMAL_BUFFER_HEADER_FLAGS_SNAPSHOT              BIT(8)
// Signals a buffer which contains data known to be corrupted //
#define MMAL_BUFFER_HEADER_FLAG_CORRUPTED              BIT(9)
// Signals that a buffer failed to be transmitted //
#define MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED    BIT(10)
// Video buffer header flags
// videobufferheaderflags
// The following flags describe properties of a video buffer header.
// As there is no collision with the MMAL_BUFFER_HEADER_FLAGS_ defines, these
// flags will also be present in the MMAL_BUFFER_HEADER_T flags field.
#define MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START_BIT 16
#define MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START \
			(1 << MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START_BIT)
// Signals an interlaced video frame //
#define MMAL_BUFFER_HEADER_VIDEO_FLAG_INTERLACED \
			(MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START << 0)
// Signals that the top field of the current interlaced frame should be
// displayed first
#define MMAL_BUFFER_HEADER_VIDEO_FLAG_TOP_FIELD_FIRST \
			(MMAL_BUFFER_HEADER_FLAG_FORMAT_SPECIFIC_START << 1)
// port parameter setting //

#define MMAL_WORKER_PORT_PARAMETER_SPACE      96        
// event messages //
#define MMAL_WORKER_EVENT_SPACE 256

// Four CC's for events //
#define MMAL_FOURCC(a, b, c, d) ((a) | (b << 8) | (c << 16) | (d << 24))

#define MMAL_EVENT_ERROR		MMAL_FOURCC('E', 'R', 'R', 'O')
#define MMAL_EVENT_EOS			MMAL_FOURCC('E', 'E', 'O', 'S')
#define MMAL_EVENT_FORMAT_CHANGED	MMAL_FOURCC('E', 'F', 'C', 'H')
#define MMAL_EVENT_PARAMETER_CHANGED	MMAL_FOURCC('E', 'P', 'C', 'H')
//----------------------------------------------------------------------------------------------------------------------------------------------------
//              FROM MMAL-ENCODINGS.H
//----------------------------------------------------------------------------------------------------------------------------------------------------
#define MMAL_ENCODING_H264             MMAL_FOURCC('H', '2', '6', '4')

#define MMAL_ENCODING_I420             MMAL_FOURCC('I', '4', '2', '0')

// An EGL image handle
#define MMAL_ENCODING_EGL_IMAGE        MMAL_FOURCC('E', 'G', 'L', 'I')

// Pre-defined H264 encoding variants //

// ISO 14496-10 Annex B byte stream format //
#define MMAL_ENCODING_VARIANT_H264_DEFAULT   0
// ISO 14496-15 AVC stream format //
#define MMAL_ENCODING_VARIANT_H264_AVC1      MMAL_FOURCC('A', 'V', 'C', '1')
// Implicitly delineated NAL units without emulation prevention //
#define MMAL_ENCODING_VARIANT_H264_RAW       MMAL_FOURCC('R', 'A', 'W', ' ')

// \defgroup MmalColorSpace List of pre-defined video color spaces
// This defines a list of common color spaces. This list isn't exhaustive and
// is only provided as a convenience to avoid clients having to use FourCC
// codes directly. However components are allowed to define and use their own
// FourCC codes.

// Unknown color space //
#define MMAL_COLOR_SPACE_UNKNOWN       0
// ITU-R BT.601-5 [SDTV] //
#define MMAL_COLOR_SPACE_ITUR_BT601    MMAL_FOURCC('Y', '6', '0', '1')
// ITU-R BT.709-3 [HDTV] //
#define MMAL_COLOR_SPACE_ITUR_BT709    MMAL_FOURCC('Y', '7', '0', '9')
// JPEG JFIF //
#define MMAL_COLOR_SPACE_JPEG_JFIF     MMAL_FOURCC('Y', 'J', 'F', 'I')
// Title 47 Code of Federal Regulations (2003) 73.682 (a) (20) //
#define MMAL_COLOR_SPACE_FCC           MMAL_FOURCC('Y', 'F', 'C', 'C')
// Society of Motion Picture and Television Engineers 240M (1999) //
#define MMAL_COLOR_SPACE_SMPTE240M     MMAL_FOURCC('Y', '2', '4', '0')
// ITU-R BT.470-2 System M //
#define MMAL_COLOR_SPACE_BT470_2_M     MMAL_FOURCC('Y', '_', '_', 'M')
// ITU-R BT.470-2 System BG //
#define MMAL_COLOR_SPACE_BT470_2_BG    MMAL_FOURCC('Y', '_', 'B', 'G')
// JPEG JFIF, but with 16..255 luma //
#define MMAL_COLOR_SPACE_JFIF_Y16_255  MMAL_FOURCC('Y', 'Y', '1', '6')
// @} MmalColorSpace List //

/----------------------------------------------------------------------------------------------------------------------------------------------------
// to prepare filecounter[FT_COUNT][FLD_COUNT]
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