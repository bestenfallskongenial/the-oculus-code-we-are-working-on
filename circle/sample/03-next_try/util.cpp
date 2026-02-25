#include "kernel.h"
#include "global.h"
#include "wavetable.h"

void            CKernel::util_prep_parameters       ()
{
    for ( int i=0; i <= DEFAULT_SLOT; i++)
    {
    g_centralModeBuffer[LF1_WAVE][i] = 0;
    g_centralModeBuffer[LF2_WAVE][i] = 1;
    g_centralModeBuffer[LF1_MULT][i] = 3;
    g_centralModeBuffer[LF2_MULT][i] = 3;
    }
}

int             CKernel::util_choose_program        ( int p_channel )
{
/*
                static int f_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new / 1024;

                if (m_shaderStatusFlags[f_calculated])
                {
                    f_activeShader = f_calculated;
                }

                return f_activeShader;
*/
                static int f_activeShader = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][raw] * g_loaded_fsh_new / 1024;
                // Only update if the f_calculated index points to a valid u_program_handle
                if (m_shaderStatusFlags[f_calculated ]==true /*&& g_menu_mode_new == 0*/) f_activeShader = f_calculated;

                return f_activeShader;
}

int             CKernel::util_choose_texture        ( int p_channel ) // i have three possible ways here! i can a) invent a mechanism to get the is valid table for the vids - i can also draw from parser.is_valid[x] 
{
                static int f_activeTexture = 0;
                if (g_validTextureCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validTextureCount ) / 1024;
                    f_activeTexture = f_calculated;
                    }
                return f_activeTexture;
}
int             CKernel::util_choose_video        ( int p_channel )
{
                static int f_activeVideo = 0;
                if (g_validVideoCount != 0) 
                    {

                    int f_calculated = g_inOutMatrixInt[p_channel][raw] * (g_validVideoCount ) / 1024;
                    f_activeVideo = f_calculated;
                    }
                return f_activeVideo;
}


void            CKernel::util_store_program         () 
{
                // 1. SHADER CHANGE CHECK
                if (g_current_gl_program != g_last_gl_program) 
                    {    
                    // SIMPLE: Use program slot if stored, otherwise DEFAULT_SLOT
                    g_currentProgramBuffer = shader_has_stored_params[g_current_gl_program] ?
                                    g_current_gl_program : DEFAULT_SLOT;
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
                    is_hold_for_2_sec_a = false;
                    is_hold_for_2_sec_b = false;
                    }

                // 3. DELETE STORED PARAMETERS
                
        else  if (shader_has_stored_params[g_current_gl_program] == true && is_hold_for_2_sec_a == true && is_hold_for_2_sec_b == true ) // -really else and not only if??
                    {  
                    shader_has_stored_params[g_current_gl_program] = false;
                    g_currentProgramBuffer = DEFAULT_SLOT;  // Back to default
                    is_hold_for_2_sec_a = false;
                    is_hold_for_2_sec_b = false;
                    }
}

/*  do f_index_ring_buffer should call you each time f_index_ring_buffer change the menu ??
void            CKernel::io_init_pickup_buffer      ()
{
                menu_pickup_flag[0]  = m_MCP300X.DoSingleEndedConversionRaw(4);
                menu_pickup_flag[1]  = m_MCP300X.DoSingleEndedConversionRaw(5);
                menu_pickup_flag[2]  = m_MCP300X.DoSingleEndedConversionRaw(6);
                menu_pickup_flag[3]  = m_MCP300X.DoSingleEndedConversionRaw(7);
                menu_pickup_flag[4]  = m_MCP300X.DoSingleEndedConversionRaw(4);
                menu_pickup_flag[5]  = m_MCP300X.DoSingleEndedConversionRaw(5);
                menu_pickup_flag[6]  = m_MCP300X.DoSingleEndedConversionRaw(6);
                menu_pickup_flag[7]  = m_MCP300X.DoSingleEndedConversionRaw(7);
                menu_pickup_flag[8]  = m_MCP300X.DoSingleEndedConversionRaw(4);
                menu_pickup_flag[9]  = m_MCP300X.DoSingleEndedConversionRaw(5);
                menu_pickup_flag[10] = m_MCP300X.DoSingleEndedConversionRaw(6);
                menu_pickup_flag[11] = m_MCP300X.DoSingleEndedConversionRaw(7);
}
*/
void            CKernel::io_read_ADC                () 
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
                    if(f_ring_buffer[channel][f_index_ring_buffer] > 1023) f_ring_buffer[channel][f_index_ring_buffer] = 1023;
          
                        g_inOutMatrixInt[channel][raw]  = ( f_ring_buffer[channel][0] +
                                                            f_ring_buffer[channel][1] +
                                                            f_ring_buffer[channel][2] +
                                                            f_ring_buffer[channel][3]) >>2 ; 

                        g_inOutMatrixInt[channel][val]    = ( g_inOutMatrixInt[channel][raw] * scaleFactors[g_attenuation] ) /1023;
                          
                        g_inOutMatrixFlt[channel][val]    = ( g_inOutMatrixInt[channel][val] /1024.0f );
                    }
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}

void            CKernel::util_random_vec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;
                uint32_t        f_x         = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[0][rnd] = / f_x * f_scale;                                               // the cast is, i assume in this place pure cosmetics
                inOutMatrixInt[0][rnd] = ( inOutMatrixFlt[0][rnd] * f_max_int);  // the cast is, i assume in this place pure cosmetics

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[1][rnd] = f_x * f_scale;
                inOutMatrixInt[1][rnd] = ( inOutMatrixFlt[1][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[2][rnd] = f_x * f_scale;
                inOutMatrixInt[2][rnd] = ( inOutMatrixFlt[2][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[3][rnd] = f_x * f_scale;
                inOutMatrixInt[3][rnd] = ( inOutMatrixFlt[3][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[4][rnd] = /* (float) */ f_x * f_scale;
                inOutMatrixInt[4][rnd] = ( inOutMatrixFlt[4][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[5][rnd] = /* (float) */ f_x * f_scale;
                inOutMatrixInt[5][rnd] = ( inOutMatrixFlt[5][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[6][rnd] = /* (float) */ f_x * f_scale;
                inOutMatrixInt[6][rnd] = ( inOutMatrixFlt[6][rnd] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                inOutMatrixFlt[7][rnd] = f_x * f_scale;
                inOutMatrixInt[7][rnd] = ( inOutMatrixFlt[7][rnd] * f_max_int);
}

void            CKernel::util_calculate_BPM         (unsigned long p_triggerTimeClockA, unsigned long p_triggerTimeClockB) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClockA != f_lastTime[0])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[0][f_timeIndex[0]] = p_triggerTimeClockA;
        
                    g_intervalBuffer[0][0] = f_timeBuffer[0][1] - f_timeBuffer[0][0];   // no need to make this global, global right i mean the content is consumed in sito or do i need a static local array?
                    g_intervalBuffer[0][1] = f_timeBuffer[0][2] - f_timeBuffer[0][1];
                    g_intervalBuffer[0][2] = f_timeBuffer[0][3] - f_timeBuffer[0][2];
                if(     g_intervalBuffer[0][1] < g_intervalBuffer[0][0] * 1.25f 
                    &&  g_intervalBuffer[0][2] < g_intervalBuffer[0][0] * 1.25f
                    &&  g_intervalBuffer[0][0] < g_intervalBuffer[0][2] * 1.25f ) 
                        {
                        unsigned long f_intervalAverage = (g_intervalBuffer[0][0] + g_intervalBuffer[0][1] + g_intervalBuffer[0][2]) / 3;
            
                        g_resultBPM[0] = 60000000 / f_intervalAverage;
            
                        g_intervalCalculated[0] = f_intervalAverage;
                        g_lastBpmCalculation[0]  = m_Timer.GetClockTicks();
                        }
                    f_lastTime[0] = p_triggerTimeClockA;

                    f_timeIndex[0] = (f_timeIndex[0] + 1) % 4;    
                    }
                if (p_triggerTimeClockB != f_lastTime[1])                                                           // Process clock u_time (instance 1)
                    {
                    f_timeBuffer[1][f_timeIndex[1]] = p_triggerTimeClockB;
        
                    g_intervalBuffer[1][0] = f_timeBuffer[1][1] - f_timeBuffer[1][0];       // takes the intervals
                    g_intervalBuffer[1][1] = f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    g_intervalBuffer[1][2] = f_timeBuffer[1][3] - f_timeBuffer[1][2];
                    if(     g_intervalBuffer[1][1] < g_intervalBuffer[1][0] * 1.25f         // calculates an average and allows 25% play ( quite high right ) 
                        &&  g_intervalBuffer[1][2] < g_intervalBuffer[1][0] * 1.25f
                        &&  g_intervalBuffer[1][0] < g_intervalBuffer[1][2] * 1.25f ) 
                        {
                        unsigned long f_intervalAverage = (g_intervalBuffer[1][0] + g_intervalBuffer[1][1] + g_intervalBuffer[1][2]) / 3;
            
                        g_resultBPM[1] = 60000000 / f_intervalAverage;
            
                        g_intervalCalculated[1] = f_intervalAverage;
                        g_lastBpmCalculation[1]  = m_Timer.GetClockTicks();
                        }
                    f_lastTime[1] = p_triggerTimeClockB;

                    f_timeIndex[1] = (f_timeIndex[1] + 1) % 4;    
                    }
}

void            CKernel::util_determine_bpm_source  () // i can make this an "inline" ternaty statement, right?
{
                if (g_lastBpmCalculation[0] > g_lastBpmCalculation[1])                                              // Compare g_lastBpmCalculation[0] and g_lastBpmCalculation[1] to decide which one is newer
                    {
                    g_activeBpmChannel = 0;                                                             // Choose channel 0 as the active BPM source
                    }
                    if (g_lastBpmCalculation[1] > g_lastBpmCalculation[0] /* && g_input_mode[5] == 2 */ ) 
                    {
                    g_activeBpmChannel = 1;                                                             // Choose channel 1 as the active BPM source
                    }
}

void            CKernel::util_update_predicted_beat ()
{
                unsigned long currentTime = m_Timer.GetClockTicks(); // Get the current u_time in clock ticks

                // Update for instance 0
                if (currentTime >= g_nextBeatTime[0]) 
                {
                    g_nextBeatTime[0] += g_intervalCalculated[0]; // Predict the next beat u_time
                }
                if (currentTime >= g_nextCircleBuffer[0]) 
                    {
                    g_lastCircleBuffer[0]       = g_nextCircleBuffer[0];
                    g_nextCircleBuffer[0]       = g_nextCircleBuffer[0] + 
                                                  (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[0]); // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[0]          = g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                // Handle BPM changes for instance 0
                if ((g_lastValidCalculationBuffer[0] != g_lastBpmCalculation[0]))
                    {
                    g_nextBeatTime[0]           = g_lastBpmCalculation[0]; // Reset to current time for new BPM

                    g_lastValidCalculationBuffer[0]    = g_lastBpmCalculation[0];
                    }
                if (g_lfoMltTmp[0] != g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[0]       = g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[0]       = g_lastBpmCalculation[g_activeBpmChannel] + 
                                                            (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[0]); // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[0]          = g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                // Update for instance 1
                if (currentTime >= g_nextBeatTime[1]) 
                    {
                    g_nextBeatTime[1] += g_intervalCalculated[1]; // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[1]) 
                    {
                    g_lastCircleBuffer[1]       = g_nextCircleBuffer[1];
                    g_nextCircleBuffer[1]       = g_nextCircleBuffer[1] + 
                                                  (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[1] ); // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);

                    g_lfoMltTmp[1]          = g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }
                // Handle BPM changes for instance 1
                if ((g_lastValidCalculationBuffer[1] != g_lastBpmCalculation[1]))
                    {
                    g_nextBeatTime[1]           = g_lastBpmCalculation[1]; // Reset to current time for new BPM

                    g_lastValidCalculationBuffer[1]    = g_lastBpmCalculation[1];
                    }
                if (g_lfoMltTmp[1] != g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[1]       = g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[1]       = g_lastBpmCalculation[g_activeBpmChannel] + 
                                                            (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[1]); // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[1]          = g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }

}

void            CKernel::util_LFO                   ()
{
                unsigned long currentTime = m_Timer.GetClockTicks(); // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_elapsedMicroseconds[0]                    =               currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0]                            =               g_nextCircleBuffer[0] - g_lastCircleBuffer[0]; // Total length of the current cycle

                int f_indexA                                =               (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];
                g_sampleIndex[0]                            =               f_indexA > 255 ? 255 : f_indexA;            // ! i like to get rid of this saveguard !


                inOutMatrixFlt[0][lf1] =                g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                inOutMatrixInt[0][lf1] =               g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1] =               currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1]        =               g_nextCircleBuffer[1] - g_lastCircleBuffer[1]; // Total length of the current cycle

                int f_indexB              =                                 (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1]          =                                 f_indexB > 255 ? 255 : f_indexB;            // ! i like to get rid of this saveguard !

                inOutMatrixFlt[0][lf2] =                g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                inOutMatrixInt[0][lf2] =               g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]];
}   

void            CKernel::util_audio_energy          (float p_adcvalue) 
{   
                const int f_maxBuffer = 33;

                const int f_averageBufferSizeTable[4][5] = 
                    {
                    {33, 25, 17,  9,  5},  // Channel 0 (lowest frequency)  
                    {25, 19, 13,  7,  4},  // Channel 1
                    {17, 13,  9,  5,  3},  // Channel 2
                    { 9,  7,  5,  3,  2}   // Channel 3 (highest frequency)
                    };
                    
                static float f_band0[f_maxBuffer] = {0};                    // Static ring buffers for each frequency band
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};
                static unsigned char f_indexBand0 = 0;                    // Static indices for ring buffers
                static unsigned char f_indexBand1 = 0;
                static unsigned char f_indexBand2 = 0;
                static unsigned char f_indexBand3 = 0;

                if ( g_sensitivityNew != g_sensitivityOld )
                    {
                    inOutMatrixFlt[0][au0] = 0;
                    inOutMatrixFlt[0][au1] = 0;
                    inOutMatrixFlt[0][au2] = 0;
                    inOutMatrixFlt[0][au3] = 0;

                    f_indexBand0 = 0;    // Reset ring buffer indices too
                    f_indexBand1 = 0;
                    f_indexBand2 = 0;
                    f_indexBand3 = 0;

                    g_sensitivityOld = g_sensitivityNew;
                    }   
                f_band0[f_indexBand0] = p_adcvalue;                           // Update ring buffers with the new ADC value
                f_band1[f_indexBand1] = p_adcvalue;
                f_band2[f_indexBand2] = p_adcvalue;
                f_band3[f_indexBand3] = p_adcvalue;

                for (unsigned char i = 0; i < f_averageBufferSizeTable[0][g_sensitivityNew]; ++i)     // Averaging the buffer contents
                    {
                    inOutMatrixFlt[0][au0] += f_band0[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[1][g_sensitivityNew]; ++i) 
                    {
                    inOutMatrixFlt[0][au1] += f_band1[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[2][g_sensitivityNew]; ++i) 
                    {
                    inOutMatrixFlt[0][au2] += f_band2[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[3][g_sensitivityNew]; ++i) 
                    {
                    inOutMatrixFlt[0][au3] += f_band3[i];
                    }
                inOutMatrixFlt[0][au0] /= f_averageBufferSizeTable[0][g_sensitivityNew];
                inOutMatrixFlt[0][au1] /= f_averageBufferSizeTable[1][g_sensitivityNew];
                inOutMatrixFlt[0][au2] /= f_averageBufferSizeTable[2][g_sensitivityNew];
                inOutMatrixFlt[0][au3] /= f_averageBufferSizeTable[3][g_sensitivityNew];

                f_indexBand0 = (f_indexBand0 + 1) % f_averageBufferSizeTable[0][g_sensitivityNew];        // Update indices
                f_indexBand1 = (f_indexBand1 + 1) % f_averageBufferSizeTable[1][g_sensitivityNew];
                f_indexBand2 = (f_indexBand2 + 1) % f_averageBufferSizeTable[2][g_sensitivityNew];
                f_indexBand3 = (f_indexBand3 + 1) % f_averageBufferSizeTable[3][g_sensitivityNew];
}
