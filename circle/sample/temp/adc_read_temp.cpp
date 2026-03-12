// readADC()
// Purpose:
// - Basic ADC sampling path for all channels.
//
// How it works:
// - Reads one raw sample per channel from the MCP300X ADC.
// - Stores each sample in a 4-slot ring buffer (per channel).
// - Averages the 4 most recent samples to reduce jitter.
// - Applies attenuation scaling and writes both int + float outputs.
//
// What it is doing:
// - This is the simplest/most direct conversion path (no audio-mode logic).

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

// io_read_ADC() - explicit per-channel audio envelope path
// Purpose:
// - Read all 8 ADC channels, generate stable control values, and generate audio envelopes for channels 0..3.
// How it works:
// - Stores each new ADC sample into a 4-slot ring buffer per channel.
// - Computes `raw` as `(s0 + s1 + s2 + s3) >> 2` for every channel.
// - Computes `val` by applying attenuation scaling, then converts it to float for `g_inOutMatrixFlt[ch][val]`.
// - Computes `erraticness[ch] = s0 - s1 + s2 - s3` for channels 0..3 and checks against +/-AUDIO_THRESHOLD.
// - On threshold hits, updates one envelope accumulator per channel (`au0`, `au1`, `au2`, `au3`) using rolling sums.
// What it is doing in practice:
// - Outputs smoothed control values for channels 0..7 and transient-driven envelope values for channels 0..3.

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

                // Channel 0 - First of pair for audio_sample[0]
                f_ring_buffer[0][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(0);
            //  if(f_ring_buffer[0][f_index_ring_buffer] > 1023) f_ring_buffer[0][f_index_ring_buffer] = 1023;

                erraticness[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(erraticness[0] > AUDIO_THRESHOLD || erraticness[0] < -AUDIO_THRESHOLD)
                    {
                    is_audio[0] = 0;
                    float s = f_ring_buffer[0][f_index_ring_buffer] / 1024.0f;

                    sum0 -= f_band0[idx0];
                    f_band0[idx0] = s;
                    sum0 += s;
                    g_inOutMatrixFlt[0][au0] = sum0 / w0;

                    ++idx0;
                    if(idx0 == w0) idx0 = 0;
                    }

                g_inOutMatrixInt[0][raw] =  (f_ring_buffer[0][0] + f_ring_buffer[0][1] + f_ring_buffer[0][2] + f_ring_buffer[0][3]) >>2 ; 

                g_inOutMatrixInt[0][val] = (g_inOutMatrixInt[0][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[0][val] = (g_inOutMatrixInt[0][val]) / (1024.0f);

                // Channel 1 - First of pair for audio_sample[1]
                f_ring_buffer[1][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(1);

                erraticness[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(erraticness[1] > AUDIO_THRESHOLD || erraticness[1] < -AUDIO_THRESHOLD)
                    {
                    is_audio[1] = 1;
                    float s = f_ring_buffer[1][f_index_ring_buffer] / 1024.0f;

                    sum1 -= f_band1[idx1];
                    f_band1[idx1] = s;
                    sum1 += s;
                    g_inOutMatrixFlt[0][au1] = sum1 / w1;

                    ++idx1;
                    if(idx1 == w1) idx1 = 0;
                    }

                g_inOutMatrixInt[1][raw] =  (f_ring_buffer[1][0] + f_ring_buffer[1][1] + f_ring_buffer[1][2] + f_ring_buffer[1][3]) >>2 ; 

                g_inOutMatrixInt[1][val] = (g_inOutMatrixInt[1][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[1][val] = (g_inOutMatrixInt[1][val]) / (1024.0f);

                // Channel 2 - Second of pair for audio_sample[0]
                f_ring_buffer[2][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(2);

                erraticness[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(erraticness[2] > AUDIO_THRESHOLD || erraticness[2] < -AUDIO_THRESHOLD)
                    {
                    is_audio[0] = 2;
                    float s = f_ring_buffer[2][f_index_ring_buffer] / 1024.0f;

                    sum2 -= f_band2[idx2];
                    f_band2[idx2] = s;
                    sum2 += s;
                    g_inOutMatrixFlt[0][au2] = sum2 / w2;

                    ++idx2;
                    if(idx2 == w2) idx2 = 0;
                    }

                g_inOutMatrixInt[2][raw] =  (f_ring_buffer[2][0] + f_ring_buffer[2][1] + f_ring_buffer[2][2] + f_ring_buffer[2][3]) >>2 ; 

                g_inOutMatrixInt[2][val] = (g_inOutMatrixInt[2][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[2][val] = (g_inOutMatrixInt[2][val]) / (1024.0f);

                // Channel 3 - Second of pair for audio_sample[1]
                f_ring_buffer[3][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(3);

                erraticness[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(erraticness[3] > AUDIO_THRESHOLD || erraticness[3] < -AUDIO_THRESHOLD)
                    {
                    is_audio[1] = 3;
                    float s = f_ring_buffer[3][f_index_ring_buffer] / 1024.0f;

                    sum3 -= f_band3[idx3];
                    f_band3[idx3] = s;
                    sum3 += s;
                    g_inOutMatrixFlt[0][au3] = sum3 / w3;

                    ++idx3;
                    if(idx3 == w3) idx3 = 0;
                    }

                g_inOutMatrixInt[3][raw] = (f_ring_buffer[3][0] + f_ring_buffer[3][1] + f_ring_buffer[3][2] + f_ring_buffer[3][3]) >>2 ; 

                g_inOutMatrixInt[3][val] = (g_inOutMatrixInt[3][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[3][val] = (g_inOutMatrixInt[3][val]) / (1024.0f);

                // Channel 4 (no audio detection)
                f_ring_buffer[4][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(4);

                g_inOutMatrixInt[4][raw] =  (f_ring_buffer[4][0] + f_ring_buffer[4][1] + f_ring_buffer[4][2] + f_ring_buffer[4][3]) >>2 ; 

                g_inOutMatrixInt[4][val] = (g_inOutMatrixInt[4][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[4][val] = (g_inOutMatrixInt[4][val]) / (1024.0f);

                // Channel 5 (no audio detection)
                f_ring_buffer[5][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(5);

                g_inOutMatrixInt[5][raw] =  (f_ring_buffer[5][0] + f_ring_buffer[5][1] + f_ring_buffer[5][2] + f_ring_buffer[5][3]) >>2 ; 

                g_inOutMatrixInt[5][val] = (g_inOutMatrixInt[5][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[5][val] = (g_inOutMatrixInt[5][val]) / (1024.0f);

                // Channel 6 (no audio detection)
                f_ring_buffer[6][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(6);

                g_inOutMatrixInt[6][raw] = (f_ring_buffer[6][0] + f_ring_buffer[6][1] + f_ring_buffer[6][2] + f_ring_buffer[6][3]) >>2 ; 

                g_inOutMatrixInt[6][val] = (g_inOutMatrixInt[6][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[6][val] = (g_inOutMatrixInt[6][val]) / (1024.0f);

                // Channel 7 (no audio detection)
                f_ring_buffer[7][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(7);

                g_inOutMatrixInt[7][raw] = (f_ring_buffer[7][0] + f_ring_buffer[7][1] + f_ring_buffer[7][2] + f_ring_buffer[7][3]) >>2 ; 

                g_inOutMatrixInt[7][val] = (g_inOutMatrixInt[7][raw] * scaleFactors[attenuation]) / 1023;                                
                
                g_inOutMatrixFlt[7][val] = (g_inOutMatrixInt[7][val]) / (1024.0f);
                
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;  // Update buffer index
}

// io_read_ADC() - explicit low-cost arithmetic path
// Purpose:
// - Read all 8 ADC channels, generate control values, and generate four transient-driven envelope outputs.
// How it works:
// - Stores each new ADC sample into a 4-slot ring buffer per channel and computes `raw` from those 4 samples.
// - Uses `erraticness[ch] = s0 - s1 + s2 - s3` for channels 0..3 to detect fast signal changes.
// - Uses `f_scale = scaleFactors[attenuation]`, then computes `val` with `(raw * f_scale) >> 10`.
// - Converts integer units to float with `* 0.0009765625f`.
// - Updates envelopes as paired groups: channel 0/2 activity writes `au0` and `au1`; channel 1/3 activity writes `au2` and `au3`.
// What it is doing in practice:
// - Publishes `raw`, `val`, and `au0..au3` on each call with fixed formulas and bounded ring-buffer state.

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

                // Channel 0 - First of pair for audio_sample[0]
                f_ring_buffer[0][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(0);

                erraticness[0]  =   f_ring_buffer[0][i0] - f_ring_buffer[0][i1] + f_ring_buffer[0][i2] - f_ring_buffer[0][i3];

                if(erraticness[0] > AUDIO_THRESHOLD || erraticness[0] < -AUDIO_THRESHOLD)
                    {
                    is_audio[0] = 0;
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
                
                g_inOutMatrixFlt[0][val] = (g_inOutMatrixInt[0][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 1 - First of pair for audio_sample[1]
                f_ring_buffer[1][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(1);

                erraticness[1] =    f_ring_buffer[1][i0] - f_ring_buffer[1][i1] + f_ring_buffer[1][i2] - f_ring_buffer[1][i3];

                if(erraticness[1] > AUDIO_THRESHOLD || erraticness[1] < -AUDIO_THRESHOLD)
                    {
                    is_audio[1] = 1;
                    float s = f_ring_buffer[1][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

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

                g_inOutMatrixInt[1][val] = (g_inOutMatrixInt[1][raw] * f_scale) >> 10; //  -> / 1023;                                
                
                g_inOutMatrixFlt[1][val] = (g_inOutMatrixInt[1][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 2 - Second of pair for audio_sample[0]
                f_ring_buffer[2][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(2);

                erraticness[2] =    f_ring_buffer[2][i0] - f_ring_buffer[2][i1] + f_ring_buffer[2][i2] - f_ring_buffer[2][i3];
                
                if(erraticness[2] > AUDIO_THRESHOLD || erraticness[2] < -AUDIO_THRESHOLD)
                    {
                    is_audio[0] = 2;
                    float s = f_ring_buffer[2][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

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

                g_inOutMatrixInt[2][val] = (g_inOutMatrixInt[2][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[2][val] = (g_inOutMatrixInt[2][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 3 - Second of pair for audio_sample[1]
                f_ring_buffer[3][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(3);

                erraticness[3] =    f_ring_buffer[3][i0] - f_ring_buffer[3][i1] + f_ring_buffer[3][i2] - f_ring_buffer[3][i3];

                if(erraticness[3] > AUDIO_THRESHOLD || erraticness[3] < -AUDIO_THRESHOLD)
                    {
                    is_audio[1] = 3;
                    float s = f_ring_buffer[3][f_index_ring_buffer] * 0.0009765625f; // -> / 1024.0f;

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

                g_inOutMatrixInt[3][val] = (g_inOutMatrixInt[3][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[3][val] = (g_inOutMatrixInt[3][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 4 (no audio detection)
                f_ring_buffer[4][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(4);

                g_inOutMatrixInt[4][raw] =  (f_ring_buffer[4][0] + f_ring_buffer[4][1] + f_ring_buffer[4][2] + f_ring_buffer[4][3]) >>2 ; 

                g_inOutMatrixInt[4][val] = (g_inOutMatrixInt[4][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[4][val] = (g_inOutMatrixInt[4][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 5 (no audio detection)
                f_ring_buffer[5][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(5);

                g_inOutMatrixInt[5][raw] =  (f_ring_buffer[5][0] + f_ring_buffer[5][1] + f_ring_buffer[5][2] + f_ring_buffer[5][3]) >>2 ; 

                g_inOutMatrixInt[5][val] = (g_inOutMatrixInt[5][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[5][val] = (g_inOutMatrixInt[5][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 6 (no audio detection)
                f_ring_buffer[6][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(6);

                g_inOutMatrixInt[6][raw] = (f_ring_buffer[6][0] + f_ring_buffer[6][1] + f_ring_buffer[6][2] + f_ring_buffer[6][3]) >>2 ; 

                g_inOutMatrixInt[6][val] = (g_inOutMatrixInt[6][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[6][val] = (g_inOutMatrixInt[6][val]) * 0.0009765625f; // -> / 1024.0f;

                // Channel 7 (no audio detection)
                f_ring_buffer[7][f_index_ring_buffer] = m_MCP300X.DoSingleEndedConversionRaw(7);

                g_inOutMatrixInt[7][raw] = (f_ring_buffer[7][0] + f_ring_buffer[7][1] + f_ring_buffer[7][2] + f_ring_buffer[7][3]) >>2 ; 

                g_inOutMatrixInt[7][val] = (g_inOutMatrixInt[7][raw] * f_scale) >> 10; //  -> / 1023;                   
                
                g_inOutMatrixFlt[7][val] = (g_inOutMatrixInt[7][val]) * 0.0009765625f; // -> / 1024.0f;
                
                f_index_ring_buffer = (f_index_ring_buffer + 1) & 3;  // Update buffer index
}