WHAT IS DONE


//  WHAT is STILL OPEN

//  *** vc04_services.cpp:

//  seems we need to adapt the texture creation and frame poller function here to use our unified texture struct

//  Correct approach

//  Integrate MMAL texture into tex_state, not outside.

//  - Extend tex_state

            GLuint gl_tex_vid;      // video texture handle
            EGLImageKHR egl_img;    // backing (changes per frame)

//  Unified init (same style as others)

//  Refactor your MMAL init to match:

        bool CKernel::initTextureVideo(tex_state* t)
        {
            glGenTextures(1, &t->gl_tex_vid);
            glBindTexture(GL_TEXTURE_2D, t->gl_tex_vid);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_2D, 0);

            return true;
        }
//  Unified update (same pipeline style)
        bool CKernel::updateTextureVideo(tex_state* t, u32 handle)
        {
            if (t->egl_img != EGL_NO_IMAGE_KHR)
            {
                eglDestroyImageKHR(m_eglDisplay, t->egl_img);
                t->egl_img = EGL_NO_IMAGE_KHR;
            }

            egl_image_brcm_vcsm_info info =
            {
                .width       = m_ResolutionX,
                .height      = m_ResolutionY,
                .vcsm_handle = handle
            };

            t->egl_img = eglCreateImageKHR(m_eglDisplay,
                                        m_eglContext,
                                        EGL_IMAGE_BRCM_VCSM,
                                        (EGLClientBuffer)&info,
                                        nullptr);

            if (t->egl_img == EGL_NO_IMAGE_KHR)
                return false;

            glBindTexture(GL_TEXTURE_2D, t->gl_tex_vid);
            glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, t->egl_img);
            glBindTexture(GL_TEXTURE_2D, 0);

            return true;
        }
//      Integration into render pipeline

//      Now it behaves like any texture:

//      setTexPrg(...);

//      Decision point inside:

        if (use_video)
        {
            bind t->gl_tex_vid
        }
        else
        {
            bind t->gl_tex_id[]
        }

//      additional informations

// our buffers members for the allocation

    olg_state                   m_ogl;

    vtx_state                m_vtx;

    glsl_state                  m_vsh;
    glsl_state                  m_fsh;
    glsl_state                  m_osh;

    tex_state               m_tex;
    tex_state               m_omt;

// example calls for wrapper
// SHADERS 
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
           fsh_flags);                          // the signature is designed to have a clear pattern, the functions to use the same parameter matrix
// PROGRAMS 
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
// TEXTURES
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
// UNIFORMS
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
// RUNTIME
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
