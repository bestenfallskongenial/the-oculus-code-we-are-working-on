#include "kernel.h"

//  #undef  __DEBUG_LOG__
    #define __DEBUG_LOG__

    #define MY_BUFFER   m_logGLSLBuffer
    #define MY_INDEX    m_logGLSLBufferIndex

bool            CKernel::shaderLog                  (   GLint       shader,
                                                        int         shaderIndex )
{
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
#ifdef __DEBUG_LOG__ 
            //  storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
/*
                storeLog( MY_BUFFER, MY_INDEX, "Program compile status idx", (u32)shaderIndex, "success" ,(u32)success);
*/
                if (success != GL_TRUE ) storeLog( MY_BUFFER, MY_INDEX,  "ERROR Compiing Program idx", (u32)shaderIndex );
/*             
                storeLog( MY_BUFFER, MY_INDEX,  "Program compile status idx", (u32)shaderIndex,
                                                (success == GL_TRUE) ? "SUCCESS" : "FAILED", EMPTYLOG );
*/                                              
#endif 
                return success == GL_TRUE;
}

bool            CKernel::programLog                 (   GLint       program,
                                                        int         program_index )
{
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
#ifdef __DEBUG_LOG__
            //  storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
/*                
                storeLog( MY_BUFFER, MY_INDEX,   g_ScnFsh[program_index], EMPTYLOG,
                                                    "Program link status idx", (u32)program_index,
                                                    "Program byte size", (u32)g_bytFsh[program_index], 
                                                    "success", (u32)success);
*/
                storeLog(   MY_BUFFER, MY_INDEX,  
                            g_ScnFsh[program_index], EMPTYLOG,
                            "Program link status idx", (u32)program_index,
                        /*  "Program byte size", (u32)g_bytFsh[program_index], */   // <-- i have a mismatch here... either in the caller or the logger or the tables
                            (success == GL_TRUE) ? "SUCCESS" : "FAILED", EMPTYLOG);
#endif 
                char log[1024];
                GLsizei logLength = 0;
                glGetProgramInfoLog(program, sizeof(log), &logLength, log);
#ifdef __DEBUG_LOG__                 
                if (logLength > 0)
                    {
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Program InfoLog", EMPTYLOG,
                                log, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG );
                    }
/*                    
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Program InfoLog length", (u32)logLength );
*/
#endif 
                GLint numUniforms;
                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
#ifdef __DEBUG_LOG__ 
/*
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Active Uniforms", (u32)numUniforms);
*/
#endif 
                for (GLint i = 0; i < numUniforms; ++i)
                    {
                    char uname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveUniform(program, i, sizeof(uname), &length, &size, &type, uname);
                    GLint location = glGetUniformLocation(program, uname);
#ifdef __DEBUG_LOG__ 
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Uniform name:", EMPTYLOG,
                                uname, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG );
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Uniform idx", (u32)i, 
                                "size", (u32)size, 
                                "type", (u32)type, 
                                "loc", (u32)location);
#endif                     
                    }

                GLint numAttributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
#ifdef __DEBUG_LOG__ 
                storeLog(   MY_BUFFER, MY_INDEX, 
                            "Active Attributes", (u32)numAttributes );
#endif 
                for (GLint i = 0; i < numAttributes; ++i)
                    {
                    char aname[256];
                    GLsizei length;
                    GLint size;
                    GLenum type;

                    glGetActiveAttrib(program, i, sizeof(aname), &length, &size, &type, aname);
                    GLint location = glGetAttribLocation(program, aname);
#ifdef __DEBUG_LOG__ 
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Attribute idx", (u32)i, 
                                "size", (u32)size, 
                                "type", (u32)type, 
                                "loc", (u32)location);
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Attribute name", EMPTYLOG,
                                aname, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG,
                                EMPTYSTR, EMPTYLOG );
#endif                     
                    }
#ifdef __DEBUG_LOG__
                nextline(   MY_BUFFER, MY_INDEX); 
            //  storeLog( MY_BUFFER, MY_INDEX, "----------------------------------------------------------------");
#endif 

                return success == GL_TRUE;
}

void            CKernel::gfx_check                  (   const char* file, 
                                                        unsigned    line )
{
                static unsigned error_count = 0;
                static bool summary_written = false;
                const unsigned ERROR_THRESHOLD = 1024;

                if (m_resetFlag && !summary_written)
                    {
                    summary_written = true;

                    CTimer* pTimer = CTimer::Get();
                    unsigned ticks = pTimer->GetTicks();
#ifdef __DEBUG_LOG__  
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "*** Final System Status ticks", (u32)ticks, 
                                "/ Errorcount", (u32)error_count);
#endif  
                    GLint value;

                    glGetIntegerv(GL_CURRENT_PROGRAM, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Current Program", (u32)value);
#endif  
                    glGetIntegerv(GL_ACTIVE_TEXTURE, &value);
#ifdef __DEBUG_LOG__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Active Texture Unit", (u32)value);
#endif  
                    GLint viewport[4];
                    glGetIntegerv(GL_VIEWPORT, viewport);
#ifdef __DEBUG_LOG__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Viewport x/", (u32)viewport[0], 
                                "y", (u32)viewport[1], 
                                "w", (u32)viewport[2], 
                                "h", (u32)viewport[3]);
#endif  
                    GLint fb;
                    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
#ifdef __DEBUG_LOG__                     
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "Current Framebuffer", (u32)fb,
                                "=== End Status Report ===", EMPTYLOG );
#endif                      
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
                            m_resetFlag = true;
                            break;

                        case GL_INVALID_FRAMEBUFFER_OPERATION:
                            error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
                            severity  = "CRITICAL";
                            m_resetFlag = true;
                            break;

                        default:
                            error_str = "UNKNOWN_ERROR";
                            severity  = "WARNING";
                            break;
                        }
#ifdef __DEBUG_LOG__ 
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                "OpenGL Error", (u32)error, 
                                "ticks", (u32)ticks, "line", (u32)line);
                    storeLog(   MY_BUFFER, MY_INDEX, 
                                severity, EMPTYLOG, 
                                error_str, EMPTYLOG, 
                                file, EMPTYLOG );
#endif  
                    error_count++;
                    if (error_count >= ERROR_THRESHOLD)
                        m_resetFlag = true;
                    }
}