// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::gfx_shader_log             (   GLint   shader, 
                                                        int     shaderIndex)
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Get compilation status
                
                m_shaderStatusFlags[shaderIndex] = (success == GL_TRUE); // Store status in our flags array
}
// we need to implement the same logging mechanism we wrote for the vc4 cedoder and vcsm code. 
// clear, deterministic without extra dependencies.
void            CKernel::gfx_program_log            (   GLint   shader, 
                                                        int     program_index) 
{
                int internal_index =0;
                if ( program_index > 0 ) 
                    {
                    internal_index = program_index - 1;
                    }

                GLint success;
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                
                CString log_message;
                log_message.Format( "----------------------------------------------------------------\n"
                                    "Program %d link status: %s\n\n", 
                                    program_index,
                                    (success == GL_TRUE ? "Success" : "Failed"));
                g_log_string.Append(log_message);

                char name[27];  // 40 chars + null terminator
                strncpy(name, &m_bufferFsh[program_index][2], 26);

                name[26] = '\0';
                log_message.Format( "%s Filename: %s Size %u\n"
                                    "----------------------------------------------------------------\n", 
                                    name, g_ScnFsh[internal_index],
                                    g_bytFsh[program_index]);
                g_log_string.Append(log_message);

                char log[1024];
                glGetProgramInfoLog(shader, sizeof(log), NULL, log);
                
                log_message.Format(" %s\n", log);    
                g_log_string.Append(log_message);  // Append the u_program_handle log to the global string
                
                GLint numUniforms;
                glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);    // Query and append active uniforms information
                
                for (GLint i = 0; i < numUniforms; ++i) 
                    {
                    char name[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;
                    glGetActiveUniform(shader, i, sizeof(name), &length, &size, &type, name);
                    
                    GLint location = glGetUniformLocation(shader, name);
                    
                    log_message.Format("Uniform   #%-2d: name=%-10s, size=%-2d, type=0x%-4x, location=%-2d\n",
                                i, name, size, type, location);
                                
                    g_log_string.Append(log_message);  // Append uniform info to the global string
                    }
                g_log_string.Append("\n");
                
                GLint numAttributes;
                glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);   // Query and append active attributes information
                
                for (GLint i = 0; i < numAttributes; ++i) 
                    {
                    char name[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;
                    glGetActiveAttrib(shader, i, sizeof(name), &length, &size, &type, name);
                    
                    GLint location = glGetAttribLocation(shader, name);
                    
                    log_message.Format("Attribute #%-2d: name=%-10s, size=%-2d, type=0x%-4x, location=%-2d\n\n",
                                i, name, size, type, location);
                    g_log_string.Append(log_message);  // Append attribute info to the global string
                    }
                //    g_log_string.Append("\n");
}

void            CKernel::gfx_check                  (   const char     *file, 
                                                        unsigned        line)
{
                static unsigned error_count = 0;
                static bool summary_written = false;  // New static flag
                const unsigned ERROR_THRESHOLD = 1024;
                
                if (resetFlag && !summary_written)
                    {
                    summary_written = true;  // Set flag before writing summary
                    
                    CTimer *pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
                    CString log_message;
                    
                    log_message.Format("\n=== Final System Status [%5u.%02u] ===\n", ticks / HZ, ticks % HZ);
                    g_error_string.Append(log_message);
                    
                    log_message.Format("Total Errors Encountered: %u\n", error_count);
                    g_error_string.Append(log_message);
                    
                    GLint value;
                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
                    log_message.Format("Current Program: %d\n", value);
                    g_error_string.Append(log_message);
                    
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
                    log_message.Format("Active Texture Unit: GL_TEXTURE%d\n", value - GL_TEXTURE0);
                    g_error_string.Append(log_message);
                    
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
                    log_message.Format("Viewport: x=%d, y=%d, width=%d, height=%d\n", 
                        viewport[0], viewport[1], viewport[2], viewport[3]);
                    g_error_string.Append(log_message);
                    
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
                    log_message.Format("Current Framebuffer: %d\n", fb);
                    g_error_string.Append(log_message);
                    
                    log_message.Format("\n=== End Status Report ===\n\n");
                    g_error_string.Append(log_message);
                    
                    return;
                    }

                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    CString log_message;
                    CTimer *pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
                    
                    const char* error_str;
                    const char* severity;
                    
                    switch(error) 
                        {
                        case GL_INVALID_ENUM:               
                            error_str = "GL_INVALID_ENUM"; 
                            severity = "WARNING ";
                            break;
                        case GL_INVALID_VALUE:              
                            error_str = "GL_INVALID_VALUE"; 
                            severity = "WARNING ";
                            break;
                        case GL_INVALID_OPERATION:          
                            error_str = "GL_INVALID_OPERATION"; 
                            severity = "WARNING ";
                            break;
                        case GL_OUT_OF_MEMORY:              
                            error_str = "GL_OUT_OF_MEMORY"; 
                            severity = "CRITICAL";
                            resetFlag = true;
                            break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION: 
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; 
                            severity = "CRITICAL";
                            resetFlag = true;
                            break;
                        default:                            
                            error_str = "UNKNOWN_ERROR"; 
                            severity = "WARNING ";
                            break;
                        }
                    
                    log_message.Format("[%5u.%02u][%-8s] OpenGL Error: %-30s (0x%x) at %s:%u\n", 
                        ticks / HZ,
                        ticks % HZ,
                        severity,
                        error_str,
                        error,
                        file,
                        line);
                        
                    g_error_string.Append(log_message);
                    
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        {
                        resetFlag = true;
                        }
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my new gfx log with my own logg code
void CKernel::gfx_shader_log(GLint shader, int shaderIndex)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    m_shaderStatusFlags[shaderIndex] = (success == GL_TRUE);

    storeLog("----------------------------------------------------------------", EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog("Program compile status idx/success",
             (u32)shaderIndex,
             (u32)success,
             EMPTYLOG,
             EMPTYLOG);
}

void CKernel::gfx_program_log(GLint shader, int program_index)
{
    int internal_index = 0;
    if (program_index > 0)
        internal_index = program_index - 1;

    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    storeLog("----------------------------------------------------------------", EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
    storeLog("Program link status idx/success",
             (u32)program_index,
             (u32)success,
             EMPTYLOG,
             EMPTYLOG);

    char name[27];
    strncpy(name, &m_bufferFsh[program_index][2], 26);
    name[26] = '\0';

    storeMsg("Program short name", name, 26);
    storeMsg("Filename", g_ScnFsh[internal_index], 64);   // same behavior conceptually
    storeLog("Program byte size",
             (u32)g_bytFsh[program_index],
             EMPTYLOG,
             EMPTYLOG,
             EMPTYLOG);

    char log[1024];
    glGetProgramInfoLog(shader, sizeof(log), NULL, log);
    storeMsg("Program InfoLog", log, sizeof(log));

    GLint numUniforms;
    glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numUniforms);

    storeLog("Active Uniforms",
             (u32)numUniforms,
             EMPTYLOG,
             EMPTYLOG,
             EMPTYLOG);

    for (GLint i = 0; i < numUniforms; ++i)
    {
        char uname[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(shader, i, sizeof(uname), &length, &size, &type, uname);
        GLint location = glGetUniformLocation(shader, uname);

        storeLog("Uniform idx/size/type/loc",
                 (u32)i,
                 (u32)size,
                 (u32)type,
                 (u32)location);

        storeMsg("Uniform name", uname, length);
    }

    GLint numAttributes;
    glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    storeLog("Active Attributes",
             (u32)numAttributes,
             EMPTYLOG,
             EMPTYLOG,
             EMPTYLOG);

    for (GLint i = 0; i < numAttributes; ++i)
    {
        char aname[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveAttrib(shader, i, sizeof(aname), &length, &size, &type, aname);
        GLint location = glGetAttribLocation(shader, aname);

        storeLog("Attribute idx/size/type/loc",
                 (u32)i,
                 (u32)size,
                 (u32)type,
                 (u32)location);

        storeMsg("Attribute name", aname, length);
    }

    storeLog("----------------------------------------------------------------", EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
}

void CKernel::gfx_check(const char* file, unsigned line)
{
    static unsigned error_count = 0;
    static bool summary_written = false;
    const unsigned ERROR_THRESHOLD = 1024;

    if (resetFlag && !summary_written)
    {
        summary_written = true;

        CTimer* pTimer = CTimer::Get();
        unsigned ticks = pTimer->GetTicks();

        storeLog("=== Final System Status ticks/count ===",
                 (u32)ticks,
                 (u32)error_count,
                 EMPTYLOG,
                 EMPTYLOG);

        GLint value;

        glGetIntegerv(GL_CURRENT_PROGRAM, &value);
        storeLog("Current Program", (u32)value, EMPTYLOG, EMPTYLOG, EMPTYLOG);

        glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
        storeLog("Active Texture Unit", (u32)value, EMPTYLOG, EMPTYLOG, EMPTYLOG);

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        storeLog("Viewport x/y",
                 (u32)viewport[0],
                 (u32)viewport[1],
                 EMPTYLOG,
                 EMPTYLOG);
        storeLog("Viewport w/h",
                 (u32)viewport[2],
                 (u32)viewport[3],
                 EMPTYLOG,
                 EMPTYLOG);

        GLint fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        storeLog("Current Framebuffer",
                 (u32)fb,
                 EMPTYLOG,
                 EMPTYLOG,
                 EMPTYLOG);

        storeLog("=== End Status Report ===", EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
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

        storeLog("OpenGL Error err/ticks/line",
                 (u32)error,
                 (u32)ticks,
                 (u32)line,
                 EMPTYLOG);

        storeLog(severity,  EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
        storeLog(error_str, EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);
        storeLog(file,      EMPTYLOG, EMPTYLOG, EMPTYLOG, EMPTYLOG);

        error_count++;
        if (error_count >= ERROR_THRESHOLD)
            resetFlag = true;
    }
}
