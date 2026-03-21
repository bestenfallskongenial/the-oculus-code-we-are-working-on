	
// my MEM and DMA buffer allocations ( maybe a common struct? )
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

	char** 				m_bufferOmt;
	char* 				m_overlyBlockBase;
	char* 				m_overlayRawBlock;
    size_t 				m_overlyBlockSize;

    char** 				m_bufferTex;
	char* 				m_textureBlockBase;
	char* 				m_textureRawBlock;
    size_t 				m_textureBlockSize;

	char**				m_bufferKnl;
	char**				m_bufferLog;

	char** 				m_bufferVsh;
	char** 				m_bufferOmf;	
    char** 				m_bufferFsh;
// my source of truth
	#define	SLOTS 		34 	// 1 default prog + 32 user progs + 1 default slot

	enum modetable		// for the g_centralModeBuffer array
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
	SENS_A,
	SENS_B,
	SENS_C,
	SENS_D,
	FRM_MODE,
	TEX_MODE,
	CLK_MODE,
	VID_MODE,
	IS_STORED,
	modetablecount
}

	int g_centralModeBuffer[SLOTS][modetablecount];

	uint8_t g_modeMap[modetablecount][17] =	
	{  	   /  A    /  B    /  LFO  / Sens       
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0} 	// the first element is the max of modes for each channel, than we have the order ( switch case applyModeToChannel(int channel) )
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

		{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{4, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

		{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
		{63, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
	};
// my IO arrays
	g_inOutMatrixInt[][];
	g_inOutMatrixFLT[][];

// my button state machine array
	#define BTN_PRESSED 0

	enum ButtonTSIndex
	{
		BTN_PRESS_START = 0, // timestamp when press starts, 0 = currently up
		BTN_RELEASE     = 1, // timestamp of last release (double-click window anchor)
		BTN_HOLD_TICK   = 2, // increments while held after long threshold
		BTN_SINGLE      = 3, // one-cycle pulse on press edge
		BTN_DOUBLE      = 4  // one-cycle pulse on second press edge in double window
	};
	unsigned g_buttons_states[2][5] = {0}

	#define f_maxBuffer 63
	#define audio
	uint32_t 		g_irregularity[4] = {0};
	static uint32_t audio_hold_A = 0;
    static uint32_t audio_hold_B = 0;