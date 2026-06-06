// Arrays used here but not declared in this snippet:

uint8_t g_modeRoof[BLOCK_COUNT * 4];

uint8_t g_modeMap[BLOCK_COUNT * 4][MAX_MODEMAP_SIZE];

uint8_t g_centralModeBuffer[PROGRAM_COUNT][BUFFER_SIZE];

bool g_menuPickUpFlag[BLOCK_COUNT * 4];

uint16_t g_inOutMatrixInt[INPUT_COUNT][MATRIX_FIELD_COUNT];

ModeFunc g_modeTable[MODE_COUNT];


// Also used, but not arrays:

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

