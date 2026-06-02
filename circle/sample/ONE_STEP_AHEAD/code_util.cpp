
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
                    switch (g_centralModeBuffer[/* current_buffer */ g_current_gl_program][FRM_MODE]) // not sure with g_current_gl_program!!!
                        {
                        case true:  GLtime = g_inOutMatrixInt[ADC_INPUT_TIME][RAW]/36.0f; break;
                        case false: GLtime = sys_time / 1000000.0f;                       break;
                        }    
                    }


void            CKernel::prepParameters             (   )               // f_buffer guess here we need much more to do!
{
                for ( int f_buffer=0; f_buffer <= DEFAULT_SLOT; f_buffer++)
                    {
                    g_centralModeBuffer[f_buffer][LF1_WAVE] = 0;
                    g_centralModeBuffer[f_buffer][LF2_WAVE] = 1;
                    g_centralModeBuffer[f_buffer][LF1_MULT] = 3;
                    g_centralModeBuffer[f_buffer][LF2_MULT] = 3;
                    g_centralModeBuffer[f_buffer][SENS_A] = 15;
                    g_centralModeBuffer[f_buffer][SENS_B] = 47;
                    g_centralModeBuffer[f_buffer][SENS_C] = 15;
                    g_centralModeBuffer[f_buffer][SENS_D] = 47;                    
                    }
}

void            CKernel::chooseIndex                (   int             p_channel, 
                                                        int&            p_activeIndex, 
                                                        int             p_maxCount, 
                                                        bool*           flags)    // non-condensed valid arrays, max number of files ( macros for example!)
{
//              static int p_activeIndex = 0;

                int f_calculated = g_inOutMatrixInt[p_channel][RAW] * p_maxCount >> 10;

                if (flags[f_calculated])
                    {
                    p_activeIndex = f_calculated;
                    }
}

void            CKernel::chooseIndexD               (   int             p_channel, 
                                                        int&            p_activeIndex, 
                                                        int             p_maxCount) // condensed valid arrays, max number of files ( macros for example!)
{
                int f_calculated = (g_inOutMatrixInt[p_channel][RAW] * p_maxCount) >> 10;

                p_activeIndex = f_calculated;
}

void            CKernel::storeModes                 (   )
{
                
                if (g_current_gl_program != g_last_gl_program)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_current_gl_program][IS_STORED] ? g_current_gl_program : DEFAULT_SLOT;
                    g_last_gl_program = g_current_gl_program;
                    }               

                if (g_centralModeBuffer[g_current_gl_program][IS_STORED] == true /* && g_currentProgramBuffer != g_current_gl_program */)
                    {  
                    memcpy(&g_centralModeBuffer[g_current_gl_program][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_current_gl_program])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_current_gl_program;
                    }
                else if (g_centralModeBuffer[g_current_gl_program][IS_STORED] == false /* && g_currentProgramBuffer != DEFAULT_SLOT */)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}




void            CKernel::button_consumer            (   int                 p_btn_id ) // this is where the magic happens: we need to set the states of menu layer, menu, we need to use one button for bpm input and so on 
{
/*
                if (g_buttons_states[p_btn_id][BTN_SINGLE]) counter += 1;
                if (g_buttons_states[p_btn_id][BTN_DOUBLE]) counter -= 1;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] == 1)
                    counter += 5;

                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =10)
                    longhold += 1;
                if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] =20)
                    longhold += 2;
*/
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

                    if(     g_lfoBpmMatrix[1][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[2][DB]  <   g_lfoBpmMatrix[0][DB] * 1.25f &&  g_lfoBpmMatrix[0][DB]  <   g_lfoBpmMatrix[2][DB] * 1.25f )
                        {
                        f_intervalAverage                 = ( g_lfoBpmMatrix[0][DB] + g_lfoBpmMatrix[1][DB] + g_lfoBpmMatrix[2][DB]) / 3;
            
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
