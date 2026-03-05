#include "kernel.h"
#include "global.h"
#include "wavetable.h"

void            CKernel::prepParameters       ()        // i guess here we need much more to do!
{
    for ( int i=0; i <= DEFAULT_SLOT; i++)
    {
    g_centralModeBuffer[LF1_WAVE][i] = 0;
    g_centralModeBuffer[LF2_WAVE][i] = 1;
    g_centralModeBuffer[LF1_MULT][i] = 3;
    g_centralModeBuffer[LF2_MULT][i] = 3;
    }
}

int             CKernel::chooseProgram        ( int p_channel )
{
                static int f_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new / 1024;

                if (m_shaderStatusFlags[f_calculated])
                {
                    f_activeShader = f_calculated;
                }

                return f_activeShader;
}

int             CKernel::chooseTexture        ( int p_channel ) // i have three possible ways here! i can a) invent a mechanism to get the is valid table for the vids - i can also draw from parser.is_valid[x] 
{
                static int f_activeTexture = 0;
                if (g_validTextureCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validTextureCount ) / 1024;
                    f_activeTexture = f_calculated;
                    }
                return f_activeTexture;
}
int             CKernel::chooseVideo        ( int p_channel )
{
                static int f_activeVideo = 0;
                if (g_validVideoCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validVideoCount ) / 1024;
                    f_activeVideo = f_calculated;
                    }
                return f_activeVideo;
}

void            CKernel::storeModes         () 
{
                // 1. SHADER CHANGE CHECK
                if (g_current_gl_program != g_last_gl_program) 
                    {    
                    // SIMPLE: Use program slot if stored, otherwise DEFAULT_SLOT
                    g_currentProgramBuffer = shader_has_stored_params[g_current_gl_program] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }

                // 2. STORE PARAMETERS
                if (shader_has_stored_params[g_current_gl_program] == false && is_hold_for_2_sec_a == true && is_hold_for_2_sec_b == true )
                    {  
                    // SIMPLE: Copy DEFAULT_SLOT contents to this program's slot
                    memcpy(&g_centralModeBuffer[0][g_current_gl_program],
                        &g_centralModeBuffer[0][DEFAULT_SLOT],
                        16 * sizeof(int));
                    
                    shader_has_stored_params[g_current_gl_program] = true;
                    g_currentProgramBuffer = g_current_gl_program;  // Now use program's slot
            //      is_hold_for_2_sec_a = false;
            //      is_hold_for_2_sec_b = false;
                    }

                // 3. DELETE STORED PARAMETERS
                
                else  if (shader_has_stored_params[g_current_gl_program] == true && is_hold_for_2_sec_a == true && is_hold_for_2_sec_b == true ) // -really else and not only if??
                    {  
                    shader_has_stored_params[g_current_gl_program] = false;
                    g_currentProgramBuffer = DEFAULT_SLOT;  // Back to default
            //      is_hold_for_2_sec_a = false;
            //      is_hold_for_2_sec_b = false;
                    }
}

void            CKernel::readADC                () 
{
                const float f_max_adc = 1023.0f;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                static int  f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                        
                static int f_index_ring_buffer;

                for (unsigned channel = 0; channel < ADC_CHANNELS; ++channel)  // Loop through each channel and read its raw value 
                    {
                    f_ring_buffer[channel][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(channel);  // or 1023?!
                    
                    if(f_ring_buffer[channel][f_index_ring_buffer] > 1023) 
                        {
                        f_ring_buffer[channel][f_index_ring_buffer] = 1023; // ??
                        }
                        g_inOutMatrixInt[channel][raw]  = ( f_ring_buffer[channel][0] +
                                                            f_ring_buffer[channel][1] +
                                                            f_ring_buffer[channel][2] +
                                                            f_ring_buffer[channel][3]) >>2 ; 

                        g_inOutMatrixInt[channel][val]    = ( g_inOutMatrixInt[channel][raw] * scaleFactors[g_attenuation] ) /1023;
                          
                        g_inOutMatrixFlt[channel][val]    = ( g_inOutMatrixInt[channel][val] /1024.0f );
                    }
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}

// after getting fucked in my head we finally found the deterministic function i am looking for!
/*
#define BTN_PRESSED 0

enum ButtonTSIndex
{
    BTN_PRESS_START = 0, // timestamp when press starts, 0 = currently up
    BTN_RELEASE     = 1, // timestamp of last release (double-click window anchor)
    BTN_HOLD_TICK   = 2, // increments while held after long threshold
    BTN_SINGLE      = 3, // one-cycle pulse on press edge
    BTN_DOUBLE      = 4  // one-cycle pulse on second press edge in double window
};
*/
// 2 buttons, 5 fields each (no BTN_STATUS needed)
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
/*
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
*/
