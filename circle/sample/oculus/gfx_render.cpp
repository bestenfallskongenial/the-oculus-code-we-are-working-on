// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                #include "kernel.h"
                #include "bcm_host.h" 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::render_shader_a(   glsl_state* m_glsl,
                                            char*       buffer,
                                            u32&        index)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);    // Now render to the main frame buffer
        
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   // Clear the background (not really necessary I suppose)
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                glBindBuffer(GL_ARRAY_BUFFER, m_glsl->gl_buf);
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__

                glUseProgram ( m_glsl->gl_prg_id[g_current_gl_program] );

                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__

                GLuint cx = m_glsl->screen_width;
                GLuint cy = m_glsl->screen_height;

                if(m_glsl->u_time[g_current_gl_program] != -1) glUniform1f(    m_glsl->u_time[g_current_gl_program], GLtime);
                if(m_glsl->u_tres[g_current_gl_program]!= -1 ) glUniform2f(    m_glsl->u_tres[g_current_gl_program], cx, cy);
                if(m_glsl->u_seed[g_current_gl_program] != -1) glUniform4f(     m_glsl->u_seed[g_current_gl_program], 
                                                                                                g_inOutMatrixFlt[0][rnd], 
                                                                                                g_inOutMatrixFlt[1][rnd], 
                                                                                                g_inOutMatrixFlt[2][rnd], 
                                                                                                g_inOutMatrixFlt[3][rnd]);
                if(m_glsl->u_aud[g_current_gl_program]!= -1 ) glUniform4f(     m_glsl->u_aud[g_current_gl_program], 
                                                                                                g_inOutMatrixFlt[0][au0], 
                                                                                                g_inOutMatrixFlt[0][au1], 
                                                                                                g_inOutMatrixFlt[0][au2], 
                                                                                                g_inOutMatrixFlt[0][au3]);
                if(m_glsl->u_col[g_current_gl_program] != -1) glUniform4f(     m_glsl->u_col[g_current_gl_program], 0.0f, 0.0f, 0.0f, g_opaque);
                if(m_glsl->u_par_a[g_current_gl_program] != -1 ) glUniform4f(  m_glsl->u_par_a[g_current_gl_program], 
                                                                                                g_inOutMatrixFlt[0][out], 
                                                                                                g_inOutMatrixFlt[1][out], 
                                                                                                g_inOutMatrixFlt[2][out], 
                                                                                                g_inOutMatrixFlt[3][out]);
                if(m_glsl->u_par_b[g_current_gl_program] != -1 ) glUniform4f(  m_glsl->u_par_b[g_current_gl_program], 
                                                                                                g_inOutMatrixFlt[4][out], 
                                                                                                g_inOutMatrixFlt[5][out], 
                                                                                                g_inOutMatrixFlt[6][out], 
                                                                                                g_inOutMatrixFlt[7][out]);
                if(m_glsl->u_tex_l[g_current_gl_program] != -1) glUniform1i(   m_glsl->u_tex_l[g_current_gl_program], p_validTextureCount);

                switch(g_centralModeBuffer[g_currentProgramBuffer][TEX_MODE]) // <- this is using g_currentProgramBuffer because we read an global mode !!! 
                    {
                    case false:     // Original mode
                        for (int i = 0; i < p_validTextureCount; i++) 
                            {
                            glActiveTexture(GL_TEXTURE0+i);
                            glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[i]);
                            if (m_glsl->u_tex_id[g_current_gl_program][i] != -1)
                                {
                                glUniform1i(m_glsl->u_tex_id[g_current_gl_program][i], i);
                                }
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                            }
                        break;

                    case true:      // Single texture mode
                        switch(p_validTextureCount) 
                            {
                            case 0:     // No textures - skip entirely
                                break;
                                
                            case 1:     // Single texture - only bind one
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex]);
                                if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                                break;
                                
                            default:    // Two or more textures - bind current and next
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex]);
                                if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                                
                                glActiveTexture(GL_TEXTURE1);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex + 1]);
                                if (m_glsl->u_tex_id[g_current_gl_program][1] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][1], 1);
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                                break;
                            }
                        break;
                    }
/* 
    debug code start <- was for displaying our "potential" decoded h264 frame, becomes a mode latter on
                 glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_H264Decoder.m_TextureA);
                if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
    debug code end 
 */
                glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__

                glBindBuffer( GL_ARRAY_BUFFER, 0 );

                glFlush();

                if (noTargetFPS==true)
                    {
                    glFinish();
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::render_shader_b(   glsl_state* m_glsl,
                                            char*       buffer,
                                            u32&        index)
{
                eglSwapBuffers(m_glsl->display, m_glsl->surface);
                #ifdef __GL_DEBUG__
                        gfx_check(buffer, index, __FILE__, __LINE__);
                #endif // __GL_DEBUG__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------