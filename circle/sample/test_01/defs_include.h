#include "CLogger.h"                        // my CLogger shimm layer <-because i have to redirect all other includes of CLogger !!

#include <stdlib.h>   // malloc, calloc, free
#include <stdint.h>   // uintptr_t

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/machineinfo.h>
#include <circle/devicenameservice.h>
#include <circle/bcmframebuffer.h>
#include <circle/chargenerator.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/dmachannel.h>

#include <circle/memory.h>										// new for dma allocated buffers!
#include <circle/new.h>												// new for dma allocated buffers!

#include "/home/bestenfalls/circle/addon/SDCard/emmc.h"

#include <circle/usb/usbhcidevice.h>
#include <circle/fs/fat/fatfs.h>

#include "bcm_host.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <circle/types.h>
#include <string.h>
