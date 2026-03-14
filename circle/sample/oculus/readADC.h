

                const float maxA = 1023.0;
                const int scaleFactors[3] = {   2047,       // 2.5V max (1023 * 2)
                                                1551,       // 3.3V max (1023 * 1.515555...)
                                                1023    };  // 5.0V max       

                #define AUDIO_THRESHOLD = 160;  // Threshold for audio detection

                #define f_maxBuffer = 33;

                float f_band0[f_maxBuffer] = {0};
                float f_band1[f_maxBuffer] = {0};
                float f_band2[f_maxBuffer] = {0};
                float f_band3[f_maxBuffer] = {0};

                uint32_t audio_hold_A = 0;
                uint32_t audio_hold_B = 0;

                int f_ring_buffer[ADC_CHANNELS][ADC_BUFFER] = { 0 };
                int f_index_ring_buffer;