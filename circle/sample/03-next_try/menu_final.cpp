// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
    for kernel.h // global.* - but i like to move global back to kernel.h

    uint8_t menu_map_max[12] =
        {
            5, 5, 5, 5,
            5, 5, 5, 5,
            4, 4, 7, 7
        };

    // per-channel, per-mode capability (boolean) 

    const bool g_channel_mode_capability[ADC_CHANNELS][NUMBER_OF_MODES] =
        {
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false },
            { true, true, true, true, true, false }
        };

    //  this is how we do it calls:

        menu_reset_pickup_flags

        menu_mode_assign_group(1,  0);   // CH0–CH3
        menu_mode_assign_group(2,  4);   // CH4–CH7
        menu_mode_assign_group(3,  8);   // LFO
        menu_mode_assign_group(4, 12);   // potential new menu layers for g_attenuation

        apply_mode_to_channel(0);
        apply_mode_to_channel(1);
        apply_mode_to_channel(2);
        apply_mode_to_channel(3);
        apply_mode_to_channel(4);
        apply_mode_to_channel(5);
        apply_mode_to_channel(6);
        apply_mode_to_channel(7);

        apply_state_to_led();

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::menu_reset_pickup_flags    ()
{
                if (g_menu_mode_new != g_menu_mode_old) 
                    {

                    for(int i = 0; i < 16; i++) 
                        {
                        menu_pickup_flag[i] = false;
                        }
                    g_menu_mode_old = g_menu_mode_new;
                    }
}

void CKernel::menu_mode_assign_group(uint8_t menu_id, uint8_t base)
{
    if (g_menu_mode_new != menu_id)
        return;

    int v;

    // slot 0
    v = (adc_raw_value[4] * menu_map_max[base + 0]) >> 10;
    if (!menu_pickup_flag[base + 0] &&
        v == g_centralModeBuffer[base + 0][g_currentProgramBuffer])
        menu_pickup_flag[base + 0] = true;
    else if (menu_pickup_flag[base + 0])
        g_centralModeBuffer[base + 0][g_currentProgramBuffer] = v;

    // slot 1
    v = (adc_raw_value[5] * menu_map_max[base + 1]) >> 10;
    if (!menu_pickup_flag[base + 1] &&
        v == g_centralModeBuffer[base + 1][g_currentProgramBuffer])
        menu_pickup_flag[base + 1] = true;
    else if (menu_pickup_flag[base + 1])
        g_centralModeBuffer[base + 1][g_currentProgramBuffer] = v;

    // slot 2
    v = (adc_raw_value[6] * menu_map_max[base + 2]) >> 10;
    if (!menu_pickup_flag[base + 2] &&
        v == g_centralModeBuffer[base + 2][g_currentProgramBuffer])
        menu_pickup_flag[base + 2] = true;
    else if (menu_pickup_flag[base + 2])
        g_centralModeBuffer[base + 2][g_currentProgramBuffer] = v;

    // slot 3
    v = (adc_raw_value[7] * menu_map_max[base + 3]) >> 10;
    if (!menu_pickup_flag[base + 3] &&
        v == g_centralModeBuffer[base + 3][g_currentProgramBuffer])
        menu_pickup_flag[base + 3] = true;
    else if (menu_pickup_flag[base + 3])
        g_centralModeBuffer[base + 3][g_currentProgramBuffer] = v;
}

void CKernel::apply_mode_to_channel(int channel)
{
if (!g_channel_mode_capability
        [channel]
        [g_centralModeBuffer[channel][g_currentProgramBuffer]])
    return;


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
            modeLF1 ();
        break;

        case 4:
            modeLF2 ();
        break;
    }
}
/*
void CKernel::modeADC (int channel) 
{
            inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][ in] // adc_float_value[channel];
            inOutMatrixInt[channel][out] = inOutMatrixInt[channel][ in] // adc_int_value[channel];            
}
*/
void CKernel::modeADC(int channel)
{
    switch (channel)
    {
        case 4:                                                 /* -------- CH4 : frame / time input -------- */
            if (g_menu_mode_new == 0)
            {
                inOutMatrixFlt[4][out] = inOutMatrixFlt[4][ in] // adc_float_value[4];
                inOutMatrixInt[4][out] = inOutMatrixInt[4][ in] // adc_int_value[4];
            }
        break;

        case 6:                                                 /* -------- CH6 : pickup + single_tex_mode -------- */
            if (!menu_pickup_flag[12])
            {
                if (inOutMatrixInt[6][raw] > g_menu_pickup_buffer[12] - ADC_TOLERANCE && inOutMatrixInt[6][raw] < g_menu_pickup_buffer[12] + ADC_TOLERANCE)
                {
                    menu_pickup_flag[12] = true;
                }
                else
                {
                    return;
                }
            }
            if (g_menu_mode_new == 0)
            {
                inOutMatrixFlt[6][out] = inOutMatrixFlt[6][ in] // adc_float_value[6];
                inOutMatrixInt[6][out] = inOutMatrixInt[6][ in] // adc_int_value[6];

                if (single_tex_mode)                    // this is an error, we integrated this in our mode_storage_buffer ! 
                    {
                    inOutMatrixInt[6][out] = inOutMatrixInt[6][raw] // adc_raw_value[6];
                    }
            }
        break;

        case 7:                                                 /* -------- CH7 : pickup + shader select -------- */
            if (!menu_pickup_flag[13])
            {
                if (inOutMatrixInt[7][raw] > g_menu_pickup_buffer[13] - ADC_TOLERANCE && inOutMatrixInt[7][raw] < g_menu_pickup_buffer[13] + ADC_TOLERANCE)
                {
                    menu_pickup_flag[13] = true;
                }
                else
                {
                    return;
                }
            }

            if (g_menu_mode_new == 0)
            {
                inOutMatrixFlt[7][out] = inOutMatrixFlt[7][ in] // adc_float_value[7];
                inOutMatrixInt[7][out] = inOutMatrixInt[7][raw] // adc_raw_value[7];
            }
        break;

        default:                                                /* -------- all other channels : normal ADC -------- */
            inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][ in] // adc_float_value[channel];
            inOutMatrixInt[channel][out] = inOutMatrixInt[channel][ in] // adc_int_value[channel];
        break;
    }
}

void CKernel::modeTRG (int channel)
{
            if (  /* adc_int_value[channel] */          inOutMatrixInt[channel][ in] >= /* g_threshold_high[channel] */ inOutMatrixInt[channel][trh] &&
                ! /* input_threshold_flag[channel] */   inOutMatrixInt[channel][trF])
            {
                inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][rnd] // g_randomFloatValue[channel];
                inOutMatrixInt[channel][out] = inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];

                 /* input_threshold_flag[channel] */    inOutMatrixInt[channel][trF] = true;
            }
            else if ( /* adc_int_value[channel] */      inOutMatrixInt[channel][ in] <  /* g_threshold_low[channel] */  inOutMatrixInt[channel][trl])
            {
                 /* input_threshold_flag[channel] */    inOutMatrixInt[channel][trF] = false;
            }
}
void CKernel::modeBPM (int channel) 
{ 
// i just realised now that you piece of shit removed the bpm logic !
if (current_time >= g_nextBeatTime[g_activeBpmChannel])
            {
                inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][rnd] // g_randomFloatValue[channel];
                inOutMatrixInt[channel][out] = inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];
            }
}

void CKernel::modePOT ()
{
// lets see later ... wait, the pot is normalized or not? look into the circuit diagram first we have this nifty little d-switch routing the pots to the adc for input mapping .. and else?
}
void CKernel::modeLF1 ()
{
            inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][lf1] // g_lfoFltOut[0];
            inOutMatrixInt[channel][out] = inOutMatrixInt[channel][lf1] // g_lfoIntOut[0];
}
void CKernel::modeLF2 ()
{
            inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][lf2] // g_lfoFltOut[1];
            inOutMatrixInt[channel][out] = inOutMatrixInt[channel][lf2] // g_lfoIntOut[1];    
}