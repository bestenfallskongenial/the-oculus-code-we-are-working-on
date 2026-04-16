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


// we have NO led related code jet!

// my own gpiopin code 

// can we use this instead of the CGPIOPin class????
// the point is GPOIPin is used by multible subsystems 
// like 
// #include <circle/actled.h> 
// #include <circle/machineinfo.h>
// #include <circle/serial.h>
// #include <circle/gpiomanager.h>
// and i dont know which more, and also is my code this far optimized to use it
// having my "own" GPOI code in my kernel class may not be helpful at all... but...

#include <circle/bcm2835.h>
#include <circle/types.h>
#include <circle/timer.h>

#define LOW  0
#define HIGH 1

#define PULL_OFF  0
#define PULL_DOWN 1
#define PULL_UP   2

static inline u32 CKernel::mmio_read32(uintptr addr)
{
    return *(volatile u32 *)addr;
}

static inline void CKernel::mmio_write32(uintptr addr, u32 value)
{
    *(volatile u32 *)addr = value;
}

void CKernel::gpio_write(unsigned pin, unsigned state, int pull)
{
    unsigned shift = (pin % 10) * 3;
    uintptr sel = ARM_GPIO_GPFSEL0 + (pin / 10) * 4;

    // set OUTPUT
    u32 v = mmio_read32(sel);
    v &= ~(7 << shift);
    v |=  (1 << shift);
    mmio_write32(sel, v);

    // optional pull
    if (pull >= 0)
    {
        u32 mask = 1 << (pin % 32);

        mmio_write32(ARM_GPIO_GPPUD, pull);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (pin / 32) * 4, mask);
        CTimer::SimpleusDelay(5);
        mmio_write32(ARM_GPIO_GPPUD, 0);
        mmio_write32(ARM_GPIO_GPPUDCLK0 + (pin / 32) * 4, 0);
    }

    // write HIGH / LOW (explicit)
    u32 mask = 1 << (pin % 32);

    if (state == HIGH)
    {
        mmio_write32(ARM_GPIO_GPSET0 + (pin / 32) * 4, mask);
    }

    if (state == LOW)
    {
        mmio_write32(ARM_GPIO_GPCLR0 + (pin / 32) * 4, mask);
    }
}


// from marcos.h

#define PACKED		__attribute__ ((packed))
#define	MAXALIGN	__attribute__ ((aligned))
#define	ALIGN(n)	__attribute__ ((aligned (n)))
#define NORETURN	__attribute__ ((noreturn))
#ifndef __clang__
#define NOOPT		__attribute__ ((optimize (0)))
#define STDOPT		__attribute__ ((optimize (2)))
#define MAXOPT		__attribute__ ((optimize (3)))
#else
#define NOOPT
#define STDOPT
#define MAXOPT
#endif
#define WEAK		__attribute__ ((weak))

#define likely(exp)	__builtin_expect (!!(exp), 1)
#define unlikely(exp)	__builtin_expect (!!(exp), 0)

#define BIT(n)		(1U << (n))

#define IS_POWEROF_2(num) ((num) != 0 && (((num) & ((num) - 1)) == 0))

// big endian (to be used for constants only)
#define BE(value)	((((value) & 0xFF00) >> 8) | (((value) & 0x00FF) << 8))

// from memio.h

#include <circle/types.h>

#ifdef __cplusplus      // why? 
extern "C" {
#endif

/// \brief Read 32-bit value from MMIO address
static inline u32 CKernel::read32 (uintptr nAddress)
{
	return *(u32 volatile *) nAddress;
}
/// \brief Write 32-bit value to MMIO address
static inline void CKernel::write32 (uintptr nAddress, u32 nValue)
{
	*(u32 volatile *) nAddress = nValue;
}
#ifdef __cplusplus
}
#endif



// from bcmwatchdog.h 
#include <circle/spinlock.h>


	static const unsigned MaxTimeoutSeconds = 15;

    CSpinLock m_SpinLock; // really ?!?!


void            CKernel::watchDogStart (unsigned nTimeoutSeconds)
{
                if (nTimeoutSeconds > MaxTimeoutSeconds)
                    {
                    nTimeoutSeconds = MaxTimeoutSeconds;
                    }
                m_SpinLock.Acquire ();  // really??

                write32 (ARM_PM_WDOG, ARM_PM_PASSWD | ((nTimeoutSeconds << 16) & ARM_PM_WDOG_TIME));

                write32 (ARM_PM_RSTC,   ARM_PM_PASSWD | ARM_PM_RSTC_REBOOT (read32 (ARM_PM_RSTC) & ARM_PM_RSTC_CLEAR));

                m_SpinLock.Release ();  // really??
}

// notes on menu!!!


// i see the logical issue here: modes are usually meant to control how the in values are processed to out values.
// things like modes and lfo parameters are stored in g_centralModeBuffer[][]
// THAN there are the functions where a knop uses the raw out value AFTER 
// modes and than map it to program, texture, video, frame, sensetivity ( i could made this a g_centralModeBuffer field too ),
// means input -> input-processing -> mode-selection -> mode-precessing -> target-selection -> target ( gl uniform OR hardware )
// because i want for example to have bpm on channel 0 control the frame of the video.
// i assume this is possible with gl code BUT the user may have not the knowlege or the will to program this features therefore the device must offer another way!   
// i could indeed pass the array (like g_centralModeBuffer ) i use as target for the mapping in modeMenuAssignGroup(uint8_t menu_id, uint8_t base, &array_to_work_on )







/* new for kernel.h
#define EMPTYSTR ""
#define EMPTYLOG 255

void storeLogLong(              char*       buffer,
                                u32&        index,
                                const char* l1,
                                u32         v1 = EMPTYLOG,
                                const char* l2 = EMPTYSTR,
                                u32         v2 = EMPTYLOG,
                                const char* l3 = EMPTYSTR,
                                u32         v3 = EMPTYLOG,
                                const char* l4 = EMPTYSTR,
                                u32         v4 = EMPTYLOG);
*/



// assume EMPTYSTR = "" and EMPTYLOG = 255

storeLog(buf, idx, "texture", EMPTYLOG, "loaded", EMPTYLOG, "successfully", EMPTYLOG, EMPTYSTR, EMPTYLOG);
// texture loaded successfully

storeLog(buf, idx, "video", EMPTYLOG, "decode", EMPTYLOG, "error", 5, EMPTYSTR, EMPTYLOG);
// video decode error 0x00000005

storeLog(buf, idx, "shader", 3);
// shader 0x00000003

storeLog(buf, idx, "frame", EMPTYLOG, "dropped", EMPTYLOG);
// frame dropped

storeLog(buf, idx, "audio", EMPTYLOG, "buffer", EMPTYLOG, "underrun", EMPTYLOG);
// audio buffer underrun

storeLog(buf, idx, "init", 1, "stage", 2);
// init 0x00000001 stage 0x00000002

storeLog(buf, idx, "gpu", EMPTYLOG, "upload", EMPTYLOG, "failed", 255);
// gpu upload failed 0x000000FF

storeLog(buf, idx, "file", EMPTYLOG, "not", EMPTYLOG, "found", EMPTYLOG);
// file not found

storeLog(buf, idx, "mode", 7, "active", EMPTYLOG);
// mode 0x00000007 active

storeLog(buf, idx, "stream", EMPTYLOG, "h264", EMPTYLOG, "idr", 1);
// stream h264 idr 0x00000001

// example variables
const char* fileName = m_bufferTex[i];
u32 fileIndex = i;

storeLog(buf, idx,
    "file", EMPTYLOG,
    fileName, EMPTYLOG,
    "not found in", fileIndex,
    EMPTYSTR, EMPTYLOG);

// output example:
// file my_texture.bmp not found in 0x00000003

