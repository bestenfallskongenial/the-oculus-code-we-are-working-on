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

	LF1_WAVE,   // 0 to 3
	LF2_WAVE,   // 0 to 3
	LF1_MULT,   // 0 to 6
	LF2_MULT,   // 0 to 6

	SENS_A,     // 0 to 63
	SENS_B,     // 0 to 63
	SENS_C,     // 0 to 63
	SENS_D,     // 0 to 63

	FRM_MODE,   // 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	TEX_MODE,   // 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	CLK_MODE,   // 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	VID_MODE,   // 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )

    STORE_SET,  // 0 or 1   ( if set to 1 and the MENU_LAYER_COUNT is zero again ( button released ?) the file operation starts )
    LOAD_SET,   // 0 or 1
    STORE_LOG,  // 0 or 1
    LOAD_KLN,   // 0 or 1

	IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

    MODETABLE_COUNT
};
//------------------------------------------------- // for the array unsigned/float g_inOutMatrix*[CHANNEL][IO_TYPE_COUNT]
//	INT	    ADC_RAW     (SCALED IN)VAL 	(SCALED)OUT 	RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
//	ch0	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch1	    U	        U	       	 	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch2	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch3	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch4	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch5	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch6	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
//	ch7	    U	        U	        	U           	U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U

//	FLT	    ADC_RAW     (SCALED IN)VAL  (SCALED)OUT 	RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
//	ch0	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch1	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch2	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch3	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch4	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch5	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch6	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//	ch7	    opt         U	        	U           	U	        G	        G	        G	        G	        G	        G           opt        opt   
//----------------------------------------------------------------------------------------------------------------------------------------------------
enum io_types                               
{
    RAW = 0,                                    		//  the position the dampened adc values per channels are stored *
    VAL,                                        		//  either the calculated/normalized int or flt value *
    OUT,                                        		//  here lands the processed ( after mode ) for the glsl uniforms      
    RND,                                        		//  either the per-p_channel random int or flt value *
    LF1,                                        		//  either the lfo one int or flt value
    LF2,                                        		//  either the lfo two int or flt value
    AU0,                                        		//  the audio band 0 flt value
    AU1,                                        		//  the audio band 1 flt value
    AU2,                                        		//  the audio band 2 flt value
    AU3,                                        		//  the audio band 3 flt value
    TRL,                                        		//  per p_channel threshold low !!! dont forget to copy the values in here    128
    TRH,                                        		//  per p_channel threshold high                                              320
    TRF,                                        		//  per p_channel threshold "flag" - where do i need you again?

    IO_TYPE_COUNT 										// 13	*means i have a unique value for each p_channel - the other values are singular, and/or only int/flt
};
//------------------------------------------------- // for the array unsigned (long?) g_lfoBpmMatrix[4][LFO_BPM_COUNT]; <- no float needed here - its stores in the io matrix
// Layer / Instance | BPM        | INTV       | NBT        | LCB        | NCB        | LBC        | LBCT       | LMT        | ELP        | CYL        | SMP        | LTIME      | TIDX       | TB            | DB             |
// ---------------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ------------- | -------------- |
// `[0]`            | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | timeBuffer[0] | deltaBuffer[0] |
// `[1]`            | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | timeBuffer[1] | deltaBuffer[1] |
// `[2]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[2] | deltaBuffer[2] |
// `[3]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[3] | unused         |
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum lfo_bpm_types
{
    BPM =0,         									// result BPM
    INTV,           									// intervalCalculated
    NBT,            									// nextBeatTime
    LCB,            									// lastCircleBuffer
   	NCB,            									// nextCircleBuffer
    LBC,            									// lastBpmCalculation
    LBCT,           									// lastBpmCalculationTMP
    LMT,            									// lfoMultiplierTMP
    ELP,            									// elapsedMicroseconds
    CYL,            									// cycleLength
    SMP,            									// sampleIndex
    LTIME,          									// lastTime
    TIDX,           									// timeIndex
    TB,             									// timeBuffer
    DB,             									// deltaBuffer

    LFO_BPM_COUNT										// 15
};
//------------------------------------------------- // g_modeLengthAdd[MODELEN_FLAG_COUNT] 
enum ModeLengthFlag
{
                MODELENDEFAULT  = 0,
                MODELEN_AUDIO_A,            				// any extra mode that modifies the number of max modes per channel must have an enum
                MODELEN_AUDIO_B,
                MODELEN_AUDIO_C,
                MODELEN_LFO_A,
                MODELEN_SENS_A,

                MODELEN_FLAG_COUNT
};
//------------------------------------------------- // unsigned int g_buttons_states[NR_BUTTONS][5] = {0} !!!
enum ButtonTSIndex  
{
    BTN_PRESS_START = 0,
    BTN_DOUBLE      = 1,
    BTN_RELEASE     = 2,
    BTN_SINGLE      = 3,
    BTN_HOLD_TICK   = 4,                            		// COUNTER: increases while held ( runtime loop iterations - unstable but sufficient - needs measurement )
    BTN_INDEX_COUNT = 5
};
//------------------------------------------------- // for unsigned filecounter[FT_COUNT][FLD_COUNT]
enum FileType
{
    FT_VSH = 0,                                     		// vertex shader 
    FT_OMF,                                         		// overlay fragment shader 
    FT_FSH,                                         		// user fragment shader
    FT_OMT,                                         		// overlay texture
    FT_TEX,                                         		// user texture
    FT_VID,                                         		// user video 
    FT_KLN,                                         		// kernel
    FRM_BF,                                         		// output-frames A & B
    LOGGER,                                         		// log-buffer
    FT_COUNT
};
enum FileField
{
    FLD_MAXSD = 0,                                  		// number of maximal files on sd
    FLD_MAXUSB,                                     		// number of maximal files on usb
    FLD_EXTCNT,                                     		// number of possible file extensions const char *g_Suf*[]
    FLD_SCANNED,                                    		// number of files scanned via scanRoot(...)  
    FLD_LOADED,                                     		// number of actual loaded files after bulkLoad(...)
    FLD_PREV,                                       		// number of actual loaded files before bulkLoad(...)
    FLD_VALID,                                      		// number of valid files after gfx_init / parsing
    FLD_SIZE,                                       		// maximal size of the files / buffer-size
    FLD_COUNT
};