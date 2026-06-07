// Arrays used here but not declared in this snippet:

uint8_t g_modeRoof[BLOCK_COUNT * 4];

uint8_t g_modeMap[BLOCK_COUNT * 4][MAX_MODEMAP_SIZE];

uint8_t g_centralModeBuffer[PROGRAM_COUNT][BUFFER_SIZE];

bool g_menuPickUpFlag[BLOCK_COUNT * 4];

uint16_t g_inOutMatrixInt[INPUT_COUNT][MATRIX_FIELD_COUNT];

ModeFunc g_modeTable[MODE_COUNT];


// Also used, but not arrays:

enum modetable
{
	CH0_MODE = 0,           // store the mode ( from g_modeTable[] ) for cannel 0
	CH1_MODE,               // store the mode ( from g_modeTable[] ) for cannel 1
	CH2_MODE,               // store the mode ( from g_modeTable[] ) for cannel 2
	CH3_MODE,               // store the mode ( from g_modeTable[] ) for cannel 3

	CH4_MODE,               // store the mode ( from g_modeTable[] ) for cannel 4
	CH5_MODE,               // store the mode ( from g_modeTable[] ) for cannel 5
	CH6_MODE,               // store the mode ( from g_modeTable[] ) for cannel 6
	CH7_MODE,               // store the mode ( from g_modeTable[] ) for cannel 7

	SEL_TIME, 				// mode to take external clock signals      <--- is not really a mode - its a flag remapping an ADC to gl_time
	SEL_TEX,				// select the texture i use	                
	SEL_VID_NUM,				// select the video i use 
	SEL_VID_FRM,				// select the frame i use

	MODE_EXT,				// 0 or 1
    DUMMY_MODE_B,
    DUMMY_MODE_C,
    DUMMY_MODE_D,

	LF1_WAVE,               // stores waveform for lfo1 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] ) 
	LF2_WAVE,               // stores waveform for lfo2 ( from g_waveTable[WAVEFORMS_COUNT][WAVESAMPLES] )
	LF1_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )
	LF2_MULT,               // stores the multiplier for lfo1 ( from g_lfoMultiplier[LFO_MULTIPLIERS_COUNT] )

	SENS_A,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA0
	SENS_B,                 // stores the sensitivity for the audio mode ( available if enabled ) bandA1
	SENS_C,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB0
	SENS_D,                 // stores the sensitivity for the audio mode ( available if enabled ) bandB1

	FLAG_TIM,   			// 0 or 1   ( wait - i propose a simple 4 to 7 and 0 as of, means anything else than 0 is the actual input p_channel problem, it need to overrule CH*_MODE )
	FLAG_TEX,   			// 0 or 1   ( how we can do it? also, dont i want more than only p_channel 4-7 assignable? )
	FLAG_EXT,   			// 0 or 1   ( whats about the approach in readADC() where i modify the number of possible modes in the modematrix )
	FLAG_VID,   			// 0 or 1   ( like if i have one here CH*_MODE "opens" up for this modes - requires a constant check and update but... )

    DUMMY_FLAG_A,
    DUMMY_FLAG_B,
    DUMMY_FLAG_C,
	DUMMY_FLAG_D,

    STORE_SET,  			// 0 or 1   ( if set to 1 and the MENU_LAYER_COUNT is zero again ( button released ?) the file operation starts )
    LOAD_SET,   			// 0 or 1
    STORE_LOG,  			// 0 or 1
    LOAD_KLN,   			// 0 or 1

	IS_STORED,                          // not really a mode but a flag - important: obey the "4 per block" rule

    MODETABLE_COUNT
};

uint8_t g_currentProgramBuffer;
uint8_t g_menu_mode_new;

// Minimum missing defines/constants:

#define BLOCK_COUNT       5
#define GROUP_COUNT       4
#define MAX_MODEMAP_SIZE  12   // for current mode groups: 5 + 2 + 4 + 1

// For your current shown data, `g_modeMap` needs at least:

uint8_t g_modeMap[20][12];

// because:

// 5 blocks * 4 slots = 20 rows
// max mode entries = 5 + 2 + 4 + 1 = 12

enum MapType
{
    MAP_MODE = 0,
    MAP_VALUE
};

static const uint8_t g_mapType[BLOCK_COUNT][4] =
{
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },   // block 0
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },   // block 1
    { MAP_MODE,  MAP_MODE,  MAP_MODE,  MAP_MODE  },   // block 2
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE },   // block 3: LFO/control values
    { MAP_VALUE, MAP_VALUE, MAP_VALUE, MAP_VALUE }    // block 4: audio sensitivity
};

static const uint8_t g_valueRoof[BLOCK_COUNT][4] =
{
    { 0, 0, 0, 0 },       // block 0: not used, MAP_MODE
    { 0, 0, 0, 0 },       // block 1: not used, MAP_MODE
    { 0, 0, 0, 0 },       // block 2: not used, MAP_MODE
    { 3, 3, 7, 7 },       // block 3: LFO wave/wave/mult/mult
    { 64, 64, 64, 64 }    // block 4: audio sensitivity
};

static const uint8_t g_groupLength[BLOCK_COUNT][4][GROUP_COUNT] =
{
    {
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 }
    },
    {
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 },
        { 5, 2, 4, 1 }
    },
    {
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 },
        { 5, 2, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

static const uint8_t g_groupModes[GROUP_COUNT][5] =
{
    {  0,  1,  2,  3,  4 },
    {  8,  9,  0,  0,  0 },
    { 12, 13, 14, 15,  0 },
    { 10,  0,  0,  0,  0 }
};

void CKernel::set_mode_roof_map(uint8_t block)
{
    const uint8_t f_first_flag = FLAG_TIM;
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

        for (uint8_t i = 0; i < g_groupLength[block][slot][GROUP_BASE]; ++i)
        {
            g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];
        }

        for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
        {
            const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);

            if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos])
            {
                continue;
            }

            for (uint8_t i = 0; i < g_groupLength[block][slot][group]; ++i)
            {
                g_modeMap[row][dst++] = g_groupModes[group][i];
            }
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

// Use result:

uint8_t index = g_centralModeBuffer[g_currentProgramBuffer][row];

if (g_mapType[block][slot] == MAP_MODE)
{
    uint8_t mode = g_modeMap[row][index];
    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(row);
    }
}
else
{
    uint8_t value = index;
}

void CKernel::getChannelModeB(int p_channel)
{
    uint8_t index = g_centralModeBuffer[g_currentProgramBuffer][p_channel];

    uint8_t mode = g_modeMap[p_channel][index];

    ModeFunc fn = g_modeTable[mode];

    if (fn)
    {
        (this->*fn)(p_channel);
    }
}

        typedef void                            (CKernel::*ModeFunc)(int);         // for the new menu selector -> easier to expand, right? "add modes by only extending the table"

                ModeFunc                        g_modeTable[12]                                 =       {   &CKernel::modeADC,          // "copy value" mode
                                                                                                            &CKernel::modeTRG,          
                                                                                                            &CKernel::modeBPM,
                                                                                                            &CKernel::modeLF1,          // "copy value" mode
                                                                                                            &CKernel::modeLF2,          // "copy value" mode
                                                                                                            &CKernel::modeSelectTex,
                                                                                                            &CKernel::modeSelectFrame,
                                                                                                            &CKernel::modeSelectVideo,
                                                                                                            nullptr,
                                                                                                            nullptr,
                                                                                                            nullptr,
                                                                                                            &CKernel::modeAudioAb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioAb1,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb0,     // "copy value" mode
                                                                                                            &CKernel::modeAudioBb1 };   // "copy value" mode