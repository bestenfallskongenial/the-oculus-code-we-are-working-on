enum modetable		                                
{
	int CH0_MODE = 0,
	int CH1_MODE,
	int CH2_MODE,
	int CH3_MODE,

	int CH4_MODE,
	int CH5_MODE,
	int CH6_MODE,
	int CH7_MODE,

	int LF1_WAVE,   // 0 to 3
	int LF2_WAVE,   // 0 to 3
	int LF1_MULT,   // 0 to 6
	int LF2_MULT,   // 0 to 6

	int SENS_A,     // 0 to 63
	int SENS_B,     // 0 to 63
	int SENS_C,     // 0 to 63
	int SENS_D,     // 0 to 63

	int FRM_MODE,   // 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	int TEX_MODE,   // 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	int CLK_MODE,   // 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	int VID_MODE,   // 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )

    int STORE_SET,  // 0 or 1   ( if set to 1 and the menu_layer is zero again ( button released ?) the file operation starts )
    int LOAD_SET,   // 0 or 1
    int STORE_LOG,  // 0 or 1
    int LOAD_KLN,   // 0 or 1

	int IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

	modetablecount = 21 + 4                 // right?
}
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
    int RAW = 0,                                    		//  the position the dampened adc values per channels are stored *
    int VAL,                                        		//  either the calculated/normalized int or flt value *
    int OUT,                                        		//  here lands the processed ( after mode ) for the glsl uniforms      
    int RND,                                        		//  either the per-p_channel random int or flt value *
    int LF1,                                        		//  either the lfo one int or flt value
    int LF2,                                        		//  either the lfo two int or flt value
    int AU0,                                        		//  the audio band 0 flt value
    int AU1,                                        		//  the audio band 1 flt value
    int AU2,                                        		//  the audio band 2 flt value
    int AU3,                                        		//  the audio band 3 flt value
    int TRL,                                        		//  per p_channel threshold low !!! dont forget to copy the values in here    128
    int TRH,                                        		//  per p_channel threshold high                                              320
    int TRF,                                        		//  per p_channel threshold "flag" - where do i need you again?

    int IO_TYPE_COUNT 										// 13	*means i have a unique value for each p_channel - the other values are singular, and/or only int/flt
}
//------------------------------------------------- // for the array unsigned (long?) g_lfoBpmMatrix[4][LFO_BPM_COUNT]; <- no float needed here - its stores in the io matrix
// Layer / Instance | BPM        | INTV       | NBT        | LCB        | NCB        | LBC        | LBCT       | LMT        | ELP        | CYL        | SMP        | LTIME      | TIDX       | TB            | DB             |
// ---------------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ------------- | -------------- |
// `[0]`            | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | instance 0 | timeBuffer[0] | deltaBuffer[0] |
// `[1]`            | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | instance 1 | timeBuffer[1] | deltaBuffer[1] |
// `[2]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[2] | deltaBuffer[2] |
// `[3]`            | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | unused     | timeBuffer[3] | unused         |
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

