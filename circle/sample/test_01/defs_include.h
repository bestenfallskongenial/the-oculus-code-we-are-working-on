#include "CLogger.h"                        // my CLogger shimm layer <-because i have to redirect all other includes of CLogger !!

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

#include "/home/bestenfalls/circle/addon/SDCard/emmc.h"
#include <circle/usb/usbhcidevice.h>
#include <circle/fs/fat/fatfs.h>



#include <circle/types.h>
#include <string.h>
