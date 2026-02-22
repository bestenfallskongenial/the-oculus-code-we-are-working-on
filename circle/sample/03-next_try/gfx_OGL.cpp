// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_init_OGL               (   CUBE_STATE_T *state)
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
                
                state->display = eglGetDisplay                      (   EGL_DEFAULT_DISPLAY     );  // get an EGL display connection
#ifdef __OLG_DEBUG__                
                assert(state->display!=EGL_NO_DISPLAY);
                glslCheck();
#endif // __OLG_DEBUG__
                result = eglInitialize                              (   state->display,         // initialize the EGL display connection
                                                                        NULL, 
                                                                        NULL                    );
#ifdef __OLG_DEBUG__
                assert(EGL_FALSE != result);//?
                glslCheck();
#endif // __OLG_DEBUG__
                result = eglChooseConfig                            (   state->display,         // get an appropriate EGL frame buffer configuration 
                                                                        attribute_list, 
                                                                        &config, 
                                                                        1, 
                                                                        &num_config             );
#ifdef __OLG_DEBUG__                
                assert(EGL_FALSE != result); //?
                glslCheck();
#endif // __OLG_DEBUG__
                result = eglBindAPI(EGL_OPENGL_ES_API);                                         // get an appropriate EGL frame buffer configuration
#ifdef __OLG_DEBUG__
                assert(EGL_FALSE != result);//?
                glslCheck();
#endif // __OLG_DEBUG__
                state->context = eglCreateContext                   (   state->display,         // create an EGL rendering context
                                                                        config, 
                                                                        EGL_NO_CONTEXT, 
                                                                        context_attributes      );
#ifdef __OLG_DEBUG__
                assert(state->context!=EGL_NO_CONTEXT);//?
                glslCheck();
#endif // __OLG_DEBUG__
                success = graphics_get_display_size                 (   0 /* LCD */,            // create an EGL window surface
                                                                        &state->screen_width, 
                                                                        &state->screen_height   );
#ifdef __OLG_DEBUG__
                assert( success >= 0 );
#endif // __OLG_DEBUG__
                dst_rect.x = 0;
                dst_rect.y = 0;
                dst_rect.width = state->screen_width;
                dst_rect.height = state->screen_height;
      
                src_rect.x = 0;
                src_rect.y = 0;
                src_rect.width = state->screen_width << 16;
                src_rect.height = state->screen_height << 16;        

                state->dispman_display = vc_dispmanx_display_open   (   0 /* LCD */ );
                dispman_update = vc_dispmanx_update_start( 0 );
      
                state->dispman_element = vc_dispmanx_element_add    (   dispman_update, 
                                                                        state->dispman_display,
                                                                        0/*layer*/, 
                                                                        &dst_rect, 
                                                                        0/*src*/,
                                                                        &src_rect, 
                                                                        DISPMANX_PROTECTION_NONE, /*protection*/
                                                                        0 /*alpha*/, 
                                                                        0 /*clamp*/, 
                                                                        DISPMANX_NO_ROTATE /*transform*/ );   // was 0/*transform*/ before DISPMANX_ROTATE_90 
      
                nativewindow.element = state->dispman_element;
                nativewindow.width = state->screen_width;
                nativewindow.height = state->screen_height;

                vc_dispmanx_update_submit_sync                      (   dispman_update  );
#ifdef __OLG_DEBUG__
                glslCheck();
#endif // __OLG_DEBUG__            
                state->surface = eglCreateWindowSurface             (   state->display, 
                                                                        config, 
                                                                        &nativewindow, 
                                                                        NULL            );
#ifdef __OLG_DEBUG__
                assert(state->surface != EGL_NO_SURFACE);//?
                glslCheck();
#endif // __OLG_DEBUG__                
                result = eglMakeCurrent                             (   state->display,     // connect the context to the surface
                                                                        state->surface, 
                                                                        state->surface, 
                                                                        state->context  );
#ifdef __OLG_DEBUG__
                assert(EGL_FALSE != result);//?
                glslCheck();
#endif // __OLG_DEBUG__
                glClearColor(0.15f, 0.25f, 0.35f, 1.0f);                                    // Set background color and clear buffers
                glClear( GL_COLOR_BUFFER_BIT );
#ifdef __OLG_DEBUG__
                glslCheck();
#endif // __OLG_DEBUG__            
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------