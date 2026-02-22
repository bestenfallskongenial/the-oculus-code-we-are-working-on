#include "kernel.h"
#include "global.h"


/*
                < type                 < name                                   < size                      < content/value
                                                                                                                    <apostroph
*/   



// multipliers for the lfo´s */

















         
/*
new menu_input_mode

[8 ( for adc inputs ) + 4 ( LFO ) + 4 ( extra mode flags )][progs on sd + progs on usb]
 menu_input_mode        LFO_mode    g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer],g_centralModeBuffer[TEX_MODE][g_currentProgramBuffer],  adc_attenuation    

eventual also the variable buffers ***_old???
*/