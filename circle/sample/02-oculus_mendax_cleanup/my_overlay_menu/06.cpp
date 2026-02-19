#include "kernel.h"
#include "global.h"

#include <string.h>

// Dedicated menu files: one fragment shader + one atlas texture.
char *MENU_FSH_EXTENSIONS[1] = { (char *)"fsh" };
char *MENU_TEX_EXTENSIONS[1] = { (char *)"bmp" };

char *SCANED_FILES_MENU_FSH[1] = { 0 };
char *SCANED_FILES_MENU_TEX[1] = { 0 };

unsigned MENU_FSH_LOADED_BYTES[1] = { 0 };
unsigned MENU_TEX_LOADED_BYTES[1] = { 0 };

// Menu layout controls.
float MENU_GPU_ORIGIN[2] = { 0.05f, 0.08f };
float MENU_GPU_TILE_SIZE[2] = { 0.08f, 0.08f };
float MENU_GPU_BACKGROUND_SCALE[2] = { 1.0f, 1.0f };

// Slots: 0..7 mode icons, 8..11 bpm digits, 12 dot, 13 bpm icon, 14..15 backgrounds.
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

MenuGpuState g_menu_state = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };

// Build absolute menu rectangles from global menu layout arrays.
void CKernel::menu_gpu_build_rectangles_from_layout_arrays()
{
    float ox = MENU_GPU_ORIGIN[0];
    float oy = MENU_GPU_ORIGIN[1];
    float tw = MENU_GPU_TILE_SIZE[0];
    float th = MENU_GPU_TILE_SIZE[1];

    for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
    {
        float sx = MENU_GPU_REL_SIZE[i][0];
        float sy = MENU_GPU_REL_SIZE[i][1];

        if (i == 14 || i == 15)
        {
            sx *= MENU_GPU_BACKGROUND_SCALE[0];
            sy *= MENU_GPU_BACKGROUND_SCALE[1];
        }

        g_menu_state.tile_rect[i].x = ox + MENU_GPU_REL_POS[i][0] * tw;
        g_menu_state.tile_rect[i].y = oy + MENU_GPU_REL_POS[i][1] * th;
        g_menu_state.tile_rect[i].w = sx * tw;
        g_menu_state.tile_rect[i].h = sy * th;
    }
}

// Build current tile indices from deterministic runtime buffers.
void CKernel::menu_gpu_build_dynamic_indices()
{
    g_menu_state.tile_index[0] = mode_storage_buffers[CH0_MODE][current_buffer];
    g_menu_state.tile_index[1] = mode_storage_buffers[CH1_MODE][current_buffer];
    g_menu_state.tile_index[2] = mode_storage_buffers[CH2_MODE][current_buffer];
    g_menu_state.tile_index[3] = mode_storage_buffers[CH3_MODE][current_buffer];
    g_menu_state.tile_index[4] = mode_storage_buffers[CH4_MODE][current_buffer];
    g_menu_state.tile_index[5] = mode_storage_buffers[CH5_MODE][current_buffer];
    g_menu_state.tile_index[6] = mode_storage_buffers[CH6_MODE][current_buffer];
    g_menu_state.tile_index[7] = mode_storage_buffers[CH7_MODE][current_buffer];

    unsigned long bpm0 = resultBPM[0] % 10000UL;
    unsigned long bpm1 = resultBPM[1] % 10UL;

    g_menu_state.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
    g_menu_state.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
    g_menu_state.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
    g_menu_state.tile_index[11] = (int) (bpm0 % 10UL);

    g_menu_state.tile_index[12] = 10;
    g_menu_state.tile_index[13] = (int) bpm1;

    g_menu_state.tile_index[14] = 48;
    g_menu_state.tile_index[15] = 49;
}

// Load menu fragment shader source from the scanned file name.
bool CKernel::gpu_render_helper_load_menu_fsh(char **fshader_buffers, unsigned fshader_buffer_size, int load_mode)
{
    if (fshader_buffers == 0 || fshader_buffers[0] == 0)
        return false;

    MENU_FSH_LOADED_BYTES[0] = 0;

    if (SCANED_FILES_MENU_FSH[0] == 0)
        return false;

    if (!filesystem_open_file(SCANED_FILES_MENU_FSH[0]))
        return false;

    MENU_FSH_LOADED_BYTES[0] = filesystem_load_file(fshader_buffers[0], fshader_buffer_size, load_mode);
    filesystem_close_file();

    return MENU_FSH_LOADED_BYTES[0] > 0;
}

// Load menu texture atlas file from the scanned file name.
bool CKernel::gpu_render_helper_load_menu_tex(char **texture_buffers, unsigned texture_buffer_size, int load_mode)
{
    if (texture_buffers == 0 || texture_buffers[0] == 0)
        return false;

    MENU_TEX_LOADED_BYTES[0] = 0;

    if (SCANED_FILES_MENU_TEX[0] == 0)
        return false;

    if (!filesystem_open_file(SCANED_FILES_MENU_TEX[0]))
        return false;

    MENU_TEX_LOADED_BYTES[0] = filesystem_load_file(texture_buffers[0], texture_buffer_size, load_mode);
    filesystem_close_file();

    return MENU_TEX_LOADED_BYTES[0] > 0;
}

// Compile any shader stage and append text log when provided.
GLuint CKernel::gpu_render_helper_compile_shader(GLenum shader_type, char *shader_source, CString *optional_log)
{
    if (shader_source == 0)
        return 0;

    GLuint shader = glCreateShader(shader_type);
    if (shader == 0)
        return 0;

    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (optional_log != 0)
    {
        char info_log[1024];
        info_log[0] = '\0';
        glGetShaderInfoLog(shader, sizeof(info_log), 0, info_log);

        CString line;
        line.Format("menu shader compile (%s): %s\n",
                    shader_type == GL_FRAGMENT_SHADER ? "fragment" : "vertex",
                    compiled == GL_TRUE ? "ok" : "failed");
        optional_log->Append(line);

        if (info_log[0] != '\0')
        {
            line.Format("%s\n", info_log);
            optional_log->Append(line);
        }
    }

    if (compiled == GL_FALSE)
    {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Link program from provided shaders and append link log when provided.
GLuint CKernel::gpu_render_helper_link_program(GLuint vertex_shader, GLuint fragment_shader, CString *optional_log)
{
    if (vertex_shader == 0 || fragment_shader == 0)
        return 0;

    GLuint program = glCreateProgram();
    if (program == 0)
        return 0;

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (optional_log != 0)
    {
        char info_log[1024];
        info_log[0] = '\0';
        glGetProgramInfoLog(program, sizeof(info_log), 0, info_log);

        CString line;
        line.Format("menu program link: %s\n", linked == GL_TRUE ? "ok" : "failed");
        optional_log->Append(line);

        if (info_log[0] != '\0')
        {
            line.Format("%s\n", info_log);
            optional_log->Append(line);
        }
    }

    if (linked == GL_FALSE)
    {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

// Build menu program from shared vertex shader + dedicated menu fragment source.
bool CKernel::gpu_render_helper_build_menu_program(GLuint shared_vertex_shader, char *menu_fragment_source, GLuint *out_program, CString *optional_log)
{
    if (out_program == 0)
        return false;

    *out_program = 0;

    GLuint menu_fragment_shader = gpu_render_helper_compile_shader(GL_FRAGMENT_SHADER, menu_fragment_source, optional_log);
    if (menu_fragment_shader == 0)
        return false;

    GLuint menu_program = gpu_render_helper_link_program(shared_vertex_shader, menu_fragment_shader, optional_log);
    glDeleteShader(menu_fragment_shader);

    if (menu_program == 0)
        return false;

    *out_program = menu_program;
    return true;
}

// Upload already decoded RGB24 atlas pixels to a GL texture.
bool CKernel::gpu_render_helper_upload_menu_atlas_rgb24(void *rgb_pixels, unsigned width, unsigned height, GLuint *out_texture)
{
    if (rgb_pixels == 0 || width == 0 || height == 0 || out_texture == 0)
        return false;

    *out_texture = 0;

    GLuint texture = 0;
    glGenTextures(1, &texture);

    if (texture == 0)
        return false;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    *out_texture = texture;
    return true;
}

// Initialize menu GL pipeline and cache all required handles.
bool CKernel::gpu_render_menu_init(GLuint shared_vertex_shader, char *menu_fsh_source, GLuint atlas_texture)
{
    if (g_menu_state.initialized)
        return true;

    if (shared_vertex_shader == 0 || menu_fsh_source == 0 || atlas_texture == 0)
        return false;

    GLuint fsh = gpu_render_helper_compile_shader(GL_FRAGMENT_SHADER, menu_fsh_source, 0);
    if (fsh == 0)
        return false;

    GLuint prg = glCreateProgram();
    glAttachShader(prg, shared_vertex_shader);
    glAttachShader(prg, fsh);
    glLinkProgram(prg);
    glDeleteShader(fsh);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(prg, GL_LINK_STATUS, &link_ok);
    if (link_ok == GL_FALSE)
    {
        glDeleteProgram(prg);
        return false;
    }

    g_menu_state.program = prg;
    g_menu_state.atlas_texture = atlas_texture;
    g_menu_state.a_vertex = glGetAttribLocation(prg, "vertex");
    g_menu_state.u_atlas = glGetUniformLocation(prg, "u_menu_atlas");
    g_menu_state.u_tile_count = glGetUniformLocation(prg, "u_menu_tile_count");
    g_menu_state.u_tile_rect = glGetUniformLocation(prg, "u_menu_tile_rect");
    g_menu_state.u_tile_index = glGetUniformLocation(prg, "u_menu_tile_index");

    menu_gpu_build_rectangles_from_layout_arrays();
    menu_gpu_build_dynamic_indices();

    g_menu_state.initialized = true;
    return true;
}

// Rebuild only rectangle placement when layout knobs changed.
void CKernel::gpu_render_menu_rebuild_layout()
{
    if (!g_menu_state.initialized)
        return;

    menu_gpu_build_rectangles_from_layout_arrays();
}

// Rebuild only tile indices when mode or BPM values changed.
void CKernel::gpu_render_menu_update_indices()
{
    if (!g_menu_state.initialized)
        return;

    menu_gpu_build_dynamic_indices();
}

// Draw menu on top of the framebuffer using one full-screen fan.
void CKernel::gpu_render_menu_draw(GLuint fullscreen_vbo)
{
    if (!g_menu_state.initialized)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);
    glUseProgram(g_menu_state.program);

    if (g_menu_state.a_vertex >= 0)
    {
        glVertexAttribPointer(g_menu_state.a_vertex, 4, GL_FLOAT, 0, 16, 0);
        glEnableVertexAttribArray(g_menu_state.a_vertex);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_menu_state.atlas_texture);

    if (g_menu_state.u_atlas >= 0)
        glUniform1i(g_menu_state.u_atlas, 0);

    if (g_menu_state.u_tile_count >= 0)
        glUniform1i(g_menu_state.u_tile_count, MENU_GPU_TILE_COUNT);

    if (g_menu_state.u_tile_rect >= 0)
        glUniform4fv(g_menu_state.u_tile_rect, MENU_GPU_TILE_COUNT, (GLfloat *) g_menu_state.tile_rect);

    if (g_menu_state.u_tile_index >= 0)
    {
        GLfloat index_values[16];
        for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
            index_values[i] = (GLfloat) g_menu_state.tile_index[i];

        glUniform1fv(g_menu_state.u_tile_index, MENU_GPU_TILE_COUNT, index_values);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisable(GL_BLEND);
}

// Release menu program and clear menu state.
void CKernel::gpu_render_menu_shutdown()
{
    if (g_menu_state.program != 0)
        glDeleteProgram(g_menu_state.program);

    g_menu_state = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };
}