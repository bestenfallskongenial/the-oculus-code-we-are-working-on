// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
*/
// the files stays at it is - we will later change the logging to the methods we developed in vc_h264_decoder.cpp and vc_vcsm.cpp - clear, deterministic without extra dependencies.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                #undef __OLG_DEBUG__
                #undef __GL_DEBUG__


                #include "kernel.h"
                #include "global.h"
                #include "bcm_host.h" 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::render_shader_a(   glsl_state* m_glsl)
{
                glBindFramebuffer(GL_FRAMEBUFFER,0);    // Now render to the main frame buffer
        
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   // Clear the background (not really necessary I suppose)
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                glBindBuffer(GL_ARRAY_BUFFER, m_glsl->gl_buf);
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__

                glUseProgram ( m_glsl->gl_prg_id[g_current_gl_program] );

                #ifdef __GL_DEBUG__
                        glslCheck();
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
                if(m_glsl->u_tex_l[g_current_gl_program] != -1) glUniform1i(   m_glsl->u_tex_l[g_current_gl_program], g_validTextureCount);

                switch(g_centralModeBuffer[g_currentProgramBuffer][TEX_MODE]) // <- this is using g_currentProgramBuffer because we read an global mode !!! 
                    {
                    case false:     // Original mode
                        for (int i = 0; i < g_validTextureCount; i++) 
                            {
                            glActiveTexture(GL_TEXTURE0+i);
                            glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[i]);
                            if (m_glsl->u_tex_id[g_current_gl_program][i] != -1)
                                {
                                glUniform1i(m_glsl->u_tex_id[g_current_gl_program][i], i);
                                }
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                            }
                        break;

                    case true:      // Single texture mode
                        switch(g_validTextureCount) 
                            {
                            case 0:     // No textures - skip entirely
                                break;
                                
                            case 1:     // Single texture - only bind one
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex]);
                                if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                                break;
                                
                            default:    // Two or more textures - bind current and next
                                glActiveTexture(GL_TEXTURE0);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex]);
                                if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                                
                                glActiveTexture(GL_TEXTURE1);
                                glBindTexture(GL_TEXTURE_2D, m_glsl->gl_tex_id[gl_current_tex + 1]);
                                if (m_glsl->u_tex_id[g_current_gl_program][1] != -1)
                                    glUniform1i(m_glsl->u_tex_id[g_current_gl_program][1], 1);
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                                break;
                            }
                        break;
                    }
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/* 
// debug code start                                                                                                 <- was for displaying our "potential" decoded h264 frame, bul latter mode
 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_H264Decoder.m_TextureA);
        if (m_glsl->u_tex_id[g_current_gl_program][0] != -1)
            glUniform1i(m_glsl->u_tex_id[g_current_gl_program][0], 0);
 debug code end 
 */
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__

                glBindBuffer( GL_ARRAY_BUFFER, 0 );

                glFlush();

                if (noTargetFPS==true)
                    {
                    glFinish();
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::render_shader_b(   glsl_state* m_glsl)
{
                eglSwapBuffers(m_glsl->display, m_glsl->surface);
                #ifdef __GL_DEBUG__
                        glslCheck();
                #endif // __GL_DEBUG__
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------