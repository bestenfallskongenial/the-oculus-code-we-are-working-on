// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

void            CKernel::calculateBPM   (   unsigned long   p_triggerTimeClockA, 
                                            unsigned long   p_triggerTimeClockB) 
{
                static unsigned long f_lastTime[2];
                static unsigned long f_timeBuffer[2][4] = {{0}};  
                static unsigned long f_intervalBuffer[2][3]= { 0 };

                unsigned long f_intervalAverage = 0;
                static int f_timeIndex[2] = {0};

                if (p_triggerTimeClockA != f_lastTime[0])                                                          // Process button u_time (instance 0)
                    {
                    f_timeBuffer[0][f_timeIndex[0]] = p_triggerTimeClockA;
        
                    f_intervalBuffer[0][0]          =   f_timeBuffer[0][1] - f_timeBuffer[0][0];   
                    f_intervalBuffer[0][1]          =   f_timeBuffer[0][2] - f_timeBuffer[0][1];
                    f_intervalBuffer[0][2]          =   f_timeBuffer[0][3] - f_timeBuffer[0][2];
                    if(     f_intervalBuffer[0][1]  <   f_intervalBuffer[0][0] * 1.25f 
                        &&  f_intervalBuffer[0][2]  <   f_intervalBuffer[0][0] * 1.25f
                        &&  f_intervalBuffer[0][0]  <   f_intervalBuffer[0][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_intervalBuffer[0][0] + f_intervalBuffer[0][1] + f_intervalBuffer[0][2]) / 3;
            
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
        
                    f_intervalBuffer[1][0]          =   f_timeBuffer[1][1] - f_timeBuffer[1][0];
                    f_intervalBuffer[1][1]          =   f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    f_intervalBuffer[1][2]          =   f_timeBuffer[1][3] - f_timeBuffer[1][2];
                    if(     f_intervalBuffer[1][1]  <   f_intervalBuffer[1][0] * 1.25f                              // calculates an average and allows 25% play ( quite high right ) 
                        &&  f_intervalBuffer[1][2]  <   f_intervalBuffer[1][0] * 1.25f
                        &&  f_intervalBuffer[1][0]  <   f_intervalBuffer[1][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_intervalBuffer[1][0] + f_intervalBuffer[1][1] + f_intervalBuffer[1][2]) / 3;
            
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
void            CKernel::predictedNextBeat ()
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
