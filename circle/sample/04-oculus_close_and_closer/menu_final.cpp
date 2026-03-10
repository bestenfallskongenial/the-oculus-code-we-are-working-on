// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// globals and variables in used here:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::resetMenuPickupFlags    ()
{
//              or simply 

//              memset(menu_pickup_flag, 0, 16 * sizeof(bool)); // ?

                if (g_menu_mode_new != g_menu_mode_old) 
                    {
                    for(int i = 0; i < 16; i++) 
                        {
                        g_menuPickUpFlag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;
                    }
}

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

void            CKernel::applyModeToChannel(int channel)
{
                switch (g_modeMap[channel][g_centralModeBuffer[g_currentProgramBuffer][channel] + 1])
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

void            CKernel::modeADC (int channel) 
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[channel][ in] // adc_float_value[channel];
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[channel][ in] // adc_int_value[channel];            
}

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
void            CKernel::modeBPM (int channel) 
{ 
                if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
                    {
                    g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[channel][rnd] // /* g_randomFloatValue */[channel];
                    g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];
                    }
}

void            CKernel::modeLF1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][lf1] // g_lfoFltOut[0] <- lfo comes always from [0] since it is global
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[0][lf1] // g_lfoIntOut[0];
}
void            CKernel::modeLF2 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[1][lf2] // g_lfoFltOut[1];
                g_inOutMatrixInt[channel][out] = g_inOutMatrixInt[1][lf2] // g_lfoIntOut[1];    
}
void            CKernel::modeAudioAb0 (int channel)     // 
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au0] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!    
}
void            CKernel::modeAudioAb1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au1] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}
void            CKernel::modeAudioBb0 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au2] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}
void            CKernel::modeAudioBb1 (int channel)
{
                g_inOutMatrixFlt[channel][out] = g_inOutMatrixFlt[0][au3] // g_lfoFltOut[1];
            //  g_inOutMatrixInt[channel][out] = // i have no int audio band!!
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// the idea is that i have a global menuMap[menu_layer_members][max_modes]

uint8_t mode_map[8][17] =                                   // mode_map is not the same as the g_centralModeBuffer mapping. i can still do the mapping!
{
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 0, 0, 0, 0}        // group for the first 4 channels 
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 0, 0, 0, 0}

{8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0}       // group for the second 4 channels
{8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0}

{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}       // group for the lfo wave and multiplier mapping 
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}       // question - can we create modes like the others to map the lfo settings 
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}        // and use them here in an combined manner?
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}

{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}
{8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,11, 0, 0, 0, 0, 0}
};

// i see the logical issue here: modes are usually meant to control how the in values are processed to out values.
// things like modes and lfo parameters are stored in g_centralModeBuffer[][]
// THAN there are the functions where a knop uses the raw out value AFTER 
// modes and than map it to program, texture, video, frame, sensetivity ( i could made this a g_centralModeBuffer field too ),
// means input -> input-processing -> mode-selection -> mode-precessing -> target-selection -> target ( gl uniform OR hardware )
// because i want for example to have bpm on channel 0 control the frame of the video.
// i assume this is possible with gl code BUT the user may have not the knowlege or the will to program this features therefore the device must offer another way!   
// i could indeed pass the array (like g_centralModeBuffer ) i use as target for the mapping in modeMenuAssignGroup(uint8_t menu_id, uint8_t base, &array_to_work_on )

// 
//               0        1        2        3 |      4        5        6        7 |        8          9         10         11 |       12         13         14           15 |
// program  mode 0 | mode 1 | mode 2 | mode 3 | mode 4 | mode 5 | mode 6 | mode 7 | lf1 wave | lf2 wave | lf1 mult | lf2 mult | tex mode | vid mode | frm mode | is_stored? |
//      0 
//      1
//      2
//      ...
//     31
// default

int g_centralModeBuffer[SLOTS][MODES];