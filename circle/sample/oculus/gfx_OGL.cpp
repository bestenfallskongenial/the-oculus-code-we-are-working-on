// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_init_OGL   (   glsl_state* m_glsl,
                                            char*       buffer,
                                            u32&        index)
{
                int32_t success = 0;
                EGLBoolean result;
                EGLint num_config;

                static EGL_DISPMANX_WINDOW_T nativewindow;

                DISPMANX_UPDATE_HANDLE_T dispman_update;
                VC_RECT_T dst_rect;
                VC_RECT_T src_rect;

                static const EGLint attribute_list[] =
                    {
                    EGL_RED_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_BLUE_SIZE, 8,
                    EGL_ALPHA_SIZE, 8,
                    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                    EGL_NONE
                    };
   
                static const EGLint context_attributes[] = 
                    {
                    EGL_CONTEXT_CLIENT_VERSION, 2,
                    EGL_NONE
                    };

                EGLConfig config;
                
                m_glsl->display             = eglGetDisplay             (   EGL_DEFAULT_DISPLAY     );  // get an EGL display connection
#ifdef __OLG_DEBUG__                
assert(m_glsl->display!=EGL_NO_DISPLAY);
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                result                      = eglInitialize             (   m_glsl->display,         // initialize the EGL display connection
                                                                            NULL, 
                                                                            NULL                    );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                result                      = eglChooseConfig           (   m_glsl->display,         // get an appropriate EGL frame buffer configuration 
                                                                            attribute_list, 
                                                                            &config, 
                                                                            1, 
                                                                            &num_config             );
#ifdef __OLG_DEBUG__                
assert(EGL_FALSE != result); //?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                result                      = eglBindAPI                (   EGL_OPENGL_ES_API);                                         // get an appropriate EGL frame buffer configuration
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                m_glsl->context             = eglCreateContext          (   m_glsl->display,         // create an EGL rendering context
                                                                            config, 
                                                                            EGL_NO_CONTEXT, 
                                                                            context_attributes      );
#ifdef __OLG_DEBUG__
assert(m_glsl->context!=EGL_NO_CONTEXT);//?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                success                     = graphics_get_display_size (   0 /* LCD */,            // create an EGL window surface
                                                                            &m_glsl->screen_width, 
                                                                            &m_glsl->screen_height   );
#ifdef __OLG_DEBUG__
assert( success >= 0 );
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                dst_rect.x                  = 0;
                dst_rect.y                  = 0;
                dst_rect.width              = m_glsl->screen_width;
                dst_rect.height             = m_glsl->screen_height;
      
                src_rect.x                  = 0;
                src_rect.y                  = 0;
                src_rect.width              = m_glsl->screen_width << 16;
                src_rect.height             = m_glsl->screen_height << 16;        

                m_glsl->dispman_display     = vc_dispmanx_display_open  (   0 /* LCD */ );
                dispman_update              = vc_dispmanx_update_start  (   0 );
      
                m_glsl->dispman_element     = vc_dispmanx_element_add   (   dispman_update, 
                                                                            m_glsl->dispman_display,
                                                                            0/*layer*/, 
                                                                            &dst_rect, 
                                                                            0/*src*/,
                                                                            &src_rect, 
                                                                            DISPMANX_PROTECTION_NONE, /*protection*/
                                                                            0 /*alpha*/, 
                                                                            0 /*clamp*/, 
                                                                            DISPMANX_NO_ROTATE /*transform*/ );   // was 0/*transform*/ before DISPMANX_ROTATE_90 
      
                nativewindow.element        = m_glsl->dispman_element;
                nativewindow.width          = m_glsl->screen_width;
                nativewindow.height         = m_glsl->screen_height;

                vc_dispmanx_update_submit_sync                          (   dispman_update  );
#ifdef __OLG_DEBUG__
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__            
                m_glsl->surface             = eglCreateWindowSurface    (   m_glsl->display, 
                                                                            config, 
                                                                            &nativewindow, 
                                                                            NULL            );
#ifdef __OLG_DEBUG__
assert(m_glsl->surface != EGL_NO_SURFACE);//?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__                
                result                      = eglMakeCurrent            (   m_glsl->display,     // connect the context to the surface
                                                                            m_glsl->surface, 
                                                                            m_glsl->surface, 
                                                                            m_glsl->context  );
#ifdef __OLG_DEBUG__
assert(EGL_FALSE != result);//?
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__
                                              glClearColor              (   0.15f, 0.25f, 0.35f, 1.0f);                                    // Set background color and clear buffers
                                              glClear                   (   GL_COLOR_BUFFER_BIT );
#ifdef __OLG_DEBUG__
gfx_check(buffer, index, __FILE__, __LINE__);
#endif // __OLG_DEBUG__            
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------