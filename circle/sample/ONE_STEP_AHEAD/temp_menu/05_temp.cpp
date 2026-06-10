// constants / enums used by set_mode_roof_map()
#define BLOCK_COUNT       7

enum source_modes
{
    IN_MODE_ADC,        // adc direct to gl
    IN_MODE_TRG,        // a adc threshold triggers to store a random volume  to gl
    IN_MODE_BMP,        // a internal / external ( later more ) BPM wll store a random value to gl
    IN_MODE_LF1,        // an internal waketable value calculated from the bpm is send continuously to gl 
    IN_MODE_LF2,        // an internal waketable value calculated from the bpm is send continuously to gl

    MODE_BAND_L0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H0,       // if audio0 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl
    MODE_BAND_L1,       // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged low 0 band output to gl
    MODE_BAND_H1,       // if audio1 is detected ( and audiomode flag set ? ) this sends the averaged high 0 band output to gl

    SOURCE_MODE_MAX
}
enum target_modes
{
    OUT_MODE_SEL_TEX    // this is not a mode in this sense, if the dedicated flag is set this sends the after mode value g_inOutMatrixFlt[p_channel][OUT] to a system member variable
    OUT_MODE_SEL_VID    // we need to do so because we want the mode effect, not the input adc this system parameter
    OUT_MODE_SEL_FRM
    OUT_MODE_PRG_CLK    // this is the "mode" where i not send and continious timestamp to gl but a value after mode 
    OUT_MODE_EXT_CLK    // here im not certain what to do about it. i can use detected beats also via TRG mode ( because it can reliable detect clocks - if applied )

    TARGET_MODE_MAX
}
enum centralModeBuffer // organised in blocks/layers for the mapMenuGroup
{
	MODE_CH0 = 0,           // store the mode ( from g_modeTable[] ) for cannel 0 -> layer 1
	MODE_CH1,               // store the mode ( from g_modeTable[] ) for cannel 1
	MODE_CH2,               // store the mode ( from g_modeTable[] ) for cannel 2
	MODE_CH3,               // store the mode ( from g_modeTable[] ) for cannel 3

	MODE_CH4,               // store the mode ( from g_modeTable[] ) for cannel 4 -> layer 2
	MODE_CH5,               // store the mode ( from g_modeTable[] ) for cannel 5
	MODE_CH6,               // store the mode ( from g_modeTable[] ) for cannel 6
	MODE_CH7,               // store the mode ( from g_modeTable[] ) for cannel 7

	LF1_WAVE,               // stores waveform index for lfo1 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )  -> layer 3
	LF2_WAVE,               // stores waveform index for lfo2 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )
	LF1_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
	LF2_MULT,               // stores the multiplier index for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )

	SENS_A,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA0  -> layer 4
	SENS_B,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandA1
	SENS_C,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB0
	SENS_D,                 // stores the sensitivity max_index for the audio mode ( available if enabled ) bandB1

    SEL_TIME,               // i alow the user to set up the input channel for the target_modes
    SEL_TEX,
    SEL_VID,
    SEL_FRM,    
    
	FLAG_TIME,   			// 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	FLAG_TEX,   			// 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	FLAG_EXT,   			// 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	FLAG_VID,   			// 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )
// i can use this for "invisible flags" too, like not set by the user
    FLAG_AUDIO_A,
    FLAG_AUDIO_B,
    DUMMY_FLAG_C,
	DUMMY_FLAG_D,

//  this are "one shot toggle flags" to trigger certain system operations 

    STORE_SET,  			// 0 or 1   ( if set to 1 and the MENU_LAYER_COUNT is zero again ( button released ?) the file operation starts )
    LOAD_SET,   			// 0 or 1
    STORE_LOG,  			// 0 or 1
    LOAD_KLN,   			// 0 or 1

	IS_STORED,              // not really a mode but a flag - important: obey the "4 per block" rule

    MODETABLE_COUNT
};

        typedef void                            (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right? "add modes by only extending the table"

                ModeFunc                        g_modeTable[9]                                 =        {   &CKernel::modeADC,          // "copy value" mode
                                                                                                            &CKernel::modeTRG,          
                                                                                                            &CKernel::modeBPM,
                                                                                                            &CKernel::modeLF1,          // "copy value" mode
                                                                                                            &CKernel::modeLF2,          // "copy value" mode

                                                                                                            &CKernel::modeAudioAb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioAb1,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb1 };   // "copy value" mode

enum MapType 
{ 
    MAP_MODE = 0, 
    MAP_VALUE 
};

enum ModeFlags 
{
    GROUP_BASE  = 0,
    GROUP_FLAG1 = 1,
    GROUP_FLAG2 = 2,
    GROUP_COUNT = 3
};
// per-block/slot: mode vs value
static const uint8_t g_mapType[BLOCK_COUNT][4] =
{
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // block 0 (CH0..CH3 modes)
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  }, // block 1 (CH4..CH7 modes)

    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 2 (LF1_WAVE..LF2_MULT)
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 3 (SENS_A..SENS_D)
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 4 (SEL_TIME..SEL_FRM) 0..7
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }, // block 5 (target flags) 0/1
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }  // block 6 (hw/sys toggles) 0/1
};
// per-block/slot: roof for MAP_VALUE blocks, and baseLen for MAP_MODE blocks (blocks 0/1)
static const uint8_t g_valueRoof[BLOCK_COUNT][4] =
{
    { 5,  5,  5,  5  },   // block 0 (MAP_MODE) baseLen
    { 5,  5,  5,  5  },   // block 1 (MAP_MODE) baseLen

    { 3,  3,  7,  7  },   // block 2 (MAP_VALUE) roofs
    { 64, 64, 64, 64 },   // block 3 (MAP_VALUE) roofs
    { 8,  8,  8,  8  },   // block 4 (MAP_VALUE) roofs (0..7)
    { 2,  2,  2,  2  },   // block 5 (MAP_VALUE) roofs (0/1)
    { 2,  2,  2,  2  }    // block 6 (MAP_VALUE) roofs (0/1)
};
// per-group lengths (used for optional groups)
static const uint8_t g_groupLen[GROUP_COUNT] =
{
    5, // GROUP_BASE
    2, // GROUP_FLAG1 -> AUDIO_A
    2  // GROUP_FLAG1+1 -> AUDIO_B
};
// per-group mode IDs (open table; order + holes preserved here)
static const uint8_t g_groupModes[GROUP_COUNT][5] =
{
    { 0, 1, 2, 3, 4 },   // GROUP_BASE
    { 5, 6, 0, 0, 0 },   // AUDIO_A
    { 7, 8, 0, 0, 0 }    // AUDIO_B
};
// runtime arrays produced/consumed by the 3 functions
uint8_t g_modeRoof[BLOCK_COUNT * 4];                            // actually we only need 2 * 4 since only block 0 and 0 need active remapping
uint8_t g_modeMap [BLOCK_COUNT * 4][SOURCE_MODE_MAX];           // actually we only need 2 * 4 since only block 0 and 0 need active remapping

void CKernel::set_mode_roof_map(uint8_t block)
{
    const uint8_t f_first_flag = FLAG_AUDIO_A;
    const uint8_t base         = block << 2;

    for (uint8_t slot = 0; slot < 4; ++slot)
    {
        const uint8_t row = base + slot;

        if (g_mapType[block][slot] == MAP_VALUE)
        {
            g_modeRoof[row] = g_valueRoof[block][slot];
            continue;
        }

        uint8_t dst = 0;

        // base group length comes from g_valueRoof for MAP_MODE blocks (e.g. 5,5,5,5)
        const uint8_t baseLen = g_valueRoof[block][slot];

        for (uint8_t i = 0; i < baseLen; ++i)
            g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];

        // optional groups (A/B)
        for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);
            if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos]) continue;

            for (uint8_t i = 0; i < g_groupLen[group]; ++i)
                g_modeMap[row][dst++] = g_groupModes[group][i];
        }

        g_modeRoof[row] = dst;
    }
}

void CKernel::mapMenuGroup(uint8_t menu_id, uint8_t block)
{
    if (g_menu_mode_new != menu_id)
    {
        return;
    }

    const uint8_t base = block << 2;
    unsigned v;

    v = (g_inOutMatrixInt[4][RAW] * g_modeRoof[base + 0]) >> 10;

    if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
    {
        g_menuPickUpFlag[base + 0] = true;
    }
    else if (g_menuPickUpFlag[base + 0])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
    }

    v = (g_inOutMatrixInt[5][RAW] * g_modeRoof[base + 1]) >> 10;

    if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
    {
        g_menuPickUpFlag[base + 1] = true;
    }
    else if (g_menuPickUpFlag[base + 1])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
    }

    v = (g_inOutMatrixInt[6][RAW] * g_modeRoof[base + 2]) >> 10;

    if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
    {
        g_menuPickUpFlag[base + 2] = true;
    }
    else if (g_menuPickUpFlag[base + 2])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
    }

    v = (g_inOutMatrixInt[7][RAW] * g_modeRoof[base + 3]) >> 10;

    if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
    {
        g_menuPickUpFlag[base + 3] = true;
    }
    else if (g_menuPickUpFlag[base + 3])
    {
        g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
    }
}

void CKernel::getChannelModeB(int p_channel)
{
    uint8_t mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];
    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}


void CKernel::applyTargetModes(int p_channel)
{
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX] && g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX] == p_channel)
                    {
                    m_activeTEX = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] && g_centralModeBuffer[g_currentProgramBuffer][SEL_VID_NUM] == p_channel)
                    {
                    m_activeVideo = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID] && g_centralModeBuffer[g_currentProgramBuffer][SEL_VID_FRM] == p_channel)
                    {
                    m_activeFrame = g_inOutMatrixInt[p_channel][OUT];
                    }

                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME] && g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME] == p_channel)
                    {
                    gl_time = g_inOutMatrixInt[p_channel][OUT];
                    }
                // i still wonder: is this rather a source-mode than a target-mode, maybe just and automated thingy running aside mode 2 "TRG"?
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT] && g_centralModeBuffer[g_currentProgramBuffer][SEL_EXT] == p_channel && g_centralModeBuffer[g_currentProgramBuffer][p_channel] != p_channel)
                    {
                    if (g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                        !g_inOutMatrixInt[p_channel][TRF])
                        {
                        calculate1BPM(1, g_currentTime);
                        g_extClockTriggerFlag[p_channel] =  = true;
                        }
                    else if (g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                        {
                        g_extClockTriggerFlag[p_channel] =  = false;
                        }
                    }                    
}

void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];       
                
                applyTargetModes(p_channel);
}

void            CKernel::modeTRG                    (   int p_channel)
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF]) /* treshold flag - but i need 4 each menu layer or is this different than g_menuPickUpFlag??? */
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];

                    if (FLAG_EXT) calculate1BPM(1, g_currentTime);

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }

            //  applyTargetModes(p_channel);
}

void            CKernel::modeBPM                    (   int p_channel /* , currentTime*/)   // <- currentTime should be g_currentTime -> global member set during Run()  - or a call parameter!
{ 
                if ( /* currentTime */ g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )// g_nextBeatTime[g_activeBpmChannel])      // <- g_nextBeatTime is now part of the g_lfoBpmMatrix -> enum lfo_bpm_types NBT -> nextBeatTime                                      
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];
                    }

                applyTargetModes(p_channel);
}

void            CKernel::modeLF1                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF1];

                applyTargetModes(p_channel);
}

void            CKernel::modeLF2                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF2]; 

                applyTargetModes(p_channel);
}
/*
void            CKernel::modeSelectTex              (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * filecounter[FT_TEX][FLD_VALID]) >> 10;

                m_activeTEX = g_inOutMatrixInt[p_channel][OUT];
}

void            CKernel::modeSelectVideo            (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * filecounter[FT_VID][FLD_VALID]) >> 10;

                m_activeVideo = g_inOutMatrixInt[p_channel][OUT];
}

void            CKernel::modeSelectFrame            (   int p_channel)
{
                g_inOutMatrixInt[p_channel][OUT] = (g_inOutMatrixInt[p_channel][RAW] * m_vid.frame_count[m_activeVideo]) >> 10;

                m_activeFrame = g_inOutMatrixInt[p_channel][OUT];                
}

void            CKernel::modeEXT                    (   int p_channel)
{
                    if (g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                        !g_inOutMatrixInt[p_channel][TRF])
                        {
                        calculate1BPM(1, g_currentTime);
                        g_extClockTriggerFlag[p_channel] =  = true;
                        }
                    else if (g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                        {
                        g_extClockTriggerFlag[p_channel] =  = false;
                        }
}
*/
void            CKernel::modeAudioAb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];

                applyTargetModes(p_channel);
}

void            CKernel::modeAudioAb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];

                applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];

                applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];

                applyTargetModes(p_channel);
}

// last but not least! ch 7 is hardwired as terget mode for "choose program"