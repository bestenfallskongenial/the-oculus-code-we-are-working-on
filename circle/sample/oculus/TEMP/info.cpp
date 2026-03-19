# Derived interfaces from function usage (excluding `kernel.h/.cpp` and `kernel_initialize.cpp`, `kernel_run.cpp`)

This file derives likely array/enum/struct shapes from call-sites and indexing patterns in the active `.cpp` files.

## 1) Overlay tile struct and fixed arrays

```cpp
struct MenuTileRect
{
    float x;
    float y;
    float w;
    float h;
};

static constexpr int MENU_GPU_TILE_COUNT = 16;
static const float kMenuRelPos[MENU_GPU_TILE_COUNT][2];
static const float kMenuRelSize[MENU_GPU_TILE_COUNT][2];
```

Why: overlay render fills `tile_rect[16]`, writes `x/y/w/h`, and uploads it as `glUniform4fv(..., 16, (GLfloat*)tile_rect)`.

## 2) Mode table index enum and central mode buffer

Observed indices used on `g_centralModeBuffer[program][index]`:
- channel slots: `CH0_MODE..CH7_MODE` (8 slots)
- LFO params: `LF1_WAVE`, `LF2_WAVE`, `LF1_MULT`, `LF2_MULT` (4 slots)
- sensitivity params: `SENS_A..SENS_D` (4 slots)
- storage flag: `is_stored` (1 slot)
- texture mode is consumed in renderer: `TEX_MODE`

The menu system uses 4 groups × 4 slots (`base=0,4,8,12`), i.e. **16 logical mode slots**.

Most consistent shape from usage:

```cpp
enum ModeTableIndex
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
    // optional extensions seen elsewhere in comments/docs: TEX_MODE, FRM_MODE, is_stored
    // if TEX_MODE/is_stored must coexist with 16-slot UI map, one of them must be remapped or table widened.
};

// Program profile buffer indexed by shader/program id and ModeTableIndex:
int g_centralModeBuffer[/*program_slot_count*/][/*mode_slot_count*/];
```

**Important conflict from usage itself:**
- `modeMenuAssignGroup(... base 12)` implies slot range `[0..15]` is live config data.
- renderer also uses `TEX_MODE`; mode-store logic uses `is_stored`.
- therefore final canonical enum either needs >16 entries or a separate metadata table for `is_stored`.

## 3) Button state matrix enum and array

Direct usage requires exactly 5 per-button fields:

```cpp
enum ButtonStateField
{
    BTN_SINGLE = 0,
    BTN_DOUBLE,
    BTN_PRESS_START,
    BTN_RELEASE,
    BTN_HOLD_TICK,
    BTN_FIELD_COUNT
};

unsigned int g_buttons_states[2][BTN_FIELD_COUNT];
```

Why: button code resets `SINGLE/DOUBLE`, tracks press start/release, increments hold tick; two physical buttons are polled.

## 4) File type / file field enums for `filecounter`

`filecounter[type][field]` is used by wrappers for scan/load/init paths.

Used file types:
- `FT_VSH`, `FT_OMF`, `FT_FSH`, `FT_OMT`, `FT_TEX`, `FT_VID`, `FT_KLN`

Used fields:
- `FLD_EXTCNT`, `FLD_SCANNED`, `FLD_MAXSD`, `FLD_MAXUSB`, `FLD_LOADED`, `FLD_PREV`, `FLD_SIZE`

Most consistent declaration from usage:

```cpp
enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FT_COUNT
};

enum FileField
{
    FLD_EXTCNT = 0,
    FLD_SCANNED,
    FLD_MAXSD,
    FLD_MAXUSB,
    FLD_LOADED,
    FLD_PREV,
    FLD_SIZE,
    FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT];
```

## 5) In/out signal matrix layout

`g_inOutMatrixFlt[ch][field]` and `g_inOutMatrixInt[ch][field]` are indexed with:
- base ADC/input pipeline: `in`, `raw`, `val`, `out`
- random/LFO/audio/event fields: `rnd`, `lf1`, `lf2`, `au0`, `au1`, `au2`, `au3`, `trH`, `trL`, `trF`

Conservative enum shape inferred from usage:

```cpp
enum InOutField
{
    in = 0,
    raw,
    val,
    out,
    rnd,
    trH,
    trL,
    trF,
    lf1,
    lf2,
    au0,
    au1,
    au2,
    au3,
    INOUT_FIELD_COUNT
};

float g_inOutMatrixFlt[8][INOUT_FIELD_COUNT];
int   g_inOutMatrixInt[8][INOUT_FIELD_COUNT];
```

## 6) Mode map table shape

Usage pattern:
- `g_modeMap[channel_or_slot][0]` stores per-slot max selectable range.
- `g_modeMap[channel][selected + 1]` maps menu selection to execution mode id (`0..11` used in switch).
- indexed with base offsets up to `base+3` where `base` can be 12.

Minimum shape from usage:

```cpp
// first dimension must cover at least 16 slot rows
// second dimension must cover [0] + selectable entries (at least up to index 12)
uint8_t g_modeMap[16][13];
```

## 7) GLSL runtime state struct

Fields required by EGL/OGL init, shader compile/link, uniforms, and draw:

```cpp
struct glsl_state
{
    // EGL / windowing
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
    uint32_t    screen_width;
    uint32_t    screen_height;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_ELEMENT_HANDLE_T dispman_element;

    // GL objects
    GLuint gl_buf;
    GLint  gl_vtx;

    GLuint gl_vsh_id[/*vsh_max*/];
    GLuint gl_fsh_id[/*fsh_max*/];
    GLuint gl_prg_id[/*fsh_max*/];

    // overlay shader/program/texture
    GLuint gl_oms_id[1];
    GLuint gl_omp_id[1];
    GLuint gl_omt_id[1];

    // user textures
    GLuint gl_tex_id[/*tex_loaded_max*/];

    // uniforms for user shader programs
    GLint u_time [/*fsh_max*/];
    GLint u_tres [/*fsh_max*/];
    GLint u_seed [/*fsh_max*/];
    GLint u_aud  [/*fsh_max*/];
    GLint u_col  [/*fsh_max*/];
    GLint u_par_a[/*fsh_max*/];
    GLint u_par_b[/*fsh_max*/];
    GLint u_tex_l[/*fsh_max*/];
    GLint u_tex_id[/*fsh_max*/][10];

    // uniforms for overlay program
    GLint u_atlas[1];
    GLint u_tile_count[1];
    GLint u_tile_rect[1];
    GLint u_tile_index[1];
};
```

## 8) Buffer pointer families implied by loaders/parsers

Bulk loaders and init functions require these pointer matrices:

```cpp
char* m_bufferVsh[/*vsh_file_slots*/];
char* m_bufferOmf[/*omf_file_slots*/];
char* m_bufferFsh[/*fsh_file_slots*/];
char* m_bufferOmt[/*omt_file_slots*/];
char* m_bufferTex[/*tex_file_slots*/];
char* m_bufferVid[/*vid_file_slots*/];
char* m_bufferKnl[/*kln_file_slots*/];
```

Plus scan name arrays / byte counters per family:

```cpp
char*    g_ScnVsh[]; unsigned g_bytVsh[]; const char* g_SufVsh[];
char*    g_ScnOmf[]; unsigned g_bytOmf[]; const char* g_SufOmf[];
char*    g_ScnFsh[]; unsigned g_bytFsh[]; const char* g_SufFsh[];
char*    g_ScnOmt[]; unsigned g_bytOmt[]; const char* g_SufOmt[];
char*    g_ScnTex[]; unsigned g_bytTex[]; const char* g_SufTex[];
char*    g_ScnVid[]; unsigned g_bytVid[]; const char* g_SufVid[];
char*    g_ScnKln[]; unsigned g_bytKln[]; const char* g_SufKln[];
```

---

If you want, next step can be: I generate a **single canonical `kernel_types.h` patch** with these inferred enums/structs/externs so compile errors point only to true logic issues.

# Derived interface sketch (from function bodies only)

Scope used for derivation:
- Read: `circle/sample/oculus/README.md`.
- Scanned only files directly under `circle/sample/oculus` (no subfolders).
- Explicitly ignored: `kernel.h`, `kernel.cpp`, `kernel_initialize.cpp`, `kernel_run.cpp`.

## 1) Arrays and enum families inferred from usage

### `g_centralModeBuffer`
Observed as `g_centralModeBuffer[program][mode_slot]` in active function code.
- First index = program slot (`f_buffer`, `g_currentProgramBuffer`, `g_current_gl_program`).
- Second index = mode selector token.

Minimum token set required by functions:
- Channels: `CH0_MODE..CH7_MODE`
- LFO config: `LF1_WAVE`, `LF2_WAVE`, `LF1_MULT`, `LF2_MULT`
- Sensitivity: `SENS_A`, `SENS_B`, `SENS_C`, `SENS_D`
- Toggles/flags: `TEX_MODE`, `FRM_MODE`, `is_stored`

Inferred enum sketch:
```cpp
enum modetable {
  CH0_MODE, CH1_MODE, CH2_MODE, CH3_MODE,
  CH4_MODE, CH5_MODE, CH6_MODE, CH7_MODE,
  LF1_WAVE, LF2_WAVE,
  LF1_MULT, LF2_MULT,
  SENS_A, SENS_B, SENS_C, SENS_D,
  TEX_MODE, FRM_MODE,
  is_stored,
  modetablecount
};

int g_centralModeBuffer[/*program_slots*/][modetablecount];
```

---

### `g_inOutMatrixInt` / `g_inOutMatrixFlt`
Observed pattern is `[channel][field]` with channels 0..7.

Required field tokens from function usage:
- `raw`, `val`, `in`, `out`, `rnd`
- trigger fields: `trH`, `trL`, `trF`
- LFO outputs: `lf1`, `lf2`
- audio bands: `au0`, `au1`, `au2`, `au3`

Inferred enum sketch:
```cpp
enum io_field {
  raw, val, in, out, rnd,
  trH, trL, trF,
  lf1, lf2,
  au0, au1, au2, au3,
  io_field_count
};

int   g_inOutMatrixInt[8][io_field_count];
float g_inOutMatrixFlt[8][io_field_count];
```

---

### `filecounter`
Used everywhere as `filecounter[file_type][file_field]`.

Inferred `FileType` tokens used by functions:
- `FT_VSH`, `FT_OMF`, `FT_FSH`, `FT_OMT`, `FT_TEX`, `FT_VID`, `FT_KLN`, `FRM_BF`, `LOGGER`

Inferred `FileField` tokens used by functions:
- `FLD_EXTCNT`, `FLD_SCANNED`, `FLD_MAXSD`, `FLD_MAXUSB`, `FLD_LOADED`, `FLD_PREV`, `FLD_SIZE`

Inferred sketch:
```cpp
enum FileType {
  FT_VSH, FT_OMF, FT_FSH, FT_OMT, FT_TEX, FT_VID, FT_KLN,
  FRM_BF, LOGGER,
  FT_COUNT
};

enum FileField {
  FLD_EXTCNT,
  FLD_SCANNED,
  FLD_MAXSD,
  FLD_MAXUSB,
  FLD_LOADED,
  FLD_PREV,
  FLD_SIZE,
  FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT];
```

---

### `g_modeMap` and `g_menuPickUpFlag`
Usage pattern:
- `g_modeMap[channel_or_slot][0]` stores max range for mapping.
- `g_modeMap[channel][selected + 1]` stores runtime mode IDs.
- `g_menuPickUpFlag[index]` is boolean latch per menu slot.

Inferred sketch:
```cpp
int  g_modeMap[modetablecount][/*1 + max menu choices*/];
bool g_menuPickUpFlag[modetablecount];
```

---

### `g_buttons_states`
Function behavior requires per-button state with these fields:
- `BTN_SINGLE`, `BTN_DOUBLE`, `BTN_PRESS_START`, `BTN_RELEASE`, `BTN_HOLD_TICK`

Inferred sketch:
```cpp
enum ButtonStateField {
  BTN_SINGLE,
  BTN_DOUBLE,
  BTN_PRESS_START,
  BTN_RELEASE,
  BTN_HOLD_TICK,
  BTN_STATE_COUNT
};

unsigned int g_buttons_states[2][BTN_STATE_COUNT];
```

---

### Wave/color static tables
Directly defined in source:
```cpp
unsigned long        g_waveTable[WAVEFORMS][WAVESAMPLES];
const unsigned char  g_rgb_color_table[49][3];
```

From usage, `WAVESAMPLES` must be 256 (`sampleWaveTable` clamps sample index to 255).

## 2) Structs inferred from function bodies

### Overlay tile rect payload
Defined in `OVrender.cpp` and passed to `glUniform4fv`:
```cpp
struct MenuTileRect {
  float x;
  float y;
  float w;
  float h;
};
```

### `glsl_state` minimum overlay members required
Function bodies require these members (overlay path):
```cpp
struct glsl_state {
  GLuint gl_buf;
  GLint  gl_vtx;

  GLuint gl_omp_id[/*>=1*/];
  GLuint gl_oms_id[/*>=1*/];
  GLuint gl_omt_id[/*>=1*/];

  GLint  u_atlas[/*>=1*/];
  GLint  u_tile_count[/*>=1*/];
  GLint  u_tile_rect[/*>=1*/];
  GLint  u_tile_index[/*>=1*/];

  // plus regular shader/program arrays used elsewhere:
  GLuint gl_vsh_id[/*...*/];
  GLuint gl_fsh_id[/*...*/];
  GLuint gl_prg_id[/*...*/];

  GLint  u_time[/*...*/];
  GLint  u_tres[/*...*/];
  GLint  u_seed[/*...*/];
  GLint  u_aud[/*...*/];
  GLint  u_col[/*...*/];
  GLint  u_par_a[/*...*/];
  GLint  u_par_b[/*...*/];
  GLint  u_tex_l[/*...*/];
  GLint  u_tex_id[/*...*/][10];

  GLuint gl_tex_id[/*...*/];
};
```

## 3) Important consistency note (from functions only)

- Function code uses `is_stored` (lowercase) as the `g_centralModeBuffer` state slot.
- If header/other draft declarations use `IS_STORED`, those declarations are not aligned with these function bodies.

# Derived definitions from function usage (excluding `kernel.h/.cpp` and `kernel_initialize.cpp` / `kernel_run.cpp`)

This file reconstructs probable enum/array/struct layouts **from call-sites and indexing usage only**.

## 1) `g_centralModeBuffer` mode-slot index enum

Observed indexes used by functions:
- `CH0_MODE..CH7_MODE`
- `LF1_WAVE`, `LF2_WAVE`
- `LF1_MULT`, `LF2_MULT`
- `SENS_A..SENS_D`
- `TEX_MODE`
- `is_stored`

A consistent inferred enum order is:

```cpp
enum modetable {
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

    TEX_MODE,
    is_stored,

    modetablecount
};
```

And buffer shape:

```cpp
// first index = shader/program slot (includes DEFAULT_SLOT)
// second index = modetable enum
int g_centralModeBuffer[MAX_PROGRAM_SLOTS][modetablecount];
```

## 2) `g_buttons_states` + button event enum

Function usage (`buttonPing`, `button_consumer`) requires 5 columns:

```cpp
enum ButtonStateField {
    BTN_SINGLE = 0,
    BTN_DOUBLE,
    BTN_PRESS_START,
    BTN_RELEASE,
    BTN_HOLD_TICK,
    BTN_FIELD_COUNT
};

// 2 buttons (A/B), 5 fields each
unsigned int g_buttons_states[2][BTN_FIELD_COUNT] = {0};
```

## 3) in/out matrix field enum

Functions index `g_inOutMatrixInt/Flt` using:
- `raw`, `val`, `rnd`, `out`
- trigger fields: `trH`, `trL`, `trF`
- lfo fields: `lf1`, `lf2`
- audio fields: `au0..au3`

A coherent enum:

```cpp
enum InOutField {
    in = 0,
    raw,
    val,
    rnd,
    trH,
    trL,
    trF,
    lf1,
    lf2,
    au0,
    au1,
    au2,
    au3,
    out,
    INOUT_FIELD_COUNT
};

int   g_inOutMatrixInt[8][INOUT_FIELD_COUNT];
float g_inOutMatrixFlt[8][INOUT_FIELD_COUNT];
```

## 4) `filecounter` 2D table + enums

Usage in wrappers/memory/helpers implies first-dimension file groups:
- `FT_VSH`, `FT_OMF`, `FT_FSH`, `FT_OMT`, `FT_TEX`, `FT_VID`, `FT_KLN`, `FRM_BF`, `LOGGER`

Second dimension fields used:
- `FLD_EXTCNT`, `FLD_SCANNED`, `FLD_LOADED`, `FLD_PREV`, `FLD_SIZE`, `FLD_MAXSD`, `FLD_MAXUSB`

Inferred definition:

```cpp
enum FileType {
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FRM_BF,
    LOGGER,
    FT_COUNT
};

enum FileField {
    FLD_EXTCNT = 0,
    FLD_SCANNED,
    FLD_LOADED,
    FLD_PREV,
    FLD_SIZE,
    FLD_MAXSD,
    FLD_MAXUSB,
    FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT];
```

## 5) `g_modeMap` shape

Usage pattern:
- `g_modeMap[channel][0]` stores scale/range count for a channel/menu slot.
- `g_modeMap[channel][selected + 1]` maps selected value to mode ID dispatch.

Minimum shape from usage:

```cpp
// channel dimension must at least cover indexes used via base+0..base+3 and channels 0..7
// second dimension needs [0] plus mapped entries (>= 12 mode IDs seen).
int g_modeMap[modetablecount][13];
```

(Second dimension can be larger; 13 is just the strict lower bound from observed dispatch IDs `0..11` plus slot `[0]`.)

## 6) parser tables / waveform/color tables

Direct definitions observed in source:

```cpp
const unsigned char g_rgb_color_table[49][3];
unsigned long g_waveTable[WAVEFORMS][WAVESAMPLES];
```

`sampleWaveTable()` clamps to `0..255`, so `WAVESAMPLES` is at least `256`.

## 7) overlay tile structs/arrays

From overlay renderer:

```cpp
struct MenuTileRect {
    float x;
    float y;
    float w;
    float h;
};

static const int MENU_GPU_TILE_COUNT = 16;
MenuTileRect tile_rect[MENU_GPU_TILE_COUNT];
int          tile_index[MENU_GPU_TILE_COUNT];
```

## 8) `glsl_state` required members (from OpenGL usage)

The following members are required because they are dereferenced in runtime functions:

```cpp
struct glsl_state {
    // EGL / display
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGL_DISPMANX_DISPLAY_HANDLE_T dispman_display;
    EGL_DISPMANX_ELEMENT_HANDLE_T dispman_element;
    uint32_t screen_width;
    uint32_t screen_height;

    // GL objects
    GLuint gl_buf;
    GLuint gl_vsh_id[MAX_SHADERS];
    GLuint gl_fsh_id[MAX_SHADERS];
    GLuint gl_prg_id[MAX_SHADERS];

    // Overlay shader/program/texture
    GLuint gl_oms_id[1];
    GLuint gl_omp_id[1];
    GLuint gl_omt_id[1];

    // User textures
    GLuint gl_tex_id[MAX_TEXTURES];

    // Attributes/uniforms
    GLint gl_vtx;
    GLint u_time[MAX_SHADERS];
    GLint u_tres[MAX_SHADERS];
    GLint u_seed[MAX_SHADERS];
    GLint u_aud[MAX_SHADERS];
    GLint u_col[MAX_SHADERS];
    GLint u_par_a[MAX_SHADERS];
    GLint u_par_b[MAX_SHADERS];
    GLint u_tex_l[MAX_SHADERS];
    GLint u_tex_id[MAX_SHADERS][10];

    // Overlay uniforms
    GLint u_atlas[1];
    GLint u_tile_count[1];
    GLint u_tile_rect[1];
    GLint u_tile_index[1];
};
```

Strict lower bounds from usage:
- `u_tex_id[][0..9]` => second dimension must be at least `10`.
- overlay arrays accessed at index `[0]` => size at least `1`.

## 9) Notes on naming conflicts found from function use

- `is_stored` appears as lowercase at runtime call-sites (even if some docs mention uppercase token).
- `button_consumer` function name differs from `button_consume` call-site in wrappers.
- `button_consumer` uses assignment in conditions (`=10`, `=20`), likely intended comparisons.

These are behavioral/code consistency issues but do not change the inferred shapes above.

# Derived interfaces from function usage (excluding `kernel.h/.cpp` and `kernel_initialize.cpp`, `kernel_run.cpp`)

This file derives likely array/enum/struct shapes from call-sites and indexing patterns in the active `.cpp` files.

## 1) Overlay tile struct and fixed arrays

```cpp
struct MenuTileRect
{
    float x;
    float y;
    float w;
    float h;
};

static constexpr int MENU_GPU_TILE_COUNT = 16;
static const float kMenuRelPos[MENU_GPU_TILE_COUNT][2];
static const float kMenuRelSize[MENU_GPU_TILE_COUNT][2];
```

Why: overlay render fills `tile_rect[16]`, writes `x/y/w/h`, and uploads it as `glUniform4fv(..., 16, (GLfloat*)tile_rect)`.

## 2) Mode table index enum and central mode buffer

Observed indices used on `g_centralModeBuffer[program][index]`:
- channel slots: `CH0_MODE..CH7_MODE` (8 slots)
- LFO params: `LF1_WAVE`, `LF2_WAVE`, `LF1_MULT`, `LF2_MULT` (4 slots)
- sensitivity params: `SENS_A..SENS_D` (4 slots)
- storage flag: `is_stored` (1 slot)
- texture mode is consumed in renderer: `TEX_MODE`

The menu system uses 4 groups × 4 slots (`base=0,4,8,12`), i.e. **16 logical mode slots**.

Most consistent shape from usage:

```cpp
enum ModeTableIndex
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
    // optional extensions seen elsewhere in comments/docs: TEX_MODE, FRM_MODE, is_stored
    // if TEX_MODE/is_stored must coexist with 16-slot UI map, one of them must be remapped or table widened.
};

// Program profile buffer indexed by shader/program id and ModeTableIndex:
int g_centralModeBuffer[/*program_slot_count*/][/*mode_slot_count*/];
```

**Important conflict from usage itself:**
- `modeMenuAssignGroup(... base 12)` implies slot range `[0..15]` is live config data.
- renderer also uses `TEX_MODE`; mode-store logic uses `is_stored`.
- therefore final canonical enum either needs >16 entries or a separate metadata table for `is_stored`.

## 3) Button state matrix enum and array

Direct usage requires exactly 5 per-button fields:

```cpp
enum ButtonStateField
{
    BTN_SINGLE = 0,
    BTN_DOUBLE,
    BTN_PRESS_START,
    BTN_RELEASE,
    BTN_HOLD_TICK,
    BTN_FIELD_COUNT
};

unsigned int g_buttons_states[2][BTN_FIELD_COUNT];
```

Why: button code resets `SINGLE/DOUBLE`, tracks press start/release, increments hold tick; two physical buttons are polled.

## 4) File type / file field enums for `filecounter`

`filecounter[type][field]` is used by wrappers for scan/load/init paths.

Used file types:
- `FT_VSH`, `FT_OMF`, `FT_FSH`, `FT_OMT`, `FT_TEX`, `FT_VID`, `FT_KLN`

Used fields:
- `FLD_EXTCNT`, `FLD_SCANNED`, `FLD_MAXSD`, `FLD_MAXUSB`, `FLD_LOADED`, `FLD_PREV`, `FLD_SIZE`

Most consistent declaration from usage:

```cpp
enum FileType
{
    FT_VSH = 0,
    FT_OMF,
    FT_FSH,
    FT_OMT,
    FT_TEX,
    FT_VID,
    FT_KLN,
    FT_COUNT
};

enum FileField
{
    FLD_EXTCNT = 0,
    FLD_SCANNED,
    FLD_MAXSD,
    FLD_MAXUSB,
    FLD_LOADED,
    FLD_PREV,
    FLD_SIZE,
    FLD_COUNT
};

int filecounter[FT_COUNT][FLD_COUNT];
```

## 5) In/out signal matrix layout

`g_inOutMatrixFlt[ch][field]` and `g_inOutMatrixInt[ch][field]` are indexed with:
- base ADC/input pipeline: `in`, `raw`, `val`, `out`
- random/LFO/audio/event fields: `rnd`, `lf1`, `lf2`, `au0`, `au1`, `au2`, `au3`, `trH`, `trL`, `trF`

Conservative enum shape inferred from usage:

```cpp
enum InOutField
{
    in = 0,
    raw,
    val,
    out,
    rnd,
    trH,
    trL,
    trF,
    lf1,
    lf2,
    au0,
    au1,
    au2,
    au3,
    INOUT_FIELD_COUNT
};

float g_inOutMatrixFlt[8][INOUT_FIELD_COUNT];
int   g_inOutMatrixInt[8][INOUT_FIELD_COUNT];
```

## 6) Mode map table shape

Usage pattern:
- `g_modeMap[channel_or_slot][0]` stores per-slot max selectable range.
- `g_modeMap[channel][selected + 1]` maps menu selection to execution mode id (`0..11` used in switch).
- indexed with base offsets up to `base+3` where `base` can be 12.

Minimum shape from usage:

```cpp
// first dimension must cover at least 16 slot rows
// second dimension must cover [0] + selectable entries (at least up to index 12)
uint8_t g_modeMap[16][13];
```

## 7) GLSL runtime state struct

Fields required by EGL/OGL init, shader compile/link, uniforms, and draw:

```cpp
struct glsl_state
{
    // EGL / windowing
    EGLDisplay  display;
    EGLSurface  surface;
    EGLContext  context;
    uint32_t    screen_width;
    uint32_t    screen_height;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_ELEMENT_HANDLE_T dispman_element;

    // GL objects
    GLuint gl_buf;
    GLint  gl_vtx;

    GLuint gl_vsh_id[/*vsh_max*/];
    GLuint gl_fsh_id[/*fsh_max*/];
    GLuint gl_prg_id[/*fsh_max*/];

    // overlay shader/program/texture
    GLuint gl_oms_id[1];
    GLuint gl_omp_id[1];
    GLuint gl_omt_id[1];

    // user textures
    GLuint gl_tex_id[/*tex_loaded_max*/];

    // uniforms for user shader programs
    GLint u_time [/*fsh_max*/];
    GLint u_tres [/*fsh_max*/];
    GLint u_seed [/*fsh_max*/];
    GLint u_aud  [/*fsh_max*/];
    GLint u_col  [/*fsh_max*/];
    GLint u_par_a[/*fsh_max*/];
    GLint u_par_b[/*fsh_max*/];
    GLint u_tex_l[/*fsh_max*/];
    GLint u_tex_id[/*fsh_max*/][10];

    // uniforms for overlay program
    GLint u_atlas[1];
    GLint u_tile_count[1];
    GLint u_tile_rect[1];
    GLint u_tile_index[1];
};
```

## 8) Buffer pointer families implied by loaders/parsers

Bulk loaders and init functions require these pointer matrices:

```cpp
char* m_bufferVsh[/*vsh_file_slots*/];
char* m_bufferOmf[/*omf_file_slots*/];
char* m_bufferFsh[/*fsh_file_slots*/];
char* m_bufferOmt[/*omt_file_slots*/];
char* m_bufferTex[/*tex_file_slots*/];
char* m_bufferVid[/*vid_file_slots*/];
char* m_bufferKnl[/*kln_file_slots*/];
```

Plus scan name arrays / byte counters per family:

```cpp
char*    g_ScnVsh[]; unsigned g_bytVsh[]; const char* g_SufVsh[];
char*    g_ScnOmf[]; unsigned g_bytOmf[]; const char* g_SufOmf[];
char*    g_ScnFsh[]; unsigned g_bytFsh[]; const char* g_SufFsh[];
char*    g_ScnOmt[]; unsigned g_bytOmt[]; const char* g_SufOmt[];
char*    g_ScnTex[]; unsigned g_bytTex[]; const char* g_SufTex[];
char*    g_ScnVid[]; unsigned g_bytVid[]; const char* g_SufVid[];
char*    g_ScnKln[]; unsigned g_bytKln[]; const char* g_SufKln[];
```

---

If you want, next step can be: I generate a **single canonical `kernel_types.h` patch** with these inferred enums/structs/externs so compile errors point only to true logic issues.