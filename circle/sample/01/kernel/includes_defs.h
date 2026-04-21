// kernel.h

#include <GLES2/gl2.h>
#include <cstdint>
#include <cstddef>
#include <cstdint>

#include "macros.h"

#include "vc04_service.h"
#include "vc04_defs.h"

extern "C" void vc_host_get_vchi_state(VCHI_INSTANCE_T *inst, VCHI_CONNECTION_T **conn);

// VCSM and MMAL

#include <circle/types.h>                                                           // for u32, size_t, etc.

#include "VCSM_defs.h"
#include "MMAL_defs.h"

#include "interface/vcos/vcos.h"
#include "interface/vcos/vcos_event.h"          // ?
#include <vc4/interface/vcinclude/common.h>     // ?
#include <vc4/vchi/vchi.h>
#include <vc4/vchiq/vchiq.h>
#include <circle/bcm2835.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <GLES2/gl2.h>
#include <GLES/glext.h>
#include <GLES2/gl2ext.h>


