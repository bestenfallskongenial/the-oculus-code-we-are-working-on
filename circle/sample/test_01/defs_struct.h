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

struct vtx_state
{
    // shared attrib/buffer
    GLuint                      gl_buf;
    GLint                       gl_vtx[MAX_SHADER];    
};

struct glsl_state
{
    GLuint                      gl_shader_id[MAX_SHADER];
    GLuint                      gl_program_id[MAX_SHADER];

    bool                        shader_valid[MAX_SHADER];
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

struct tex_state
{
    u32         max_tex_size;

    bool        tex_valid[MAX_TEXTURE];

    unsigned    width[MAX_TEXTURE];
    unsigned    height[MAX_TEXTURE];
    unsigned    offset[MAX_TEXTURE];

    unsigned    file_size[MAX_TEXTURE];
    unsigned    image_size[MAX_TEXTURE];

    u8*         data[MAX_TEXTURE];
    size_t      size[MAX_TEXTURES];

    GLuint      gl_tex_id[MAX_TEXTURE];
    GLint       u_tex_id[MAX_SHADER][MAX_TEXTURE];
       // for my video frame texture 
    GLuint          gl_tex_vid;                 // video texture handle
    EGLImageKHR     m_EGLimage = nullptr;       // backing (changes per frame) 
};

struct h264_state
{
    // raw input will be populated by parser_init
    u8*             data[MAX_VIDEOS];
    size_t          size[MAX_VIDEOS];
    // frame table -  the actual data we need for decoding 
    void*           frame_address[MAX_VIDEOS][MAX_FRAMES];
    size_t          frame_offset[MAX_VIDEOS][MAX_FRAMES];
    size_t          frame_length[MAX_VIDEOS][MAX_FRAMES];
    size_t          idr_offset[MAX_VIDEOS];                 // size_t idr_offset[MAX_VIDEOS]; ??
	size_t			idr_sc_len[MAX_VIDEOS];
    // extradata
    u8              extradata[MAX_VIDEOS][1024];
    size_t          extradata_len[MAX_VIDEOS];
    bool            extradata_valid[MAX_VIDEOS];
    // parsed metadata
    u16             video_width[MAX_VIDEOS];
    u16             video_height[MAX_VIDEOS];
    u8              vid_profile[MAX_VIDEOS];
    u8              vid_level[MAX_VIDEOS];
    // state
    size_t          frame_count[MAX_VIDEOS];                // unsigned frame_count[MAX_VIDEOS]; ??
    bool            vid_valid[MAX_VIDEOS];
    // shared base
    char*           block_base; // void*   block_base;
    // constraints from the firmware
    u16             max_width;
    u16             max_height;
    u8              max_profile;
    u8              max_level;
};