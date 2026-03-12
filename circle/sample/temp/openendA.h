#ifndef OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H
#define OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H

// TEMP INVENTORY HEADER
// Purpose:
// - Gather variables/arrays/structs used across the 04-oculus_close_and_closer files.
// - Show confusing or mismatching names in-context so naming can be unified.
// - This file is intentionally documentation-first and not wired into build yet.

#include <stdint.h>
#include <stddef.h>

// ---------------------------------------------------------------------------------------------------------------------
// 1) Structs encountered in this sample
// ---------------------------------------------------------------------------------------------------------------------

struct RGB
{
    uint8_t red;
    uint8_t grn;
    uint8_t blu;
};

struct glsl_state
{
    // EGL/window handles + shader/program/texture/uniform tables
    uint32_t screen_width;
    uint32_t screen_height;

    // NOTE: exact VC/EGL/GLES types intentionally omitted here to keep this temp header lightweight.
    // In production, keep the original full typed struct from kernel.h.

    unsigned gl_vsh_id_placeholder;
    unsigned gl_fsh_id_placeholder;
    unsigned gl_prg_id_placeholder;
    unsigned gl_tex_id_placeholder;
};

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
    unsigned program;
    unsigned atlas_texture;
    int a_vertex;
    int u_atlas;
    int u_tile_count;
    int u_tile_rect;
    int u_tile_index;
    int tile_index[16];
    MenuTileRect tile_rect[16];
};

// ---------------------------------------------------------------------------------------------------------------------
// 2) Core mode / IO model (target naming)
// ---------------------------------------------------------------------------------------------------------------------

// Canonical buffers used by menu_final.cpp / util.cpp / features.cpp.
int   g_centralModeBuffer[16][33];
int   g_inOutMatrixInt[8][16];
float g_inOutMatrixFlt[8][16];

uint8_t menu_map_max[12];
bool    g_menuPickUpFlag[16];
bool    g_channel_mode_capability[8][6];

// Current selection / slot m_glsl
int g_currentProgramBuffer;
int g_current_gl_program;
int g_last_gl_program;
int g_current_menu_layer;

// ---------------------------------------------------------------------------------------------------------------------
// 3) Timing / BPM / LFO m_glsl (currently split across old/new names)
// ---------------------------------------------------------------------------------------------------------------------

unsigned long g_resultBPM[2];
unsigned long g_nextBeatTime[2];
unsigned long g_intervalCalculated[2];
unsigned long g_lastBpmCalculation[2];

unsigned long g_lastCircleBuffer[2];
unsigned long g_nextCircleBuffer[2];
unsigned long g_elapsedMicroseconds[2];
unsigned long g_cycleLength[2];
int           g_sampleIndex[2];

int g_activeBpmChannel;

// ---------------------------------------------------------------------------------------------------------------------
// 4) Menu/button/input m_glsl
// ---------------------------------------------------------------------------------------------------------------------

unsigned int g_buttons_states[2][5];
unsigned long g_currentTime;
unsigned long g_double_click_time;
unsigned long g_long_click_time;

int g_menu_mode_new;
int g_menu_mode_old;

bool is_hold_for_2_sec_a;
bool is_hold_for_2_sec_b;

// ---------------------------------------------------------------------------------------------------------------------
// 5) Filesystem/media scanning/loading m_glsl
// ---------------------------------------------------------------------------------------------------------------------

// list of extensions used in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames?
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLM_SD + KLN_USB]     	= { 0 };
// array to store the length of the loased files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLM_SD + KLN_USB]      = { 0 };

// ---------------------------------------------------------------------------------------------------------------------
// 6) Render/menu overlay helper globals from "the functions.cpp"
// ---------------------------------------------------------------------------------------------------------------------

float MENU_GPU_ORIGIN[2];
float MENU_GPU_TILE_SIZE[2];
float MENU_GPU_BACKGROUND_SCALE[2];
float MENU_GPU_REL_POS[16][2];
float MENU_GPU_REL_SIZE[16][2];
int   MENU_GPU_TILE_COUNT;
MenuGpuState g_menu_state;

// ---------------------------------------------------------------------------------------------------------------------
// 7) Known confusing / mismatching names (rename candidates)
// ---------------------------------------------------------------------------------------------------------------------

// Old                            -> Candidate canonical name
// -------------------------------------------------------------------------------------------------

// active_bpm_channel             -> g_activeBpmChannel
// g_nextBeatTime                 -> g_nextBeatTime
// g_inOutMatrixInt / g_inOutMatrixInt  -> ONE spelling only (recommend g_inOutMatrixInt)
// g_inOutMatrixFlt / g_inOutMatrixFlt  -> ONE spelling only (recommend g_inOutMatrixFlt)
// g_current_gl_program                 -> g_current_gl_program
// p_validTextureCount / p_validTextureCount -> one global symbol (pick one prefix strategy)
// m_bufferOmt / m_bufferOmt      -> one member spelling
// m_bufferVid / m_bufferVid    -> one member spelling
// maxUsb / maxUsb                -> one enum token spelling
// trL,trH vs trL,trH             -> one enum token spelling

// Wrapper/API drift to normalize:
// wrapper_from_sd   vs wrapper_load_sd
// wrapper_load_usb  vs wrapper_init_usb
// buttonPing(int,int) is called without args in wrappers.cpp

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1

#define         LOG_SD                  8


#define         VSH_USB                 0	// max number of u_vertex shader on sd
#define         OMF_USB            		0	// max number of fragment shader on sd
#define         FSH_USB            		32	// max number of fragment shader on sd
#define         OMT_USB            		0	// max number of fragment shader on sd
#define         TEX_USB            		8	// max number of textures on sd
#define         VID_USB            		8	// max number of videos on sd
#define         KLN_USB                 1

#define         FRM_USB                 1

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

// because i need to declare my other arrays right?

enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,         // i decided to add the output-frames A & B
    LOGGER,         // and logger buffer information here
    FT_COUNT
};

enum FileField
{
    FLD_MAXSD = 0,
    FLD_MAXUSB,
    FLD_EXTCNT,
    FLD_LOADED,
    FLD_SIZE,
    FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT] =
{
    /* VSH */ { VSH_SD, VSH_USB, VSH_EXT, 0, VSH_SIZ },
    /* OMF */ { OMF_SD, OMF_USB, OMF_EXT, 0, OMF_SIZ },
    /* FSH */ { FSH_SD, FSH_USB, FSH_EXT, 0, FSH_SIZ },
    /* OMT */ { OMT_SD, OMT_USB, OMT_EXT, 0, OMT_SIZ },
    /* TEX */ { TEX_SD, TEX_USB, TEX_EXT, 0, TEX_SIZ },
    /* VID */ { VID_SD, VID_USB, VID_EXT, 0, VID_SIZ },
    /* KLN */ { KLN_SD, KLN_USB, KLN_EXT, 0, KLN_SIZ },
    /* FRM */ { FRM_SD, FRM_USB,       0, 0, FRM_SIZ },     // i decided to add the output-frames A & B
    /* LOG */ { LOG_SD, LOG_USB,       0, 0, LOG_SIZ }      // and logger buffer information here      
};

// list of extensions used in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames?
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLM_SD + KLN_USB]     	= { 0 };
// array to store the length of the loased files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLM_SD + KLN_USB]      = { 0 };

                char** 				    m_bufferVid;
                char* 				    m_videoBlockBase;
                char* 				    m_videoRawBlock;
                size_t 				    m_videoBlockSize;

                char**				    m_bufferFrA;
                char* 				    m_frameBlockBaseA;
                char* 				    m_frameRawBlockA;
                size_t 				    m_frameBlockSizeA;

                char**				    m_bufferFrB;
                char* 				    m_frameBlockBaseB;
                char* 				    m_frameRawBlockB;
                size_t 				    m_frameBlockSizeB;	

                char** 				    m_bufferOmt;
                char* 				    m_overlyBlockBase;
                char* 				    m_overlayRawBlock;
                size_t 				    m_overlyBlockSize;

                char** 				    m_bufferTex;
                char* 				    m_textureBlockBase;
                char* 				    m_textureRawBlock;
                size_t 				    m_textureBlockSize;

                char**				    m_bufferKnl;
                char**				    m_bufferLog;
                char** 				    m_bufferVsh;
                char** 				    m_bufferOmf;                
                char** 				    m_bufferFsh;

enum ButtonTSIndex
{
    BTN_PRESS_START = 0, // timestamp when press starts, 0 = currently up
    BTN_RELEASE     = 1, // timestamp of last release (double-click window anchor)
    BTN_HOLD_TICK   = 2, // increments while held after long threshold
    BTN_SINGLE      = 3, // one-cycle pulse on press edge
    BTN_DOUBLE      = 4  // one-cycle pulse on second press edge in double window
};

unsigned int g_buttons_states[2][5] = {0};



#endif // OCULUS_CLOSE_AND_CLOSER_TEMP_HEADER_H