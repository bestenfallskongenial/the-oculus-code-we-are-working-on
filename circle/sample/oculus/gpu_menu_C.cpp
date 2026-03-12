// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MenuGpuState g_menu_state = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// compile shader stage
GLuint CKernel::gpu_render_helper_compile_shader(GLenum shader_type, char *shader_source, CString *optional_log)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, 0);
    glCompileShader(shader);

    return shader;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// link program from vertex + fragment shader ids
GLuint CKernel::gpu_render_helper_link_program(GLuint vertex_shader, GLuint fragment_shader, CString *optional_log)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    return program;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// create menu program with shared vertex shader and menu fragment source
// ? what is this? what is this for ? where has it reference in gfx.cpp ?
bool CKernel::gpu_render_helper_build_menu_program(GLuint shared_vertex_shader, char *menu_fragment_source, GLuint *out_program, CString *optional_log)
{
    GLuint menu_fragment_shader = gpu_render_helper_compile_shader(GL_FRAGMENT_SHADER, menu_fragment_source, optional_log);
    GLuint menu_program = gpu_render_helper_link_program(shared_vertex_shader, menu_fragment_shader, optional_log);

    glDeleteShader(menu_fragment_shader);

    *out_program = menu_program;
    return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// upload decoded rgb24 atlas to gl texture
bool CKernel::gpu_render_helper_upload_menu_atlas_rgb24(void *rgb_pixels, unsigned width, unsigned height, GLuint *out_texture)
{
    GLuint texture = 0;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    *out_texture = texture;
    return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// initialize menu render states and build static rectangles + current indices
bool CKernel::gpu_render_menu_init(GLuint shared_vertex_shader, char *menu_fsh_source, GLuint atlas_texture)
{
    GLuint fsh = gpu_render_helper_compile_shader(GL_FRAGMENT_SHADER, menu_fsh_source, 0);

    GLuint prg = glCreateProgram();
    glAttachShader(prg, shared_vertex_shader);
    glAttachShader(prg, fsh);
    glLinkProgram(prg);
    glDeleteShader(fsh);

    g_menu_state.program = prg;
    g_menu_state.atlas_texture = atlas_texture;
    g_menu_state.a_vertex = glGetAttribLocation(prg, "vertex");
    g_menu_state.u_atlas = glGetUniformLocation(prg, "u_menu_atlas");
    g_menu_state.u_tile_count = glGetUniformLocation(prg, "u_menu_tile_count");
    g_menu_state.u_tile_rect = glGetUniformLocation(prg, "u_menu_tile_rect");
    g_menu_state.u_tile_index = glGetUniformLocation(prg, "u_menu_tile_index");

    float ox = MENU_GPU_ORIGIN[0];
    float oy = MENU_GPU_ORIGIN[1];
    float tw = MENU_GPU_TILE_SIZE[0];
    float th = MENU_GPU_TILE_SIZE[1];

    for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
    {
        float sx = MENU_GPU_REL_SIZE[i][0];
        float sy = MENU_GPU_REL_SIZE[i][1];

        if (i == 14 || i == 15)
        {
            sx *= MENU_GPU_BACKGROUND_SCALE[0];
            sy *= MENU_GPU_BACKGROUND_SCALE[1];
        }

        g_menu_state.tile_rect[i].x = ox + MENU_GPU_REL_POS[i][0] * tw;
        g_menu_state.tile_rect[i].y = oy + MENU_GPU_REL_POS[i][1] * th;
        g_menu_state.tile_rect[i].w = sx * tw;
        g_menu_state.tile_rect[i].h = sy * th;
    }

    g_menu_state.tile_index[0] = g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
    g_menu_state.tile_index[1] = g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
    g_menu_state.tile_index[2] = g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
    g_menu_state.tile_index[3] = g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
    g_menu_state.tile_index[4] = g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
    g_menu_state.tile_index[5] = g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
    g_menu_state.tile_index[6] = g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
    g_menu_state.tile_index[7] = g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

    unsigned long bpm0 = g_resultBPM[0] % 10000UL;
    unsigned long bpm1 = g_resultBPM[1] % 10UL;

    g_menu_state.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
    g_menu_state.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
    g_menu_state.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
    g_menu_state.tile_index[11] = (int) (bpm0 % 10UL);
    g_menu_state.tile_index[12] = 10;
    g_menu_state.tile_index[13] = (int) bpm1;
    g_menu_state.tile_index[14] = 48;
    g_menu_state.tile_index[15] = 49;

    g_menu_state.initialized = true;
    return true;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// rebuild layout values from global menu knobs
void CKernel::gpu_render_menu_rebuild_layout()
{
    float ox = MENU_GPU_ORIGIN[0];
    float oy = MENU_GPU_ORIGIN[1];
    float tw = MENU_GPU_TILE_SIZE[0];
    float th = MENU_GPU_TILE_SIZE[1];

    for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
    {
        float sx = MENU_GPU_REL_SIZE[i][0];
        float sy = MENU_GPU_REL_SIZE[i][1];

        if (i == 14 || i == 15)
        {
            sx *= MENU_GPU_BACKGROUND_SCALE[0];
            sy *= MENU_GPU_BACKGROUND_SCALE[1];
        }

        g_menu_state.tile_rect[i].x = ox + MENU_GPU_REL_POS[i][0] * tw;
        g_menu_state.tile_rect[i].y = oy + MENU_GPU_REL_POS[i][1] * th;
        g_menu_state.tile_rect[i].w = sx * tw;
        g_menu_state.tile_rect[i].h = sy * th;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// rebuild dynamic indices from mode buffers and bpm values
void CKernel::gpu_render_menu_update_indices()
{
    g_menu_state.tile_index[0] = g_centralModeBuffer[g_currentProgramBuffer][CH0_MODE];
    g_menu_state.tile_index[1] = g_centralModeBuffer[g_currentProgramBuffer][CH1_MODE];
    g_menu_state.tile_index[2] = g_centralModeBuffer[g_currentProgramBuffer][CH2_MODE];
    g_menu_state.tile_index[3] = g_centralModeBuffer[g_currentProgramBuffer][CH3_MODE];
    g_menu_state.tile_index[4] = g_centralModeBuffer[g_currentProgramBuffer][CH4_MODE];
    g_menu_state.tile_index[5] = g_centralModeBuffer[g_currentProgramBuffer][CH5_MODE];
    g_menu_state.tile_index[6] = g_centralModeBuffer[g_currentProgramBuffer][CH6_MODE];
    g_menu_state.tile_index[7] = g_centralModeBuffer[g_currentProgramBuffer][CH7_MODE];

    unsigned long bpm0 = g_resultBPM[0] % 10000UL;
    unsigned long bpm1 = g_resultBPM[1] % 10UL;

    g_menu_state.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
    g_menu_state.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
    g_menu_state.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
    g_menu_state.tile_index[11] = (int) (bpm0 % 10UL);
    g_menu_state.tile_index[12] = 10;
    g_menu_state.tile_index[13] = (int) bpm1;
    g_menu_state.tile_index[14] = 48;
    g_menu_state.tile_index[15] = 49;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// draw menu layer using current menu states arrays
void CKernel::gpu_render_menu_draw(GLuint fullscreen_vbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);
    glUseProgram(g_menu_state.program);

    glVertexAttribPointer(g_menu_state.a_vertex, 4, GL_FLOAT, 0, 16, 0);
    glEnableVertexAttribArray(g_menu_state.a_vertex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_menu_state.atlas_texture);

    glUniform1i(g_menu_state.u_atlas, 0);
    glUniform1i(g_menu_state.u_tile_count, MENU_GPU_TILE_COUNT);
    glUniform4fv(g_menu_state.u_tile_rect, MENU_GPU_TILE_COUNT, (GLfloat *) g_menu_state.tile_rect);

    GLfloat index_values[16];
    for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
        index_values[i] = (GLfloat) g_menu_state.tile_index[i];

    glUniform1fv(g_menu_state.u_tile_index, MENU_GPU_TILE_COUNT, index_values);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_BLEND);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// shutdown menu render program and reset cached states
void CKernel::gpu_render_menu_shutdown()
{
    glDeleteProgram(g_menu_state.program);
    g_menu_state = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------