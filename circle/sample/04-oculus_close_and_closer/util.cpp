#include "kernel.h"
#include "global.h"
#include "wavetable.h"

void            CKernel::prepParameters       ()        // f_buffer guess here we need much more to do!
{
                for ( int f_buffer=0; f_buffer <= DEFAULT_SLOT; f_buffer++)
                    {
                    g_centralModeBuffer[f_buffer][LF1_WAVE] = 0;
                    g_centralModeBuffer[f_buffer][LF2_WAVE] = 1;
                    g_centralModeBuffer[f_buffer][LF1_MULT] = 3;
                    g_centralModeBuffer[f_buffer][LF2_MULT] = 3;
                    }
}

void            CKernel::chooseProgram        ( int p_channel, &g_activeShader )
{
                static int g_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new >> 10; // -> / 1024; <- why no bracelets here too?

                if (m_shaderStatusFlags[f_calculated])
                    {
                    g_activeShader = f_calculated;
                    }
}

void             CKernel::chooseTexture        ( int p_channel, &g_activeTexture ) // f_buffer have three possible ways here! f_buffer can a) invent a mechanism to get the is valid table for the vids - f_buffer can also draw from parser.is_valid[x] 
{
                static int g_activeTexture = 0;
                if (g_validTextureCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validTextureCount) >> 10; // -> / 1024; ) / 1024;
                    g_activeTexture = f_calculated;
                    }
}
int             CKernel::chooseVideo        ( int p_channel, &g_activeVideo )
{
                static int g_activeVideo = 0;
                if (g_validVideoCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validVideoCount) >> 10; // -> / 1024; ) / 1024;
                    g_activeVideo = f_calculated;
                    }
}

int             CKernel::chooseFrame        ( int p_channel, &g_activeFrame )
{
                static int g_activeFrame = 0;
                if (m_H264Parser.m_frame_count[g_activeVideo] != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] *  (m_H264Parser.m_frame_count[g_activeVideo]) >> 10; // -> / 1024; ) / 1024;
                    g_activeFrame = f_calculated;
                    }
}

void            CKernel::storeModesV1         () 
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               
                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true )
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0],
                        &g_centralModeBuffer[DEFAULT_SLOT][0],
                        16 * sizeof(int));
                    
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else  if (g_centralModeBuffer[g_current_gl_program][is_stored] == false )
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}

void            CKernel::storeModesV2         ()
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               

                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true && g_currentProgramBuffer != g_current_gl_program)
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0],
                           &g_centralModeBuffer[DEFAULT_SLOT][0],
                           16 * sizeof(int));
                    
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else if (g_centralModeBuffer[g_current_gl_program][is_stored] == false && g_currentProgramBuffer != DEFAULT_SLOT)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}

unsigned int g_buttons_states[2][5] = {0};

void            CKernel::buttonPing(int p_btn_id, int pin)
{
                g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
                g_buttons_states[p_btn_id][BTN_DOUBLE] = 0;

            //  g_buttons_states[p_btn_id][BTN_STATUS] = CGPIOPin(pin, GPIOModeInputPullUp).Read();    

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() == BTN_PRESSED &&
            //   if (g_buttons_states[p_btn_id][BTN_STATUS] == BTN_PRESSED &&     
                    g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
                {
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_SINGLE] = 1;

                    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
                        (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
                        g_buttons_states[p_btn_id][BTN_DOUBLE] = 1;

                    g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
                }

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() != BTN_PRESSED &&
            //  if (g_buttons_states[p_btn_id][BTN_STATUS] != BTN_PRESSED &&    
                    g_buttons_states[p_btn_id][BTN_PRESS_START] != 0)
                {
                    g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]   = 0;
                }

                if (g_buttons_states[p_btn_id][BTN_PRESS_START] != 0 &&
                    (g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
                {
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
                }
}

void CKernel::button_consumer(int p_btn_id)
{
    if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
    if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

    // long-entry edge (once)
    if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1)
        counter += 5;

    // periodic hold event (once per threshold)
    if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10)
        longhold += 1;
    if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20)
        longhold += 2;

}

