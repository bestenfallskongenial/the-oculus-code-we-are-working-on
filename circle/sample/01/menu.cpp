//----------------------------------------------------------------------------------------------------------------------------------------------------

// globals and variablesVAL used here:
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::resetPickUpFlags    ()
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

// i see the logical issue here: modes are usually meant to control how the in values are processed to out values.
// things like modes and lfo parameters are stored in g_centralModeBuffer[][]
// THAN there are the functions where a knop uses the raw out value AFTER 
// modes and than map it to program, texture, video, frame, sensetivity ( i could made this a g_centralModeBuffer field too ),
// means input -> input-processing -> mode-selection -> mode-precessing -> target-selection -> target ( gl uniform OR hardware )
// because i want for example to have bpm on channel 0 control the frame of the video.
// i assume this is possible with gl code BUT the user may have not the knowlege or the will to program this features therefore the device must offer another way!   
// i could indeed pass the array (like g_centralModeBuffer ) i use as target for the mapping in modeMenuAssignGroup(uint8_t menu_id, uint8_t base, &array_to_work_on )

void            CKernel::modeMenuAssignGroup(uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                int v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeMap[base + 0][0]) >> 10;        // slot 0

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                    }
                v = (g_inOutMatrixInt[5][RAW] * g_modeMap[base + 1][0]) >> 10;        // slot 1

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                    }
                v = (g_inOutMatrixInt[6][RAW] * g_modeMap[base + 2][0]) >> 10;        // slot 2

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                    }
                v = (g_inOutMatrixInt[7][RAW] * g_modeMap[base + 3][0]) >> 10;        // slot 3

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::setChannelMode(int channel) // this is also a wrapper right ??
{
                switch (g_modeMap[channel][g_centralModeBuffer[g_currentProgramBuffer][channel] + 1]) // <- is this correct! g_modeMap gives me the max of modes ( needed for mapping ), also shall it determine if i can use a channelVAL this function...
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
                    //  modeTex (channel);
                    break;
                    case 6:
                    //  modeVid (channel);
                    break;
                    case 7:
                    //  modeFrm (channel);
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeADC (int channel) 
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[channel][VAL] // adc_float_value[channel];
                g_inOutMatrixInt[channel][OUT] = g_inOutMatrixInt[channel][VAL] // adc_int_value[channel];            
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeTRG (int channel)
{
                if (  g_inOutMatrixInt[channel][VAL] >= g_inOutMatrixInt[channel][TRH] &&
                    ! g_inOutMatrixInt[channel][TRF]) // treshold flag - but i need 4 each menu layer or is this different than g_menuPickUpFlag???
                    {
                    g_inOutMatrixFlt[channel][OUT]    = g_inOutMatrixFlt[channel][RND] // /* g_randomFloatValue */[channel];
                    g_inOutMatrixInt[channel][OUT]    = g_inOutMatrixInt[channel][RND] // g_randomIntegerValue[channel];

                    g_inOutMatrixInt[channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[channel][VAL] <= g_inOutMatrixInt[channel][TRL])
                    {
                    g_inOutMatrixInt[channel][TRF] = false;
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeBPM (int channel) 
{ 
                if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
                    {
                    g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[channel][RND] // /* g_randomFloatValue */[channel];
                    g_inOutMatrixInt[channel][OUT] = g_inOutMatrixInt[channel][RND] // g_randomIntegerValue[channel];
                    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeLF1 (int channel)
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[0][LF1] // g_lfoFltOut[0] <- lfo comes always from [0] since it is global
                g_inOutMatrixInt[channel][OUT] = g_inOutMatrixInt[0][LF1] // g_lfoIntOut[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeLF2 (int channel)
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[1][LF2] // g_lfoFltOut[1];
                g_inOutMatrixInt[channel][OUT] = g_inOutMatrixInt[1][LF2] // g_lfoIntOut[1];    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeAudioAb0 (int channel)     // 
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[0][AU0] // g_lfoFltOut[1];  
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeAudioAb1 (int channel)
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[0][AU1] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeAudioBb0 (int channel)
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[0][AU2] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::modeAudioBb1 (int channel)
{
                g_inOutMatrixFlt[channel][OUT] = g_inOutMatrixFlt[0][AU3] // g_lfoFltOut[1];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------

