// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// README goal (this file)
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// one-style reference for ALL files inside circle/sample/oculus.
// each section follows the same template:
//     1) purpose
//     2) needed headers / external dependencies
//     3) needed variables (preferably public member declarations in kernel.h where cross-file access is required)
//     4) key functions / entry points
//     5) implementation notes

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Makefile
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - Build orchestration for the sample target (toolchain flags, linked Circle components, source list, image output).

// needed headers / dependencies:
// - Circle build system includes and platform variables.

// needed variables:
// - source/object lists that include kernel, gfx, io, fs, menu and helper modules.

// key entry points:
// - make targets provided by Circle sample build flow.

// implementation notes:
// - keep source list consistent with files documented below; stale entries break reproducible builds.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// kernel.h
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - central declaration hub for constants, enums, data tables, subsystem members, and CKernel API.

// needed headers / dependencies:
// - Circle core headers (timer, fs, usb, gpio, watchdog, machine info, scheduler)
// - EGL/GLES headers and bcm_host/vchiq/vcsm/h264 interfaces
// - sensor/button/spi/ws28xx addon interfaces

// needed variables (high-impact/public member candidates):
// - mode/state tables: g_centralModeBuffer[][], g_modeMap[modetablecount][17], g_menuPickUpFlag[]
// - io matrices: g_inOutMatrixInt[ADC_CHANNELS][io_type_count], g_inOutMatrixFlt[ADC_CHANNELS][io_type_count]
// - button + timing arrays: g_buttonTS[][BTN_INDEX_COUNT], g_resultBPM[], g_predictedTime[]
// - file counters + scanned names + loaded bytes: filecounter[FT_COUNT][FLD_COUNT], g_Scn*, g_byt*
// - loaded buffers: m_bufferVsh/m_bufferFsh/m_bufferOmf/m_bufferOmt/m_bufferTex/m_bufferVid/m_bufferKnl/m_bufferLog/m_bufferFrA/m_bufferFrB
// - dma/mem block tracking: m_*RawBlock + m_*BlockBase + m_*BlockSize
// - runtime handles: g_hFile, glsl_state program/texture/uniform handles, parser/decoder state
// - shared constant tables: g_rgb_color_table[49][3], g_waveTable[WAVEFORMS][WAVESAMPLES]
//
// needed enums / structs:
// - enums: TShutdownMode, modetable, io_types, filecount, fileindex, ButtonTSIndex, colorindex, FileType, FileField
// - structs: RGB, glsl_state, bufferInfo

// key entry points:
// - CKernel constructor/destructor
// - Initialize(), Run(), and module helpers declared for filesystem/gfx/io/menu/memory/logging

// implementation notes:
// - prefer declarations here when accessed from multiple translation units.
// - keep enum order stable where used as direct matrix indices.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// kernel.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - construct and destruct CKernel subsystem objects and baseline runtime defaults.

// needed headers / dependencies:
// - kernel.h plus Circle subsystem classes instantiated as CKernel members.

// needed variables:
// - all CKernel member instances initialized in ctor initializer list.

// key functions / entry points:
// - CKernel::CKernel(void)
// - CKernel::~CKernel(void)

// implementation notes:
// - constructor order must match dependency order expected by Initialize().

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// kernel_initialize.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - hardware and service initialization stage before runtime loop starts.

// needed headers / dependencies:
// - boot-time Circle services (exception/timer/interrupt/fs/usb/gpio/etc.).

// needed variables:
// - CKernel member services + configuration flags used to gate startup path.

// key functions / entry points:
// - boolean CKernel::Initialize(void)

// implementation notes:
// - return FALSE immediately on mandatory subsystem initialization failure.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// kernel_run.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - runtime orchestration loop and shutdown mode selection.

// needed headers / dependencies:
// - initialized subsystems from Initialize(); wrapper pipeline and render path.

// needed variables:
// - loop state, mode state, input/output matrices, selected assets/program/frame state.

// key functions / entry points:
// - TShutdownMode CKernel::Run(void)

// implementation notes:
// - keep pipeline order deterministic: io -> mode application -> render -> maintenance.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// bufferToScreen.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - direct framebuffer text debug output independent from CScreen/CString output stack.

// needed headers / dependencies:
//     #include <circle/bcmframebuffer.h>
//     #include <circle/chargenerator.h>

// needed variables:
//     static CBcmFrameBuffer gE_FrameBuffer (0, 0, 32, 0, TRUE)
//     static CCharGenerator  gE_CharGenerator
//     static u32*      gE_PixelBuffer
//     static unsigned  gE_PitchBytes, gE_ScreenWidth, gE_ScreenHeight
//     static unsigned  gE_CharWidth, gE_CharHeight, gE_Cols, gE_Rows

// key functions / entry points:
// - screen_plot(unsigned x, unsigned y, u32 color)
// - screen_draw_char(char ch, unsigned charCol, unsigned charRow, u32 fgColor, u32 bgColor)
// - screen_init(void)
// - screen_clear_screen(u32 bgColor)
// - screen_draw_buffer_segment(...)
// - screen_get_grid(unsigned &cols, unsigned &rows)

// implementation notes:
// - these globals define the debug text plane geometry; re-init required after display mode changes.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// startupScreen.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - serialize startup diagnostics (machine, clocks, options, DMA channel) into a text buffer.

// needed headers / dependencies:
// - machine info and options services already active in CKernel.

// needed variables:
// - m_MachineInfo
// - m_Options

// key functions / entry points:
// - bool CKernel::startupScreen(char* buffer, u32& index)

// implementation notes:
// - written text is intended for screen buffer and/or log file persistence.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// logging.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - low-level formatted append helpers to build text logs in fixed memory buffers.

// needed headers / dependencies:
// - integer/string formatting support from the kernel environment.

// needed variables:
// - target buffer pointer + rolling write index passed by reference.

// key functions / entry points:
// - storeLog(char* buffer, u32& index, const char* label, u32 value0, u32 value1, u32 value2, u32 value3, u32 value4)
// - storeMsg(char* buffer, u32& index, const char* label, const void* tx_msg, u32 total_size)
// - nextline(char* buffer, u32& index)

// implementation notes:
// - keep all callers consistent about index ownership to avoid overlapping log writes.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// memory.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - allocate and release grouped buffers for filesystem payloads (shader, texture, video, frame, log, kernel).

// needed headers / dependencies:
//     #include <circle/memory.h>
//     #include <circle/new.h>

// needed variables:
// - buffer pointer arrays: m_bufferVsh/m_bufferOmf/m_bufferFsh/m_bufferOmt/m_bufferTex/m_bufferVid/m_bufferKnl/m_bufferLog/m_bufferFrA/m_bufferFrB
// - raw/aligned dma tracking: m_*RawBlock, m_*BlockBase, m_*BlockSize
// - filecounter[][] sizes used during allocation planning

// key functions / entry points:
// - alllocateBufferMEM(size_t count, size_t bufferSize)
// - alllocateBufferDMA(size_t count, size_t bufferSize, char*& rawBlock, char*& alignedBlock, size_t& alignedTotalSize)
// - clearBufferMEM(char** buffers, size_t count)
// - clearBufferDMA(char** buffers, char* rawBlock)
// - wrapperInitDMA(), wrapperInitMEM()
// - wrapperDMAcleanUp(), wrapperMEMcleanUp()

// implementation notes:
// - allocation wrappers establish cross-module storage contract used by filesystem and gfx loaders.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// filesystem.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - file system lifecycle + file scan/filter/load/save + bulk asset loading.

// needed headers / dependencies:
// - FAT filesystem and block devices (SD/eMMC, USB mass storage partitions).

// needed variables:
// - unsigned g_hFile (active file handle)
// - per-filetype arrays: g_ScnVsh/g_ScnOmf/g_ScnFsh/g_ScnOmt/g_ScnTex/g_ScnVid/g_ScnKln
// - loaded-byte arrays: g_bytVsh/g_bytOmf/g_bytFsh/g_bytOmt/g_bytTex/g_bytVid/g_bytKln
// - extension arrays: g_SufVsh/g_SufOmf/g_SufFsh/g_SufOmt/g_SufTex/g_SufVid/g_SufKln
// - limits and counters from filecounter[FT_*][FLD_*]

// key functions / entry points:
// - Mount(const char* p_deviceName)
// - UnMount()
// - openFile(const char* p_fileName)
// - loadToBuffer(char* p_buffer, unsigned p_bufferSize)
// - saveToBuffer(const char* p_fileName, const char* p_buffer, unsigned p_bufferSize)
// - closeFile()
// - bulkLoad(char* p_fileNameArray[], unsigned p_loadedBytes[], char** p_bufferArray, int p_maxFiles, int& p_validFiles, int& p_prevFiles, unsigned p_fileSize)
// - IsValidFile(const char* pFileName, const char* extension)
// - scanRoot(char** p_fileNameArray, const char* p_fileExtension[], int p_extentionCount, int& p_scannedFiles, unsigned p_maxFiles)
// - updateUSB(const char* p_deviceName)
// - removeUSB(CDevice* f_partitionName, void* p_pContext)

// implementation notes:
// - scan and load counters are intentionally externalized so wrappers can compose staged pipelines.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// filesystem_user.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - user-level file operation stub for mode/state persistence.

// needed headers / dependencies:
// - filesystem helpers and mode buffers.

// needed variables:
// - g_centralModeBuffer[][] (source for serialization path).

// key functions / entry points:
// - util_save_modes_file()

// implementation notes:
// - currently placeholder; behavior and format still need consolidation with wrappers/menu workflow.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// wrappers.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - grouped high-level runtime steps for loading, init, io processing, and mode dispatch.

// needed headers / dependencies:
// - filesystem, memory, gfx init, adc read, mode application utilities.

// needed variables:
// - filecounter[][], g_Scn*, g_byt*, m_buffer* arrays
// - glsl state members and current program/texture/frame selectors

// key functions / entry points:
// - wrapper_from_sd()
// - wrapper_load_usb()
// - wrapper_init_gl_sd()
// - wrapper_init_gl_usb()
// - wrapper_io()
// - wrapper_modes()

// implementation notes:
// - wrappers are the concrete implementation of the intended pipeline segmentation.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// readADC.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - read ADC channels, perform short-window smoothing, produce scaled int/float control and audio-envelope signals.

// needed headers / dependencies:
// - ADC device abstraction and kernel mode/attenuation tables.

// needed variables:
// - g_inOutMatrixInt[][] / g_inOutMatrixFlt[][]
// - g_centralModeBuffer[][] fields for attenuation and sensitivity (SENS_A..SENS_D)
// - g_irregularity[4], menu_map_max[4] and internal ring/hold state for transient detection

// key functions / entry points:
// - readADC()

// implementation notes:
// - deterministic ring-buffer processing enables repeatable control and menu scaling behavior.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// menu_final.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - map menu layer state into channel mode behavior and apply processing transforms.

// needed headers / dependencies:
// - mode tables, pickup flags, io matrices, button timing state.

// needed variables:
// - g_menuPickUpFlag[4*menu_layers]
// - g_centralModeBuffer[g_currentProgramBuffer][modetablecount]
// - g_modeMap[modetablecount][17]
// - g_inOutMatrixInt[][] / g_inOutMatrixFlt[][]

// key functions / entry points:
// - resetMenuPickupFlags()
// - modeMenuAssignGroup(uint8_t menu_id, uint8_t base)
// - applyModeToChannel(int channel)
// - modeADC(int channel)
// - modeTRG(int channel)
// - modeBPM(int channel)
// - modeLF1(int channel)
// - modeLF2(int channel)
// - modeAudioAb0(int channel)
// - modeAudioAb1(int channel)
// - modeAudioBb0(int channel)
// - modeAudioBb1(int channel)

// implementation notes:
// - this is the canonical mode dispatch layer; any new mode must be wired here and in mode tables.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// features.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - signal helper algorithms: randomization, BPM derivation, beat prediction, wavetable sampling.

// needed headers / dependencies:
// - timing source + waveform tables + io matrices.

// needed variables:
// - g_resultBPM[]
// - g_predictedTime[]
// - g_waveTable[WAVEFORMS][WAVESAMPLES]
// - g_inOutMatrixInt[][] / g_inOutMatrixFlt[][]

// key functions / entry points:
// - randomVec8(uint32_t p_seed)
// - calculate2BPM(unsigned long p_triggerTimeClockA, unsigned long p_triggerTimeClockB)
// - calculate1BPM(int source, unsigned long p_triggerTimeClock)
// - predictedNextBeat2()
// - predictedNextBeat1(int source)
// - sampleWaveTable()

// implementation notes:
// - functions here feed both rendering uniforms and menu-relevant timing indicators.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// util.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - runtime utility glue: parameter prep, asset chooser functions, mode-store helpers, button event processing.

// needed headers / dependencies:
// - mode matrices, file counters, button status arrays, runtime selectors.

// needed variables:
// - g_inOutMatrixInt[][]
// - g_centralModeBuffer[][]
// - g_buttonTS[][]
// - loaded/valid counters for shader/texture/video/frame selection

// key functions / entry points:
// - prepParameters()
// - chooseProgram(...)
// - chooseTexture(...)
// - chooseVideo(...)
// - chooseFrame(...)
// - storeModesV1()
// - storeModesV2()
// - buttonPing(int p_btn_id, int pin)
// - button_consumer(int p_btn_id)

// implementation notes:
// - chooser output must remain bounded by loaded/valid counts to prevent invalid resource indices.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// helpers.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - parser wrappers and update helpers used by higher-level wrappers/run path.

// needed headers / dependencies:
// - H.264 parser/decoder path and texture parser path.

// needed variables:
// - parser/decoder members and file index routing state.

// key functions / entry points:
// - parser_h264(int p_fromFile, int p_toFile)
// - parser_bmp(int p_fromFile, int p_toFile)
// - parser_o_bmp(int p_fileIndex)
// - checkUpdate()
// - Update()

// implementation notes:
// - keep parser entry points thin so wrappers control sequencing and resource ownership.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// table_colors.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - static RGB lookup table for UI and debug color mapping.

// needed headers / dependencies:
// - none beyond kernel constants/types.

// needed variables:
// - const unsigned char g_rgb_color_table[49][3]

// key functions / entry points:
// - none (data table translation unit)

// implementation notes:
// - index stability is important where menu/color modes rely on fixed slots.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// table_waveforms.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - static waveform lookup table used by LFO and waveform sampling utilities.

// needed headers / dependencies:
// - waveform dimension constants WAVEFORMS and WAVESAMPLES.

// needed variables:
// - unsigned long g_waveTable[WAVEFORMS][WAVESAMPLES]

// key functions / entry points:
// - none (data table translation unit)

// implementation notes:
// - dimensions and ordering must match sampler expectations in features.cpp.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_OGL.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - initialize DispmanX/EGL/GLES rendering context and surface binding.

// needed headers / dependencies:
//     #include "bcm_host.h"
//     #include <EGL/egl.h>
//     #include <GLES2/gl2.h>

// needed variables:
// - glsl_state context handles + nativewindow/dispmanx structures.

// key functions / entry points:
// - gfx_init_OGL(glsl_state* m_glsl, int p_width, int p_height)

// implementation notes:
// - must complete before shader/program/texture initialization in gfx_init.cpp.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_debug.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - collect compile/link/runtime GL diagnostics into text buffers for persistence/display.

// needed headers / dependencies:
// - GLES shader/program status APIs + logging buffer utilities.

// needed variables:
// - m_shaderStatusFlags[]
// - m_programStatusFlags[]

// key functions / entry points:
// - shaderLog(char* buffer, u32& index, GLint shader, int shaderIndex)
// - programLog(char* buffer, u32& index, GLint shader, int program_index)
// - gfx_check(char* buffer, u32& index, const char* file, unsigned line)

// implementation notes:
// - all three functions share the same buffer/index contract; caller controls sink and capacity.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_init.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - compile shaders, link programs, bind uniforms/attributes, allocate/upload textures, and setup fullscreen VBO.

// needed headers / dependencies:
// - GLES2 APIs + loaded shader/texture buffers from filesystem/memory wrappers.

// needed variables:
// - glsl_state arrays: gl_vsh_id[], gl_fsh_id[], gl_prg_id[], gl_tex_id[], u_tex_id[][]
// - glsl_state uniforms: u_time[], u_tres[], u_seed[], u_aud[], u_col[], u_par_a[], u_par_b[], u_tex_l[]
// - m_bufferVsh/m_bufferOmf/m_bufferFsh/m_bufferOmt/m_bufferTex payload buffers
// - loaded counters for shader/program/texture resource loops

// key functions / entry points:
// - initVshaders(glsl_state* m_glsl, ...)
// - initOshader(glsl_state* m_glsl, ...)
// - initFshaders(glsl_state* m_glsl, ...)
// - initOprogram(glsl_state* m_glsl, ...)
// - initFprograms(glsl_state* m_glsl, ...)
// - initOuniforms(glsl_state* m_glsl, ...)
// - initFuniforms(glsl_state* m_glsl, ...)
// - initOtexture(glsl_state* m_glsl, ...)
// - initUtextures(glsl_state* m_glsl, ...)
// - initVbuffer(glsl_state* m_glsl, ...)

// implementation notes:
// - overlay init path and user shader path must remain clearly separated but synchronized in lifecycle.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gfx_render.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - execute rendering passes (main pass + second pass/overlay flow) using current mode/io state.

// needed headers / dependencies:
// - initialized glsl_state + active programs/textures + uniform sources.

// needed variables:
// - g_inOutMatrixFlt[][]
// - g_centralModeBuffer[][]
// - active program/texture/video/frame indices

// key functions / entry points:
// - render_shader_a(glsl_state* m_glsl, ...)
// - render_shader_b(glsl_state* m_glsl, ...)

// implementation notes:
// - render functions are expected to consume already-prepared selectors and matrices from wrappers/util/menu.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// menu.fsh
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - fragment shader source for GPU-driven menu/overlay atlas pass.

// needed headers / dependencies:
// - GLES fragment shader compiler at runtime.

// needed variables:
// - uniforms consumed by menu pass (atlas texture, tile count, tile rect array, tile index array).

// key functions / entry points:
// - N/A (shader source file)

// implementation notes:
// - must stay aligned with uniform names expected by gpu_render_menu_* init/draw code.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gpu_menu_A.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - archived/alternative notes stage for menu GPU pipeline (currently not active implementation body).

// needed headers / dependencies:
// - references menu GPU state/layout constants.

// needed variables:
// - g_menu_state + MENU_GPU_* constants (if activated).

// key functions / entry points:
// - currently no active compiled entry points in this translation unit.

// implementation notes:
// - treat as design/reference variant; active implementation lives in gpu_menu_B.cpp / gpu_menu_C.cpp.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gpu_menu_B.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - GPU menu implementation variant B: load assets, compile/link menu shaders, build layout, update indices, draw, shutdown.

// needed headers / dependencies:
// - GLES2 APIs, menu atlas payload, shared fullscreen vertex shader/program path.

// needed variables:
// - g_menu_state
// - MENU_GPU_ORIGIN / MENU_GPU_TILE_SIZE / MENU_GPU_REL_POS / MENU_GPU_REL_SIZE / MENU_GPU_BACKGROUND_SCALE
// - g_centralModeBuffer[][] and g_resultBPM[] for runtime tile-index updates

// key functions / entry points:
// - gpu_render_helper_load_menu_fsh(...)
// - gpu_render_helper_load_menu_tex(...)
// - gpu_render_helper_compile_shader(...)
// - gpu_render_helper_link_program(...)
// - gpu_render_helper_build_menu_program(...)
// - gpu_render_helper_upload_menu_atlas_rgb24(...)
// - gpu_render_menu_init(...)
// - gpu_render_menu_rebuild_layout()
// - gpu_render_menu_update_indices()
// - gpu_render_menu_draw(GLuint fullscreen_vbo)
// - gpu_render_menu_shutdown()

// implementation notes:
// - this file is a complete menu pass pipeline and should be the baseline for further cleanup.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// gpu_menu_C.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - GPU menu implementation variant C (parallel variant with same functional stages as B).

// needed headers / dependencies:
// - same as gpu_menu_B.cpp (GLES2 + menu state/constants).

// needed variables:
// - g_menu_state, MENU_GPU_* constants, g_centralModeBuffer[][], g_resultBPM[]

// key functions / entry points:
// - gpu_render_helper_compile_shader(...)
// - gpu_render_helper_link_program(...)
// - gpu_render_helper_build_menu_program(...)
// - gpu_render_helper_upload_menu_atlas_rgb24(...)
// - gpu_render_menu_init(...)
// - gpu_render_menu_rebuild_layout()
// - gpu_render_menu_update_indices()
// - gpu_render_menu_draw(GLuint fullscreen_vbo)
// - gpu_render_menu_shutdown()

// implementation notes:
// - maintain one canonical implementation eventually; duplicated variants increase maintenance overhead.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INFO-README.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - ad-hoc function inventory and migration notes used as historical scratchpad/reference.

// needed headers / dependencies:
// - none required for runtime (documentation-only content).

// needed variables:
// - N/A (text/reference only).

// key functions / entry points:
// - none (contains textual listings of functions from many modules).

// implementation notes:
// - useful as raw inventory but superseded by this README for normalized style.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INFO-cutouts.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - scratchpad for extracted experiments/snippets and explanatory notes.

// needed headers / dependencies:
// - depends on snippet being copied; not core runtime source of truth.

// needed variables:
// - mixed prototype/reference variables for menu/audio/parser notes.

// key functions / entry points (snippet-level references):
// - audioEnergy(...)
// - GenerateH264ParserInfo(...)
// - GenerateBmpParserInfo(...)
// - GenerateBmpOverlayInfo(...)
// - display_startup_screen(...)

// implementation notes:
// - treat as notes repository; not a stable production module contract.

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// INFO-roadmap.cpp
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// purpose:
// - roadmap/planning notes and migration ideas for future cleanup.

// needed headers / dependencies:
// - none required for runtime (planning text).

// needed variables:
// - N/A (documentation/planning).

// key functions / entry points:
// - contains draft references like display_startup_screen variants.

// implementation notes:
// - planning document; keep implementation commitments in active modules and this README.