#include "kernel.h"
#include "global.h"
#include "wavetable.h"
/*
extern "C" {
    void vc_host_get_vchi_state(VCHI_INSTANCE_T *initialise_instance, VCHI_CONNECTION_T **connection);
}
*/

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
int             CKernel::util_choose_program        ()
{
                static int f_activeShader = 0;  // Static variable to maintain value between function calls
                int f_calculated = adc_raw_value[ADC_SELECT_PRG] * g_loaded_fsh_new / 1024;  // *** was adc_int_value but that is effected by the g_attenuation
                // Only update if the f_calculated index points to a valid u_program_handle
                if (m_shaderStatusFlags[f_calculated ]==true /*&& g_menu_mode_new == 0*/) f_activeShader = f_calculated;

                return f_activeShader;
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

int             CKernel::util_choose_texture        ()
{
                static int f_activeTexture = 0;
                if (g_validTextureCount != 0) 
                    {
                    int f_calculated = adc_raw_value[ADC_SELECT_TEX] * (g_validTextureCount ) / 1024; // *** was adc_int_value but that is effected by the g_attenuation
                    f_activeTexture = f_calculated;
                    }
                return f_activeTexture;
}

void            CKernel::util_random_vec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int f_max_int = 1024;
                const float f_scale = 1.0f / 4294967295.0f;
                uint32_t f_x = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[0] = (float)f_x * f_scale;
                g_randomIntegerValue[0] = (uint32_t)(g_randomFloatValue[0] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[1] = (float)f_x * f_scale;
                g_randomIntegerValue[1] = (uint32_t)(g_randomFloatValue[1] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[2] = (float)f_x * f_scale;
                g_randomIntegerValue[2] = (uint32_t)(g_randomFloatValue[2] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[3] = (float)f_x * f_scale;
                g_randomIntegerValue[3] = (uint32_t)(g_randomFloatValue[3] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[4] = (float)f_x * f_scale;
                g_randomIntegerValue[4] = (uint32_t)(g_randomFloatValue[4] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[5] = (float)f_x * f_scale;
                g_randomIntegerValue[5] = (uint32_t)(g_randomFloatValue[5] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[6] = (float)f_x * f_scale;
                g_randomIntegerValue[6] = (uint32_t)(g_randomFloatValue[6] * f_max_int);

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_randomFloatValue[7] = (float)f_x * f_scale;
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
        
                    g_intervalBuffer[0][0] = f_timeBuffer[0][1] - f_timeBuffer[0][0];
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
        
                    g_intervalBuffer[1][0] = f_timeBuffer[1][1] - f_timeBuffer[1][0];
                    g_intervalBuffer[1][1] = f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    g_intervalBuffer[1][2] = f_timeBuffer[1][3] - f_timeBuffer[1][2];
                    if(     g_intervalBuffer[1][1] < g_intervalBuffer[1][0] * 1.25f 
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

void            CKernel::util_determine_bpm_source  ()
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
                    g_lastCircleBuffer[0] = g_nextCircleBuffer[0];
                    g_nextCircleBuffer[0] = g_nextCircleBuffer[0] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[0]); // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[0]          = g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                // Handle BPM changes for instance 0
                if ((g_lastValidCalculationBuffer[0] != g_lastBpmCalculation[0]))
                    {
                    g_nextBeatTime[0]           = g_lastBpmCalculation[0]; // Reset to current time for new BPM

                    g_lastValidCalculationBuffer[0]    = g_lastBpmCalculation[0];
                    }
                if (/*(g_lastValidCalculationBuffer[0] != g_lastBpmCalculation[0]) ||*/ g_lfoMltTmp[0] != g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[0]       = g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[0]       = g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[0]); // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[0]          = g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                // Update for instance 1
                if (currentTime >= g_nextBeatTime[1]) 
                    {
                    g_nextBeatTime[1] += g_intervalCalculated[1]; // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[1]) 
                    {
                    g_lastCircleBuffer[1] = g_nextCircleBuffer[1];
                    g_nextCircleBuffer[1] = g_nextCircleBuffer[1] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[1] ); // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);

                    g_lfoMltTmp[1]          = g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }
                // Handle BPM changes for instance 1
                if ((g_lastValidCalculationBuffer[1] != g_lastBpmCalculation[1]))
                    {
                    g_nextBeatTime[1]           = g_lastBpmCalculation[1]; // Reset to current time for new BPM

                    g_lastValidCalculationBuffer[1]    = g_lastBpmCalculation[1];
                    }
                if (/*(g_lastValidCalculationBuffer[1] != g_lastBpmCalculation[1]) ||*/ g_lfoMltTmp[1] != g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[1]       = g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[1]       = g_lastBpmCalculation[g_activeBpmChannel] + (g_intervalCalculated[g_activeBpmChannel] * g_lfoMltTmp[1]); // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);
                    
                    g_lfoMltTmp[1]          = g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }

}

void            CKernel::util_LFO                   ()
{
                unsigned long currentTime = m_Timer.GetClockTicks(); // Get the current u_time in clock ticks

                g_elapsedMicroseconds[0] =   currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0] = g_nextCircleBuffer[0] - g_lastCircleBuffer[0]; // Total length of the current cycle

            //    g_sampleIndex[0] = (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];
                int f_indexA = (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];
                g_sampleIndex[0] = f_indexA > 255 ? 255 : f_indexA;


                g_lfoFltOut[0] = (float)g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]] / 1023.0f;
                g_lfoIntOut  [0] =        g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1] =   currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1] = g_nextCircleBuffer[1] - g_lastCircleBuffer[1]; // Total length of the current cycle

            //    g_sampleIndex[1] = (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];  // was 256
                int f_indexB = (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1] = f_indexB > 255 ? 255 : f_indexB;

                g_lfoFltOut[1] = (float)g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]] / 1023.0f; // was 0123.0f
                g_lfoIntOut  [1] =        g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]];
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

bool            CKernel::util_check_for_update      ()
{
                CString line;    

                // Read physical pin state - 0 means pressed, 1 means released
                unsigned pinStateA = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();

                // If button A is pressed (pinStateA == 0)  
                if (pinStateA == 0)  
                {
                    unsigned long startTime = m_Timer.GetTicks();
                    unsigned long duration = 5000000; // 5 seconds window for confirmation

                    // Wait for button B press for confirmation
                    while (m_Timer.GetTicks() - startTime < duration)
                    {
                        unsigned pinStateB = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();
                        
            //          unsigned long elapsed = m_Timer.GetTicks() - startTime;
            //          unsigned long remaining = duration - elapsed;

            //          unsigned long secondsRemaining = remaining / 1000000; // Convert microseconds to second

                        m_Screen.Write(CLEAR_SCREEN, strlen(CLEAR_SCREEN));
                        m_Screen.Write( "\n\n  do you want to update? press X\n"
                                        "  reset in 5 Seconds\n\n"
                                        "  prepare the USB Stick with the new Firmware File\n\n",
                                strlen( "\n\n  do you want to update? press X\n"
                                        "  reset in 5 Seconds\n\n"
                                        "  prepare the USB Stick with the new Firmware File\n\n" ));      
            //          m_Screen.Write(line, strlen(line));
                        m_Timer.MsDelay(100);  // Update display every 100ms
                        
                        if (pinStateB == 0)  // If button B is pressed, start update process
                        {
                            m_Screen.Write("  okay, lets do it\n\n", strlen("okay, lets do it\n\n"));

                            // Load current firmware from SD to buffer 0
                            if (!filesystem_load_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 0))
                            {
                                m_Screen.Write("  problem with the old firmware file / sd card - please try again\n", 
                                        strlen("  problem with the old firmware file / sd card - please try again\n"));
                                m_Timer.MsDelay(2000);  
                                return false;                      
                            }

                            line.Format ("  %u Byte loaded from SD\n\n",  loaded_bytes_kernel[0]);
                            m_Screen.Write(line, strlen(line));

                            m_Screen.Write("  insert USB stick\n\n", 
                                    strlen("  insert USB stick\n\n"));

                            // Load new firmware from USB to buffer 1
                            if (!filesystem_load_kernel(PARTITION_NAME_USB, FILENAME_KERNEL, 1))
                            {
                                m_Screen.Write("  problem with the new firmware file / USB stick - please try again\n", 
                                        strlen("  problem with the old firmware file / USB stick - please try again\n"));            
                                m_Timer.MsDelay(2000);
                                return false;                
                            }

                            line.Format ("  %u Byte loaded from USB\n\n",  loaded_bytes_kernel[1]);
                            m_Screen.Write(line, strlen(line));

                            // Write new firmware from buffer 1 to SD
                            if (!filesystem_save_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 1))
                                {
                                m_Screen.Write("  Error ! Restoring the firmware\n", 
                                        strlen("  Error ! Restoring the firmware\n"));             
                                if (!filesystem_save_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 0))
                                    {
                                    m_Screen.Write("  Writing / Restoring the Firmware Failed - you need to restore it manually !!!\n", 
                                            strlen("  Writing / Restoring the Firmware Failed - you need to restore it manually !!!\n"));             
                                    m_Timer.MsDelay(2000);
                                    return false;    
                                    }
                                }
                            m_Screen.Write("  update successful - system will reboot now\n", 
                                    strlen("  update successful - system will reboot now\n"));      
                            m_Timer.MsDelay(2000);
                            return true;   
                        }
                    }
                    return false; 
                } 
                return false;      
}

void            CKernel::util_save_modes_file       ()    // whats up here???
{
                CString temp_string;

                const char* table_attenuator[3]= { "2.5", "3.3", "5.0" };
                const char* table_waveform[4] = { "Sin","Saw","Tr1","Tr2" };
                const char* table_intensity[5] = { "12.5%", "25%", "50%", "75%", "100%" };
                const char* table_menu[NUMBER_OF_MODES]={ "ADC","TRG","BPM","LF1","LF2","AUD"};
                const char* file_header =   "# --------------------------------------------------------------------------------\n"
                                            "# Mode Config File\n"
                                            "# --------------------------------------------------------------------------------\n"
                                            "# How to Edit the Config File:\n"
                                            "# Add/Remove '#' or whole lines\n"
                                            "# Valid Entries for CV XY are:\n"
                                            "#  ADC - Direct CV-In\n"
                                            "#  TRG - Trigger Synced Randomizer\n"
                                            "#  BPM - Beat Synced Randomizer\n"
                                            "#  LF1 - LFO 1\n"
                                            "#  LF2 - LFO 2\n"
                                            "#  AUD - 4 Band Audio Energy Analyzer\n"
                                            "#  ( is not availeble for CV BX! )\n\n"
                                            "# Valid Entries for LFO 1 & 2:\n"
                                            "#  Sin / Saw / TR1 / TR2 ( rising/falling Triangle )\n"
                                            "# Valid Entries for Divider 1 & 2:\n"
                                            "#  1 / 2 / 4 / 8 / 16 / 32 / 64\n"
                                            "# Valid Entries Frame/Texture Mode:\n"
                                            "#  ON / OFF\n\n";
                g_modes.Append(file_header);

                temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                    "#  Global Settings\n" 
                                    "# --------------------------------------------------------------------------------\n"
                                    "  BPM:                 %-3u\n"
                                    "  CLK:                 %-3u\n\n"
                                    "  CV-Attenuation:      %s Volt\n"
                                    "  Audio-Sensisitivity: %s\n",
                                    g_resultBPM[0], 
                                    g_resultBPM[1],
                                    table_attenuator[g_attenuation],
                                    table_intensity[g_sensitivityNew]);

                g_modes.Append(temp_string);

                for(int i = 0; i < g_linked_programs_counter; i++) 
                    {
                    temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                        "#  p_fileName: %s\n"
                                        "# --------------------------------------------------------------------------------\n", g_fshScannedFileNames[i+1]);
                    g_modes.Append(temp_string);
                    g_modes.Append("\n");

                    temp_string.Format( "  CV A0: %s\n"
                                        "  CV A1: %s\n"
                                        "  CV A2: %s\n"
                                        "  CV A3: %s\n\n",table_menu[g_centralModeBuffer[CH0_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH1_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH2_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH3_MODE][i]]);
                    g_modes.Append(temp_string);

                    temp_string.Format( "  CV B0: %s\n"
                                        "  CV B1: %s\n"
                                        "  CV B2: %s\n"
                                        "  CV B3: %s\n\n",table_menu[g_centralModeBuffer[CH4_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH5_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH6_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH7_MODE][i]]);
                    g_modes.Append(temp_string);       

                    temp_string.Format( "  Wave    LFO 1: %s\n"
                                        "  Divider LFO 1: %d\n\n"
                                        "  Wave    LFO 2: %s\n"
                                        "  Divider LFO 2: %d\n\n",table_waveform[g_centralModeBuffer[LF1_WAVE][i]],
                                                                    g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][i]],
                                                                table_waveform[g_centralModeBuffer[LF2_WAVE][i]],
                                                                    g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][i]]);
                    g_modes.Append(temp_string);  
                    
                    temp_string.Format("  Frame   Mode: %s\n", (g_centralModeBuffer[FRM_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);
            
                    temp_string.Format("  Texture Mode: %s\n", (g_centralModeBuffer[TEX_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);

                    g_modes.Append("\n");
                    }
}

void CKernel::GenerateH264ParserInfo( int p_fileIndex)
{
        CString bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
        filesystem_save_log_file( "emmc1-1", g_vidLogNames[p_fileIndex], bufferParser);   
}
void CKernel::GenerateBmpParserInfo( int p_fileIndex)
{
        CString bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
        filesystem_save_log_file( "emmc1-1", g_texLogNames[p_fileIndex], bufferParser);   
}

void            CKernel::parser_h264               (int p_fromFile, int p_toFile)
{
    for (int i = p_fromFile; i < p_toFile; i++) 
        {
                m_H264Parser.ParseVideoAuto(i, m_bufferVideo, g_vidLoadedBytes );
                GenerateH264ParserInfo  (i);
        }
}

void            CKernel::parser_bmp               (int p_fromFile, int p_toFile)
{
    for (int i = p_fromFile; i < p_toFile; i++) 
        {
                m_H264Parser.ParseBPM(i, g_texScannedFileNames, m_bufferTexture, g_texLoadedBytes );
                GenerateBmpParserInfo  (i);
        }
}

void            CKernel::parser_overlay_bmp               (int index)
{        
                m_H264Parser.ParseBPM(0 , g_omtScannedFileNames  /*"Overlay Atlas"*/ , m_BufferOverlayTexture, g_omtLoadedBytes );

                // okay, as far as i see i must store the parser results locally because my later code will start at the index 0, too
                // i might not need a m_H264Parser.reset() 

                bool m_overlay_texture_valid    = m_H264Parser.m_tex_valid[0];
                u32  m_overlay_texture_size     = m_H264Parser.m_tex_file_size[0];
                u32  m_overlay_data_offset      = m_H264Parser.m_tex_data_offset[0];
                u16  m_overlay_width            = m_H264Parser.m_tex_width[0];
                u16  m_overlay_height           = m_H264Parser.m_tex_height[0];
                u32  m_overlay_image_size       = m_H264Parser.m_tex_image_size[0];

                GenerateBmpParserInfo  (0);
        
}
