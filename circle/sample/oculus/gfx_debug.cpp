// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my new gfx log with my own log code
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::shaderLog(char* buffer, u32& index, GLint shader, int shaderIndex)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    m_shaderStatusFlags[shaderIndex] = (success == GL_TRUE);

    storeLog(buffer, index, "----------------------------------------------------------------");
    storeLog(buffer, index, "Program compile status idx/success", (u32)shaderIndex, (u32)success);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::programLog(char* buffer, u32& index, GLint shader, int program_index)
{
    int internal_index = 0;
    if (program_index > 0)
        internal_index = program_index - 1;

    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    storeLog(buffer, index, "----------------------------------------------------------------");
    storeLog(buffer, index, "Program link status idx/success", (u32)program_index, (u32)success);

    char name[27];
    strncpy(name, &m_bufferFsh[program_index][2], 26);
    name[26] = '\0';

    storeMsg(buffer, index, "Program short name", name, 26);
    storeMsg(buffer, index, "Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
    storeLog(buffer, index, "Program byte size", (u32)g_bytFsh[program_index]);

    char log[1024];
    glGetProgramInfoLog(shader, sizeof(log), NULL, log);
    storeMsg(buffer, index, "Program InfoLog", log, sizeof(log));

    GLint numUniforms;
    glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);

    storeLog(buffer, index, "Active Uniforms", (u32)numUniforms);

    for (GLint i = 0; i < numUniforms; ++i)
    {
        char uname[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(shader, i, sizeof(uname), &length, &size, &type, uname);
        GLint location = glGetUniformLocation(shader, uname);

        storeLog(buffer, index, "Uniform idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);

        storeMsg(buffer, index, "Uniform name", uname, length);
    }

    GLint numAttributes;
    glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    storeLog(buffer, index, "Active Attributes", (u32)numAttributes);

    for (GLint i = 0; i < numAttributes; ++i)
    {
        char aname[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveAttrib(shader, i, sizeof(aname), &length, &size, &type, aname);
        GLint location = glGetAttribLocation(shader, aname);

        storeLog(buffer, index, "Attribute idx/size/type/loc", (u32)i, (u32)size, (u32)type, (u32)location);

        storeMsg(buffer, index, "Attribute name", aname, length);
    }

    storeLog(buffer, index, "----------------------------------------------------------------");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::gfx_check(char* buffer, u32& index, const char* file, unsigned line)
{
    static unsigned error_count = 0;
    static bool summary_written = false;
    const unsigned ERROR_THRESHOLD = 1024;

    if (resetFlag && !summary_written)
    {
        summary_written = true;

        CTimer* pTimer = CTimer::Get();
        unsigned ticks = pTimer->GetTicks();

        storeLog(buffer, index, "=== Final System Status ticks/count ===", (u32)ticks, (u32)error_count);

        GLint value;

        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        storeLog(buffer, index, "Current Program", (u32)value);

        glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
        storeLog(buffer, index, "Active Texture Unit", (u32)value);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        storeLog(buffer, index, "Viewport x/y", (u32)viewport[0], (u32)viewport[1]);
        storeLog(buffer, index, "Viewport w/h", (u32)viewport[2], (u32)viewport[3]);

        GLint fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        storeLog(buffer, index, "Current Framebuffer", (u32)fb);

        storeLog(buffer, index, "=== End Status Report ===");
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

        storeLog(buffer, index, "OpenGL Error err/ticks/line", (u32)error, (u32)ticks, (u32)line);

        storeLog(buffer, index, severity);
        storeLog(buffer, index, error_str);
        storeLog(buffer, index, file);

        error_count++;
        if (error_count >= ERROR_THRESHOLD)
            resetFlag = true;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------