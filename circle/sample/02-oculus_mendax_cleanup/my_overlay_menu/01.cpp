#include <GLES2/gl2.h>

#include <stdint.h>
#include <string.h>

// -----------------------------------------------------------------------------
// menu_gpu.cpp
//
// Purpose:
//   Deterministic, minimal GPU overlay path for atlas-based system menu.
//   This file is intentionally standalone and integration-friendly.
//
// Design constraints baked in:
//   - GLES2 compatible
//   - Fullscreen fragment-pass overlay
//   - No FBO requirement
//   - No dynamic allocation
//   - CPU uploads only tile indices + fixed slot rectangles
// -----------------------------------------------------------------------------

static const int MENU_GPU_ATLAS_GRID            = 8;    // 8x8 tiles in atlas
static const int MENU_GPU_MAX_TILE_INDEX        = 63;   // 0..63
static const int MENU_GPU_SLOT_COUNT            = 20;   // deterministic upper bound
static const int MENU_GPU_MODE_SLOT_COUNT       = 8;    // mode slots from CH0..CH7
static const int MENU_GPU_BPM_DIGIT_SLOT_COUNT  = 4;    // e.g. 123.4

struct MenuGpuSlotRect
{
    float x;
    float y;
    float w;
    float h;
};

struct MenuGpuLayout
{
    MenuGpuSlotRect rect[MENU_GPU_SLOT_COUNT];
};

struct MenuGpuPayload
{
    int tile_index[MENU_GPU_SLOT_COUNT];
    int enabled[MENU_GPU_SLOT_COUNT];
};

struct MenuGpuContext
{
    GLuint program;
    GLint u_atlas;
    GLint u_screen;
    GLint u_origin;
    GLint u_slot_rect;
    GLint u_slot_tile;
    GLint u_slot_enable;

    GLuint atlas_texture;
    float screen_w;
    float screen_h;

    MenuGpuLayout layout;
};

namespace
{
    static const char* kMenuGpuVertexShader =
        "attribute vec4 vertex;\n"
        "varying vec2 v_uv;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
        "    v_uv = vertex.zw;\n"
        "}\n";

    static const char* kMenuGpuFragmentShader =
        "precision mediump float;\n"
        "const int SLOT_COUNT = 20;\n"
        "const float GRID = 8.0;\n"
        "uniform sampler2D u_atlas;\n"
        "uniform vec2 u_screen;\n"
        "uniform vec2 u_origin;\n"
        "uniform vec4 u_slot_rect[SLOT_COUNT];\n"
        "uniform float u_slot_tile[SLOT_COUNT];\n"
        "uniform float u_slot_enable[SLOT_COUNT];\n"
        "varying vec2 v_uv;\n"
        "\n"
        "vec4 sample_tile(float tile_index, vec2 pixel_pos, vec4 rect)\n"
        "{\n"
        "    float idx = clamp(floor(tile_index + 0.5), 0.0, 63.0);\n"
        "    float tx = mod(idx, GRID);\n"
        "    float ty = floor(idx / GRID);\n"
        "\n"
        "    vec2 local = (pixel_pos - rect.xy) / rect.zw;\n"
        "    vec2 tile_uv = (vec2(tx, ty) + local) / GRID;\n"
        "    return texture2D(u_atlas, tile_uv);\n"
        "}\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec2 pixel = v_uv * u_screen;\n"
        "    vec4 out_color = vec4(0.0);\n"
        "    float hit = 0.0;\n"
        "\n"
        "    for (int i = 0; i < SLOT_COUNT; ++i)\n"
        "    {\n"
        "        if (u_slot_enable[i] < 0.5)\n"
        "        {\n"
        "            continue;\n"
        "        }\n"
        "\n"
        "        vec4 r = u_slot_rect[i];\n"
        "        vec2 p0 = u_origin + r.xy;\n"
        "        vec2 p1 = p0 + r.zw;\n"
        "\n"
        "        if (pixel.x >= p0.x && pixel.x < p1.x && pixel.y >= p0.y && pixel.y < p1.y)\n"
        "        {\n"
        "            vec4 abs_rect = vec4(p0.x, p0.y, r.z, r.w);\n"
        "            out_color = sample_tile(u_slot_tile[i], pixel, abs_rect);\n"
        "            hit = max(hit, out_color.a);\n"
        "        }\n"
        "    }\n"
        "\n"
        "    if (hit < 0.001)\n"
        "    {\n"
        "        discard;\n"
        "    }\n"
        "\n"
        "    gl_FragColor = out_color;\n"
        "}\n";

    static GLuint menu_gpu_compile_shader(GLenum type, const char* source)
    {
        GLuint shader = glCreateShader(type);
        if (shader == 0)
        {
            return 0;
        }

        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);

        GLint ok = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (ok != GL_TRUE)
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    static int menu_gpu_clamp_tile(int idx)
    {
        if (idx < 0)
        {
            return 0;
        }
        if (idx > MENU_GPU_MAX_TILE_INDEX)
        {
            return MENU_GPU_MAX_TILE_INDEX;
        }
        return idx;
    }

    static int menu_gpu_digit_to_tile(int digit)
    {
        // Atlas convention for this module:
        // tiles 32..41 = '0'..'9'
        if (digit < 0)
        {
            digit = 0;
        }
        if (digit > 9)
        {
            digit = 9;
        }
        return 32 + digit;
    }

    static void menu_gpu_layout_default(MenuGpuLayout* layout)
    {
        // Explicit rectangles relative to origin (pixels).
        // Bread-and-butter layout: two 2x2 blocks + one bpm strip.

        const float icon = 32.0f;
        const float gap  = 4.0f;

        // CH0..CH3 : left 2x2 block
        layout->rect[0] = {   0.0f,          0.0f,          icon, icon };
        layout->rect[1] = {   icon + gap,    0.0f,          icon, icon };
        layout->rect[2] = {   0.0f,          icon + gap,    icon, icon };
        layout->rect[3] = {   icon + gap,    icon + gap,    icon, icon };

        // CH4..CH7 : right 2x2 block
        const float block_dx = (icon * 2.0f) + (gap * 6.0f);
        layout->rect[4] = {   block_dx + 0.0f,         0.0f,          icon, icon };
        layout->rect[5] = {   block_dx + icon + gap,   0.0f,          icon, icon };
        layout->rect[6] = {   block_dx + 0.0f,         icon + gap,    icon, icon };
        layout->rect[7] = {   block_dx + icon + gap,   icon + gap,    icon, icon };

        // BPM strip: 4 digits + dot + icon + scalable background
        const float bpm_y = (icon * 2.0f) + (gap * 4.0f);
        const float ch_w  = 24.0f;
        const float ch_h  = 32.0f;

        layout->rect[8]  = { 0.0f,               bpm_y, ch_w, ch_h };              // d0
        layout->rect[9]  = { ch_w + 2.0f,        bpm_y, ch_w, ch_h };              // d1
        layout->rect[10] = { (ch_w * 2.0f) + 4,  bpm_y, ch_w, ch_h };              // d2
        layout->rect[11] = { (ch_w * 3.0f) + 6,  bpm_y, ch_w, ch_h };              // d3
        layout->rect[12] = { (ch_w * 3.0f) + 6 + ch_w, bpm_y + 18.0f, 8.0f, 8.0f}; // dot
        layout->rect[13] = { (ch_w * 4.0f) + 24, bpm_y, 32.0f, 32.0f };            // bpm icon

        // Two scalable background tiles (activity feedback)
        layout->rect[14] = { -8.0f,              -8.0f,               88.0f, 88.0f };
        layout->rect[15] = { block_dx - 8.0f,    -8.0f,               88.0f, 88.0f };

        // Remaining slots disabled by default but explicit.
        for (int i = 16; i < MENU_GPU_SLOT_COUNT; ++i)
        {
            layout->rect[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
        }
    }

    static void menu_gpu_payload_from_state(
        MenuGpuPayload* payload,
        const uint8_t mode_storage_buffers[8],
        const int result_bpm[2])
    {
        memset(payload, 0, sizeof(*payload));

        for (int i = 0; i < MENU_GPU_MODE_SLOT_COUNT; ++i)
        {
            payload->tile_index[i] = menu_gpu_clamp_tile((int) mode_storage_buffers[i]);
            payload->enabled[i] = 1;
        }

        // Compose four digits as [BPM integer(3), BPM decimal(1)].
        // result_bpm[0] = integer part, result_bpm[1] = decimal digit or sub-value.
        const int bpm_int = result_bpm[0] < 0 ? 0 : result_bpm[0];
        const int bpm_dec = result_bpm[1] < 0 ? 0 : result_bpm[1];

        const int d0 = (bpm_int / 100) % 10;
        const int d1 = (bpm_int / 10)  % 10;
        const int d2 = bpm_int % 10;
        const int d3 = bpm_dec % 10;

        payload->tile_index[8]  = menu_gpu_digit_to_tile(d0);
        payload->tile_index[9]  = menu_gpu_digit_to_tile(d1);
        payload->tile_index[10] = menu_gpu_digit_to_tile(d2);
        payload->tile_index[11] = menu_gpu_digit_to_tile(d3);

        payload->enabled[8]  = 1;
        payload->enabled[9]  = 1;
        payload->enabled[10] = 1;
        payload->enabled[11] = 1;

        // Dot and BPM icon (fixed atlas indices for now)
        payload->tile_index[12] = 42; // '.'
        payload->enabled[12] = 1;

        payload->tile_index[13] = 20; // BPM icon
        payload->enabled[13] = 1;

        // Background tiles (fixed index, scalable via rect size)
        payload->tile_index[14] = 4;
        payload->tile_index[15] = 4;
        payload->enabled[14] = 1;
        payload->enabled[15] = 1;
    }
}

// -----------------------------------------------------------------------------
// Public integration surface (simple C-style hooks)
// -----------------------------------------------------------------------------

bool menu_gpu_init(MenuGpuContext* ctx, GLuint atlas_texture, float screen_w, float screen_h)
{
    if (!ctx)
    {
        return false;
    }

    memset(ctx, 0, sizeof(*ctx));

    GLuint vs = menu_gpu_compile_shader(GL_VERTEX_SHADER, kMenuGpuVertexShader);
    if (vs == 0)
    {
        return false;
    }

    GLuint fs = menu_gpu_compile_shader(GL_FRAGMENT_SHADER, kMenuGpuFragmentShader);
    if (fs == 0)
    {
        glDeleteShader(vs);
        return false;
    }

    ctx->program = glCreateProgram();
    if (ctx->program == 0)
    {
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    glAttachShader(ctx->program, vs);
    glAttachShader(ctx->program, fs);
    glLinkProgram(ctx->program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint linked = 0;
    glGetProgramiv(ctx->program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE)
    {
        glDeleteProgram(ctx->program);
        memset(ctx, 0, sizeof(*ctx));
        return false;
    }

    ctx->u_atlas      = glGetUniformLocation(ctx->program, "u_atlas");
    ctx->u_screen     = glGetUniformLocation(ctx->program, "u_screen");
    ctx->u_origin     = glGetUniformLocation(ctx->program, "u_origin");
    ctx->u_slot_rect  = glGetUniformLocation(ctx->program, "u_slot_rect");
    ctx->u_slot_tile  = glGetUniformLocation(ctx->program, "u_slot_tile");
    ctx->u_slot_enable= glGetUniformLocation(ctx->program, "u_slot_enable");

    ctx->atlas_texture = atlas_texture;
    ctx->screen_w = screen_w;
    ctx->screen_h = screen_h;

    menu_gpu_layout_default(&ctx->layout);

    return true;
}

void menu_gpu_shutdown(MenuGpuContext* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->program != 0)
    {
        glDeleteProgram(ctx->program);
    }

    memset(ctx, 0, sizeof(*ctx));
}

void menu_gpu_render(
    MenuGpuContext* ctx,
    const uint8_t mode_storage_buffers[8],
    const int result_bpm[2],
    float origin_x,
    float origin_y)
{
    if (!ctx || ctx->program == 0)
    {
        return;
    }

    MenuGpuPayload payload;
    menu_gpu_payload_from_state(&payload, mode_storage_buffers, result_bpm);

    float rect_data[MENU_GPU_SLOT_COUNT * 4];
    float tile_data[MENU_GPU_SLOT_COUNT];
    float enable_data[MENU_GPU_SLOT_COUNT];

    for (int i = 0; i < MENU_GPU_SLOT_COUNT; ++i)
    {
        rect_data[(i * 4) + 0] = ctx->layout.rect[i].x;
        rect_data[(i * 4) + 1] = ctx->layout.rect[i].y;
        rect_data[(i * 4) + 2] = ctx->layout.rect[i].w;
        rect_data[(i * 4) + 3] = ctx->layout.rect[i].h;

        tile_data[i] = (float) menu_gpu_clamp_tile(payload.tile_index[i]);
        enable_data[i] = payload.enabled[i] ? 1.0f : 0.0f;
    }

    glUseProgram(ctx->program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctx->atlas_texture);

    if (ctx->u_atlas >= 0)       glUniform1i(ctx->u_atlas, 0);
    if (ctx->u_screen >= 0)      glUniform2f(ctx->u_screen, ctx->screen_w, ctx->screen_h);
    if (ctx->u_origin >= 0)      glUniform2f(ctx->u_origin, origin_x, origin_y);
    if (ctx->u_slot_rect >= 0)   glUniform4fv(ctx->u_slot_rect, MENU_GPU_SLOT_COUNT, rect_data);
    if (ctx->u_slot_tile >= 0)   glUniform1fv(ctx->u_slot_tile, MENU_GPU_SLOT_COUNT, tile_data);
    if (ctx->u_slot_enable >= 0) glUniform1fv(ctx->u_slot_enable, MENU_GPU_SLOT_COUNT, enable_data);

    // Draw call intentionally not done here.
    // Integration layer should call glDrawArrays(GL_TRIANGLE_FAN, 0, 4)
    // using the already prepared fullscreen quad state.
}