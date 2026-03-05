// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// globals and variables in used here:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::resetMenuPickupFlags    ()
{
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

                v = (g_inOutMatrixInt[4][raw] * menu_map_max[base + 0]) >> 10;        // slot 0

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[base + 0][g_currentProgramBuffer])
                    {
                    g_menuPickUpFlag[base + 0] = true;
                    }
                else if (g_menuPickUpFlag[base + 0])
                    {
                    g_centralModeBuffer[base + 0][g_currentProgramBuffer] = v;
                    }
                v = (g_inOutMatrixInt[5][raw] * menu_map_max[base + 1]) >> 10;        // slot 1

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[base + 1][g_currentProgramBuffer])
                    {
                    g_menuPickUpFlag[base + 1] = true;
                    }
                else if (g_menuPickUpFlag[base + 1])
                    {
                    g_centralModeBuffer[base + 1][g_currentProgramBuffer] = v;
                    }
                v = (g_inOutMatrixInt[6][raw] * menu_map_max[base + 2]) >> 10;        // slot 2

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[base + 2][g_currentProgramBuffer])
                    {
                    g_menuPickUpFlag[base + 2] = true;
                    }
                else if (g_menuPickUpFlag[base + 2])
                    {
                    g_centralModeBuffer[base + 2][g_currentProgramBuffer] = v;
                    }
                v = (g_inOutMatrixInt[7][raw] * menu_map_max[base + 3]) >> 10;        // slot 3

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[base + 3][g_currentProgramBuffer])
                    {
                    g_menuPickUpFlag[base + 3] = true;
                    }
                else if (g_menuPickUpFlag[base + 3])
                    {
                    g_centralModeBuffer[base + 3][g_currentProgramBuffer] = v;
                    }
}

void            CKernel::applyModeToChannel(int channel)
{
                if (!g_channel_mode_capability[channel][g_centralModeBuffer[channel][g_currentProgramBuffer]])
                    {
                    return;
                    }
                switch (g_centralModeBuffer[channel][g_currentProgramBuffer])
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* 
                        need to figure out the enums here again!

g_inOutMatrixInt[CHANNEL][ENUM_SOURCE]
g_inOutMatrixFlt[CHANNEL][ENUM_SOURCE]
g_menuPickUpFlag[4*menu_layers]


INT	    ADC_RAW     (SCALED)IN  (SCALED)OUT RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
ch0	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch1	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch2	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch3	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch4	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch5	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch6	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U
ch7	    U	        U	        U           U	        G	        G	        opt   	    opt   	    opt   	    opt         U           U


FLT	    ADC_RAW     (SCALED)IN  (SCALED)OUT RND 	    LF1 	    LF2 	    AUD0        AUD1        AUD2 	    AUD3        TRL         TRH
ch0	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch1	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch2	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch3	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch4	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch5	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch6	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   
ch7	    opt         U	        U           U	        G	        G	        G	        G	        G	        G           opt        opt   

enum:   raw             in          out         rnd         lf1         lf2         au0         au1         au2         au3         trL         trH


all the G ( global ) sit in channel 0 - U are unique values per channel 
*/

enum io_types
{
int raw = 0,        //  the position the dampened adc values per channels are stored *
int  in,            //  either the calculated int or flt value *
int out,            //  here lands the processed ( after mode ) for the glsl uniforms      
int rnd,            //  either the per-channel random int or flt value *
int lf1,            //  either the lfo one int or flt value
int lf2,            //  either the lfo two int or flt value
int au0,            //  the audio band 0 flt value
int au1,            //  the audio band 1 flt value
int au2,            //  the audio band 2 flt value
int au3,            //  the audio band 3 flt value
                    // *means i have a unique value for each channel - the other values are singular, and/or only int/flt
int trL,            //  per channel threshold low !!! dont forget to copy the values in here    128
int trH,            //  per channel threshold high                                              320
//  trF,            //  per channel threshold "flag"
int io_type_count
}


/*
Discrete/equality pickup

Pattern: compute quantized value (new_mode, waveform, multiply) and arm when it equals stored value.

Example:
new_mode = ...; if (!flag && new_mode == stored)

Analog/tolerance pickup with bypass (only CH6/CH7 runtime path)

Pattern: arm when raw ADC enters buffer ± TOLERANCE, else allow processing if flag or mode is non-zero.

Example CH6: if (!flag12 && raw6 in window) ... else if (flag12 || CH6_MODE != 0)

*/