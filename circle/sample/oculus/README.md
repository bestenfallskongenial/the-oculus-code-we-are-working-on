// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// bufferToScreen.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  Self-contained framebuffer setup:
//      - width/height are queried from firmware via mailbox when constructed with 0,0.
//      - This reflects the effective display mode (typically driven by config.txt/EDID firmware states).

//  needed headers from circle OS

    #include <circle/bcmframebuffer.h>
    #include <circle/chargenerator.h>

//  important definitions ( preferable as public members in kernel.h - the project header  )

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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// features.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  filesystem_user.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  mute - need to be refactored completely, last-to-implement feature!

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  filesystem.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//      ! important:

//      public:
//          unsigned g_hFile

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_debug.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// logs gl status from compiling snd linking into a buffer
// gl status check and log into a buffer
//      - i need to pass buffer and index to make it modular 
//      ! the gfx_checks should rather store to a hardwired buffer...

//  void CKernel::shaderLog (   char*       buffer,             // buffer ( from memory.cpp ) where to store
                                u32&        index,              // index in the buffer - needs to be global since the saveFromBuffer() need to know the position and size! 
                                GLint       shader,             // the shader in question in our pool of 1 + 32 
                                int         shaderIndex)        // the index in the  m_shaderStatusFlags array from compiling
//  void CKernel::programLog(   char*       buffer, 
                                u32&        index, 
                                GLint       shader, 
                                int         program_index)
//  void CKernel::gfx_check (   char*       buffer, 
                                u32&        index, 
                                const char* file, 
                                unsigned    line)

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_init.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// compiles the vertex/fragment shaders, links the programs, sets up the uniforms and inits the textures
//      - important - double check the OverLayMenu init sequence!
//      - more and clearer separation for the parameter struct
//      ! the gfx_checks should rather store to a hardwired buffer...

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_OGL.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// init EGL / Dispmanx

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_render.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// render loop:
//      - some confusion about the texture mode, the "framerate break"
//      - we need a second pass reeder for the OverLayMenu-shader! 
//      ? gpt said i have a "swap already for the second pass" ? artifact? where, what?
//      ! the gfx_checks should rather store to a hardwired buffer...

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// readADC.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  io_read_ADC() - explicit audio hold + menu scaling path
//      Purpose:
//      - Read all 8 ADC channels, compute control/envelope outputs, and apply temporary menu scaling during audio activity.
//  How it works:
//      - Uses 4-sample per-channel ring buffers to compute `raw`, then computes scaled `val` and float output values.
//      - Detects transients on channels 0..3 with `g_irregularity[ch] = s0 - s1 + s2 - s3` and +/-AUDIO_THRESHOLD.
//      - Sets hold timers on transient events: channels 0/2 set `audio_hold_A`, channels 1/3 set `audio_hold_B`.
//      - Computes `menu_map_max[0..3]` from active holds: 5 when none are active, 7 with one active hold, 9 with both.
//      - Decrements each hold timer on every call until it reaches zero.
//  What it is doing in practice:
//      - Produces deterministic signal outputs (`raw`, `val`, `au0..au3`) and deterministic temporary UI range expansion.

//  void            CKernel::readADC    () 