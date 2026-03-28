// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my adc read and convert function plus audio detection!
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::readADC() 
{
                const float maxA = 1023.0;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                const int AUDIO_THRESHOLD = 160;  // Threshold for audio detection

                const int f_maxBuffer = 33;

                static float f_band0[f_maxBuffer] = {0};
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};

                static float sum0 = 0;
                static float sum1 = 0;
                static float sum2 = 0;
                static float sum3 = 0;

                static unsigned char idx0 = 0;
                static unsigned char idx1 = 0;
                static unsigned char idx2 = 0;
                static unsigned char idx3 = 0;

                static uint32_t audio_hold_A = 0;
                static uint32_t audio_hold_B = 0;

                int w0 = g_centralModeBuffer[g_currentProgramBuffer][SENS_A] & 63;
                int w1 = g_centralModeBuffer[g_currentProgramBuffer][SENS_B] & 63;
                int w2 = g_centralModeBuffer[g_currentProgramBuffer][SENS_C] & 63;
                int w3 = g_centralModeBuffer[g_currentProgramBuffer][SENS_D] & 63;

                static int f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                static int f_index_ring_buffer;

                int i0 =  f_index_ring_buffer;
                int i1 = (f_index_ring_buffer - 1) & 3;
                int i2 = (f_index_ring_buffer - 2) & 3;
                int i3 = (f_index_ring_buffer - 3) & 3;

                const int f_scale = scaleFactors[attenuation];

                    g_modeMap[0][0] = 5;
                    g_modeMap[1][0] = 5;
                    g_modeMap[2][0] = 5;
                    g_modeMap[3][0] = 5;

                f_ring_buffer[0][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(0);    // Channel 0 - First of pair for audio_sample[0]

                g_irregularity[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(g_irregularity[0] > AUDIO_THRESHOLD || g_irregularity[0] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 0;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[0][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][au0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][au1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }

                g_inOutMatrixInt[0][raw] =  (f_ring_buffer[0][0] + f_ring_buffer[0][1] + f_ring_buffer[0][2] + f_ring_buffer[0][3]) >>2 ; 

                g_inOutMatrixInt[0][val] = (g_inOutMatrixInt[0][raw] * f_scale) >> 10; //  -> / 1023;                                
                
                g_inOutMatrixFlt[0][val] = (g_inOutMatrixInt[0][val]) * 0.0009765625f;

                f_ring_buffer[1][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(1);    // Channel 1 - First of pair for audio_sample[1]

                g_irregularity[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(g_irregularity[1] > AUDIO_THRESHOLD || g_irregularity[1] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 1;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] = v;
                        g_modeMap[1][0] = v;
                        g_modeMap[2][0] = v;
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[1][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][au2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][au3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }

                g_inOutMatrixInt[1][raw] =  (f_ring_buffer[1][0] + f_ring_buffer[1][1] + f_ring_buffer[1][2] + f_ring_buffer[1][3]) >>2 ; 

                g_inOutMatrixInt[1][val] = (g_inOutMatrixInt[1][raw] * f_scale) >> 10;                               
                
                g_inOutMatrixFlt[1][val] = (g_inOutMatrixInt[1][val]) * 0.0009765625f;

                f_ring_buffer[2][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(2);    // Channel 2 - Second of pair for audio_sample[0]

                g_irregularity[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(g_irregularity[2] > AUDIO_THRESHOLD || g_irregularity[2] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 2;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[2][f_index_ring_buffer] * 0.0009765625f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][au0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][au1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }

                g_inOutMatrixInt[2][raw] =  (f_ring_buffer[2][0] + f_ring_buffer[2][1] + f_ring_buffer[2][2] + f_ring_buffer[2][3]) >>2 ; 

                g_inOutMatrixInt[2][val] = (g_inOutMatrixInt[2][raw] * f_scale) >> 10;                 
                
                g_inOutMatrixFlt[2][val] = (g_inOutMatrixInt[2][val]) * 0.0009765625f;

                f_ring_buffer[3][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(3);    // Channel 3 - Second of pair for audio_sample[1]

                g_irregularity[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(g_irregularity[3] > AUDIO_THRESHOLD || g_irregularity[3] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 3;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
                        g_modeMap[3][0] = v;

                    float s = f_ring_buffer[3][f_index_ring_buffer] * 0.0009765625f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][au2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][au3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }

                g_inOutMatrixInt[3][raw] = (f_ring_buffer[3][0] + f_ring_buffer[3][1] + f_ring_buffer[3][2] + f_ring_buffer[3][3]) >>2 ; 

                g_inOutMatrixInt[3][val] = (g_inOutMatrixInt[3][raw] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[3][val] = (g_inOutMatrixInt[3][val]) * 0.0009765625f;

                f_ring_buffer[4][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(4);    // Channel 4 (no audio detection)

                g_inOutMatrixInt[4][raw] =  (f_ring_buffer[4][0] + f_ring_buffer[4][1] + f_ring_buffer[4][2] + f_ring_buffer[4][3]) >>2 ; 

                g_inOutMatrixInt[4][val] = (g_inOutMatrixInt[4][raw] * f_scale) >> 10;               
                
                g_inOutMatrixFlt[4][val] = (g_inOutMatrixInt[4][val]) * 0.0009765625f;

                f_ring_buffer[5][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(5);

                g_inOutMatrixInt[5][raw] =  (f_ring_buffer[5][0] + f_ring_buffer[5][1] + f_ring_buffer[5][2] + f_ring_buffer[5][3]) >>2 ; 

                g_inOutMatrixInt[5][val] = (g_inOutMatrixInt[5][raw] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[5][val] = (g_inOutMatrixInt[5][val]) * 0.0009765625f;

                f_ring_buffer[6][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(6);

                g_inOutMatrixInt[6][raw] = (f_ring_buffer[6][0] + f_ring_buffer[6][1] + f_ring_buffer[6][2] + f_ring_buffer[6][3]) >>2 ; 

                g_inOutMatrixInt[6][val] = (g_inOutMatrixInt[6][raw] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[6][val] = (g_inOutMatrixInt[6][val]) * 0.0009765625f;

                f_ring_buffer[7][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(7);

                g_inOutMatrixInt[7][raw] = (f_ring_buffer[7][0] + f_ring_buffer[7][1] + f_ring_buffer[7][2] + f_ring_buffer[7][3]) >>2 ; 

                g_inOutMatrixInt[7][val] = (g_inOutMatrixInt[7][raw] * f_scale) >> 10;                  
                
                g_inOutMatrixFlt[7][val] = (g_inOutMatrixInt[7][val]) * 0.0009765625f;
                
                if(audio_hold_A > 0) --audio_hold_A;
                if(audio_hold_B > 0) --audio_hold_B;

                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// my little helpers 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::checkUpdate    () // aka is a new firmware present?!
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;    // there is the second ( usb ) firmware
                    }
                else
                    {
                    return false;    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::Update         ()
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel loaded from sd ( the running kernel )
                // - filesystem is already mounted by caller

                if (saveFromBuffer(PARTITION_NAME_SD, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                // fallback attempt (allowed to fail)
                saveFromBuffer(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]);

                return false;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            set_pot_routing         (   bool        adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::prepParameters       ()        // f_buffer guess here we need much more to do!
{
                for ( int f_buffer=0; f_buffer <= DEFAULT_SLOT; f_buffer++)
                    {
                    g_centralModeBuffer[f_buffer][LF1_WAVE] = 0;
                    g_centralModeBuffer[f_buffer][LF2_WAVE] = 1;
                    g_centralModeBuffer[f_buffer][LF1_MULT] = 3;
                    g_centralModeBuffer[f_buffer][LF2_MULT] = 3;
                    g_centralModeBuffer[f_buffer][SENS_A] = 15;
                    g_centralModeBuffer[f_buffer][SENS_B] = 47;
                    g_centralModeBuffer[f_buffer][SENS_C] = 15;
                    g_centralModeBuffer[f_buffer][SENS_D] = 47;                    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::chooseProgram        ( int p_channel, &p_activeShader )
{
                static int p_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new >> 10; // <- why no bracelets here too?

                if (m_shaderStatusFlags[f_calculated])  // comes from gfx.cpp ->
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
void            CKernel::storeModesV1         ()  // faster
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][is_stored] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               
                if (g_centralModeBuffer[g_current_gl_program][is_stored] == true )
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program]));
                    
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else  if (g_centralModeBuffer[g_current_gl_program][is_stored] == false )
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::storeModesV2         ()    // "saver"
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
// my abstract stuff like random, lfo, bpm needed for glsl shader uniform control
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::randomVec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;
                uint32_t        f_x         = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[0][rnd] = / f_x * f_scale;
                g_inOutMatrixInt[0][rnd] = ( g_inOutMatrixFlt[0][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[1][rnd] = f_x * f_scale;
                g_inOutMatrixInt[1][rnd] = ( g_inOutMatrixFlt[1][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[2][rnd] = f_x * f_scale;
                g_inOutMatrixInt[2][rnd] = ( g_inOutMatrixFlt[2][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[3][rnd] = f_x * f_scale;
                g_inOutMatrixInt[3][rnd] = ( g_inOutMatrixFlt[3][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[4][rnd] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[4][rnd] = ( g_inOutMatrixFlt[4][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[5][rnd] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[5][rnd] = ( g_inOutMatrixFlt[5][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[6][rnd] = /* (float) */ f_x * f_scale;
                g_inOutMatrixInt[6][rnd] = ( g_inOutMatrixFlt[6][rnd] * f_max_int);
                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[7][rnd] = f_x * f_scale;
                g_inOutMatrixInt[7][rnd] = ( g_inOutMatrixFlt[7][rnd] * f_max_int);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// question here - should i rather have one funtion for both channels or should i seperate the functions and call per channel?
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::calculate2BPM   (   unsigned long   p_triggerTimeClockA, 
                                            unsigned long   p_triggerTimeClockB) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static unsigned long f_deltaBuffer[2][3]= { 0 };

                unsigned long f_intervalAverage = 0;
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClockA != f_lastTime[0])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[0][f_timeIndex[0]] = p_triggerTimeClockA;
        
                    f_deltaBuffer[0][0]          =   f_timeBuffer[0][1] - f_timeBuffer[0][0];   
                    f_deltaBuffer[0][1]          =   f_timeBuffer[0][2] - f_timeBuffer[0][1];
                    f_deltaBuffer[0][2]          =   f_timeBuffer[0][3] - f_timeBuffer[0][2];

                    if(     f_deltaBuffer[0][1]  <   f_deltaBuffer[0][0] * 1.25f &&  f_deltaBuffer[0][2]  <   f_deltaBuffer[0][0] * 1.25f &&  f_deltaBuffer[0][0]  <   f_deltaBuffer[0][2] * 1.25f ) // calculates an average and allows 25% play ( quite high right ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[0][0] + f_deltaBuffer[0][1] + f_deltaBuffer[0][2]) / 3;
            
                        g_resultBPM[0]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[0]     =   f_intervalAverage;
                        g_lastBpmCalculation[0]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[0]                   =   p_triggerTimeClockA;

                    f_timeIndex[0]                  = ( f_timeIndex[0] + 1) % 4;    
                    }
                if (p_triggerTimeClockB != f_lastTime[1])                                                           // Process clock u_time (instance 1)
                    {
                    f_timeBuffer[1][f_timeIndex[1]] = p_triggerTimeClockB;
        
                    f_deltaBuffer[1][0]          =   f_timeBuffer[1][1] - f_timeBuffer[1][0];
                    f_deltaBuffer[1][1]          =   f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    f_deltaBuffer[1][2]          =   f_timeBuffer[1][3] - f_timeBuffer[1][2];

                    if(     f_deltaBuffer[1][1]  <   f_deltaBuffer[1][0] * 1.25f &&  f_deltaBuffer[1][2]  <   f_deltaBuffer[1][0] * 1.25f &&  f_deltaBuffer[1][0]  <   f_deltaBuffer[1][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[1][0] + f_deltaBuffer[1][1] + f_deltaBuffer[1][2]) / 3;
            
                        g_resultBPM[1]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[1]     =   f_intervalAverage;
                        g_lastBpmCalculation[1]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[1]                   =   p_triggerTimeClockB;

                    f_timeIndex[1]                  = ( f_timeIndex[1] + 1) % 4;    
                    }
                g_activeBpmChannel                  = ( g_lastBpmCalculation[0] > g_lastBpmCalculation[1]) ? 0 : 1; // what was the last bpm input? 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::calculate1BPM   (   int chn, unsigned long   p_triggerTimeClock) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static unsigned long f_deltaBuffer[2][3]= { 0 };

                unsigned long f_intervalAverage = 0;
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClock != f_lastTime[chn])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[chn][f_timeIndex[chn]] = p_triggerTimeClock;
        
                    f_deltaBuffer[chn][0]          =   f_timeBuffer[chn][1] - f_timeBuffer[chn][0];   
                    f_deltaBuffer[chn][1]          =   f_timeBuffer[chn][2] - f_timeBuffer[chn][1];
                    f_deltaBuffer[chn][2]          =   f_timeBuffer[chn][3] - f_timeBuffer[chn][2];
                    
                    if(     f_deltaBuffer[chn][1]  <   f_deltaBuffer[chn][0] * 1.25f &&  f_deltaBuffer[chn][2]  <   f_deltaBuffer[chn][0] * 1.25f &&  f_deltaBuffer[chn][0]  <   f_deltaBuffer[chn][2] * 1.25f ) // calculates an average and allows 25% play ( quite high right ) 
                        {
                        f_intervalAverage           = ( f_deltaBuffer[chn][0] + f_deltaBuffer[chn][1] + f_deltaBuffer[chn][2]) / 3;
            
                        g_resultBPM[chn]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[chn]     =   f_intervalAverage;
                        g_lastBpmCalculation[chn]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[chn]                   =   p_triggerTimeClock;

                    f_timeIndex[chn]                  = ( f_timeIndex[chn] + 1) % 4;    
                    }

                g_activeBpmChannel                  = ( g_lastBpmCalculation[0] > g_lastBpmCalculation[1]) ? 0 : 1; // what was the last bpm input? 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::predictedNextBeat2 ()
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                if (currentTime >= g_nextBeatTime[0])
                    {
                    g_nextBeatTime[0]               +=  g_intervalCalculated[0];                                                                        // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[0]) 
                    {
                    g_lastCircleBuffer[0]           =   g_nextCircleBuffer[0];
                    g_nextCircleBuffer[0]           =   g_nextCircleBuffer[0] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[0]);
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
                if ((g_lastBpmCalculationTMP[0]     !=  g_lastBpmCalculation[0]))
                    {
                    g_nextBeatTime[0]               =   g_lastBpmCalculation[0];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[0]      =   g_lastBpmCalculation[0];
                    }
                if (g_lfoMultiplierTMP[0]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]])
                    {
                    g_lastCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[0]);
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
                if (currentTime >= g_nextBeatTime[1])
                    {
                    g_nextBeatTime[1]               +=  g_intervalCalculated[1];                                                                                   // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[1]) 
                    {
                    g_lastCircleBuffer[1]           =   g_nextCircleBuffer[1];
                    g_nextCircleBuffer[1]           =   g_nextCircleBuffer[1] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[1] );                                  // g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]];
                    }
                if ((g_lastBpmCalculationTMP[1]     !=  g_lastBpmCalculation[1]))                                                                   // Handle BPM changes for instance 1
                    {
                    g_nextBeatTime[1]               =   g_lastBpmCalculation[1];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[1]      =   g_lastBpmCalculation[1];
                    }
                if (g_lfoMultiplierTMP[1]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]])
                    {
                    g_lastCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[1]);                                   // g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF2_MULT]];
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::predictedNextBeat1 (int chn)
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                if (currentTime >= g_nextBeatTime[chn])
                    {
                    g_nextBeatTime[chn]               +=  g_intervalCalculated[0];                                                                        // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[chn]) 
                    {
                    g_lastCircleBuffer[chn]           =   g_nextCircleBuffer[chn];
                    g_nextCircleBuffer[chn]           =   g_nextCircleBuffer[chn] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[chn]);
                    g_lfoMultiplierTMP[chn]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
                if ((g_lastBpmCalculationTMP[chn]     !=  g_lastBpmCalculation[chn]))
                    {
                    g_nextBeatTime[chn]               =   g_lastBpmCalculation[chn];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[chn]      =   g_lastBpmCalculation[chn];
                    }
                if (g_lfoMultiplierTMP[chn]           !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]])
                    {
                    g_lastCircleBuffer[chn]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[chn]           =   g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMultiplierTMP[chn]);
                    g_lfoMultiplierTMP[chn]           =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][LF1_MULT]];
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::sampleWaveTable                   ()
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                          // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_elapsedMicroseconds[0]            =   currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0]                    =   g_nextCircleBuffer[0] - g_lastCircleBuffer[0];                                                    // Total length of the current cycle
                int f_indexA                        =  (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];                                               // 255 is not the amplitude! its the number of samples
                g_sampleIndex[0]                    =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255!
                g_inOutMatrixFlt[0][lf1]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][g_sampleIndex[0]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][lf1]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF1_WAVE]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1]            =   currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1]                    =   g_nextCircleBuffer[1] - g_lastCircleBuffer[1];                                                   // Total length of the current cycle
                int f_indexB                        =  (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1]                    =   f_indexB > 255 ? 255 : f_indexB;                                                                  // ! i like to get rid of this saveguard !
                g_inOutMatrixFlt[0][lf2]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][g_sampleIndex[1]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][lf2]            =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][LF2_WAVE]][g_sampleIndex[1]];
}   
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------