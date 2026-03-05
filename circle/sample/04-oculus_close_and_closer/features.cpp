void            CKernel::randomVec8           (uint32_t p_seed)                                               // create 8 unique normalised to 1.0 float and to 1024 int values
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;
                uint32_t        f_x         = p_seed;

                f_x ^= f_x << 13; f_x ^= f_x >> 17; f_x ^= f_x << 5;
                g_inOutMatrixFlt[0][rnd] = / f_x * f_scale;                                               // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][rnd] = ( g_inOutMatrixFlt[0][rnd] * f_max_int);  // the cast is, i assume in this place pure cosmetics
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


/*
g_resultBPM g_nextBeatTime  g_intervalCalculated    g_elapsedMicroseconds   g_sampleIndex   g_cycleLength   g_lfoMultiplier g_lfoMultiplierTMP   g_lastBpmCalculation   g_lastBpmCalculationTMP g_lastCircleBuffer  g_nextCircleBuffer      

i love to have a similar array i i have for the in/output matrix
*/
void            CKernel::calculateBPM         (   unsigned long   p_triggerTimeClockA, 
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
        
                    f_intervalBuffer[0][0]          =   f_timeBuffer[0][1] - f_timeBuffer[0][0];   // no need to make this global, global right i mean the content is consumed in sito or do i need a static local array?
                    f_intervalBuffer[0][1]          =   f_timeBuffer[0][2] - f_timeBuffer[0][1];
                    f_intervalBuffer[0][2]          =   f_timeBuffer[0][3] - f_timeBuffer[0][2];
                    if(     f_intervalBuffer[0][1]  <   f_intervalBuffer[0][0] * 1.25f 
                        &&  f_intervalBuffer[0][2]  <   f_intervalBuffer[0][0] * 1.25f
                        &&  f_intervalBuffer[0][0]  <   f_intervalBuffer[0][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_intervalBuffer[0][0] + 
                                                        f_intervalBuffer[0][1] + 
                                                        f_intervalBuffer[0][2]) / 3;
            
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
        
                    f_intervalBuffer[1][0]          =   f_timeBuffer[1][1] - f_timeBuffer[1][0];       // takes the intervals
                    f_intervalBuffer[1][1]          =   f_timeBuffer[1][2] - f_timeBuffer[1][1];
                    f_intervalBuffer[1][2]          =   f_timeBuffer[1][3] - f_timeBuffer[1][2];
                    if(     f_intervalBuffer[1][1]  <   f_intervalBuffer[1][0] * 1.25f         // calculates an average and allows 25% play ( quite high right ) 
                        &&  f_intervalBuffer[1][2]  <   f_intervalBuffer[1][0] * 1.25f
                        &&  f_intervalBuffer[1][0]  <   f_intervalBuffer[1][2] * 1.25f ) 
                        {
                        f_intervalAverage           = ( f_intervalBuffer[1][0] + 
                                                        f_intervalBuffer[1][1] + 
                                                        f_intervalBuffer[1][2]) / 3;
            
                        g_resultBPM[1]              =   60000000 / f_intervalAverage;
            
                        g_intervalCalculated[1]     =   f_intervalAverage;
                        g_lastBpmCalculation[1]     =   m_Timer.GetClockTicks();
                        }
                    f_lastTime[1]                   =   p_triggerTimeClockB;

                    f_timeIndex[1]                  = ( f_timeIndex[1] + 1) % 4;    
                    }
}

void            CKernel::predictedNextBeat ()
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                      // Get the current u_time in clock ticks

                g_activeBpmChannel                  = ( g_lastBpmCalculation[0] > g_lastBpmCalculation[1]) ? 0 : 1;                                  // what was the last bpm input? 

                if (currentTime >= g_nextBeatTime[0])                                                                                               // Update for instance 0
                {
                    g_nextBeatTime[0]               +=  g_intervalCalculated[0];                                                                                   // Predict the next beat u_time
                }
                if (currentTime >= g_nextCircleBuffer[0]) 
                    {
                    g_lastCircleBuffer[0]           =   g_nextCircleBuffer[0];
                    g_nextCircleBuffer[0]           =   g_nextCircleBuffer[0] + 
                                                       (g_intervalCalculated[g_activeBpmChannel] * 
                                                        g_lfoMultiplierTMP[0]);                                   // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                if ((g_lastBpmCalculationTMP[0]     !=  g_lastBpmCalculation[0]))                                                                   // Handle BPM changes for instance 0
                    {
                    g_nextBeatTime[0]               =   g_lastBpmCalculation[0];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[0]      =   g_lastBpmCalculation[0];
                    }
                if (g_lfoMultiplierTMP[0]           !=  g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[0]           =   g_lastBpmCalculation[g_activeBpmChannel] + 
                                                       (g_intervalCalculated[g_activeBpmChannel] * 
                                                        g_lfoMultiplierTMP[0]);                                   // g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]]);
                    g_lfoMultiplierTMP[0]           =   g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][g_currentProgramBuffer]];
                    }
                if (currentTime >= g_nextBeatTime[1])                                                                                               // Update for instance 1
                    {
                    g_nextBeatTime[1]               +=  g_intervalCalculated[1];                                                                                   // Predict the next beat u_time
                    }
                if (currentTime >= g_nextCircleBuffer[1]) 
                    {
                    g_lastCircleBuffer[1]           =   g_nextCircleBuffer[1];
                    g_nextCircleBuffer[1]           =   g_nextCircleBuffer[1] + 
                                                       (g_intervalCalculated[g_activeBpmChannel] * 
                                                        g_lfoMultiplierTMP[1] );                                  // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }
                if ((g_lastBpmCalculationTMP[1]     !=  g_lastBpmCalculation[1]))                                                                   // Handle BPM changes for instance 1
                    {
                    g_nextBeatTime[1]               =   g_lastBpmCalculation[1];                                                                      // Reset to current time for new BPM
                    g_lastBpmCalculationTMP[1]      =   g_lastBpmCalculation[1];
                    }
                if (g_lfoMultiplierTMP[1]           !=  g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]])
                    {
                    g_lastCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel];
                    g_nextCircleBuffer[1]           =   g_lastBpmCalculation[g_activeBpmChannel] + 
                                                       (g_intervalCalculated[g_activeBpmChannel] * 
                                                        g_lfoMultiplierTMP[1]);                                   // g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]]);
                    g_lfoMultiplierTMP[1]           =   g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][g_currentProgramBuffer]];
                    }
}

void            CKernel::sampleWaveTable                   ()
{
                unsigned long currentTime           =   m_Timer.GetClockTicks();                                                                          // Get the current u_time in clock ticks why not the start_time_fps_calculation or currentTime from Run()??

                g_elapsedMicroseconds[0]            =   currentTime - g_lastCircleBuffer[0];
                g_cycleLength[0]                    =   g_nextCircleBuffer[0] - g_lastCircleBuffer[0];                                                    // Total length of the current cycle
                int f_indexA                        =  (g_elapsedMicroseconds[0] * 255) / g_cycleLength[0];
                g_sampleIndex[0]                    =   f_indexA > 255 ? 255 : f_indexA;                                                                  // ! i like to get rid of this saveguard !
                g_inOutMatrixFlt[0][lf1]            =   g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][lf1]            =   g_waveTable[g_centralModeBuffer[LF1_WAVE][g_currentProgramBuffer]][g_sampleIndex[0]];

                g_elapsedMicroseconds[1]            =   currentTime - g_lastCircleBuffer[1];
                g_cycleLength[1]                    =   g_nextCircleBuffer[1] - g_lastCircleBuffer[1];                                                   // Total length of the current cycle
                int f_indexB                        =  (g_elapsedMicroseconds[1] * 255) / g_cycleLength[1];
                g_sampleIndex[1]                    =   f_indexB > 255 ? 255 : f_indexB;                                                                  // ! i like to get rid of this saveguard !
                g_inOutMatrixFlt[0][lf2]            =   g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]] / 1023.0f;  // the cast is, i assume in this place pure cosmetics
                g_inOutMatrixInt[0][lf2]            =   g_waveTable[g_centralModeBuffer[LF2_WAVE][g_currentProgramBuffer]][g_sampleIndex[1]];
}   

void            CKernel::audioEnergy          (float p_adcvalue) 
{   
                const int f_maxBuffer = 33;

                const int f_averageBufferSizeTable[4][5] = 
                    {
                    {33, 25, 17,  9,  5},                                                                                                           // Channel 0 (lowest frequency)  
                    {25, 19, 13,  7,  4},                                                                                                           // Channel 1
                    {17, 13,  9,  5,  3},                                                                                                           // Channel 2
                    { 9,  7,  5,  3,  2}                                                                                                            // Channel 3 (highest frequency)
                    };
                    
                static float f_band0[f_maxBuffer] = {0};                                                                                            // Static ring buffers for each frequency band
                static float f_band1[f_maxBuffer] = {0};
                static float f_band2[f_maxBuffer] = {0};
                static float f_band3[f_maxBuffer] = {0};
                static unsigned char f_indexBand0 = 0;                                                                                              // Static indices for ring buffers
                static unsigned char f_indexBand1 = 0;
                static unsigned char f_indexBand2 = 0;
                static unsigned char f_indexBand3 = 0;

                if ( g_sensitivityNew != g_sensitivityOld )
                    {
                    g_inOutMatrixFlt[0][au0] = 0;
                    g_inOutMatrixFlt[0][au1] = 0;
                    g_inOutMatrixFlt[0][au2] = 0;
                    g_inOutMatrixFlt[0][au3] = 0;

                    f_indexBand0 = 0;                                                                                                               // Reset ring buffer indices too
                    f_indexBand1 = 0;
                    f_indexBand2 = 0;
                    f_indexBand3 = 0;

                    g_sensitivityOld = g_sensitivityNew;
                    }   
                f_band0[f_indexBand0] = p_adcvalue;                                                                                                 // Update ring buffers with the new ADC value
                f_band1[f_indexBand1] = p_adcvalue;
                f_band2[f_indexBand2] = p_adcvalue;
                f_band3[f_indexBand3] = p_adcvalue;

                for (unsigned char i = 0; i < f_averageBufferSizeTable[0][g_sensitivityNew]; ++i)                                                   // Averaging the buffer contents
                    {
                    g_inOutMatrixFlt[0][au0] += f_band0[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[1][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au1] += f_band1[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[2][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au2] += f_band2[i];
                    }
                for (unsigned char i = 0; i < f_averageBufferSizeTable[3][g_sensitivityNew]; ++i) 
                    {
                    g_inOutMatrixFlt[0][au3] += f_band3[i];
                    }
                g_inOutMatrixFlt[0][au0] /= f_averageBufferSizeTable[0][g_sensitivityNew];
                g_inOutMatrixFlt[0][au1] /= f_averageBufferSizeTable[1][g_sensitivityNew];
                g_inOutMatrixFlt[0][au2] /= f_averageBufferSizeTable[2][g_sensitivityNew];
                g_inOutMatrixFlt[0][au3] /= f_averageBufferSizeTable[3][g_sensitivityNew];

                f_indexBand0 = (f_indexBand0 + 1) % f_averageBufferSizeTable[0][g_sensitivityNew];                                                  // Update indices
                f_indexBand1 = (f_indexBand1 + 1) % f_averageBufferSizeTable[1][g_sensitivityNew];
                f_indexBand2 = (f_indexBand2 + 1) % f_averageBufferSizeTable[2][g_sensitivityNew];
                f_indexBand3 = (f_indexBand3 + 1) % f_averageBufferSizeTable[3][g_sensitivityNew];
}

/*
    we need a combined array for the bpm/lfo timing stuff here, same as in circle/sample/04-oculus_close_and_closer/menu_final.cpp
*/