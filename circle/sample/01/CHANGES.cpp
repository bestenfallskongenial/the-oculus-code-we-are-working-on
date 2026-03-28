CHANGES.ccp         this file

datamanagement.cpp  combines    filesystem.cpp & memory.cpp -> wrappers move to wrappers.cpp

gfx_init.cpp        combines    

gfx_render.cpp

logging.cpp         combines    logger.cpp and buffer_to_screen.cpp

menu.cpp            

util.cpp            combines    features.cpp and helpers.cpp and utils.cpp

vc04_defs.h         combines    the defs from VCSM and MMAL

vc04_logger.cpp     for the vc04 logging -> we are still in reverseengineering the vc04 code 

vc04_service.cpp    combines my old VCSM and MMAL class impelentations in one file

vc04_service.h      shall  become part of kernel.h

wrappers.cpp        takes the wrapper of all the function calls needed for this project, gives therefore the framework and meaning to the rest of the code!
