    // to prepare filecounter[FT_COUNT][FLD_COUNT]
    #define         VSH_SD             		    1	// max number of u_vertex shader on sd
    #define         OMF_SD             		    1	// max number of fragment shader on sd
    #define         FSH_SD             		    1	// max number of fragment shader on sd
    #define         OMT_SD             		    1	// max number of fragment shader on sd
    #define         TEX_SD             		    0	// max number of textures on sd
    #define         VID_SD             		    0	// max number of videos on sd
    #define         KLN_SD                      1

    #define         FRM_SD                      1   // i put them here because if my mem/dma allocation
    #define         LOG_SD                      24  // here is the trick:
                                                // i will use a mix of hardwired and "open" logs. examples:
                                                // just changed the model here. got one global log // screen buffer defined as member to access it on boot ( before alloc )
                                                // than a common log buffer for the file operations, mem-alloc, bmp parser, the annex b parser, the vcsm init, the mmal init, the mmal runtime 

                                                // m_bufferLog[0]       - startup - memory - init log
                                                // m_bufferLog[1]       - vc_sm
                                                // m_bufferLog[2]       - mmal
                                                // m_bufferLog[3]       - check glsl
                                                // m_bufferLog[4]       - glsl compile
                                                // m_bufferLog[5  - 15] - texture parser
                                                // m_bufferLog[16 - 23] - video parser

    #define         VSH_USB                     0	// max number of u_vertex shader on sd
    #define         OMF_USB            		    0	// max number of fragment shader on sd
    #define         FSH_USB            		    32	// max number of fragment shader on sd
    #define         OMT_USB            		    0	// max number of fragment shader on sd
    #define         TEX_USB            		    8	// max number of textures on sd
    #define         VID_USB            		    8	// max number of videos on sd
    #define         KLN_USB                     1

    #define         FRM_USB                     1   // i put them here because if my mem/dma allocation
    #define         LOG_USB                     1

    #define         VSH_EXT                     1
    #define         OMF_EXT                     1
    #define         FSH_EXT                     1
    #define         OMT_EXT                     1
    #define         TEX_EXT                     1
    #define         VID_EXT                     1
    #define         KLN_EXT                     1

    #define         VSH_SIZ                     (1024*32)
    #define         OMF_SIZ                     (1024*32)
    #define         FSH_SIZ                     (1024*32)
    #define         OMT_SIZ                     (1024*1024*4)
    #define         TEX_SIZ                     (1024*1024*4)
    #define         VID_SIZ                     (1024*1024*8)
    #define         KLN_SIZ                     (1024*1024*2)

    #define         FRM_SIZ                     (1024*1024)

    #define         LOG_SIZ                     (1024*64)

    #define         ADC_SELECT_PRG              7
    #define         ADC_SELECT_TEX              6
    #define         ADC_SELECT_VID              5
    #define			ADC_INPUT_CLK			    5	// adc p_channel use as clock ! BIGGER THAN 3 !