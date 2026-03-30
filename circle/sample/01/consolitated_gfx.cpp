void CKernel::initShaders(
    int fromFile,
    int toFile,
    const char** buffer,
    GLuint* target,
    GLenum type)
{
    for (int i = fromFile; i < toFile; i++)
    {
        const char* src = buffer[i];

        target[i] = glCreateShader(type);
        glShaderSource(target[i], 1, &src, 0);
        glCompileShader(target[i]);

#ifdef __GL_DEBUG__
        check();
#endif
    }
}

// calls

// VSH (buffer prepared accordingly)
initShaders(0, vsh_count, m_bufferVsh,  m_glsl->gl_vsh_id, GL_VERTEX_SHADER);

// FSH
initShaders(0, fsh_count, m_bufferFsh,  m_glsl->gl_fsh_id, GL_FRAGMENT_SHADER);

// OMF (single)
initShaders(0, 1,         m_bufferOmf,  m_glsl->gl_oms_id, GL_FRAGMENT_SHADER);

void CKernel::initPrograms(
    int fromFile,
    int toFile,
    GLuint* vshArray,   // e.g. gl_vsh_id
    GLuint* fshArray,   // e.g. gl_fsh_id or gl_oms_id
    GLuint* prgArray)   // e.g. gl_prg_id or gl_omp_id
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!m_shaderStatusFlags[i]) continue;

        prgArray[i] = glCreateProgram();

        glAttachShader(prgArray[i], vshArray[0]);   // your design: VSH[0]
        glAttachShader(prgArray[i], fshArray[i]);   // FSH[i] or OMS[i]

        glLinkProgram(prgArray[i]);

        GLint linkStatus;
        glGetProgramiv(prgArray[i], GL_LINK_STATUS, &linkStatus);

        programLog(prgArray[i], i);
#ifdef __GL_DEBUG__
        check();
#endif

        if (linkStatus == GL_FALSE)
        {
            glDeleteProgram(prgArray[i]);
            prgArray[i] = 0;
            m_shaderStatusFlags[i] = false;
        }
        else
        {
            g_linked_programs_counter++;
        }
    }

    m_Watchdog.Start(TIMEOUT * 3);
}

// calls

// User programs
initPrograms(0, fsh_count,
             m_glsl->gl_vsh_id,
             m_glsl->gl_fsh_id,
             m_glsl->gl_prg_id);

// Overlay program (single)
initPrograms(0, 1,
             m_glsl->gl_vsh_id,
             m_glsl->gl_oms_id,
             m_glsl->gl_omp_id);

             void CKernel::initUniforms(
    glsl_state* s,
    int fromFile,
    int toFile,
    GLuint* prgArray)
{
#ifdef __GL_DEBUG__
    check();
#endif

    for (int i = fromFile; i < toFile; i++)
    {
        if (!m_shaderStatusFlags[i]) continue;

        glUseProgram(prgArray[i]);

#ifdef __GL_DEBUG__
        check();
#endif

        // shared
        s->gl_vtx = glGetAttribLocation(prgArray[i], "vertex");

        // user uniforms (will be -1 for overlay)
        s->u_time[i]  = glGetUniformLocation(prgArray[i], "time");
        s->u_tres[i]  = glGetUniformLocation(prgArray[i], "tres");
        s->u_seed[i]  = glGetUniformLocation(prgArray[i], "p_seed");
        s->u_aud[i]   = glGetUniformLocation(prgArray[i], "audio");
        s->u_col[i]   = glGetUniformLocation(prgArray[i], "color");
        s->u_par_a[i] = glGetUniformLocation(prgArray[i], "par_a");
        s->u_par_b[i] = glGetUniformLocation(prgArray[i], "par_b");
        s->u_tex_l[i] = glGetUniformLocation(prgArray[i], "tex_l");

        s->u_tex_id[i][0] = glGetUniformLocation(prgArray[i], "tex[0]");
        s->u_tex_id[i][1] = glGetUniformLocation(prgArray[i], "tex[1]");
        s->u_tex_id[i][2] = glGetUniformLocation(prgArray[i], "tex[2]");
        s->u_tex_id[i][3] = glGetUniformLocation(prgArray[i], "tex[3]");
        s->u_tex_id[i][4] = glGetUniformLocation(prgArray[i], "tex[4]");
        s->u_tex_id[i][5] = glGetUniformLocation(prgArray[i], "tex[5]");
        s->u_tex_id[i][6] = glGetUniformLocation(prgArray[i], "tex[6]");
        s->u_tex_id[i][7] = glGetUniformLocation(prgArray[i], "tex[7]");
        s->u_tex_id[i][8] = glGetUniformLocation(prgArray[i], "tex[8]");
        s->u_tex_id[i][9] = glGetUniformLocation(prgArray[i], "tex[9]");

        // overlay uniforms (will be -1 for user shaders)
        s->u_atlas[i]      = glGetUniformLocation(prgArray[i], "u_menu_atlas");
        s->u_tile_count[i] = glGetUniformLocation(prgArray[i], "u_menu_tile_count");
        s->u_tile_rect[i]  = glGetUniformLocation(prgArray[i], "u_menu_tile_rect");
        s->u_tile_index[i] = glGetUniformLocation(prgArray[i], "u_menu_tile_index");

#ifdef __GL_DEBUG__
        check();
#endif
    }
}

// calls

// User
initUniforms(m_glsl, 0, fsh_count, m_glsl->gl_prg_id);

// Overlay
initUniforms(m_glsl, 0, 1,         m_glsl->gl_omp_id);

/*
Invariants preserved
gl_vsh_id[0] + gl_fsh_id[i]  -> gl_prg_id[i]  -> uniforms[i]
gl_vsh_id[0] + gl_oms_id[0]  -> gl_omp_id[0]  -> uniforms[0]

*/

void CKernel::render_uniforms_all(glsl_state* s, menu_glsl_state* m)
{
#ifdef __GL_DEBUG__
    check();
#endif

    // ================= USER =================
    int i = g_current_gl_program;

    glUseProgram(s->gl_prg_id[i]);
#ifdef __GL_DEBUG__
    check();
#endif

    GLuint cx = s->screen_width;
    GLuint cy = s->screen_height;

    if (s->u_time[i]  != -1) glUniform1f(s->u_time[i], GLtime);
    if (s->u_tres[i]  != -1) glUniform2f(s->u_tres[i], cx, cy);
    if (s->u_seed[i]  != -1) glUniform4f(s->u_seed[i],
        g_inOutMatrixFlt[0][RND],
        g_inOutMatrixFlt[1][RND],
        g_inOutMatrixFlt[2][RND],
        g_inOutMatrixFlt[3][RND]);

    if (s->u_aud[i]   != -1) glUniform4f(s->u_aud[i],
        g_inOutMatrixFlt[0][AU0],
        g_inOutMatrixFlt[0][AU1],
        g_inOutMatrixFlt[0][AU2],
        g_inOutMatrixFlt[0][AU3]);

    if (s->u_col[i]   != -1) glUniform4f(s->u_col[i], 0.0f, 0.0f, 0.0f, g_opaque);

    if (s->u_par_a[i] != -1) glUniform4f(s->u_par_a[i],
        g_inOutMatrixFlt[0][OUT],
        g_inOutMatrixFlt[1][OUT],
        g_inOutMatrixFlt[2][OUT],
        g_inOutMatrixFlt[3][OUT]);

    if (s->u_par_b[i] != -1) glUniform4f(s->u_par_b[i],
        g_inOutMatrixFlt[4][OUT],
        g_inOutMatrixFlt[5][OUT],
        g_inOutMatrixFlt[6][OUT],
        g_inOutMatrixFlt[7][OUT]);

    if (s->u_tex_l[i] != -1) glUniform1i(s->u_tex_l[i], p_validTextureCount);

#ifdef __GL_DEBUG__
    check();
#endif


    // ================= MENU =================
    glUseProgram(m->gl_omp_id[0]);
#ifdef __GL_DEBUG__
    check();
#endif

    if (m->u_tile_count[0] != -1)
        glUniform1i(m->u_tile_count[0], MENU_GPU_TILE_COUNT);

    if (m->u_tile_rect[0] != -1)
        glUniform4fv(m->u_tile_rect[0], MENU_GPU_TILE_COUNT, m->tile_rect);

    if (m->u_tile_index[0] != -1)
        glUniform1fv(m->u_tile_index[0], MENU_GPU_TILE_COUNT, m->tile_index);

#ifdef __GL_DEBUG__
    check();
#endif
}

// call

render_uniforms_all(m_glsl, m_menu);


//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                #include "kernel.h"
                #include "bcm_host.h" 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------      
void            CKernel::shaderLog(GLint shader, int shaderIndex)
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

                m_shaderStatusFlags[shaderIndex] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__                 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::programLog(GLint shader, int program_index)
{
                int internal_index = 0;
                if (program_index > 0)
                    internal_index = program_index - 1;

                GLint success;
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                m_shaderStatusFlags[program_index] = (success == GL_TRUE);
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif // __DEBUG_LOG__
                char name[27];
                strncpy(name, &m_bufferFsh[program_index][2], 26);
                name[26] = '\0';
#ifdef __DEBUG_LOG__ 
                storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
                storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
#endif // __DEBUG_LOG__
                char log[1024];
                glGetProgramInfoLog(shader, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif // __DEBUG_LOG__
                GLint numUniforms;
                glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numUniforms; ++i)
                {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(shader, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(shader, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Uniform name", uname, length);
#endif // __DEBUG_LOG__                    
                }

                GLint numAttributes;
                glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numAttributes; ++i)
                {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(shader, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(shader, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif // __DEBUG_LOG__                    
                }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif // __DEBUG_LOG__                 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_check( const char* file, unsigned line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (resetFlag && !summary_written)
                {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);

                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Program", (u32)value);
#endif // __DEBUG_LOG__ 
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Active Texture Unit", (u32)value);
#endif // __DEBUG_LOG__ 
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport x/y", (u32)viewport[0], (u32)viewport[1]);
                    storeLog( MY_BUFFER, MY_INDEX, "Viewport w/h", (u32)viewport[2], (u32)viewport[3]);
#endif // __DEBUG_LOG__ 
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __DEBUG_LOG__                     
                    storeLog( MY_BUFFER, MY_INDEX, "Current Framebuffer", (u32)fb);
                    storeLog( MY_BUFFER, MY_INDEX, "=== End Status Report ===");
#endif // __DEBUG_LOG__                     
                    return;
                }

                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                {
                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();

                    const char* error_str;
                    const char* severity;

                    switch (error)
                    {
                        case GL_INVALID_ENUM:
                            error_str = "GL_INVALID_ENUM";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_VALUE:
                            error_str = "GL_INVALID_VALUE";
                            severity  = "WARNING";
                            break;

                        case GL_INVALID_OPERATION:
                            error_str = "GL_INVALID_OPERATION";
                            severity  = "WARNING";
                            break;

                        case GL_OUT_OF_MEMORY:
                            error_str = "GL_OUT_OF_MEMORY";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
                            resetFlag = true;
                            break;

                        default:
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
                            break;
                    }
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "OpenGL Error err/ticks/line", (u32)error, (u32)ticks, (u32)line);
                    storeLog( MY_BUFFER, MY_INDEX, severity);
                    storeLog( MY_BUFFER, MY_INDEX, error_str);
                    storeLog( MY_BUFFER, MY_INDEX, file);
#endif // __DEBUG_LOG__ 
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        resetFlag = true;
                }
}

