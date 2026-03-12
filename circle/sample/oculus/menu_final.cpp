// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// globals and variables in used here:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::resetMenuPickupFlags    ()
{
//              or simply 

//              memset(menu_pickup_flag, 0, 16 * sizeof(bool)); // ?

                if (g_menu_mode_new != g_menu_mode_old) 
                    {
                    for(int i = 0; i < modetablecount; i++) 
                        {
                        g_menuPickUpFlag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeMenuAssignGroup(uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                int v;

                v = (g_inOutMatrixInt[4][raw] * g_modeMap[base + 0][0]) >> 10;        // slot 0

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }
                v = (g_inOutMatrixInt[5][raw] * g_modeMap[base + 1][0]) >> 10;        // slot 1

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }
                v = (g_inOutMatrixInt[6][raw] * g_modeMap[base + 2][0]) >> 10;        // slot 2

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }
                v = (g_inOutMatrixInt[7][raw] * g_modeMap[base + 3][0]) >> 10;        // slot 3

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::applyModeToChannel(int channel)
{
                switch (g_modeMap[channel][g_centralModeBuffer[g_currentProgramBuffer][channel] + 1]) <- is this correct! g_modeMap gives me the max of modes ( needed for mapping ), also shall it determine if i can use a channel in this function...
                    {
                    case 0:
                        modeADC (channel);
                    break;

                    case 1:
                        modeTRG (channel);
                    break;

                    case 2:
                        modeBPM (channel);
                    break;

                    case 3:
                        modeLF1 (channel);
                    break;
                    case 4:
                        modeLF2 (channel);
                    break;
                    case 5:
                        modeTex (channel);
                    break;
                    case 6:
                        modeVid (channel);
                    break;
                    case 7:
                        modeFrm (channel);
                    break;                    
                    case 8:
                        modeAudioAb0 (channel);     // this modes will be "available" if audio is detected.
                    break;

                    case 9:
                        modeAudioAb1 (channel);
                    break;

                    case 10:
                        modeAudioBb0 (channel);
                    break;

                    case 11:
                        modeAudioBb1 (channel);
                    break;                    
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeADC (int channel) 
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[channel][ in] // adc_float_value[channel];
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[channel][ in] // adc_int_value[channel];            
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeTRG (int channel)
{
                if (  g_inOutMatrixInt[channel][ in] >= g_inOutMatrixInt[channel][trH] &&
                    ! g_inOutMatrixInt[channel][trF])
                    {
                    g_inOutMatrixFlt[channel][out]    = g_inOutMatrixFlt[channel][rnd] // /* g_randomFloatValue */[channel];
                    g_inOutMatrixInt[channel][out]    = g_inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];

                    g_inOutMatrixInt[channel][trF]    = true;
                    }
                else if ( g_inOutMatrixInt[channel][ in] <= g_inOutMatrixInt[channel][trL])
                    {
                    g_inOutMatrixInt[channel][trF] = false;
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeBPM (int channel) 
{ 
                if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
                    {
                    g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[channel][rnd] // /* g_randomFloatValue */[channel];
                    g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeLF1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][lf1] // g_lfoFltOut[0] <- lfo comes always from [0] since it is global
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[0][lf1] // g_lfoIntOut[0];
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeLF2 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[1][lf2] // g_lfoFltOut[1];
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[1][lf2] // g_lfoIntOut[1];    
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioAb0 (int channel)     // 
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au0] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!    
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioAb1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au1] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioBb0 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au2] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::modeAudioBb1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au3] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
