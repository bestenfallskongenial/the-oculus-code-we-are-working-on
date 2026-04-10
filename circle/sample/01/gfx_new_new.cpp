// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// logging
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
#endif // __DEBUG_LOG__

                return success == GL_TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
            //  int internal_index = 0;                     // i wonder, if and why we need it, was the indexing in the buffer incorrect? !!! DOUBLECHECK !!!
            //  if (program_index > 0)
            //      internal_index = program_index - 1;

                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);

#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
                storeLog( MY_BUFFER, MY_INDEX, "Program link status idx/success", (u32)program_index, (u32)success);
#endif // __DEBUG_LOG__
            //  char name[27];
            //  strncpy(name, &m_bufferFsh[program_index][2], 26);
            //  name[26] = '\0';
#ifdef __DEBUG_LOG__ 
            //  storeMsg( MY_BUFFER, MY_INDEX, "Program short name", name, 26);
            //  storeMsg( MY_BUFFER, MY_INDEX, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
                storeLog( MY_BUFFER, MY_INDEX, "Program byte size", (u32)g_bytFsh[program_index]);
#endif // __DEBUG_LOG__
                char log[1024];
                glGetProgramInfoLog(program, sizeof(log), NULL, log);
#ifdef __DEBUG_LOG__                 
                storeMsg( MY_BUFFER, MY_INDEX, "Program InfoLog", log, sizeof(log));
#endif // __DEBUG_LOG__
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Uniforms", (u32)numUniforms);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numUniforms; ++i)
                {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(program, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(program, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Uniform name", uname, length);
#endif // __DEBUG_LOG__                    
                }

                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog( MY_BUFFER, MY_INDEX, "Active Attributes", (u32)numAttributes);
#endif // __DEBUG_LOG__
                for (GLint i = 0; i < numAttributes; ++i)
                {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(program, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(program, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog( MY_BUFFER, MY_INDEX, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);
                    storeMsg( MY_BUFFER, MY_INDEX, "Attribute name", aname, length);
#endif // __DEBUG_LOG__                    
                }
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif // __DEBUG_LOG__

                return success == GL_TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// #define         check() 				gfx_check(__FILE__, __LINE__) 	// my own assertion implementation
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_check                  (   const char* file, 
                                                        unsigned    line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (resetFlag && !summary_written)
                {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
#ifdef __DEBUG_LOG__  
                    storeLog( MY_BUFFER, MY_INDEX, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);
#endif // __DEBUG_LOG__ 
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// init OGL
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initOGL                   (   olg_state*  o ) // never touch a running horse and if remove the assertions!
{
                int32_t success = 0;
                EGLBoolean result;
                EGLint num_config;

                static EGL_DISPMANX_WINDOW_T nativewindow;

                DISPMANX_UPDATE_HANDLE_T dispman_update;
                VC_RECT_T dst_rect;
                VC_RECT_T src_rect;

                static const EGLint attribute_list[] =      {   EGL_RED_SIZE, 8,
                                                                EGL_GREEN_SIZE, 8,
                                                                EGL_BLUE_SIZE, 8,
                                                                EGL_ALPHA_SIZE, 8,
                                                                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                                                EGL_NONE };
   
                static const EGLint context_attributes[] =  {   EGL_CONTEXT_CLIENT_VERSION, 
                                                                2,
                                                                EGL_NONE };

                EGLConfig config;
                
                o->display                  = eglGetDisplay             (   EGL_DEFAULT_DISPLAY     );  // get an EGL display connection
#ifdef __OLG_DEBUG__                
assert(o->display!=EGL_NO_DISPLAY);
                check();
#endif // __OLG_DEBUG__
                result                      = eglInitialize             (   o->display,         // initialize the EGL display connection
                                                                            NULL, 
                                                                            NULL                    );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                result                      = eglChooseConfig           (   o->display,         // get an appropriate EGL frame buffer configuration 
                                                                            attribute_list, 
                                                                            &config, 
                                                                            1, 
                                                                            &num_config             );
#ifdef __OLG_DEBUG__                
assert(EGL_FALSE != result); //?
                check();
#endif // __OLG_DEBUG__
                result                      = eglBindAPI                (   EGL_OPENGL_ES_API); // get an appropriate EGL frame buffer configuration
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
                check();
#endif // __OLG_DEBUG__
                o->context                  = eglCreateContext          (   o->display,         // create an EGL rendering context
                                                                            config, 
                                                                            EGL_NO_CONTEXT, 
                                                                            context_attributes      );
#ifdef __OLG_DEBUG__
assert(o->context!=EGL_NO_CONTEXT);//?
                check();
#endif // __OLG_DEBUG__
                success                     = graphics_get_display_size (   0 /* LCD */,            // create an EGL window surface
                                                                            &o->screen_width, 
                                                                            &o->screen_height   );
#ifdef __OLG_DEBUG__
assert( success >= 0 );
                check();
#endif // __OLG_DEBUG__
                dst_rect.x                  = 0;
                dst_rect.y                  = 0;
                dst_rect.width              = o->screen_width;
                dst_rect.height             = o->screen_height;
      
                src_rect.x                  = 0;
                src_rect.y                  = 0;
                src_rect.width              = o->screen_width << 16;
                src_rect.height             = o->screen_height << 16;        

                o->dispman_display          = vc_dispmanx_display_open  (   0 /* LCD */ );
                dispman_update              = vc_dispmanx_update_start  (   0 );
      
                o->dispman_element          = vc_dispmanx_element_add   (   dispman_update, 
                                                                            o->dispman_display,
                                                                            0/*layer*/, 
                                                                            &dst_rect, 
                                                                            0/*src*/,
                                                                            &src_rect, 
                                                                            DISPMANX_PROTECTION_NONE, /*protection*/
                                                                            0 /*alpha*/, 
                                                                            0 /*clamp*/, 
                                                                            DISPMANX_NO_ROTATE /*transform*/ );   // was 0/*transform*/ before DISPMANX_ROTATE_90 
      
                nativewindow.element        = o->dispman_element;
                nativewindow.width          = o->screen_width;
                nativewindow.height         = o->screen_height;

                vc_dispmanx_update_submit_sync                          (   dispman_update  );
#ifdef __OLG_DEBUG__
                check();
#endif // __OLG_DEBUG__            
                o->surface                  = eglCreateWindowSurface    (   o->display, 
                                                                            config, 
                                                                            &nativewindow, 
                                                                            NULL            );
#ifdef __OLG_DEBUG__
                assert(o->surface != EGL_NO_SURFACE);   //?
                check();
#endif // __OLG_DEBUG__                
                result                      = eglMakeCurrent            (   o->display,     // connect the context to the surface
                                                                            o->surface, 
                                                                            o->surface, 
                                                                            o->context  );
#ifdef __OLG_DEBUG__
                assert(EGL_FALSE != result);    //?
                check();
#endif // __OLG_DEBUG__
                                              glClearColor              (   0.15f, 0.25f, 0.35f, 1.0f); // Set background color and clear buffers
                                              glClear                   (   GL_COLOR_BUFFER_BIT );
#ifdef __OLG_DEBUG__
                check();
#endif // __OLG_DEBUG__            
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// init GL
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initVbuffer                (   olg_state*  o,
                                                        vtx_state*  v )                              // Function to initialize Buffers 
{
                static const GLfloat vertex_data[] =  {  -1.0,-1.0, 1.0, 1.0, 1.0,-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0  };

                glClearColor(0.0, 1.0, 1.0, 1.0);
                glGenBuffers(1, &v->gl_buf);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                glViewport(0, 0, o->screen_width, o->screen_height);                        // Prepare viewport
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);                                          // Upload vertex data to a buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
                glVertexAttribPointer(v->gl_vtx[0], 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(v->gl_vtx[0]);
#ifdef __GL_DEBUG__
                check();
#endif // __GL_DEBUG__
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initShader                 (   vtx_state*  v,
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        char**      srcBuffer,
                                                        int         fromFile,
                                                        int         toFile,
                                                        GLenum      type,
                                                        bool*       flags)
{
                for (int i = fromFile; i < toFile; i++)
                    {
                    char* src = srcBuffer[i];

                    s->gl_shader_id[i] = glCreateShader(type);
                    glShaderSource(s->gl_shader_id[i], 1, &src, 0);
                    glCompileShader(s->gl_shader_id[i]);

                    flags[i] = shaderLog(s->gl_shader_id[i], i);   // ← assign here
#ifdef __GL_DEBUG__
                    check();
#endif
                    }
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initProgram               (   vtx_state*  v,
                                                        glsl_state* vsh,
                                                        glsl_state* fsh,
                                                        tex_state*  t,
                                                        int         fromFile,
                                                        int         toFile,
                                                        unsigned&   valid_count,
                                                        bool*       flags_vsh,
                                                        bool*       flags_fsh)
{
                for (int i = fromFile; i < toFile; i++)
                    {
                    fsh->gl_program_id[valid_count] = glCreateProgram();

                    if (flags_vsh[0] && flags_fsh[i] && fsh->gl_program_id[valid_count] != 0)
                        {
                        glAttachShader(fsh->gl_program_id[valid_count], vsh->gl_shader_id[0]);
                        glAttachShader(fsh->gl_program_id[valid_count], fsh->gl_shader_id[i]);

                        glLinkProgram(fsh->gl_program_id[valid_count]);
                        }
                    if (programLog(fsh->gl_program_id[valid_count], i))
                        {
                        valid_count++;
                        }
                    else
                        {
                        glDeleteProgram(fsh->gl_program_id[valid_count]);
                    //  fsh->gl_program_id[valid_count] = 0;
                        }
                }
                m_Watchdog.Start(TIMEOUT * 3);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initTexture                (   vtx_state*  v,
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        char**      buffer,
                                                        int         fromFile,
                                                        int         toFile,
                                                        unsigned&   valid_count,
                                                        bool*       flags,
                                                        GLint       wrap_s,
                                                        GLint       wrap_t )
{
                for (int i = fromFile; i < toFile; i++)
                    {
                    //  t->gl_tex_id[valid_count] = 0;
                    if (flags[i])
                        {
                        glGenTextures(1, &t->gl_tex_id[valid_count]);
                        glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[valid_count]);

                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                        GLvoid* bitmapData = &buffer[i][t->offset[i]];

                        glTexImage2D(GL_TEXTURE_2D,
                                    0,
                                    GL_RGB,
                                    t->width[i],
                                    t->height[i],
                                    0,
                                    GL_RGB,
                                    GL_UNSIGNED_BYTE,
                                    bitmapData);

                        if (glGetError() == GL_NO_ERROR)
                            {
                            valid_count++;
                            }
                        else
                            {
                            glDeleteTextures(1, &t->gl_tex_id[valid_count]);
                            }
                    //  glBindTexture(GL_TEXTURE_2D, 0); // or outside the if loop!
                        }
                    m_Watchdog.Start(TIMEOUT);
                    }
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initUniform                (   vtx_state*  v,   // since i call it after initProgram() i have a dense packing
                                                        glsl_state* s,
                                                        tex_state*  t,
                                                        int         fromFile,   // alway 0, kept the parameter for my uniform signature pattern
                                                        int         toFile /*,
                                                        bool*       flags*/)  // no need since dense packing right
{
                for (int i = fromFile; i < toFile; i++)
                    {
                    //  if (!flags[i])  // no need since dense packing right
                    //      continue;
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
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// runtime buffer ( first and last )
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::frmBufferSet(vtx_state* v)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);

                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#ifdef __GL_DEBUG__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, v->gl_buf);
#ifdef __GL_DEBUG__
                check();
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::frmBufferSwap              (   olg_state* o )
{
                eglSwapBuffers(o->display, o->surface);
#ifdef __GL_DEBUG__
                check();
#endif
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setUniPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                        int         gl_current_tex,         // shall we add an active texture uniform for the user here?
                                                        unsigned    p_validTextureCount )
{
                glUseProgram(s->gl_program_id[g_current_gl_program]);
#ifdef __GL_DEBUG__
                check();
#endif
                GLuint cx = o->screen_width;
                GLuint cy = o->screen_height;

                if(s->u_time[g_current_gl_program] != -1)  glUniform1f(s->u_time[g_current_gl_program], GLtime);
                if(s->u_tres[g_current_gl_program]!= -1 )  glUniform2f(s->u_tres[g_current_gl_program], cx, cy);
                if(s->u_seed[g_current_gl_program] != -1)  glUniform4f(s->u_seed[g_current_gl_program], g_inOutMatrixFlt[0][RND], 
                                                                                                        g_inOutMatrixFlt[1][RND], 
                                                                                                        g_inOutMatrixFlt[2][RND], 
                                                                                                        g_inOutMatrixFlt[3][RND]);
                if(s->u_aud[g_current_gl_program]!= -1 )   glUniform4f(s->u_aud[g_current_gl_program],  g_inOutMatrixFlt[0][AU0], 
                                                                                                        g_inOutMatrixFlt[0][AU1], 
                                                                                                        g_inOutMatrixFlt[0][AU2], 
                                                                                                        g_inOutMatrixFlt[0][AU3]);
                if(s->u_col[g_current_gl_program] != -1)   glUniform4f(s->u_col[g_current_gl_program],  0.0f, 0.0f, 0.0f, g_opaque);    // is a stub for a potential color mode, alfa was to see my text display
                if(s->u_par_a[g_current_gl_program] != -1) glUniform4f(s->u_par_a[g_current_gl_program],g_inOutMatrixFlt[0][OUT], 
                                                                                                        g_inOutMatrixFlt[1][OUT], 
                                                                                                        g_inOutMatrixFlt[2][OUT], 
                                                                                                        g_inOutMatrixFlt[3][OUT]);
                if(s->u_par_b[g_current_gl_program] != -1) glUniform4f(s->u_par_b[g_current_gl_program],g_inOutMatrixFlt[4][OUT], 
                                                                                                        g_inOutMatrixFlt[5][OUT], 
                                                                                                        g_inOutMatrixFlt[6][OUT], 
                                                                                                        g_inOutMatrixFlt[7][OUT]);
                if(s->u_tex_l[g_current_gl_program] != -1) glUniform1i(s->u_tex_l[g_current_gl_program],p_validTextureCount);           // relict? 
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setTexPrg                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t,
                                                        int         gl_current_tex,
                                                        unsigned    p_validTextureCount )
{
#ifdef __H264_DEBUG_TEX__
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_TextureA);

                if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);
#ifdef __GL_DEBUG__
                check();
#endif  // __GL_DEBUG__

#endif  // __H264_DEBUG_TEX__

#ifndef __H264_DEBUG_TEX__

                switch(g_centralModeBuffer[g_currentProgramBuffer][TEX_MODE])
                    {
                    case false:
                        for (int i = 0; i < p_validTextureCount; i++)   // we need to implement 
                            {
                            glActiveTexture(GL_TEXTURE0+i);
                            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[i]);

                            if (t->u_tex_id[g_current_gl_program][i] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][i], i);
#ifdef __GL_DEBUG__
                            check();
#endif  // __GL_DEBUG__
                            }
                    break;

                    case true:
                        switch(p_validTextureCount)
                        {
                        case 0:
                        break;

                        case 1:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);

#ifdef __GL_DEBUG__
                            check();
#endif  // __GL_DEBUG__
                        break;
                        default:
                            glActiveTexture(GL_TEXTURE0);
                            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex]);

                            if (t->u_tex_id[g_current_gl_program][0] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][0], 0);
#ifdef __GL_DEBUG__
                            check();
#endif  // __GL_DEBUG__

                            glActiveTexture(GL_TEXTURE1);
                            glBindTexture(GL_TEXTURE_2D, s->gl_tex_id[gl_current_tex + 1]);

                            if (t->u_tex_id[g_current_gl_program][1] != -1) glUniform1i(t->u_tex_id[g_current_gl_program][1], 1);

#ifdef __GL_DEBUG__
                            check();
#endif  // __GL_DEBUG__
                        break;
                        }
                    break;
                    }

#endif  // __H264_DEBUG_TEX__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::drawGLsPrg()
{
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#ifdef __GL_DEBUG__
                check();
#endif
                glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            Ckernel::frmRateBreak               (   bool* noTargetFPS )
{
                glFlush();

                if ( noTargetFPS )  // <- this is my "fps break" - explain why
                    {
                    glFinish();
#ifdef __GL_DEBUG__
                    check();
#endif
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// this is the update function, not really render, i may call it before the actual render loop as part of the menu  
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::updateOvlState             (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
            const float ox = s->kMenuOrigin[0];
            const float oy = s->kMenuOrigin[1];
            const float tw = s->kMenuTileSize[0];
            const float th = s->kMenuTileSize[1];

            for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
                {
                float sx = s->kMenuRelSize[i][0];
                float sy = s->kMenuRelSize[i][1];

                if (i == 14 || i == 15)
                    {
                    sx *= s->kMenuBackgroundScale[0];
                    sy *= s->kMenuBackgroundScale[1];
                    }

                s->tile_rect_x[i] = ox + s->kMenuRelPos[i][0] * tw;
                s->tile_rect_y[i] = oy + s->kMenuRelPos[i][1] * th;
                s->tile_rect_w[i] = sx * tw;
                s->tile_rect_h[i] = sy * th;

                s->tile_rect[i * 4 + 0] = s->tile_rect_x[i];
                s->tile_rect[i * 4 + 1] = s->tile_rect_y[i];
                s->tile_rect[i * 4 + 2] = s->tile_rect_w[i];
                s->tile_rect[i * 4 + 3] = s->tile_rect_h[i];
                }

                s->tile_index[0] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
                s->tile_index[1] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
                s->tile_index[2] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
                s->tile_index[3] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
                s->tile_index[4] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
                s->tile_index[5] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
                s->tile_index[6] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
                s->tile_index[7] = (GLfloat) g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

                const unsigned long bpm0 = g_resultBPM[0] % 10000UL;
                const unsigned long bpm1 = g_resultBPM[1] % 10UL;

                s->tile_index[8]  = (GLfloat) ((bpm0 / 1000UL) % 10UL);
                s->tile_index[9]  = (GLfloat) ((bpm0 / 100UL) % 10UL);
                s->tile_index[10] = (GLfloat) ((bpm0 / 10UL) % 10UL);
                s->tile_index[11] = (GLfloat) (bpm0 % 10UL);
                s->tile_index[12] = 10.0f;
                s->tile_index[13] = (GLfloat) bpm1;
                s->tile_index[14] = 48.0f;
                s->tile_index[15] = 49.0f;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setUniOvl                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                glUseProgram(s->gl_program_id[0]);
#ifdef __GL_DEBUG__
                check();
#endif

                if (s->u_tile_count[0] != -1) glUniform1i(s->u_tile_count[0], MENU_GPU_TILE_COUNT);

                if (s->u_tile_rect[0] != -1) glUniform4fv(s->u_tile_rect[0], MENU_GPU_TILE_COUNT, s->tile_rect);

                if (s->u_tile_index[0] != -1) glUniform1fv(s->u_tile_index[0], MENU_GPU_TILE_COUNT, s->tile_index);
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::setTexOvl                  (   olg_state*  o, 
                                                        glsl_state* s, 
                                                        tex_state*  t )
{
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, t->gl_tex_id[0]);

                if (s->u_atlas[0] != -1) glUniform1i(s->u_atlas[0], 0);
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::drawGLsOvl                 ()
{
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glDisable(GL_BLEND);
#ifdef __GL_DEBUG__
                check();
#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*

okay, what we have done here:

    - we changed the shaderLog() and programLog() from void to bool, means we also removed
      the call parameter bool* flags - its set now on caller side
    - we simplified the function initShader() and initTexture that it give out a dense indexed
      id / handle for the programs / textures -> we now need to refactor the select*() functions

// NEW generic not condensed valid arrays, max number of files ( macros for example!)

void CKernel::chooseIndexSparse(int p_channel, int& p_activeIndex, int p_maxCount, bool* flags)
{
    static int p_activeIndex = 0;

    int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

    if (flags[f_calculated])
    {
        p_activeIndex = f_calculated;
    }
}

// NEW generic condensed valid arrays, max number of files ( macros for example!)

void CKernel::chooseIndexDense(int p_channel, int& p_activeIndex, int p_maxCount)
{
    int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

    p_activeIndex = f_calculated;
}

the involved array/enums:

// the array for the loader constance - i think its better than scatter the values / constants everywhere around

#define         VSH_SD             		1	// max number of u_vertex shader on sd
#define         OMF_SD             		1	// max number of fragment shader on sd
#define         FSH_SD             		1	// max number of fragment shader on sd
#define         OMT_SD             		1	// max number of fragment shader on sd
#define         TEX_SD             		0	// max number of textures on sd
#define         VID_SD             		0	// max number of videos on sd
#define         KLN_SD                  1

#define         FRM_SD                  1   // i put them here because if my mem/dma allocation
#define         LOG_SD                  24  // here is the trick:
                                            // i will use a mix of hardwired and "open" logs. examples:

                                            // m_bufferLog[0]       - startup - memory - init log
                                            // m_bufferLog[1]       - vc_sm
                                            // m_bufferLog[2]       - mmal
                                            // m_bufferLog[3]       - check glsl
                                            // m_bufferLog[4]       - glsl compile
                                            // m_bufferLog[5  - 15] - texture parser
                                            // m_bufferLog[16 - 23] - video parser

#define         VSH_USB                 0	// max number of u_vertex shader on sd
#define         OMF_USB            		0	// max number of fragment shader on sd
#define         FSH_USB            		32	// max number of fragment shader on sd
#define         OMT_USB            		0	// max number of fragment shader on sd
#define         TEX_USB            		8	// max number of textures on sd
#define         VID_USB            		8	// max number of videos on sd
#define         KLN_USB                 1

#define         FRM_USB                 1   // i put them here because if my mem/dma allocation
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
    FLD_SCANNED,    // new
    FLD_LOADED,
    FLD_PREV,       // new
    FLD_VALID, // <- p_validCount 
    FLD_SIZE,
    FLD_COUNT
};

unsigned filecounter[FT_COUNT][FLD_COUNT] =
{  // MAXSD   MAXUSB    EXTCNT      SCANNED   LOADED  PREV    V_CNT    SIZE  
    { VSH_SD, VSH_USB,  VSH_EXT,    0,        0,      0,      0,       VSH_SIZ },  // VSH vertex shader
    { OMF_SD, OMF_USB,  OMF_EXT,    0,        0,      0,      0,       OMF_SIZ },  // OMF overlay fragment shader
    { FSH_SD, FSH_USB,  FSH_EXT,    0,        0,      0,      0,       FSH_SIZ },  // FSH user fragment shader
    { OMT_SD, OMT_USB,  OMT_EXT,    0,        0,      0,      0,       OMT_SIZ },  // OMT overlay texture ( atlas)
    { TEX_SD, TEX_USB,  TEX_EXT,    0,        0,      0,      0,       TEX_SIZ },  // TEX user texture
    { VID_SD, VID_USB,  VID_EXT,    0,        0,      0,      0,       VID_SIZ },  // VID video buffer
    { KLN_SD, KLN_USB,  KLN_EXT,    0,        0,      0,      0,       KLN_SIZ },  // KLN kernel buffer
    { FRM_SD, FRM_USB,        0,    0,        0,      0,      0,       FRM_SIZ },  // FRM decoded frames A & B
    { LOG_SD, LOG_USB,        0,    0,        0,      0,      0,       LOG_SIZ }   // LOG logging buffers   
};
// lists of extensions possible in my scanroot directory function per filetype 
        const   char                   *g_SufVsh[VSH_EXT]			    = { "vsh" }; 
        const   char                   *g_SufOmf[OMF_EXT]			    = { "omf" };	// is a fsh file but used for the overlay atlas
        const   char                   *g_SufFsh[FSH_EXT]			    = { "fsh" };
        const   char                   *g_SufOmt[OMT_EXT]			    = { "omt" }; // is a bpm file but used for the overlay atlas
        const   char                   *g_SufTex[TEX_EXT]			    = { "bmp" };
        const   char                   *g_SufVid[VID_EXT]			    = { "264" }; // i guess i will remove the whole parse code for anything but h264
        const   char                   *g_SufKln[KLN_EXT]			    = { "img" };
// array to store the scanned filenames
                char                   *g_ScnVsh[VSH_SD + VSH_USB]     	= { 0 };
        		char				   *g_ScnOmf[OMF_SD + OMF_USB] 		= { 0 };
                char                   *g_ScnFsh[FSH_SD + FSH_USB]     	= { 0 };
        		char				   *g_ScnOmt[OMT_SD + OMT_USB] 		= { 0 };
                char                   *g_ScnTex[TEX_SD + TEX_USB]     	= { 0 };
                char                   *g_ScnVid[VID_SD + VID_USB]     	= { 0 };
                char                   *g_ScnKln[KLN_SD + KLN_USB]     	= { 0 };
// array to store the length of the loaded files
                unsigned                g_bytVsh[VSH_SD + VSH_USB]      = { 0 };
                unsigned                g_bytOmf[OMF_SD + OMF_USB]      = { 0 };
                unsigned                g_bytFsh[FSH_SD + FSH_USB]      = { 0 };
                unsigned                g_bytOmt[OMT_SD + OMT_USB]      = { 0 };
                unsigned                g_bytTex[TEX_SD + TEX_USB]      = { 0 };
                unsigned                g_bytVid[VID_SD + VID_USB]      = { 0 };
                unsigned                g_bytKln[KLN_SD + KLN_USB]      = { 0 };

    struct vtx_state
{
    // shared attrib/buffer
    GLuint                      gl_buf;                         // this is also an extra struct we need to pass too
    GLint                       gl_vtx[MAX_SHADER];    
};

struct olg_state
{
    // EGL Window
    uint32_t                    screen_width;
    uint32_t                    screen_height;

    DISPMANX_ELEMENT_HANDLE_T   dispman_element;
    DISPMANX_DISPLAY_HANDLE_T   dispman_display;

    EGLDisplay                  display;
    EGLSurface                  surface;
    EGLContext                  context;
};
struct tex_state
{

    unsigned                    width[MAX_TEXTURE];
    unsigned                    height[MAX_TEXTURE];
    unsigned                    offset[MAX_TEXTURE];
	GLuint                      gl_tex_id[MAX_TEXTURE];
	GLint                       u_tex_id[MAX_SHADER][MAX_TEXTURE];
};

struct glsl_state
{
    GLuint                      gl_shader_id[MAX_SHADER];
    GLuint                      gl_program_id[MAX_SHADER];
    // user uniforms                                            // this is the actual common shader struct we define for 
    GLint                       u_time[MAX_SHADER];
    GLint                       u_tres[MAX_SHADER];
    GLint                       u_seed[MAX_SHADER];
    GLint                       u_aud[MAX_SHADER];
    GLint                       u_col[MAX_SHADER];
    GLint                       u_par_a[MAX_SHADER];
    GLint                       u_par_b[MAX_SHADER];
    GLint                       u_tex_l[MAX_SHADER];
    // overlay uniforms
    GLint                       u_atlas[MAX_OMF];
    GLint                       u_tile_count[MAX_OMF];
    GLint                       u_tile_rect[MAX_OMF];
    GLint                       u_tile_index[MAX_OMF];

    // overlay data
    float                       kMenuOrigin[2];
    float                       kMenuTileSize[2];
    float                       kMenuBackgroundScale[2];

    float                       kMenuRelPos[MAX_TILES][2];
    float                       kMenuRelSize[MAX_TILES][2];

    float                       tile_rect_x[MAX_TILES];
    float                       tile_rect_y[MAX_TILES];
    float                       tile_rect_w[MAX_TILES];
    float                       tile_rect_h[MAX_TILES];

    GLfloat                     tile_rect[MAX_TILES * 4];
    GLfloat                     tile_index[MAX_TILES];
};


    olg_state                   m_ogl;

    vtx_state                m_vtx;

    glsl_state                  m_vsh;
    glsl_state                  m_fsh;
    glsl_state                  m_osh;

    tex_state               m_tex;
    tex_state               m_omt;

// example calls

// SHADERS (delta)
initShader(&m_vtx, &m_vsh, &m_tex,
           m_bufferVsh,
           filecounter[FT_VSH][FLD_PREV],
           filecounter[FT_VSH][FLD_LOADED],
           GL_VERTEX_SHADER,
           vsh_flags);

initShader(&m_vtx, &m_osh, &m_omt,
           m_bufferOmf,
           filecounter[FT_OMF][FLD_PREV],
           filecounter[FT_OMF][FLD_LOADED],
           GL_FRAGMENT_SHADER,
           omf_flags);

initShader(&m_vtx, &m_fsh, &m_tex,
           m_bufferFsh,
           filecounter[FT_FSH][FLD_PREV],
           filecounter[FT_FSH][FLD_LOADED],
           GL_FRAGMENT_SHADER,
           fsh_flags);


// PROGRAMS (delta)
initProgram(&m_vtx,
             &m_vsh,
             &m_fsh,
             &m_tex,
             filecounter[FT_FSH][FLD_PREV],
             filecounter[FT_FSH][FLD_LOADED],
             filecounter[FT_FSH][FLD_VALID],
             vsh_flags,
             fsh_flags);

initProgram(&m_vtx,
             &m_vsh,
             &m_osh,
             &m_omt,
             filecounter[FT_OMF][FLD_PREV],
             filecounter[FT_OMF][FLD_LOADED],
             filecounter[FT_OMF][FLD_VALID],
             vsh_flags,
             omf_flags);


// TEXTURES (delta)
initTexture(&m_vtx,
             &m_fsh,
             &m_tex,
             m_bufferTex,
             filecounter[FT_TEX][FLD_PREV],
             filecounter[FT_TEX][FLD_LOADED],
             filecounter[FT_TEX][FLD_VALID],
             tex_flags,
             GL_REPEAT,
             GL_REPEAT);

initTexture(&m_vtx,
             &m_osh,
             &m_omt,
             m_bufferOmt,
             filecounter[FT_OMT][FLD_PREV],
             filecounter[FT_OMT][FLD_LOADED],
             filecounter[FT_OMT][FLD_VALID],
             omt_flags,
             GL_CLAMP_TO_EDGE,
             GL_CLAMP_TO_EDGE);

initUniform(&m_vtx,
             &m_fsh,
             &m_tex,
             0,
             filecounter[FT_FSH][FLD_VALID]);

initUniform(&m_vtx,
             &m_osh,
             &m_omt,
             0,
             filecounter[FT_OMF][FLD_VALID]);

             // runtime loop

if (overlay_enabled)
    {
    updateOvlState(&m_osh);
    }

frmBufferSet(&m_vtx);

// pass 1
setUniPrg(&m_ogl, &m_vtx, &m_fsh, &m_tex);
setTexPrg(&m_ogl, &m_vtx, &m_fsh, &m_tex, gl_current_tex);
drawGLsPrg(&m_ogl, &m_vtx, &m_fsh, &m_tex);

// pass 2 (only if enabled)
if (overlay_enabled)
    {

    setUniOvl(&m_ogl, &m_vtx, &m_osh, &m_omt);
    setTexOvl(&m_ogl, &m_vtx, &m_osh, &m_omt);
    drawGLsOvl(&m_ogl, &m_vtx, &m_osh, &m_omt);
    }
frmRateBreak();

frmBufferSwap(&m_ogl);             
*/