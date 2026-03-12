adc_read.cpp

    if its work its fine as it is. adc, normalisation, attenuation and audio ...
    oh no: i need to create a mechanism like modeMenuAssignGroup(x,  y);   // sensitivity instead of a fixed table

bufferToScreen.cpp

    hope this works as intended! the goal again is to remove the CScreen, CLogger, CString methods/libs from the code

features.cpp

    we still need to combine the g_nextBeatTime, g_lastBpmCalculationTMP, g_nextCircleBuffer etc in one global array

filesystem_user.cpp

    this function need to be created using our self-made storeLog()/saveBuffer etc code...
    it is meant to create, store and read a human readeble and editable file from the g_centralModeBuffer array
    means we also need to check if the shaders are the same as last time ( size or name, both retrieved while loading )

filesystem.cpp

    complete, as god will

gfx_debug.cpp

    complete. im not really pleased that i pass the buffer and index for m_bufferLog here to but i need modularity, right? 
    on the other hand - hard-wiring the buffers for the glsl logs was a plan...
    but still: check() is a different thing in total, wonder - are check() errors crashing my system? than logging is futile

gfx_init.cpp

    complete. im not really pleased that i pass the buffer and index for m_bufferLog here to but i need modularity, right? 

gfx_OGL.cpp

    complete, i like to remove the complete asset calls here too

gfx_render.cpp

    the menu render function is still missing. look up gpu_menu_A to gpu_menu_C.cpp for the concept  

gpu_menu_A.cpp / gpu_menu_B.cpp / gpu_menu_C.cpp

    this files contain a skeleton for the overlay display code according to my plan, we need to figure out what we have and 
    what is redundant and how we adapt it for the existing code - i decided in gfx_init.cpp that i "copy&paste"
    the regular functions for the single overlay atlas and the second pass overlay shader...
    
    by the way, i still need the function ( util.cpp )

helpers.cpp

    the parser wrappers will become something else completely, since i will fill and store m_bufferLog[x] per file
    important is checkUpdate() and the Update() function here, 
    it should be "triggered" via modeMenuAssignGroup(x,  y);   // file operations 
    like other fle functions ( like load the mode table mentioned above )

info.cpp

    comments and stuff i removed from the respective files !!! we still need to read and retrieve the infos here !!!

kernel_initialize.cpp

    we need to clean it up here. worth mentioning here is the fact that we try to remove CScreen, CLogger, CString methods/libs
    and will "reintrgrate" the h264, the vc_sm and the logger classs into CKernel !!!

kernel_run.cpp

    our runtime-loop. we will see later !!! 

kernel.cpp

    constructor/deconstructor - do we need the memory cleanup? i mean we have boot on power on and power of,
    maybe i could create a "clear shader" functionality but having a reset ( warmstart ) does the same in about the same speed

kernel.h

    this will be funny... but most of it is clear

logging.cpp

    my logger code, but we declare the funcions like 

    #define STOREDEBUG_WHITESPACE 255

    void    storeLog                                (   const char* label,
                                                        u32  value1 = STOREDEBUG_WHITESPACE,
                                                        u32  value2 = STOREDEBUG_WHITESPACE,
                                                        u32  value3 = STOREDEBUG_WHITESPACE,
                                                        u32  value4 = STOREDEBUG_WHITESPACE );        // NEW

Makefile

        later !!!

memory.cpp

    we need to increase the number of m_bufferLog[x], also i like to remove the logger / replace it with my own 
    mechanism and store them to m_bufferLog[0] - start upload for example

menu_final.cpp

    okay, looks good but... i need to resolve the confusion about the "modes" and the "extra-modes" and therefore the "routing"
    means i need to make a decision

menu.fsh

        lets hope this is the correct shader for our Texture Atlas Overlay Menu ( this was the beginning o the complete refactoring )

README.cpp

        i tried to get all the functions together here and also some definitions for kernel.h !!! doublecheck !!!
        for now the infos are scattered around in all this files here !!!

start_up_screen.cpp         

    bool CKernel::display_startup_screen(char* buffer, u32& index)

    i rather like to have 

    bool CKernel::display_startup_screen()

    where the buffers ( m_bufferLog ) are hardwired like 
    
    m_bufferLog[0] - start upload
    m_bufferLog[1] - VC_SM
    m_bufferLog[2] - h264 m_H264Decoder

    etc...

    we need more to store  

    m_bufferLog[u] - video logs ( frame table )
    m_bufferLog[v] - texture logs 
    m_bufferLog[w] - gl error logs
    m_bufferLog[x] - gles compiler logs
    m_bufferLog[y] - mode file

table_colors.cpp

        the color table. we need the function that uses g_centralModeBuffer and the pickup flag array to to get the colors to
        drive the ws212 leds

table_waveforms.cpp

        my wavetables. i wonder if i should "increase" the length from 256 to 512/1024 bytes.. thinking about it 
        i guess the higher resolution will get lost in the sampleLFO function anyway

util.cpp

        some decisions to make here too !!!

wrappers.cpp

        i like to gather the runtime stuff here to have more clarity... on the other hand... jumptables?! speed !??


        additional notes:

        i will need to implement the parser, the h264 decoder and the vcsm class code here, but the unfinished new versions
        where i can - hopefully - share some functionality and make use of the common m_bufferLog for all parse and log files!

    