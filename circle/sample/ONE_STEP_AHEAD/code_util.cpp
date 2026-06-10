
#include "kernel.h"

    #define MY_BUFFER   m_bufferLog                 // not used here!
    #define MY_INDEX    m_bufferLogIndex

bool            CKernel::checkUpdate                (   )
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;
                    }
                else
                    {
                    return false;    
                    }
}

void        CKernel::get_gl_time( unsigned sys_time )
{
                    switch (g_centralModeBuffer[/* current_buffer */ g_gl_program_current][FLAG_TIME]) // not sure with g_gl_program_current!!!
                        {
                        case true:  GLtime = g_inOutMatrixInt[ADC_INPUT_TIME][RAW]/36.0f; break;
                        case false: GLtime = sys_time / 1000000.0f;                       break;
                        }    
                    }

void            CKernel::randomVec8                 (   uint32_t            p_seed )
{
                const int       f_max_int   = 1023; // 1024;
                const float     f_scale     = 1.0f / 4294967295.0f;

                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[0][RND] = p_seed * f_scale;
                g_inOutMatrixInt[0][RND] = ( g_inOutMatrixFlt[0][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[1][RND] = p_seed * f_scale;
                g_inOutMatrixInt[1][RND] = ( g_inOutMatrixFlt[1][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[2][RND] = p_seed * f_scale;
                g_inOutMatrixInt[2][RND] = ( g_inOutMatrixFlt[2][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[3][RND] = p_seed * f_scale;
                g_inOutMatrixInt[3][RND] = ( g_inOutMatrixFlt[3][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[4][RND] = p_seed * f_scale;
                g_inOutMatrixInt[4][RND] = ( g_inOutMatrixFlt[4][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[5][RND] = p_seed * f_scale;
                g_inOutMatrixInt[5][RND] = ( g_inOutMatrixFlt[5][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[6][RND] = p_seed * f_scale;
                g_inOutMatrixInt[6][RND] = ( g_inOutMatrixFlt[6][RND] * f_max_int);
                p_seed ^= p_seed << 13; p_seed ^= p_seed >> 17; p_seed ^= p_seed << 5;
                g_inOutMatrixFlt[7][RND] = p_seed * f_scale;
                g_inOutMatrixInt[7][RND] = ( g_inOutMatrixFlt[7][RND] * f_max_int);
}

void            CKernel::calculate1BPM              (   int             p_source, 
                                                        unsigned long   p_triggerTimeClock)  // p_triggerTimeClock ****
{
                unsigned long f_intervalAverage = 0;

                if (p_triggerTimeClock != g_lfoBpmMatrix[p_source][LTIME])
                    {
                    g_lfoBpmMatrix[g_lfoBpmMatrix[p_source][TIDX]][TB] = p_triggerTimeClock;
        
                    g_lfoBpmMatrix[0][DB]                 =   g_lfoBpmMatrix[1][TB] - g_lfoBpmMatrix[0][TB];   
                    g_lfoBpmMatrix[1][DB]                 =   g_lfoBpmMatrix[2][TB] - g_lfoBpmMatrix[1][TB];
                    g_lfoBpmMatrix[2][DB]                 =   g_lfoBpmMatrix[3][TB] - g_lfoBpmMatrix[2][TB];

                    if( g_lfoBpmMatrix[1][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  
                        g_lfoBpmMatrix[2][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  
                        g_lfoBpmMatrix[0][DB]  <   g_lfoBpmMatrix[2][DB] * 1.25f )
                        {
                        f_intervalAverage                 = (   g_lfoBpmMatrix[0][DB] + 
                                                                g_lfoBpmMatrix[1][DB] + 
                                                                g_lfoBpmMatrix[2][DB]) / 3;
            
                        g_lfoBpmMatrix[p_source][BPM]     =   60000000 / f_intervalAverage;
            
                        g_lfoBpmMatrix[p_source][INTV]    =   f_intervalAverage;
                        g_lfoBpmMatrix[p_source][LBC]     =   m_Timer.GetClockTicks();
                        }
                    g_lfoBpmMatrix[p_source][LTIME]       =   p_triggerTimeClock;

                    g_lfoBpmMatrix[p_source][TIDX]        = ( g_lfoBpmMatrix[p_source][TIDX] + 1) % 4;    
                    }
}

void            CKernel::predict1Beat               (   int             p_source, 
                                                        int             p_lfoMultIn )
{
                unsigned long currentTime               =   m_Timer.GetClockTicks();

                if (currentTime >= g_lfoBpmMatrix[p_source][NBT])
                    {
                    g_lfoBpmMatrix[p_source][NBT]      +=  g_lfoBpmMatrix[p_source][INTV];
                    }
                if (currentTime >= g_lfoBpmMatrix[p_source][NCB]) 
                    {
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[p_source][NCB];
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[p_source][NCB] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]); // why again g_lfoMultiplierTMP? isnt it stored already, do we need to back it up?
                    g_lfoBpmMatrix[p_source][LMT]        =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
                if ((g_lfoBpmMatrix[p_source][LBCT]    !=  g_lfoBpmMatrix[p_source][LBC]))
                    {
                    g_lfoBpmMatrix[p_source][NBT]       =   g_lfoBpmMatrix[p_source][LBC];
                    g_lfoBpmMatrix[p_source][LBCT]      =   g_lfoBpmMatrix[p_source][LBC];
                    }
                if (g_lfoBpmMatrix[p_source][LMT]      !=  g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]])
                    {
                    g_lfoBpmMatrix[p_source][LCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC];
                    g_lfoBpmMatrix[p_source][NCB]       =   g_lfoBpmMatrix[g_activeBpmChannel][LBC] + (g_lfoBpmMatrix[g_activeBpmChannel][INTV] * g_lfoBpmMatrix[p_source][LMT]);
                    g_lfoBpmMatrix[p_source][LMT]       =   g_lfoMultiplier[g_centralModeBuffer[g_currentProgramBuffer][p_lfoMultIn]];
                    }
}

void            CKernel::sample1WaveTable           (   int             p_source, 
                                                        int             p_lfoIn, 
                                                        int             p_lfoOut )
{
                unsigned long currentTime               =   m_Timer.GetClockTicks();    // ? ****

                g_lfoBpmMatrix[p_source][ELP]           =   currentTime - g_lfoBpmMatrix[p_source][LCB];
                g_lfoBpmMatrix[p_source][CYL]           =   g_lfoBpmMatrix[p_source][NCB] - g_lfoBpmMatrix[p_source][LCB];
                int f_indexA                            =  (g_lfoBpmMatrix[p_source][ELP] * 255) / g_lfoBpmMatrix[p_source][CYL];
                g_lfoBpmMatrix[p_source][SMP]           =   f_indexA > 255 ? 255 : f_indexA;                                                                  // means i need a wraparound - on the other hand: i should have a clear calculation here that will never create a index >255! ****
                g_inOutMatrixFlt[0][p_lfoOut]           =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]] / 1023.0f;
                g_inOutMatrixInt[0][p_lfoOut]           =   g_waveTable[g_centralModeBuffer[g_currentProgramBuffer][p_lfoIn]][g_lfoBpmMatrix[p_source][SMP]];
}