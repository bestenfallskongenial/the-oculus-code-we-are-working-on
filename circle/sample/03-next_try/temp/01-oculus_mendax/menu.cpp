#include "kernel.h"
#include "global.h"

void            CKernel::menu_general               ()
{
                if (g_menu_mode_new != g_menu_mode_old) 
                    {

                    for(int i = 0; i < 16; i++) 
                        {
                        menu_pickup_flag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;
                    }
}

void            CKernel::menu_mode_default_A        ()  // default function for the cannels 
{
                int chn = 0;
                
                int brightness  = 0;

                int brightnessA = 0;
                int brightnessB = 0;
                int brightnessC = 0;
                int brightnessD = 0;

                unsigned long current_time = m_Timer.GetClockTicks();
                if ( g_audio_source_channel != -1 )                                               // lets create an additional function to do this 
                    {                                                                           // what are we doing here? when we in aud mode we display the energy
                        util_audio_energy(adc_float_value[g_audio_source_channel]);
                        output_float_value[0] = u_audioSmoothBand[0];
                        output_float_value[1] = u_audioSmoothBand[1];
                        output_float_value[2] = u_audioSmoothBand[2];
                        output_float_value[3] = u_audioSmoothBand[3];

                        brightnessA = (u_audioSmoothBand[0] * 63) + 1;  // gives 1-63
                        brightnessB = (u_audioSmoothBand[1] * 63) + 1;
                        brightnessC = (u_audioSmoothBand[2] * 63) + 1;
                        brightnessD = (u_audioSmoothBand[3] * 63) + 1;

                                led_col_new[0][R] = g_rgb_color_table[A_0 + g_sensitivityNew][R] + ((g_rgb_color_table[A_0 + g_sensitivityNew][R] != 0) * brightnessA);
                                led_col_new[0][G] = g_rgb_color_table[A_0 + g_sensitivityNew][G] + ((g_rgb_color_table[A_0 + g_sensitivityNew][G] != 0) * brightnessA);
                                led_col_new[0][B] = g_rgb_color_table[A_0 + g_sensitivityNew][B] + ((g_rgb_color_table[A_0 + g_sensitivityNew][B] != 0) * brightnessA);

                                led_col_new[1][R] = g_rgb_color_table[A_0 + g_sensitivityNew][R] + ((g_rgb_color_table[A_0 + g_sensitivityNew][R] != 0) * brightnessB);
                                led_col_new[1][G] = g_rgb_color_table[A_0 + g_sensitivityNew][G] + ((g_rgb_color_table[A_0 + g_sensitivityNew][G] != 0) * brightnessB);
                                led_col_new[1][B] = g_rgb_color_table[A_0 + g_sensitivityNew][B] + ((g_rgb_color_table[A_0 + g_sensitivityNew][B] != 0) * brightnessB);

                                led_col_new[2][R] = g_rgb_color_table[A_0 + g_sensitivityNew][R] + ((g_rgb_color_table[A_0 + g_sensitivityNew][R] != 0) * brightnessC);
                                led_col_new[2][G] = g_rgb_color_table[A_0 + g_sensitivityNew][G] + ((g_rgb_color_table[A_0 + g_sensitivityNew][G] != 0) * brightnessC);
                                led_col_new[2][B] = g_rgb_color_table[A_0 + g_sensitivityNew][B] + ((g_rgb_color_table[A_0 + g_sensitivityNew][B] != 0) * brightnessC);

                                led_col_new[3][R] = g_rgb_color_table[A_0 + g_sensitivityNew][R] + ((g_rgb_color_table[A_0 + g_sensitivityNew][R] != 0) * brightnessD);
                                led_col_new[3][G] = g_rgb_color_table[A_0 + g_sensitivityNew][G] + ((g_rgb_color_table[A_0 + g_sensitivityNew][G] != 0) * brightnessD);
                                led_col_new[3][B] = g_rgb_color_table[A_0 + g_sensitivityNew][B] + ((g_rgb_color_table[A_0 + g_sensitivityNew][B] != 0) * brightnessD);                                                                                                                                                                                                                        
                    }
                else
                    {
                    chn = 0;

                    switch(g_centralModeBuffer[CH0_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC
                            output_float_value[0] = adc_float_value[0];
                            output_int_value[0]   = adc_int_value[0];

                                brightness = output_int_value[0]>> 4;       // same here? an function for adc mode? 

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);
                        break;
                    
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[0] >= g_threshold_high[0] && !input_threshold_flag[0]) 
                            {
                            output_float_value[0] = g_randomFloatValue[0];
                            output_int_value[0]   = g_randomIntegerValue[0];

                                brightness = output_int_value[0]>> 4;       // and trigger mode?

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[0] = true;
                            }
                        else if (adc_int_value[0] < g_threshold_low[0]) 
                            {
                            input_threshold_flag[0] = false;
                            }
                        break;

                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[0] = g_randomFloatValue[0];
                            output_int_value[0]   = g_randomIntegerValue[0];

                                led_col_new[chn][R] = 159;                  // and bpm mode?
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[0]>> 4; 

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }
                        break;
                        case 3:     // LFO 1 MODE
                            output_float_value[0] = g_lfoFltOut[0];
                            output_int_value[0]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;         // and for lfo 1? 

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:     // MODE 2 MODE
                            output_float_value[0] = g_lfoFltOut[1];
                            output_int_value[0]   = g_lfoIntOut[1];

                                brightness = g_lfoIntOut[1]>> 4;         // and lfo 2?

                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;
                        }
    // normal channel 1
                    chn = 1;

                    switch(g_centralModeBuffer[CH1_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC
                            output_float_value[1] = adc_float_value[1];
                            output_int_value[1]   = adc_int_value[1];

                                brightness = output_int_value[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);
                        break;
                    
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[1] >= g_threshold_high[1] && !input_threshold_flag[1]) 
                            {
                            output_float_value[1] = g_randomFloatValue[1];
                            output_int_value[1]   = g_randomIntegerValue[1];

                                brightness = output_int_value[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[1] = true;
                            }
                        else if (adc_int_value[1] < g_threshold_low[1]) 
                            {
                            input_threshold_flag[1] = false;
                            }
                        break;
                        
                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[1] = g_randomFloatValue[1];
                            output_int_value[1]   = g_randomIntegerValue[1];

                                led_col_new[chn][R] = 159;
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }
                        break;               
                        case 3:
                            output_float_value[1] = g_lfoFltOut[0];
                            output_int_value[1]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:
                            output_float_value[1] = g_lfoFltOut[1];
                            output_int_value[1]   = g_lfoIntOut[1];                            

                                brightness = g_lfoIntOut[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;
                        }
    // normal channel 2
                    chn = 2;

                    switch(g_centralModeBuffer[CH2_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC
                            output_float_value[2] = adc_float_value[2];
                            output_int_value[2]   = adc_int_value[2];

                                brightness = output_int_value[2]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);                          
                        break;
                    
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[2] >= g_threshold_high[2] && !input_threshold_flag[2]) 
                            {
                            output_float_value[2] = g_randomFloatValue[2];
                            output_int_value[2]   = g_randomIntegerValue[2];

                                brightness = output_int_value[2]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[2] = true;
                            }
                        else if (adc_int_value[2] < g_threshold_low[2]) 
                            {
                            input_threshold_flag[2] = false;
                            }
                        break;

                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[2] = g_randomFloatValue[2];
                            output_int_value[2]   = g_randomIntegerValue[2];

                                led_col_new[chn][R] = 159;
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[2]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }
                        break;
                        case 3:
                            output_float_value[2] = g_lfoFltOut[0];
                            output_int_value[2]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:
                            output_float_value[2] = g_lfoFltOut[1];
                            output_int_value[2]   = g_lfoIntOut[1];

                                brightness = g_lfoIntOut[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;                        
                        }
    // normal channel 3
                    chn = 3;

                    switch(g_centralModeBuffer[CH3_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC
                            output_float_value[3] = adc_float_value[3];
                            output_int_value[3]   = adc_int_value[3];

                                brightness = output_int_value[3]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);                                  
                        break;
                    
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[3] >= g_threshold_high[3] && !input_threshold_flag[3]) 
                            {
                            output_float_value[3] = g_randomFloatValue[3];
                            output_int_value[3]   = g_randomIntegerValue[3];

                                brightness = output_int_value[3]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[3] = true;
                            }
                        else if (adc_int_value[3] < g_threshold_low[3]) 
                            {
                            input_threshold_flag[3] = false;
                            }
                        break;

                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[3] = g_randomFloatValue[3];
                            output_int_value[3]   = g_randomIntegerValue[3];

                                led_col_new[chn][R] = 159;
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[3]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }
                        break;
                        case 3:
                            output_float_value[3] = g_lfoFltOut[0];
                            output_int_value[3]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:
                            output_float_value[3] = g_lfoFltOut[1];
                            output_int_value[3]   = g_lfoIntOut[1];

                                brightness = g_lfoIntOut[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;                        
                        }
                    }
}


void            CKernel::menu_mode_default_B        ()  // default function for the cannels 4-7
{
                unsigned long current_time = m_Timer.GetClockTicks();

                int brightness  = 0;

// normal channel 4

                int chn = 4;

                switch(g_centralModeBuffer[CH4_MODE][g_currentProgramBuffer]) 
                    {
                    case 0:  // g_input_mode = 0 - Direct ADC special - frame/time input

                            if ( g_menu_mode_new == 0 ) output_float_value[4] = adc_float_value[4];
                            if ( g_menu_mode_new == 0 ) output_int_value[4]   = adc_int_value[4];

                                brightness = output_int_value[4]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness); 
                    break;

                    case 1:  // g_input_mode = 1 - Threshold trigger
                    if ( adc_int_value[4] >= g_threshold_high[4] && !input_threshold_flag[4]) 
                        {
                        output_float_value[4] = g_randomFloatValue[4];
                        output_int_value[4]   = g_randomIntegerValue[4];

                            brightness = output_int_value[4]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                        input_threshold_flag[4] = true;
                        }
                    else if (adc_int_value[4] < g_threshold_low[4]) 
                        {
                        input_threshold_flag[4] = false;
                        }
                    break;

                    case 2:  // BPM trigger
                    if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                        {
                        output_float_value[4] = g_randomFloatValue[4];
                        output_int_value[4]   = g_randomIntegerValue[4];

                            led_col_new[chn][R] = 159;
                            led_col_new[chn][G] = 159;
                            led_col_new[chn][B] = 159;
                        }
                    else
                        {
                            brightness = output_int_value[4]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                        }
                    break;
                    case 3:
                        output_float_value[4] = g_lfoFltOut[0];
                        output_int_value[4]   = g_lfoIntOut[0];

                            brightness = g_lfoIntOut[0]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                    break;
                    case 4:
                        output_float_value[4] = g_lfoFltOut[1];
                        output_int_value[4]   = g_lfoIntOut[1];

                            brightness = g_lfoIntOut[1]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                    break;                        
                    }
// normal channel 5
                chn = 5;

                switch(g_centralModeBuffer[CH5_MODE][g_currentProgramBuffer]) 
                    {
                    case 0:  // g_input_mode = 0 - Direct ADC
                        output_float_value[5] = adc_float_value[5];
                        output_int_value[5]   = adc_int_value[5];

                            brightness = output_int_value[5]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);                                   
                    break;

                    case 1:  // g_input_mode = 1 - Threshold trigger special clk input
                    if ( adc_int_value[5] >= g_threshold_high[5] && !input_threshold_flag[5]) 
                        {
                        output_float_value[5] = g_randomFloatValue[5];
                        output_int_value[5]   = g_randomIntegerValue[5];

                            led_col_new[chn][R] = 223;
                            led_col_new[chn][G] = 223;
                            led_col_new[chn][B] = 223;

                        current_time_for_bpm = current_time;

                        input_threshold_flag[5] = true;
                        }
                    else if (adc_int_value[5] < g_threshold_low[5]) 
                        {
                        input_threshold_flag[5] = false;
                        }
                    else
                        {
                            brightness = output_int_value[5]>> 4;
                                led_col_new[chn][R] = (g_rgb_color_table[CLK][R] + brightness);
                                led_col_new[chn][G] = (g_rgb_color_table[CLK][G] + brightness);
                                led_col_new[chn][B] = (g_rgb_color_table[CLK][B] + brightness);
                        }

                    break;

                    case 2:  // BPM trigger
                    if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                        {
                        output_float_value[5] = g_randomFloatValue[5];
                        output_int_value[5]   = g_randomIntegerValue[5];

                            led_col_new[chn][R] = 159;
                            led_col_new[chn][G] = 159;
                            led_col_new[chn][B] = 159;
                        }
                    else
                        {
                            brightness = output_int_value[5]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                        }
                    break;
                    case 3:
                        output_float_value[5] = g_lfoFltOut[0];
                        output_int_value[5]   = g_lfoIntOut[0];

                            brightness = g_lfoIntOut[0]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                    break;
                    case 4:
                        output_float_value[5] = g_lfoFltOut[1];
                        output_int_value[5]   = g_lfoIntOut[1];

                            brightness = g_lfoIntOut[1]>> 4;

                                led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                    break;                        
                    }
// normal channel 6
                chn = 6;

                if (!menu_pickup_flag[12] && 
                    (adc_raw_value[6] > g_menu_pickup_buffer[12] - ADC_TOLERANCE) &&
                    (adc_raw_value[6] < g_menu_pickup_buffer[12] + ADC_TOLERANCE)) 
                    {
                    menu_pickup_flag[12] = true;
                    }
                else if (menu_pickup_flag[12] || g_centralModeBuffer[CH6_MODE][g_currentProgramBuffer] != 0)
                    {    
                    switch(g_centralModeBuffer[CH6_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC
                            if ( g_menu_mode_new == 0 ) output_float_value[6] = adc_float_value[6];
                            if ( g_menu_mode_new == 0 ) output_int_value[6]   = adc_int_value[6];
                            if ( single_tex_mode == true ) output_int_value[6] = adc_raw_value[6];          // this is an error, we integrated this in our mode_storage_buffer !

                                brightness = output_int_value[6]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[A25 + g_attenuation][R] + ((g_rgb_color_table[A25 + g_attenuation][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A25 + g_attenuation][G] + ((g_rgb_color_table[A25 + g_attenuation][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A25 + g_attenuation][B] + ((g_rgb_color_table[A25 + g_attenuation][B] != 0) * brightness);                                 
                        break;
                
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[6] >= g_threshold_high[6] && !input_threshold_flag[6]) 
                            {
                            output_float_value[6] = g_randomFloatValue[6];
                            output_int_value[6]   = g_randomIntegerValue[6];

                                brightness = output_int_value[6]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[6] = true;
                            }
                        else if (adc_int_value[6] < g_threshold_low[6]) 
                            {
                            input_threshold_flag[6] = false;
                            }
                        break;

                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[6] = g_randomFloatValue[6];
                            output_int_value[6]   = g_randomIntegerValue[6];

                                led_col_new[chn][R] = 159;
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[6]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }                            
                        break;
                        case 3:
                            output_float_value[6] = g_lfoFltOut[0];
                            output_int_value[6]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:
                            output_float_value[6] = g_lfoFltOut[1];
                            output_int_value[6]   = g_lfoIntOut[1];

                                brightness = g_lfoIntOut[1]>> 4;
                                
                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;                        
                        }        
                if ( g_menu_mode_new == 0 ) g_menu_pickup_buffer[12] = adc_raw_value[6];
                        }
                    else
                        {
                        led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                        led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                        led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);    

                    //  if ( g_menu_mode_new == 0 ) g_menu_pickup_buffer[12] = adc_int_value[6];
                        }
                        
    // normal channel 7
                chn = 7;

                if (!menu_pickup_flag[13] && 
                    (adc_raw_value[7] > g_menu_pickup_buffer[13] - ADC_TOLERANCE) &&
                    (adc_raw_value[7] < g_menu_pickup_buffer[13] + ADC_TOLERANCE)) 
                    {
                    menu_pickup_flag[13] = true;
                    }
                else if (menu_pickup_flag[13] || g_centralModeBuffer[CH7_MODE][g_currentProgramBuffer] != 0)
                    {    
                    switch(g_centralModeBuffer[CH7_MODE][g_currentProgramBuffer]) 
                        {
                        case 0:  // g_input_mode = 0 - Direct ADC special shader select
                                if ( g_menu_mode_new == 0 ) output_float_value[7] = adc_float_value[7];
                                if ( g_menu_mode_new == 0 ) output_int_value[7]   = adc_raw_value[7];
                                

                                brightness = output_int_value[7]>> 4;
                                    led_col_new[chn][R] = g_rgb_color_table[A_3 + g_current_gl_program][R] + ((g_rgb_color_table[A_3 + g_current_gl_program][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[A_3 + g_current_gl_program][G] + ((g_rgb_color_table[A_3 + g_current_gl_program][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[A_3 + g_current_gl_program][B] + ((g_rgb_color_table[A_3 + g_current_gl_program][B] != 0) * brightness);

                        break;
                        case 1:  // g_input_mode = 1 - Threshold trigger
                        if ( adc_int_value[7] >= g_threshold_high[7] && !input_threshold_flag[7]) 
                            {
                            output_float_value[7] = g_randomFloatValue[7];
                                output_int_value[7] = g_randomIntegerValue[7];

                                brightness = output_int_value[7]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[TRG][R] + ((g_rgb_color_table[TRG][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[TRG][G] + ((g_rgb_color_table[TRG][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[TRG][B] + ((g_rgb_color_table[TRG][B] != 0) * brightness);

                            input_threshold_flag[7] = true;
                            }
                        else if (adc_int_value[7] < g_threshold_low[7]) 
                            {
                            input_threshold_flag[7] = false;
                            }
                        break;

                        case 2:  // BPM trigger
                        if (current_time >= g_nextBeatTime[g_activeBpmChannel]) 
                            {
                            output_float_value[7] = g_randomFloatValue[7];
                            output_int_value[7]   = g_randomIntegerValue[7];

                                led_col_new[chn][R] = 159;
                                led_col_new[chn][G] = 159;
                                led_col_new[chn][B] = 159;
                            }
                        else
                            {
                                brightness = output_int_value[7]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[BPM][R] + ((g_rgb_color_table[BPM][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[BPM][G] + ((g_rgb_color_table[BPM][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[BPM][B] + ((g_rgb_color_table[BPM][B] != 0) * brightness);
                            }
                        break;
                        case 3:
                            output_float_value[7] = g_lfoFltOut[0];
                            output_int_value[7]   = g_lfoIntOut[0];

                                brightness = g_lfoIntOut[0]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF1][R] + ((g_rgb_color_table[LF1][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF1][G] + ((g_rgb_color_table[LF1][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF1][B] + ((g_rgb_color_table[LF1][B] != 0) * brightness);
                        break;
                        case 4:
                            output_float_value[7] = g_lfoFltOut[1];
                            output_int_value[7]   = g_lfoIntOut[1];

                                brightness = g_lfoIntOut[1]>> 4;

                                    led_col_new[chn][R] = g_rgb_color_table[LF2][R] + ((g_rgb_color_table[LF2][R] != 0) * brightness);
                                    led_col_new[chn][G] = g_rgb_color_table[LF2][G] + ((g_rgb_color_table[LF2][G] != 0) * brightness);
                                    led_col_new[chn][B] = g_rgb_color_table[LF2][B] + ((g_rgb_color_table[LF2][B] != 0) * brightness);
                        break;                        
                        }                       
                if ( g_menu_mode_new == 0 ) g_menu_pickup_buffer[13] = adc_raw_value[7];
                }
                else
                    {
                    led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                    led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                    led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);    
                //    if ( g_menu_mode_new == 0 ) g_menu_pickup_buffer[13] = adc_int_value[7];
                    }  
}

void            CKernel::menu_mode_assign_A         ()  // mapping to the A channels ( 4 to 7 )
{
                int chn = 8;
                int new_mode;
                int brightness  = 0;

                if (g_menu_mode_new == 1)
                    {
// map function 0
                    new_mode = (adc_raw_value[4] * NUMBER_OF_MODES) >> 10;

                    if  (!menu_pickup_flag[0] && new_mode == g_centralModeBuffer[CH0_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[0] = true;
                        }
                    else if (menu_pickup_flag[0]) 
                        {
                            if (g_centralModeBuffer[CH0_MODE][g_currentProgramBuffer] == AUD_MODE) { g_audio_source_channel = -1; }

                            if (new_mode == AUD_MODE) 
                                {                      // Trying to enable audio mode
                                if (g_audio_source_channel == -1) 
                                    {     // No audio source active
                                    g_audio_source_channel = 0;         // Take audio source
                                    }
                                else 
                                    {                             // Audio source already taken
                                    new_mode = g_centralModeBuffer[CH0_MODE][g_currentProgramBuffer];      // Keep previous mode
                                    }
                                }
                        g_centralModeBuffer[CH0_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[0])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[0]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                
// map function 1                               
                    chn = 9;
                    new_mode = (adc_raw_value[5] * NUMBER_OF_MODES) >> 10;

                    if (!menu_pickup_flag[1] && new_mode == g_centralModeBuffer[CH1_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[1] = true;
                        }
                    else if (menu_pickup_flag[1]) 
                        {
                            if (g_centralModeBuffer[CH1_MODE][g_currentProgramBuffer] == AUD_MODE) { g_audio_source_channel = -1; }
                            if (new_mode == AUD_MODE) 
                                {                      // Trying to enable audio mode
                                if (g_audio_source_channel == -1) 
                                    {     // No audio source active
                                    g_audio_source_channel = 1;         // Take audio source
                                    } 
                                else 
                                    {                             // Audio source already taken
                                    new_mode = g_centralModeBuffer[CH1_MODE][g_currentProgramBuffer];      // Keep previous mode
                                    }
                                }
                        g_centralModeBuffer[CH1_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[1])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[1]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
// map function 2
                    chn = 10;

                    new_mode = (adc_raw_value[6] * NUMBER_OF_MODES) >> 10;
                    if (!menu_pickup_flag[2] && new_mode == g_centralModeBuffer[CH2_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[2] = true;
                        }
                    else if (menu_pickup_flag[2]) 
                        {
                            if (g_centralModeBuffer[CH2_MODE][g_currentProgramBuffer] == AUD_MODE) { g_audio_source_channel = -1; }
                            if (new_mode == AUD_MODE) 
                                {                      // Trying to enable audio mode
                                if (g_audio_source_channel == -1) 
                                    {     // No audio source active
                                    g_audio_source_channel = 2;         // Take audio source
                                    } 
                                else 
                                    {                             // Audio source already taken
                                    new_mode = g_centralModeBuffer[CH2_MODE][g_currentProgramBuffer];      // Keep previous mode
                                    }
                                }
                        g_centralModeBuffer[CH2_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[2])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[2]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
// map function 3
                    chn = 11;

                    new_mode = (adc_raw_value[7] * NUMBER_OF_MODES) >> 10;
                    if (!menu_pickup_flag[3] && new_mode == g_centralModeBuffer[CH3_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[3] = true;
                        }
                    else if (menu_pickup_flag[3]) 
                        {
                            if (g_centralModeBuffer[CH3_MODE][g_currentProgramBuffer] == AUD_MODE) { g_audio_source_channel = -1; }
                            if (new_mode == AUD_MODE) 
                                {                      // Trying to enable audio mode
                                if (g_audio_source_channel == -1) 
                                    {     // No audio source active
                                    g_audio_source_channel = 3;         // Take audio source
                                    } 
                                else 
                                    {                             // Audio source already taken
                                    new_mode = g_centralModeBuffer[CH3_MODE][g_currentProgramBuffer];      // Keep previous mode
                                    }
                                }
                        g_centralModeBuffer[CH3_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[3])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_1][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[3]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
                    }
}

void            CKernel::menu_mode_assign_B         ()  // mapping to the A channels ( 4 to 7 )
{
                int chn = 12;

                int new_mode;

                int brightness  = 0;

                if (g_menu_mode_new == 2) 
                    {
// map function 4
                    new_mode = (adc_raw_value[4] * ( NUMBER_OF_MODES - 1) ) >> 10;
                    if  (!menu_pickup_flag[4] && new_mode == g_centralModeBuffer[CH4_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[4] = true;
                        }
                    else if (menu_pickup_flag[4]) 
                        {
                        g_centralModeBuffer[CH4_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[4])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_2][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[4]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
// map function 5
                    chn = 13;

                    new_mode = (adc_raw_value[5] * ( NUMBER_OF_MODES - 1) ) >> 10;
                    if (!menu_pickup_flag[5] && new_mode == g_centralModeBuffer[CH5_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[5] = true;
                        }
                    else if (menu_pickup_flag[5]) 
                        {
                        g_centralModeBuffer[CH5_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[5])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_2][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[5]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
// map function 6
                    chn = 14;

                    new_mode = (adc_raw_value[6] * ( NUMBER_OF_MODES - 1 )) >> 10;
                    if (!menu_pickup_flag[6] && new_mode == g_centralModeBuffer[CH6_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[6] = true;
                        }
                    else if (menu_pickup_flag[6]) 
                        {
                        g_centralModeBuffer[CH6_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[6])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_2][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[6]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
// map function 7
                    chn = 15;

                    new_mode = (adc_raw_value[7] * ( NUMBER_OF_MODES - 1 )) >> 10;
                    if (!menu_pickup_flag[7] && new_mode == g_centralModeBuffer[CH7_MODE][g_currentProgramBuffer])
                        {
                        menu_pickup_flag[7] = true;
                        }
                    else if (menu_pickup_flag[7]) 
                        {
                        g_centralModeBuffer[CH7_MODE][g_currentProgramBuffer] = new_mode;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[7])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_2][B]);
                        }
                    else 
                        {
                        brightness = output_int_value[7]>> 4;

                            led_col_new[chn][R] = g_rgb_color_table[A50 + new_mode][R] + ((g_rgb_color_table[A50 + new_mode][R] != 0) * brightness);
                            led_col_new[chn][G] = g_rgb_color_table[A50 + new_mode][G] + ((g_rgb_color_table[A50 + new_mode][G] != 0) * brightness);
                            led_col_new[chn][B] = g_rgb_color_table[A50 + new_mode][B] + ((g_rgb_color_table[A50 + new_mode][B] != 0) * brightness);
                        }                                 
                    }
}

void            CKernel::menu_mode_assign_LFO       ()  // mapping the LFO-parameters
{
                int chn = 16;

                if (g_menu_mode_new == 3 ) 
                    {
                    int waveformA = (adc_raw_value[4] * ( WAVEFORMS )) >> 10;

                    if (!menu_pickup_flag[8] && (waveformA == g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer])) 
                        {
                        menu_pickup_flag[8] = true;
                        }
                    else if (menu_pickup_flag[8]) 
                        {
                        g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer] = waveformA;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[8])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_L][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_L][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_L][B]);
                        }
                    else 
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[LF1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[LF1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[LF1][B]);
                        }    
                    chn = 17;

                    int waveformB = (adc_raw_value[5] * ( WAVEFORMS )) >> 10;

                    if (!menu_pickup_flag[9] && (waveformB == g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer])) 
                        {
                        menu_pickup_flag[9] = true;
                        }
                    else if (menu_pickup_flag[9]) 
                        {
                        g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer] = waveformB;   // Process new value after pickup
                        }
                    if (!menu_pickup_flag[9])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_L][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_L][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_L][B]);
                        }
                    else 
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[LF2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[LF2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[LF2][B]);
                        }                       
                    chn = 18;

                    int multiplyA = (adc_raw_value[6] * ( 7 )) >> 10;       // was ( 7 ) - LFO_MULTIPLIERS doesnt work

                    if (!menu_pickup_flag[10] && (multiplyA == g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer])) 
                        {
                        menu_pickup_flag[10] = true;
                        }
                    else if (menu_pickup_flag[10]) 
                        {
                        g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer] = multiplyA;   // Process new value after pickup
                        }      
                    if (!menu_pickup_flag[10])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_L][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_L][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_L][B]);
                        }
                    else 
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[LF1][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[LF1][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[LF1][B]);
                        }                                   
                    chn = 19;

                    int multiplyB = (adc_raw_value[7] * ( 7 )) >> 10;

                    if (!menu_pickup_flag[11] && (multiplyB == g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer])) 
                        {
                        menu_pickup_flag[11] = true;
                        }
                    else if (menu_pickup_flag[11]) 
                        {
                        g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer] = multiplyB;   // Process new value after pickup
                        }    
                    if (!menu_pickup_flag[11])
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[M_L][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[M_L][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[M_L][B]);
                        }
                    else 
                        {
                            led_col_new[chn][R] = (g_rgb_color_table[LF2][R]);
                            led_col_new[chn][G] = (g_rgb_color_table[LF2][G]);
                            led_col_new[chn][B] = (g_rgb_color_table[LF2][B]);
                        }                                                                                                          
                    }
}