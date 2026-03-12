// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::chooseProgram        ( int p_channel, &p_activeShader )
{
                static int p_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new >> 10; // <- why no bracelets here too?

                if (m_shaderStatusFlags[f_calculated])
                    {
                    p_activeShader = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void             CKernel::chooseTexture        ( int p_channel, &p_activeTexture, &p_validTextureCount) // f_buffer have three possible ways here! f_buffer can a) invent a mechanism to get the is valid table for the vids - f_buffer can also draw from parser.is_valid[x] 
{                                                                                                       // wait, we have a parser and this parser is giving like for program a valid status array!!!
                static int p_activeTexture = 0;
                if (p_validTextureCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (p_validTextureCount) >> 10;
                    p_activeTexture = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int             CKernel::chooseVideo        ( int p_channel, &p_activeVideo, &p_validVideoCount )
{
                static int p_activeVideo = 0;
                if (p_validVideoCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (p_validVideoCount) >> 10;
                    p_activeVideo = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int             CKernel::chooseFrame        ( int p_channel, &p_activeFrame, &p_validFrameCount ) // or direct m_H264Parser.m_frame_count[p_activeVideo]? <- why is this different?
{
                static int p_activeFrame = 0;
                if (p_validFrameCount != 0) // <- !!! this is the point where i decided to include the h264 / vc_sm / parser class into the CKernel code ( again ) !!!
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] *  (p_validFrameCount) >> 10;
                    p_activeFrame = f_calculated;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeModesV2         ()
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               

                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true && g_currentProgramBuffer != g_current_gl_program)
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else if (g_centralModeBuffer[g_current_gl_program][is_stored] == false && g_currentProgramBuffer != DEFAULT_SLOT)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// for kernel.h -> unsigned int g_buttons_states[2][5] = {0} !!!

void            CKernel::buttonPing(int p_btn_id, int pin)
{
                g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
                g_buttons_states[p_btn_id][BTN_DOUBLE] = 0;

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() == BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
                    {
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_SINGLE] = 1;

                    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
                        {
                        g_buttons_states[p_btn_id][BTN_DOUBLE] = 1;
                        }
                    g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
                    }

                if (CGPIOPin(pin, GPIOModeInputPullUp).Read() != BTN_PRESSED && g_buttons_states[p_btn_id][BTN_PRESS_START] != 0)
                    {
                    g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
                    g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]   = 0;
                    }
                if (g_buttons_states[p_btn_id][BTN_PRESS_START] != 0 && (g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
                    {
                    g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::button_consumer(int p_btn_id)
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------