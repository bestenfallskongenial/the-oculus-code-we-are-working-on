#include "kernel.h"
#include "global.h"

// menu gpu files on storage
char *MENU_FSH_EXTENSIONS_2[1] = { (char *)"fsh" };
char *MENU_TEX_EXTENSIONS_2[1] = { (char *)"bmp" };

char *SCANED_FILES_MENU_FSH_2[1] = { 0 };
char *SCANED_FILES_MENU_TEX_2[1] = { 0 };

unsigned MENU_FSH_LOADED_BYTES_2[1] = { 0 };
unsigned MENU_TEX_LOADED_BYTES_2[1] = { 0 };

// menu layout controls
float MENU_GPU_ORIGIN_2[2] = { 0.05f, 0.08f };
float MENU_GPU_TILE_SIZE_2[2] = { 0.08f, 0.08f };
float MENU_GPU_BACKGROUND_SCALE_2[2] = { 1.0f, 1.0f };

// slots 0..7 mode, 8..11 bpm digits, 12 dot, 13 bpm icon, 14..15 backgrounds
float MENU_GPU_REL_POS_2[16][2] =
{
	{0.0f, 0.0f}, {1.1f, 0.0f}, {2.2f, 0.0f}, {3.3f, 0.0f},
	{0.0f, 1.1f}, {1.1f, 1.1f}, {2.2f, 1.1f}, {3.3f, 1.1f},
	{0.0f, 2.6f}, {0.9f, 2.6f}, {1.8f, 2.6f}, {2.7f, 2.6f},
	{3.6f, 2.6f}, {4.1f, 2.6f},
	{-0.1f, -0.1f},
	{-0.1f, 2.5f}
};

float MENU_GPU_REL_SIZE_2[16][2] =
{
	{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
	{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
	{0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f}, {0.75f, 0.95f},
	{0.35f, 0.95f}, {0.75f, 0.95f},
	{4.4f, 2.2f},
	{5.8f, 1.2f}
};

int MENU_GPU_TILE_COUNT_2 = 16;

struct MenuTileRect2
{
	float x;
	float y;
	float w;
	float h;
};

struct MenuGpuState2
{
	bool initialized;
	GLuint program;
	GLuint atlas_texture;
	GLint a_vertex;
	GLint u_atlas;
	GLint u_tile_count;
	GLint u_tile_rect;
	GLint u_tile_index;
	int tile_index[16];
	MenuTileRect2 tile_rect[16];
};

MenuGpuState2 g_menu_state_2 = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };

// load menu fragment shader source directly
bool CKernel::gpu_render_helper_load_menu_fsh_2(char **fshader_buffers, unsigned fshader_buffer_size, int load_mode)
{
	MENU_FSH_LOADED_BYTES_2[0] = 0;
	filesystem_open_file(SCANED_FILES_MENU_FSH_2[0]);
	MENU_FSH_LOADED_BYTES_2[0] = filesystem_load_file(fshader_buffers[0], fshader_buffer_size, load_mode);
	filesystem_close_file();
	return MENU_FSH_LOADED_BYTES_2[0] > 0;
}

// load menu atlas texture bytes directly
bool CKernel::gpu_render_helper_load_menu_tex_2(char **texture_buffers, unsigned texture_buffer_size, int load_mode)
{
	MENU_TEX_LOADED_BYTES_2[0] = 0;
	filesystem_open_file(SCANED_FILES_MENU_TEX_2[0]);
	MENU_TEX_LOADED_BYTES_2[0] = filesystem_load_file(texture_buffers[0], texture_buffer_size, load_mode);
	filesystem_close_file();
	return MENU_TEX_LOADED_BYTES_2[0] > 0;
}

// compile shader stage
GLuint CKernel::gpu_render_helper_compile_shader_2(GLenum shader_type, char *shader_source)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, 0);
	glCompileShader(shader);
	return shader;
}

// link program
GLuint CKernel::gpu_render_helper_link_program_2(GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	return program;
}

// build menu program
bool CKernel::gpu_render_helper_build_menu_program_2(GLuint shared_vertex_shader, char *menu_fragment_source, GLuint *out_program)
{
	GLuint menu_fragment_shader = gpu_render_helper_compile_shader_2(GL_FRAGMENT_SHADER, menu_fragment_source);
	GLuint menu_program = gpu_render_helper_link_program_2(shared_vertex_shader, menu_fragment_shader);
	glDeleteShader(menu_fragment_shader);
	*out_program = menu_program;
	return true;
}

// upload rgb24 atlas texture
bool CKernel::gpu_render_helper_upload_menu_atlas_rgb24_2(void *rgb_pixels, unsigned width, unsigned height, GLuint *out_texture)
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

// init menu program and prebuild layout + indices
bool CKernel::gpu_render_menu_init_2(GLuint shared_vertex_shader, char *menu_fsh_source, GLuint atlas_texture)
{
	GLuint fsh = gpu_render_helper_compile_shader_2(GL_FRAGMENT_SHADER, menu_fsh_source);
	GLuint prg = glCreateProgram();
	glAttachShader(prg, shared_vertex_shader);
	glAttachShader(prg, fsh);
	glLinkProgram(prg);
	glDeleteShader(fsh);

	g_menu_state_2.program = prg;
	g_menu_state_2.atlas_texture = atlas_texture;
	g_menu_state_2.a_vertex = glGetAttribLocation(prg, "vertex");
	g_menu_state_2.u_atlas = glGetUniformLocation(prg, "u_menu_atlas");
	g_menu_state_2.u_tile_count = glGetUniformLocation(prg, "u_menu_tile_count");
	g_menu_state_2.u_tile_rect = glGetUniformLocation(prg, "u_menu_tile_rect");
	g_menu_state_2.u_tile_index = glGetUniformLocation(prg, "u_menu_tile_index");

	for (int i = 0; i < MENU_GPU_TILE_COUNT_2; i++)
	{
		float sx = MENU_GPU_REL_SIZE_2[i][0];
		float sy = MENU_GPU_REL_SIZE_2[i][1];

		if (i == 14 || i == 15)
		{
			sx *= MENU_GPU_BACKGROUND_SCALE_2[0];
			sy *= MENU_GPU_BACKGROUND_SCALE_2[1];
		}

		g_menu_state_2.tile_rect[i].x = MENU_GPU_ORIGIN_2[0] + MENU_GPU_REL_POS_2[i][0] * MENU_GPU_TILE_SIZE_2[0];
		g_menu_state_2.tile_rect[i].y = MENU_GPU_ORIGIN_2[1] + MENU_GPU_REL_POS_2[i][1] * MENU_GPU_TILE_SIZE_2[1];
		g_menu_state_2.tile_rect[i].w = sx * MENU_GPU_TILE_SIZE_2[0];
		g_menu_state_2.tile_rect[i].h = sy * MENU_GPU_TILE_SIZE_2[1];
	}

	g_menu_state_2.tile_index[0] = mode_storage_buffers[CH0_MODE][current_buffer];
	g_menu_state_2.tile_index[1] = mode_storage_buffers[CH1_MODE][current_buffer];
	g_menu_state_2.tile_index[2] = mode_storage_buffers[CH2_MODE][current_buffer];
	g_menu_state_2.tile_index[3] = mode_storage_buffers[CH3_MODE][current_buffer];
	g_menu_state_2.tile_index[4] = mode_storage_buffers[CH4_MODE][current_buffer];
	g_menu_state_2.tile_index[5] = mode_storage_buffers[CH5_MODE][current_buffer];
	g_menu_state_2.tile_index[6] = mode_storage_buffers[CH6_MODE][current_buffer];
	g_menu_state_2.tile_index[7] = mode_storage_buffers[CH7_MODE][current_buffer];

	unsigned long bpm0 = resultBPM[0] % 10000UL;
	unsigned long bpm1 = resultBPM[1] % 10UL;
	g_menu_state_2.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
	g_menu_state_2.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
	g_menu_state_2.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
	g_menu_state_2.tile_index[11] = (int) (bpm0 % 10UL);
	g_menu_state_2.tile_index[12] = 10;
	g_menu_state_2.tile_index[13] = (int) bpm1;
	g_menu_state_2.tile_index[14] = 48;
	g_menu_state_2.tile_index[15] = 49;

	g_menu_state_2.initialized = true;
	return true;
}

// rebuild rectangles inline
void CKernel::gpu_render_menu_rebuild_layout_2()
{
	for (int i = 0; i < MENU_GPU_TILE_COUNT_2; i++)
	{
		float sx = MENU_GPU_REL_SIZE_2[i][0];
		float sy = MENU_GPU_REL_SIZE_2[i][1];

		if (i == 14 || i == 15)
		{
			sx *= MENU_GPU_BACKGROUND_SCALE_2[0];
			sy *= MENU_GPU_BACKGROUND_SCALE_2[1];
		}

		g_menu_state_2.tile_rect[i].x = MENU_GPU_ORIGIN_2[0] + MENU_GPU_REL_POS_2[i][0] * MENU_GPU_TILE_SIZE_2[0];
		g_menu_state_2.tile_rect[i].y = MENU_GPU_ORIGIN_2[1] + MENU_GPU_REL_POS_2[i][1] * MENU_GPU_TILE_SIZE_2[1];
		g_menu_state_2.tile_rect[i].w = sx * MENU_GPU_TILE_SIZE_2[0];
		g_menu_state_2.tile_rect[i].h = sy * MENU_GPU_TILE_SIZE_2[1];
	}
}

// rebuild indices inline
void CKernel::gpu_render_menu_update_indices_2()
{
	g_menu_state_2.tile_index[0] = mode_storage_buffers[CH0_MODE][current_buffer];
	g_menu_state_2.tile_index[1] = mode_storage_buffers[CH1_MODE][current_buffer];
	g_menu_state_2.tile_index[2] = mode_storage_buffers[CH2_MODE][current_buffer];
	g_menu_state_2.tile_index[3] = mode_storage_buffers[CH3_MODE][current_buffer];
	g_menu_state_2.tile_index[4] = mode_storage_buffers[CH4_MODE][current_buffer];
	g_menu_state_2.tile_index[5] = mode_storage_buffers[CH5_MODE][current_buffer];
	g_menu_state_2.tile_index[6] = mode_storage_buffers[CH6_MODE][current_buffer];
	g_menu_state_2.tile_index[7] = mode_storage_buffers[CH7_MODE][current_buffer];

	unsigned long bpm0 = resultBPM[0] % 10000UL;
	unsigned long bpm1 = resultBPM[1] % 10UL;
	g_menu_state_2.tile_index[8] = (int) ((bpm0 / 1000UL) % 10UL);
	g_menu_state_2.tile_index[9] = (int) ((bpm0 / 100UL) % 10UL);
	g_menu_state_2.tile_index[10] = (int) ((bpm0 / 10UL) % 10UL);
	g_menu_state_2.tile_index[11] = (int) (bpm0 % 10UL);
	g_menu_state_2.tile_index[12] = 10;
	g_menu_state_2.tile_index[13] = (int) bpm1;
	g_menu_state_2.tile_index[14] = 48;
	g_menu_state_2.tile_index[15] = 49;
}

// draw menu full screen fan
void CKernel::gpu_render_menu_draw_2(GLuint fullscreen_vbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, fullscreen_vbo);
	glUseProgram(g_menu_state_2.program);
	glVertexAttribPointer(g_menu_state_2.a_vertex, 4, GL_FLOAT, 0, 16, 0);
	glEnableVertexAttribArray(g_menu_state_2.a_vertex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_menu_state_2.atlas_texture);
	glUniform1i(g_menu_state_2.u_atlas, 0);
	glUniform1i(g_menu_state_2.u_tile_count, MENU_GPU_TILE_COUNT_2);
	glUniform4fv(g_menu_state_2.u_tile_rect, MENU_GPU_TILE_COUNT_2, (GLfloat *) g_menu_state_2.tile_rect);

	GLfloat index_values[16];
	for (int i = 0; i < MENU_GPU_TILE_COUNT_2; i++)
		index_values[i] = (GLfloat) g_menu_state_2.tile_index[i];

	glUniform1fv(g_menu_state_2.u_tile_index, MENU_GPU_TILE_COUNT_2, index_values);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisable(GL_BLEND);
}

// shutdown menu gpu program
void CKernel::gpu_render_menu_shutdown_2()
{
	glDeleteProgram(g_menu_state_2.program);
	g_menu_state_2 = { false, 0, 0, -1, -1, -1, -1, -1, {0}, {{0.0f, 0.0f, 0.0f, 0.0f}} };
}