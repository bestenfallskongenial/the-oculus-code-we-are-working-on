
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::initVshaders          (   glsl_states    *m_glsl, 
                                                        int             p_fromFile, 
                                                        int             p_toFile)    // Function to initialize Vertex Shaders
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    const char *SourcePrtVshader = m_bufferVsh[0]; // will be later maybe changed to multible instances of vshader

                    m_glsl->gl_vsh_id[i] = glCreateShader(GL_VERTEX_SHADER);
                    glShaderSource(m_glsl->gl_vsh_id[i], 1, &SourcePrtVshader, 0);  // will be later maybe changed to multible instances of vshader
                    glCompileShader(m_glsl->gl_vsh_id[i]);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    }
}

void            CKernel::initOshader   (   glsl_states    *m_glsl )    // Function to initialize Fragment Shaders                        <- *m_glsl should change too, right?
{
                    const char *SourcePrtFshader = m_bufferOmf[0]; // because this array is only [1] for consistency

                    m_glsl->gl_oms_id[0] = glCreateShader(GL_FRAGMENT_SHADER);       // gl_oms_id is new for the overlay shader
                    glShaderSource(m_glsl->gl_oms_id[0], 1, &SourcePrtFshader, 0);   // will m_glsl not also become a seperate struct here?!
                    glCompileShader(m_glsl->gl_oms_id[0]);
                    shaderLog(m_glsl->gl_oms_id[0], 1);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__

}
void            CKernel::initFshaders          (   glsl_states    *m_glsl, 
                                                        int             p_fromFile, 
                                                        int             p_toFile)    // Function to initialize Fragment Shaders                        <- we need a copy here for the overlay shader
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    const char *SourcePrtFshader = m_bufferFsh[i];

                    m_glsl->gl_fsh_id[i] = glCreateShader(GL_FRAGMENT_SHADER);
                    glShaderSource(m_glsl->gl_fsh_id[i], 1, &SourcePrtFshader, 0);
                    glCompileShader(m_glsl->gl_fsh_id[i]);
                    shaderLog(m_glsl->gl_fsh_id[i], i);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    }
}
void            CKernel::initOprogram   (   glsl_states    *m_glsl)                                                                   // <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                    if (m_shaderStatusFlags[i])                                     // we need to figure out from where this comes, we need also a separate method here
                        {
                        m_glsl->gl_omp_id[0] = glCreateProgram();
                        glAttachShader(m_glsl->gl_omp_id[0], m_glsl->gl_vsh_id[0]);
                        glAttachShader(m_glsl->gl_omp_id[0], m_glsl->gl_oms_id[0]);
                        glLinkProgram(m_glsl->gl_omp_id[0]);

                        GLint linkStatus;
                        glGetProgramiv(m_glsl->gl_omp_id[0], GL_LINK_STATUS, &linkStatus);

                        programLog(m_glsl->gl_omp_id[0],i);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__

                        if (linkStatus == GL_FALSE) 
                            {
                            glDeleteProgram(m_glsl->gl_omp_id[0]);
                            m_glsl->gl_omp_id[0] = 0;
                            m_shaderStatusFlags[i] = false;
                            }
                        else
                            {
                        //  g_linked_programs_counter++; commented out because this shader program is not part of the user pipeline! Count valid programs for chooseProgram()       ????????????
                            }
                        }
                m_Watchdog.Start(TIMEOUT*3); // new watchdog        
}
void            CKernel::initFprograms          (   glsl_states    *m_glsl, 
                                                        int             p_fromFile, 
                                                        int             p_toFile)                                                                   // <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                    if (m_shaderStatusFlags[i]) 
                        {
                        m_glsl->gl_prg_id[i] = glCreateProgram();
                        glAttachShader(m_glsl->gl_prg_id[i], m_glsl->gl_vsh_id[0]);
                        glAttachShader(m_glsl->gl_prg_id[i], m_glsl->gl_fsh_id[i]);
                        glLinkProgram(m_glsl->gl_prg_id[i]);

                        GLint linkStatus;
                        glGetProgramiv(m_glsl->gl_prg_id[i], GL_LINK_STATUS, &linkStatus);

                        programLog(m_glsl->gl_prg_id[i],i);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__

                        if (linkStatus == GL_FALSE) 
                            {
                            glDeleteProgram(m_glsl->gl_prg_id[i]);
                            m_glsl->gl_prg_id[i] = 0;
                            m_shaderStatusFlags[i] = false;
                            }
                        else
                            {
                            g_linked_programs_counter++;  // Count valid programs for chooseProgram()       ????????????
                            }
                        }
                    m_Watchdog.Start(TIMEOUT*3); // new watchdog        
                    }
}
void            CKernel::initOuniforms  (   glsl_states    *m_glsl)    // Function to initialize Uniforms                           <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                if (m_shaderStatusFlags[i])
                    {
                    glUseProgram(m_glsl->gl_omp_id[0]);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    m_glsl->gl_vtx           = glGetAttribLocation( m_glsl->gl_omp_id[0], "vertex" );

                    m_glsl->u_atlas[0]       = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_atlas" );
                    m_glsl->u_tile_count[0]  = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_count" );
                    m_glsl->u_tile_rect[0]   = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_rect" );
                    m_glsl->u_tile_index[0]  = glGetUniformLocation(m_glsl->gl_omp_id[0], "u_menu_tile_index" );
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    }
}

void            CKernel::initFuniforms          (   glsl_states    *m_glsl, 
                                                        int             p_fromFile, 
                                                        int             p_toFile)    // Function to initialize Uniforms                           <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    if (m_shaderStatusFlags[i])
                        {
                        glUseProgram(m_glsl->gl_prg_id[i]);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        m_glsl->gl_vtx           = glGetAttribLocation( m_glsl->gl_prg_id[i], "vertex" );

                        m_glsl->u_time[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "time" );
                        m_glsl->u_tres[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "tres" );
                        m_glsl->u_seed[i]        = glGetUniformLocation(m_glsl->gl_prg_id[i], "p_seed" );
                        m_glsl->u_aud[i]         = glGetUniformLocation(m_glsl->gl_prg_id[i], "audio" );
                        m_glsl->u_col[i]         = glGetUniformLocation(m_glsl->gl_prg_id[i], "color" );
                        m_glsl->u_par_a[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "par_a" );
                        m_glsl->u_par_b[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "par_b" );

                        m_glsl->u_tex_l[i]       = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex_l" );

                        m_glsl->u_tex_id[i][0]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[0]" );
                        m_glsl->u_tex_id[i][1]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[1]" );
                        m_glsl->u_tex_id[i][2]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[2]" );
                        m_glsl->u_tex_id[i][3]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[3]" );
                        m_glsl->u_tex_id[i][4]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[4]" );
                        m_glsl->u_tex_id[i][5]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[5]" );
                        m_glsl->u_tex_id[i][6]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[6]" );
                        m_glsl->u_tex_id[i][7]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[7]" );
                        m_glsl->u_tex_id[i][8]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[8]" );
                        m_glsl->u_tex_id[i][9]   = glGetUniformLocation(m_glsl->gl_prg_id[i], "tex[9]" );
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        }
                    }
}

void            CKernel::initOtexture       (   glsl_states     *m_glsl)                                                  //  <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                // g_validTextureCount = 0;  // Counter for valid textures only

                    if(m_H264SystemParser.m_tex_valid[0] == true) // do we really need this too? - yes, otherwise we never know the size of it!! 
                        {
                        glGenTextures(1, &m_glsl->gl_omt_id[0]);  // Use counter instead of i - no in this case we use not this one
                        glBindTexture(GL_TEXTURE_2D, m_glsl->gl_omt_id[0]);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        GLvoid* bitmapData = &m_bufferOmt[0][m_H264SystemParser.m_tex_data_offset[0]]; // oh, we need to figure out how we do the bmp parsing for only the atlas!!

                        glTexImage2D(GL_TEXTURE_2D, 
                                   0, 
                                   GL_RGB, 
                                   m_H264SystemParser.m_tex_width[0], // oh, we need to figure out how we do the bmp parsing for only the atlas!!
                                   m_H264SystemParser.m_tex_height[0], 
                                   0, 
                                   GL_RGB, 
                                   GL_UNSIGNED_BYTE, 
                                   bitmapData);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                    //  g_validTextureCount++;  the texture atlas is not part of the user pipeline ! Increment only after successful texture creation
                        }
                    m_Watchdog.Start(TIMEOUT);       // new watchdog
}
void            CKernel::initUtextures              (   glsl_states    *m_glsl, 
                                                            int             p_fromFile, 
                                                            int             p_toFile)                                                  //  <- we need a dedicated copy here, clear seperation but complete emulations ( variables/arrays, etc )
{
                // g_validTextureCount = 0;  // Counter for valid textures only

                for (int i = p_fromFile; i < p_toFile; i++)
                    {
                    if(m_H264Parser.m_tex_valid[i] == true)
                        {
                        glGenTextures(1, &m_glsl->gl_tex_id[g_validTextureCount]);  // Use counter instead of i
                        glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[g_validTextureCount]);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        GLvoid* bitmapData = &m_bufferTex[i][m_H264Parser.m_tex_data_offset[i]];

                        glTexImage2D(GL_TEXTURE_2D, 
                                   0, 
                                   GL_RGB, 
                                   m_H264Parser.m_tex_width[i],
                                   m_H264Parser.m_tex_height[i], 
                                   0, 
                                   GL_RGB, 
                                   GL_UNSIGNED_BYTE, 
                                   bitmapData);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        glBindTexture(GL_TEXTURE_2D, 0);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                        g_validTextureCount++;  // Increment only after successful texture creation
                        }
                    m_Watchdog.Start(TIMEOUT);       // new watchdog
                    }
}

void            CKernel::initVbuffer          (   glsl_states    *m_glsl)                              // Function to initialize Buffers 
{
                static const GLfloat vertex_data[] = 
                    {  -1.0,-1.0, 1.0, 1.0,
                        1.0,-1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0,
                       -1.0, 1.0, 1.0, 1.0  };

                glClearColor(0.0, 1.0, 1.0, 1.0);

                glGenBuffers(1, &m_glsl->gl_buf);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                
                glViewport(0, 0, m_glsl->screen_width, m_glsl->screen_height);                        // Prepare viewport
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
                
                glBindBuffer(GL_ARRAY_BUFFER, m_glsl->gl_buf);                                          // Upload vertex data to a buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
                glVertexAttribPointer(m_glsl->gl_vtx, 4, GL_FLOAT, 0, 16, 0);
                glEnableVertexAttribArray(m_glsl->gl_vtx);
#ifdef __GL_DEBUG__
glslCheck();
#endif // __GL_DEBUG__
}