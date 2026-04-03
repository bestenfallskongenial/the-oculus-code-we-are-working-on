 // need
public:
    


vertex_state    m_vertex;
glsl_state      m_vsh;
glsl_state      m_fsh;
glsl_state      m_osh;

texture_state   m_tex;
texture_state   m_omt;


void CKernel::initShaders(  vertex_state* v,
                            glsl_state* s,
                            texture_state* t,
                            char** srcBuffer, // removed the const here
                            int fromFile,
                            int toFile,
                            GLenum type,
                            bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        char* src = srcBuffer[i]; // and here

        s->gl_shader_id[i] = glCreateShader(type);
        glShaderSource(s->gl_shader_id[i], 1, &src, 0);
        glCompileShader(s->gl_shader_id[i]);

        shaderLog(s->gl_shader_id[i], i, flags); // should rather return 0/1 that we can do  flags[i] = ... right here!!!

#ifdef __GL_DEBUG__
        check();
#endif
    }
}
void CKernel::initPrograms( vertex_state* v,
                            glsl_state* s,
                            texture_state* t,
                            int fromFile,
                            int toFile,
                            bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags[i])
            continue;

        fsh->gl_program_id[i] = glCreateProgram();

        glAttachShader(fsh->gl_program_id[i], vsh->gl_shader_id[0]);
        glAttachShader(fsh->gl_program_id[i], fsh->gl_shader_id[i]);

        glLinkProgram(fsh->gl_program_id[i]);

        programLog(fsh->gl_program_id[i], i, flags); // should rather return 0/1 that we can do  flags[i] = ... right here!!!

#ifdef __GL_DEBUG__
        check();
#endif

        if (!flags[i])
        {
            glDeleteProgram(fsh->gl_program_id[i]);
            fsh->gl_program_id[i] = 0;
        }
        else
        {
            g_linked_programs_counter++;
        }
    }

    m_Watchdog.Start(TIMEOUT * 3);
}
void CKernel::initUniforms( vertex_state* v,
                            glsl_state* s,
                            texture_state* t,
                            int fromFile,
                            int toFile,
                            bool* flags)
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags[i])
            continue;

        glUseProgram(s->gl_program_id[i]);

#ifdef __GL_DEBUG__
        check();
#endif
        v->gl_vtx[i]       = glGetAttribLocation(s->gl_program_id[i], "vertex");

        s->u_time[i]       = glGetUniformLocation(s->gl_program_id[i], "time");
        s->u_tres[i]       = glGetUniformLocation(s->gl_program_id[i], "tres");
        s->u_seed[i]       = glGetUniformLocation(s->gl_program_id[i], "p_seed");
        s->u_aud[i]        = glGetUniformLocation(s->gl_program_id[i], "audio");
        s->u_col[i]        = glGetUniformLocation(s->gl_program_id[i], "color");
        s->u_par_a[i]      = glGetUniformLocation(s->gl_program_id[i], "par_a");
        s->u_par_b[i]      = glGetUniformLocation(s->gl_program_id[i], "par_b");

        s->u_tex_l[i]      = glGetUniformLocation(s->gl_program_id[i], "tex_l");

        s->u_atlas[i]      = glGetUniformLocation(s->gl_program_id[i], "u_menu_atlas");
        s->u_tile_count[i] = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_count");
        s->u_tile_rect[i]  = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_rect");
        s->u_tile_index[i] = glGetUniformLocation(s->gl_program_id[i], "u_menu_tile_index");

        t->u_tex_id[i][0] = glGetUniformLocation(s->gl_program_id[i], "tex[0]");
        t->u_tex_id[i][1] = glGetUniformLocation(s->gl_program_id[i], "tex[1]");
        t->u_tex_id[i][2] = glGetUniformLocation(s->gl_program_id[i], "tex[2]");
        t->u_tex_id[i][3] = glGetUniformLocation(s->gl_program_id[i], "tex[3]");
        t->u_tex_id[i][4] = glGetUniformLocation(s->gl_program_id[i], "tex[4]");
        t->u_tex_id[i][5] = glGetUniformLocation(s->gl_program_id[i], "tex[5]");
        t->u_tex_id[i][6] = glGetUniformLocation(s->gl_program_id[i], "tex[6]");
        t->u_tex_id[i][7] = glGetUniformLocation(s->gl_program_id[i], "tex[7]"); // in theory a loop up to TEX_MAX but im lazy!!

#ifdef __GL_DEBUG__
        check();
#endif
    }
}

void CKernel::initTextures(
    vertex_state* v,
    glsl_state* s,
    texture_state* t,
    char** buffer,
    int fromFile,
    int toFile,
    bool* flags,
    GLint wrap_s,
    GLint wrap_t)
{
    for (int i = fromFile; i < toFile; i++)
    {
        if (!flags[i])  // parser does the first pass
            continue;

        glGenTextures(1, &t->gl_tex_id[i]);
        glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[i]);
#ifdef __GL_DEBUG__
        check();
#endif
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
        check();
#endif
        GLvoid* bitmapData = &buffer[i][t->offset[i]]; // parser need to be refactored to fill the tex struct!

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     t->width[i],
                     t->height[i],
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     bitmapData);

        flags[i] = (glGetError() == GL_NO_ERROR);   // second pass overwrite - get gl error clears the error queue check too!
// #ifdef __GL_DEBUG__
//         check();        
// #endif
        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
        check();
#endif
        m_Watchdog.Start(TIMEOUT); // there was an purpose here, just because some textures may take longer?
    }
}

void CKernel::render_uniforms_all(glsl_state* s, glsl_state* m) // shall i combine them here or just keep the two separate calls?
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
/* calls, but i still need to add the parameters in the call signature i introduced earlier to have a unified style

initShaders(&m_vsh, 0, vsh_count, m_bufferVsh, GL_VERTEX_SHADER, vsh_flags);
initShaders(&m_fsh, 0, fsh_count, m_bufferFsh, GL_FRAGMENT_SHADER, fsh_flags);
initShaders(&m_osh, 0, omf_count, m_bufferOsh, GL_FRAGMENT_SHADER, omf_flags);

initPrograms(&m_vsh, &m_fsh, 0, fsh_count, fsh_flags);
initPrograms(&m_vsh, &m_osh, 0, omf_count, omf_flags);

initUniforms(&m_vertex, &m_fsh, &m_tex, 0, fsh_count, fsh_flags);
initUniforms(&m_vertex, &m_osh, &m_omt, 0, omf_count, omf_flags);

initTextures(&m_tex, 0, tex_count, m_bufferTex, tex_flags, p_validTextureCount, GL_REPEAT, GL_REPEAT);
initTextures(&m_omt, 0, 1,         m_bufferOtm, omt_flags, dummyTextureCount, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

render_uniforms_all(m_glsl, m_menu);
*/