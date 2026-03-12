// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::io_read_ADC() 
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

                int w0 = f_averageBufferSizeTable[0][g_sensitivityNew];
                int w1 = f_averageBufferSizeTable[1][g_sensitivityNew];
                int w2 = f_averageBufferSizeTable[2][g_sensitivityNew];
                int w3 = f_averageBufferSizeTable[3][g_sensitivityNew];

                static int f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                static int f_index_ring_buffer;

                int i0 =  f_index_ring_buffer;
                int i1 = (f_index_ring_buffer - 1) & 3;
                int i2 = (f_index_ring_buffer - 2) & 3;
                int i3 = (f_index_ring_buffer - 3) & 3;

                const int f_scale = scaleFactors[attenuation];

                    g_modeMap[0][0] =
                    g_modeMap[1][0] =
                    g_modeMap[2][0] =
                    g_modeMap[3][0] = 5;

                f_ring_buffer[0][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(0);    // Channel 0 - First of pair for audio_sample[0]

                erraticness[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(erraticness[0] > AUDIO_THRESHOLD || erraticness[0] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[0] = 0;

                    audio_hold_A = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
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

                erraticness[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(erraticness[1] > AUDIO_THRESHOLD || erraticness[1] < -AUDIO_THRESHOLD && m_audio_mode_activated )
                    {
                    is_audio[1] = 1;

                    audio_hold_B = AUDIO_MENU_HOLD;

                    uint8_t v = 5 + ((audio_hold_A > 0) + (audio_hold_B > 0)) * 2;

                        g_modeMap[0][0] =
                        g_modeMap[1][0] =
                        g_modeMap[2][0] =
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

                erraticness[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(erraticness[2] > AUDIO_THRESHOLD || erraticness[2] < -AUDIO_THRESHOLD && m_audio_mode_activated )
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

                erraticness[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(erraticness[3] > AUDIO_THRESHOLD || erraticness[3] < -AUDIO_THRESHOLD && m_audio_mode_activated )
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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
