// bufferToScreen.cpp

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
for kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


boolean CKernel::screen_init                              (   void );
void    CKernel::screen_clear_screen                      (   u32 bgColor );
void    CKernel::screen_draw_buffer_segment     (   const char *pSourceBuffer,
                                                                        u32 startIndex,
                                                                        u32 endIndex,
                                                                        unsigned startCol,
                                                                        unsigned startRow,
                                                                        u32 fgColor,
                                                                        u32 bgColor );
unsigned CKernel::screen_get_grid                         (    unsigned &cols, 
                                                                        unsigned &rows )

#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>

    Self-contained framebuffer setup:
    width/height are queried from firmware via mailbox when constructed with 0,0.
    This reflects the effective display mode (typically driven by config.txt/EDID firmware states).


static CBcmFrameBuffer gE_FrameBuffer (0, 0, 32, 0, TRUE);
static CCharGenerator  gE_CharGenerator;

static u32      *gE_PixelBuffer  = 0;
static unsigned  gE_PitchBytes   = 0;
static unsigned  gE_ScreenWidth  = 0;
static unsigned  gE_ScreenHeight = 0;
static unsigned  gE_CharWidth    = 0;
static unsigned  gE_CharHeight   = 0;
static unsigned  gE_Cols         = 0;
static unsigned  gE_Rows         = 0;
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// features.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
g_resultBPM g_nextBeatTime  g_intervalCalculated    g_elapsedMicroseconds   g_sampleIndex   g_cycleLength   g_lfoMultiplier g_lfoMultiplierTMP   g_lastBpmCalculation   g_lastBpmCalculationTMP g_lastCircleBuffer  g_nextCircleBuffer      

i love to have a similar array i i have for the in/output matrix
*/

/*
void            CKernel::audioEnergy          (float p_adcvalue) 
{   
                const int f_maxBuffer = 33;

                const int f_averageBufferSizeTable[4][5] = 
                    {
                    {33, 25, 17,  9,  5},                                                                                                           // Channel 0 (lowest frequency)  
                    {25, 19, 13,  7,  4},                                                                                                           // Channel 1
                    {17, 13,  9,  5,  3},                                                                                                           // Channel 2
                    { 9,  7,  5,  3,  2}                                                                                                            // Channel 3 (highest frequency)
                    };
                    
                static float f_band0[f_maxBuffer] = {0};                                                                                            // Static ring buffers for each frequency band
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};
                static unsigned char f_indexBand0 = 0;                                                                                              // Static indices for ring buffers
                static unsigned char f_indexBand1 = 0;
                static unsigned char f_indexBand2 = 0;
                static unsigned char f_indexBand3 = 0;

                if ( g_sensitivityNew != g_sensitivityOld )
                    {
                    g_inOutMatrixFlt[0][au0] = 0;
                    g_inOutMatrixFlt[0][au1] = 0;
                    g_inOutMatrixFlt[0][au2] = 0;
                    g_inOutMatrixFlt[0][au3] = 0;

                    f_indexBand0 = 0;                                                                                                               // Reset ring buffer indices too
                    f_indexBand1 = 0;
                    f_indexBand2 = 0;
                    f_indexBand3 = 0;

                    g_sensitivityOld = g_sensitivityNew;
                    }   
                f_band0[f_indexBand0] = p_adcvalue;                                                                                                 // Update ring buffers with the new ADC value
                f_band1[f_indexBand1] = p_adcvalue;
                f_band2[f_indexBand2] = p_adcvalue;
                f_band3[f_indexBand3] = p_adcvalue;

                for (unsigned char i = 0; i < f_averageBufferSizeTable[0][g_sensitivityNew]; ++i)                                                   // Averaging the buffer contents
                    {
                    g_inOutMatrixFlt[0][au0] += f_band0[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[1][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au1] += f_band1[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[2][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au2] += f_band2[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[3][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au3] += f_band3[i];
                    }
                g_inOutMatrixFlt[0][au0] /= f_averageBufferSizeTable[0][g_sensitivityNew];
                g_inOutMatrixFlt[0][au1] /= f_averageBufferSizeTable[1][g_sensitivityNew];
                g_inOutMatrixFlt[0][au2] /= f_averageBufferSizeTable[2][g_sensitivityNew];
                g_inOutMatrixFlt[0][au3] /= f_averageBufferSizeTable[3][g_sensitivityNew];

                f_indexBand0 = (f_indexBand0 + 1) % f_averageBufferSizeTable[0][g_sensitivityNew];                                                  // Update indices
                f_indexBand1 = (f_indexBand1 + 1) % f_averageBufferSizeTable[1][g_sensitivityNew];
                f_indexBand2 = (f_indexBand2 + 1) % f_averageBufferSizeTable[2][g_sensitivityNew];
                f_indexBand3 = (f_indexBand3 + 1) % f_averageBufferSizeTable[3][g_sensitivityNew];
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
    we need a combined array for the bpm/lfo timing stuff here, same as in circle/sample/04-oculus_close_and_closer/menu_final.cpp
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// filesystem_user.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// menu_final.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/* 
                        need to figure out the enums here again!

g_inOutMatrixInt[CHANNEL][ENUM_SOURCE]
g_inOutMatrixFlt[CHANNEL][ENUM_SOURCE]
g_menuPickUpFlag[4*menu_layers]


INT	    ADC_RAW     (SCALED)IN  (SCALED)OUT RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
ch0	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch1	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch2	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch3	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch4	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch5	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch6	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch7	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U


FLT	    ADC_RAW     (SCALED)IN  (SCALED)OUT RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
ch0	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch1	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch2	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch3	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch4	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch5	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch6	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch7	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   

enum:   raw         in          out         rnd         lf1         lf2         au0         au1         au2         au3         trL         trH


all the G ( global ) sit in channel 0 - U are unique values per channel 
*/

enum io_types
{
int raw = 0,        //  the position the dampened adc values per channels are stored *
int  in,            //  either the calculated int or flt value *
int out,            //  here lands the processed ( after mode ) for the glsl uniforms      
int rnd,            //  either the per-channel random int or flt value *
int lf1,            //  either the lfo one int or flt value
int lf2,            //  either the lfo two int or flt value
int au0,            //  the audio band 0 flt value
int au1,            //  the audio band 1 flt value
int au2,            //  the audio band 2 flt value
int au3,            //  the audio band 3 flt value
                    // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
int trL,            //  per channel threshold low !!! dont forget to copy the values in here    128
int trH,            //  per channel threshold high                                              320
//  trF,            //  per channel threshold "flag"
int io_type_count
}


/*
Discrete/equality pickup

Pattern: compute quantized value (new_mode, waveform, multiply) and arm when it equals stored value.

Example:
new_mode = ...; if (!flag && new_mode == stored)

Analog/tolerance pickup with bypass (only CH6/CH7 runtime path)

Pattern: arm when raw ADC enters buffer ± TOLERANCE, else allow processing if flag or mode is non-zero.

Example CH6: if (!flag12 && raw6 in window) ... else if (flag12 || CH6_MODE != 0)

*/

uint8_t mode_map[8][17] =
{
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}
{5, 0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0}

};
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// the_OV_functions.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// we need to figure out what we already have done in gfx_init.cpp and how we nust implement the rest here following OUR framework!

/*
// menu asset extension lists used by scan/load flow
char *MENU_FSH_EXTENSIONS[1] = { (char *)"fsh" };   // is implemented elsewhere!!
char *MENU_TEX_EXTENSIONS[1] = { (char *)"bmp" };   // is implemented elsewhere!!

// scanned filenames and loaded byte counters
char *SCANED_FILES_MENU_FSH[1] = { 0 };             // is implemented elsewhere!!
char *SCANED_FILES_MENU_TEX[1] = { 0 };             // is implemented elsewhere!!
unsigned MENU_FSH_LOADED_BYTES[1] = { 0 };          // is implemented elsewhere!!
unsigned MENU_TEX_LOADED_BYTES[1] = { 0 };          // is implemented elsewhere!!

// global menu layout values
float MENU_GPU_ORIGIN[2] = { 0.05f, 0.08f };
float MENU_GPU_TILE_SIZE[2] = { 0.08f, 0.08f };
float MENU_GPU_BACKGROUND_SCALE[2] = { 1.0f, 1.0f };

// slots: 0..7 mode icons, 8..11 bpm digits, 12 dot, 13 bpm icon, 14..15 background blocks
float MENU_GPU_REL_POS[16][2] =
{
    {0.0f, 0.0f}, {1.1f, 0.0f}, {2.2f, 0.0f}, {3.3f, 0.0f},
    {0.0f, 1.1f}, {1.1f, 1.1f}, {2.2f, 1.1f}, {3.3f, 1.1f},

    {0.0f, 2.6f}, {0.9f, 2.6f}, {1.8f, 2.6f}, {2.7f, 2.6f},
    {3.6f, 2.6f}, {4.1f, 2.6f},

    {-0.1f, -0.1f},
    {-0.1f, 2.5f}
};

float MENU_GPU_REL_SIZE[16][2] =
{
    {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
    {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},

    {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f},
    {0.35f, 0.95f}, {0.75f, 0.95f},

    {4.4f, 2.2f},
    {5.8f, 1.2f}
};

int MENU_GPU_TILE_COUNT = 16;

struct MenuTileRect
{
    float x;
    float y;
    float w;
    float h;
};

struct MenuGpuState
{
    bool initialized;
    GLuint program;
    GLuint atlas_texture;
    GLint a_vertex;
    GLint u_atlas;
    GLint u_tile_count;
    GLint u_tile_rect;
    GLint u_tile_index;
    int tile_index[16];
    MenuTileRect tile_rect[16];
};
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// util.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
// after getting fucked in my head we finally found the deterministic function f_buffer am looking for!
/*
#define BTN_PRESSED 0

enum ButtonTSIndex
{
    BTN_PRESS_START = 0, // timestamp when press starts, 0 = currently up
    BTN_RELEASE     = 1, // timestamp of last release (double-click window anchor)
    BTN_HOLD_TICK   = 2, // increments while held after long threshold
    BTN_SINGLE      = 3, // one-cycle pulse on press edge
    BTN_DOUBLE      = 4  // one-cycle pulse on second press edge in double window
};
*/
// 2 buttons, 5 fields each (no BTN_STATUS needed)

