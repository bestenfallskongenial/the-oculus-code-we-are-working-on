#include "kernel.h"
#include "global.h"
#include "additions_to_global.h"

namespace
{
    static const unsigned MENU_HELPER_MAX_FILES = 1;
}

bool gpu_render_helper_allocate_menu_buffers(
    CKernel* kernel,
    char*** out_menu_fsh_buffer,
    char*** out_menu_tex_buffer,
    unsigned menu_fsh_buffer_size,
    unsigned menu_tex_buffer_size)
{
    if (kernel == 0 || out_menu_fsh_buffer == 0 || out_menu_tex_buffer == 0)
        return false;

    *out_menu_fsh_buffer = kernel->memory_init_buffer(MENU_HELPER_MAX_FILES, menu_fsh_buffer_size);
    *out_menu_tex_buffer = kernel->memory_init_buffer(MENU_HELPER_MAX_FILES, menu_tex_buffer_size);

    if (*out_menu_fsh_buffer == 0 || *out_menu_tex_buffer == 0)
        return false;

    return true;
}

bool gpu_render_helper_load_menu_files(
    CKernel* kernel,
    char** menu_fsh_buffer,
    char** menu_tex_buffer,
    unsigned menu_fsh_buffer_size,
    unsigned menu_tex_buffer_size)
{
    if (kernel == 0 || menu_fsh_buffer == 0 || menu_tex_buffer == 0)
        return false;

    unsigned scanned_menu_fsh = kernel->filesystem_ScanRootDir(
        SCANED_FILES_MENU_FSH,
        MENU_FSH_EXTENSIONS,
        1,
        MENU_HELPER_MAX_FILES);

    unsigned scanned_menu_tex = kernel->filesystem_ScanRootDir(
        SCANED_FILES_MENU_TEX,
        MENU_TEX_EXTENSIONS,
        1,
        MENU_HELPER_MAX_FILES);

    unsigned loaded_fsh = kernel->filesystem_process_files(
        SCANED_FILES_MENU_FSH,
        MENU_FSH_LOADED_BYTES,
        menu_fsh_buffer,
        scanned_menu_fsh,
        0,
        menu_fsh_buffer_size,
        1);

    unsigned loaded_tex = kernel->filesystem_process_files(
        SCANED_FILES_MENU_TEX,
        MENU_TEX_LOADED_BYTES,
        menu_tex_buffer,
        scanned_menu_tex,
        0,
        menu_tex_buffer_size,
        2);

    return (loaded_fsh > 0 && loaded_tex > 0);
}

GLuint gpu_render_helper_compile_shader(GLenum shader_type, const char* shader_source)
{
    if (shader_source == 0)
        return 0;

    GLuint shader_id = glCreateShader(shader_type);
    if (shader_id == 0)
        return 0;

    glShaderSource(shader_id, 1, &shader_source, 0);
    glCompileShader(shader_id);

    GLint compile_ok = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_ok);

    if (compile_ok == GL_FALSE)
    {
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

GLuint gpu_render_helper_link_program(GLuint shared_vertex_shader, GLuint menu_fragment_shader)
{
    if (shared_vertex_shader == 0 || menu_fragment_shader == 0)
        return 0;

    GLuint program_id = glCreateProgram();
    if (program_id == 0)
        return 0;

    glAttachShader(program_id, shared_vertex_shader);
    glAttachShader(program_id, menu_fragment_shader);
    glLinkProgram(program_id);

    GLint link_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_ok);

    if (link_ok == GL_FALSE)
    {
        glDeleteProgram(program_id);
        return 0;
    }

    return program_id;
}

GLuint gpu_render_helper_build_menu_program(GLuint shared_vertex_shader, const char* menu_fsh_source)
{
    GLuint fsh_id = gpu_render_helper_compile_shader(GL_FRAGMENT_SHADER, menu_fsh_source);
    if (fsh_id == 0)
        return 0;

    GLuint prg_id = gpu_render_helper_link_program(shared_vertex_shader, fsh_id);
    glDeleteShader(fsh_id);

    return prg_id;
}

bool gpu_render_helper_upload_menu_atlas(
    GLuint* out_texture_id,
    const void* atlas_pixels,
    int atlas_width,
    int atlas_height)
{
    if (out_texture_id == 0 || atlas_pixels == 0)
        return false;

    if (atlas_width <= 0 || atlas_height <= 0)
        return false;

    glGenTextures(1, out_texture_id);
    if (*out_texture_id == 0)
        return false;

    glBindTexture(GL_TEXTURE_2D, *out_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        atlas_width,
        atlas_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        atlas_pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}