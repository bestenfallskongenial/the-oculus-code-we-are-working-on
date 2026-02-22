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
                static int f_activeShader = 0;  // Static variable to maintain value between function calls
                int f_calculated = adc_raw_value[/*ADC_SELECT_PRG*/ p_channel] * g_loaded_fsh_new / 1024;  // *** was adc_int_value but that is effected by the g_attenuation
                // Only update if the f_calculated index points to a valid u_program_handle
                if (m_shaderStatusFlags[f_calculated ]==true /*&& g_menu_mode_new == 0*/) f_activeShader = f_calculated;

                return f_activeShader;
}

int             CKernel::util_choose_texture        ( int p_channel ) // i have three possible ways here! i can a) invent a mechanism to get the is valid table for the vids - i can also draw from parser.is_valid[x] 
{
                static int f_activeTexture = 0;
                if (g_validTextureCount != 0) 
                    {
                    int f_calculated = adc_raw_value[/*ADC_SELECT_TEX*/ p_channel] * (g_validTextureCount ) / 1024; // *** was adc_int_value but that is effected by the g_attenuation
                    f_activeTexture = f_calculated;
                    }
                return f_activeTexture;
}
int             CKernel::util_choose_video        ( int p_channel )
{
                static int f_activeVideo = 0;
                if (g_validVideoCount != 0) 
                    {
                    int f_calculated = adc_raw_value[/*ADC_SELECT_VID*/ p_channel] * (g_validVideoCount ) / 1024; // *** was adc_int_value but that is effected by the g_attenuation
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
                const float f_max_adc = 1023.0;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                static int  f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                        
                static int f_index_ring_buffer;

                for (unsigned channel = 0; channel < ADC_CHANNELS; ++channel)  // Loop through each channel and read its raw value 
                    {
                    f_ring_buffer[channel][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(channel);  // or 1023?!
                    if(f_ring_buffer[channel][f_index_ring_buffer] > 1023) f_ring_buffer[channel][f_index_ring_buffer] = 1023;
          
                        g_inOutMatrixInt[channel][0]    = ( f_ring_buffer[channel][0] +
                                                            f_ring_buffer[channel][1] +
                                                            f_ring_buffer[channel][2] +
                                                            f_ring_buffer[channel][3]) >>2 ; 
/*                                                          
                               adc_raw_value[channel]   = ( f_ring_buffer[channel][0] +
                                                            f_ring_buffer[channel][1] +
                                                            f_ring_buffer[channel][2] +
                                                            f_ring_buffer[channel][3]) >>2 ; 
*/
                        g_inOutMatrixInt[channel][1]    = ( g_inOutMatrixInt[channel][0] * scaleFactors[g_attenuation] ) /1023;
                    //  adc_int_value[channel]          = ( adc_raw_value[channel] * scaleFactors[g_attenuation] ) / 1023;                                
                        g_inOutMatrixFlt[channel][1]    = ( g_inOutMatrixInt[channel][1] /1024.0f );
                    //  adc_float_value[channel] 	    = (	adc_int_value[channel]) / (1024.0f);    // f_max_adc +1 or GLfloat normalize the raw value to GLfloat 0.0 to 1.0
                    }
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;
}

/*

 lets see. lets put all adc out -> modificator -> uniform out in one single matrix like inOutMatrix[channels][types]
 the idea is simple, have everything in one place, right? 
 the list of globals shrink, the code is theoretical simpler to read...
 and we have many of our uniforms at one place for the glsl code

 inOutMatrixInt[channels][ adc_in_raw] 0    how can i "cramp" in the raw? a seperate array? use the indexes over 4 ( 3 ?)?
                                            i wonder because like to have the same indexing for both io arrays, meakes sense right? 
 inOutMatrixInt[channels][adc_out_int] 1 
 inOutMatrixInt[channels][adc_out_int] 2 
 inOutMatrixInt[channels][lf1_out_int] 3 
 inOutMatrixInt[channels][lf2_out_int] 4 

 inOutMatrixInt[channels][  rnd_int_0] 9  
 inOutMatrixInt[channels][  rnd_int_1] 10 
 inOutMatrixInt[channels][  rnd_int_2] 11 
 inOutMatrixInt[channels][  rnd_int_3] 12 
 inOutMatrixInt[channels][  rnd_int_4] 13 
 inOutMatrixInt[channels][  rnd_int_5] 14 
 inOutMatrixInt[channels][  rnd_int_6] 15 
 inOutMatrixInt[channels][  rnd_int_7] 16 

 inOutMatrixFlt[channels][ adc_in_flt] 1 
 inOutMatrixFlt[channels][adc_out_flt] 2 
 inOutMatrixFlt[channels][lf1_out_flt] 3 
 inOutMatrixFlt[channels][lf2_out_flt] 4 
  ????
 inOutMatrixFlt[channels][ aud_band_0] 5  
 inOutMatrixFlt[channels][ aud_band_1] 6 
 inOutMatrixFlt[channels][ aud_band_2] 7 
 inOutMatrixFlt[channels][ aud_band_3] 8 
  ????
 inOutMatrixFlt[channels][  rnd_flt_0] 9  
 inOutMatrixFlt[channels][  rnd_flt_1] 10 
 inOutMatrixFlt[channels][  rnd_flt_2] 11  
 inOutMatrixFlt[channels][  rnd_flt_3] 12  
 inOutMatrixFlt[channels][  rnd_flt_4] 13  
 inOutMatrixFlt[channels][  rnd_flt_5] 14 
 inOutMatrixFlt[channels][  rnd_flt_6] 15 
 inOutMatrixFlt[channels][  rnd_flt_7] 16 

  than we need an enum io_types = {}, like this right?

enum io_types
{
    IO_ADC_RAW = 0,
    IO_ADC_INT,
    IO_ADC_FLT,

    IO_ADC_OUT_INT,
    IO_ADC_OUT_FLT,

    IO_RND_INT,
    IO_RND_FLT,

    IO_LF1_INT,
    IO_LF1_FLT,

    IO_LF2_INT,
    IO_LF2_FLT,

    IO_TYPE_COUNT
};

  than - shouldt we do the same for the many lfo arrays?
  
*/

void            CKernel::util_random_vec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;
                uint32_t        f_x         = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[0]   = (float)f_x * f_scale;
                g_randomIntegerValue[0] = (uint32_t)(g_randomFloatValue[0] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[1]   = (float)f_x * f_scale;
                g_randomIntegerValue[1] = (uint32_t)(g_randomFloatValue[1] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[2]   = (float)f_x * f_scale;
                g_randomIntegerValue[2] = (uint32_t)(g_randomFloatValue[2] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[3]   = (float)f_x * f_scale;
                g_randomIntegerValue[3] = (uint32_t)(g_randomFloatValue[3] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[4]   = (float)f_x * f_scale;
                g_randomIntegerValue[4] = (uint32_t)(g_randomFloatValue[4] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[5]   = (float)f_x * f_scale;
                g_randomIntegerValue[5] = (uint32_t)(g_randomFloatValue[5] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[6]   = (float)f_x * f_scale;
                g_randomIntegerValue[6] = (uint32_t)(g_randomFloatValue[6] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[7]   = (float)f_x * f_scale;
                g_randomIntegerValue[7] = (uint32_t)(g_randomFloatValue[7] * f_max_int);
}

void            CKernel::util_calculate_BPM         (unsigned long p_triggerTimeClockA, unsigned long p_triggerTimeClockB) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClockA != f_lastTime[0])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[0][f_timeIndex[0]] = p_triggerTimeClockA;
        
                    g_intervalBuffer[0][0] = f_timeBuffer[0][1] - f_timeBuffer[0][0];   // ne need to make this global?
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

                g_elapsedMicroseconds[0]  = currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0]          = g_nextCircleBuffer[0] - g_lastCircleBuffer[0]; // Total length of the current cycle

                int f_indexA              = (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];
                g_sampleIndex[0]          = f_indexA > 255 ? 255 : f_indexA;


                g_lfoFltOut[0]     = (float)g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]] / 1023.0f;
                g_lfoIntOut[0]       =        g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1]  = currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1]          = g_nextCircleBuffer[1] - g_lastCircleBuffer[1]; // Total length of the current cycle

                int f_indexB              = (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1]          = f_indexB > 255 ? 255 : f_indexB;

                g_lfoFltOut[1]     = (float)g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]] / 1023.0f; // was 0123.0f
                g_lfoIntOut[1]       =        g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]];
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
                    u_audioSmoothBand[0] = 0;
                    u_audioSmoothBand[1] = 0;
                    u_audioSmoothBand[2] = 0;
                    u_audioSmoothBand[3] = 0;

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
                    u_audioSmoothBand[0] += f_band0[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[1][g_sensitivityNew]; ++i) 
                    {
                    u_audioSmoothBand[1] += f_band1[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[2][g_sensitivityNew]; ++i) 
                    {
                    u_audioSmoothBand[2] += f_band2[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[3][g_sensitivityNew]; ++i) 
                    {
                    u_audioSmoothBand[3] += f_band3[i];
                    }
                u_audioSmoothBand[0] /= f_averageBufferSizeTable[0][g_sensitivityNew];
                u_audioSmoothBand[1] /= f_averageBufferSizeTable[1][g_sensitivityNew];
                u_audioSmoothBand[2] /= f_averageBufferSizeTable[2][g_sensitivityNew];
                u_audioSmoothBand[3] /= f_averageBufferSizeTable[3][g_sensitivityNew];

                f_indexBand0 = (f_indexBand0 + 1) % f_averageBufferSizeTable[0][g_sensitivityNew];        // Update indices
                f_indexBand1 = (f_indexBand1 + 1) % f_averageBufferSizeTable[1][g_sensitivityNew];
                f_indexBand2 = (f_indexBand2 + 1) % f_averageBufferSizeTable[2][g_sensitivityNew];
                f_indexBand3 = (f_indexBand3 + 1) % f_averageBufferSizeTable[3][g_sensitivityNew];
}
