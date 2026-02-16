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
/*
void            CKernel::menu_mode_default_A        ()
{
                // Mapping phase only: no per-channel processing/output writes here.
}    

void            CKernel::menu_mode_default_B        ()
{
                // Mapping phase only: no per-channel processing/output writes here.
}
                // here we only concern ourselves with the mapping of the modes, the actual processing is done in the process function. This way we can keep the menu code and the processing code separate and clean.
void            CKernel::menu_mode_assign_A         ()
{
                if (g_menu_mode_new == 1)
                    {
                    int new_mode = (adc_raw_value[4] * (NUMBER_OF_MODES - 1)) >> 10;
                    // means we need to revert this mapping back to the original mode index before we can apply the policy, because the policy is based on the original mode index, not the mapped one, this is a bit tricky but we can do it by simply applying the inverse of the mapping function, which in this case is just a linear mapping, so we can just multiply the new_mode by the number of modes and divide by 1024 to get the original mode index, then we can apply the policy and then we can map it back to the new_mode index.
                    // we simply comment out/remove the next line and apply the policy directly to the new_mode index, because the policy is based on the new_mode index, not the original mode index, this way we can avoid any confusion and we can keep the code simple and clean.
                //  new_mode = map_mode_with_policy(0, new_mode);
                    if (!menu_pickup_flag[0] && new_mode == mode_storage_buffers[CH0_MODE][current_buffer])
                        {
                        menu_pickup_flag[0] = true;
                        }
                    else if (menu_pickup_flag[0])
                        {
                        mode_storage_buffers[CH0_MODE][current_buffer] = new_mode;
                        }
                    new_mode = (adc_raw_value[5] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(1, new_mode);
                    if (!menu_pickup_flag[1] && new_mode == mode_storage_buffers[CH1_MODE][current_buffer])
                        {
                        menu_pickup_flag[1] = true;
                        }
                    else if (menu_pickup_flag[1])
                        {
                        mode_storage_buffers[CH1_MODE][current_buffer] = new_mode;
                        }
                    new_mode = (adc_raw_value[6] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(2, new_mode);
                    if (!menu_pickup_flag[2] && new_mode == mode_storage_buffers[CH2_MODE][current_buffer])
                        {
                        menu_pickup_flag[2] = true;
                        }
                    else if (menu_pickup_flag[2])
                        {
                        mode_storage_buffers[CH2_MODE][current_buffer] = new_mode;
                        }
                    new_mode = (adc_raw_value[7] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(3, new_mode);
                    if (!menu_pickup_flag[3] && new_mode == mode_storage_buffers[CH3_MODE][current_buffer])
                        {
                        menu_pickup_flag[3] = true;
                        }
                    else if (menu_pickup_flag[3])
                        {
                        mode_storage_buffers[CH3_MODE][current_buffer] = new_mode;
                        }
                    }
}
                // same as above but for the second 4 channels
void            CKernel::menu_mode_assign_B         ()
{
                if (g_menu_mode_new == 2)
                    {
                    int new_mode = (adc_raw_value[4] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(4, new_mode);
                    if (!menu_pickup_flag[4] && new_mode == mode_storage_buffers[CH4_MODE][current_buffer])
                        {
                        menu_pickup_flag[4] = true;
                        }
                    else if (menu_pickup_flag[4])
                        {
                        mode_storage_buffers[CH4_MODE][current_buffer] = new_mode;
                        }

                    new_mode = (adc_raw_value[5] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(5, new_mode);
                    if (!menu_pickup_flag[5] && new_mode == mode_storage_buffers[CH5_MODE][current_buffer])
                        {
                        menu_pickup_flag[5] = true;
                        }
                    else if (menu_pickup_flag[5])
                        {
                        mode_storage_buffers[CH5_MODE][current_buffer] = new_mode;
                        }

                    new_mode = (adc_raw_value[6] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(6, new_mode);
                    if (!menu_pickup_flag[6] && new_mode == mode_storage_buffers[CH6_MODE][current_buffer])
                        {
                        menu_pickup_flag[6] = true;
                        }
                    else if (menu_pickup_flag[6])
                        {
                        mode_storage_buffers[CH6_MODE][current_buffer] = new_mode;
                        }

                    new_mode = (adc_raw_value[7] * (NUMBER_OF_MODES - 1)) >> 10;
                //  new_mode = map_mode_with_policy(7, new_mode);
                    if (!menu_pickup_flag[7] && new_mode == mode_storage_buffers[CH7_MODE][current_buffer])
                        {
                        menu_pickup_flag[7] = true;
                        }
                    else if (menu_pickup_flag[7])
                        {
                        mode_storage_buffers[CH7_MODE][current_buffer] = new_mode;
                        }
                    }
}
                // this is the function where we map the LFO parameters, we have to be careful here because we have to make sure that we don't change the parameters while the LFO is running, that's why we have the pickup flags, we only change the parameters after the pickup is done, this way we avoid any sudden changes in the LFO behavior while it's running.
void            CKernel::menu_mode_assign_LFO       ()  // mapping the LFO-parameters
{
                int chn = 16;

                if (g_menu_mode_new == 3 )
                    {
                    int waveformA = (adc_raw_value[4] * ( WAVEFORMS )) >> 10;

                    if (!menu_pickup_flag[8] && (waveformA == mode_storage_buffers[LF1_WAVE][current_buffer]))
                        {
                        menu_pickup_flag[8] = true;
                        }
                    else if (menu_pickup_flag[8])
                        {
                        mode_storage_buffers[LF1_WAVE][current_buffer] = waveformA;   // Process new value after pickup
                        }

                    int waveformB = (adc_raw_value[5] * ( WAVEFORMS )) >> 10;

                    if (!menu_pickup_flag[9] && (waveformB == mode_storage_buffers[LF2_WAVE][current_buffer]))
                        {
                        menu_pickup_flag[9] = true;
                        }
                    else if (menu_pickup_flag[9])
                        {
                        mode_storage_buffers[LF2_WAVE][current_buffer] = waveformB;   // Process new value after pickup
                        }

                    int multiplyA = (adc_raw_value[6] * ( 7 )) >> 10;       // was ( 7 ) - LFO_MULTIPLIERS doesnt work

                    if (!menu_pickup_flag[10] && (multiplyA == mode_storage_buffers[LF1_MULT][current_buffer]))
                        {
                        menu_pickup_flag[10] = true;
                        }
                    else if (menu_pickup_flag[10])
                        {
                        mode_storage_buffers[LF1_MULT][current_buffer] = multiplyA;   // Process new value after pickup
                        }

                    int multiplyB = (adc_raw_value[7] * ( 7 )) >> 10;

                    if (!menu_pickup_flag[11] && (multiplyB == mode_storage_buffers[LF2_MULT][current_buffer]))
                        {
                        menu_pickup_flag[11] = true;
                        }
                    else if (menu_pickup_flag[11])
                        {
                        mode_storage_buffers[LF2_MULT][current_buffer] = multiplyB;   // Process new value after pickup
                        }
                    }
}
*/
// replaces menu_mode_assign_A menu_mode_assign_B and menu_mode_assign_LFO by using this array here

// 0–7  : ADC modes
// 8–9  : LFO waves
// 10–11: LFO mult
//        constexpr uint8_t menu_map_max[12] =
//        {
//            NUMBER_OF_MODES - 1, NUMBER_OF_MODES - 1,
//            NUMBER_OF_MODES - 1, NUMBER_OF_MODES - 1,
//            NUMBER_OF_MODES - 1, NUMBER_OF_MODES - 1,
//            NUMBER_OF_MODES - 1, NUMBER_OF_MODES - 1,
//            WAVEFORMS, WAVEFORMS,
//            7, 7
//        };

uint8_t menu_map_max[12] =
{
    5, 5, 5, 5,
    5, 5, 5, 5,
    4, 4, 7, 7
};


void CKernel::menu_mode_assign_group(uint8_t menu_id, uint8_t base)
{
    if (g_menu_mode_new != menu_id)
        return;

    int v;

    // slot 0
    v = (adc_raw_value[4] * menu_map_max[base + 0]) >> 10;
    if (!menu_pickup_flag[base + 0] &&
        v == mode_storage_buffers[base + 0][current_buffer])
        menu_pickup_flag[base + 0] = true;
    else if (menu_pickup_flag[base + 0])
        mode_storage_buffers[base + 0][current_buffer] = v;

    // slot 1
    v = (adc_raw_value[5] * menu_map_max[base + 1]) >> 10;
    if (!menu_pickup_flag[base + 1] &&
        v == mode_storage_buffers[base + 1][current_buffer])
        menu_pickup_flag[base + 1] = true;
    else if (menu_pickup_flag[base + 1])
        mode_storage_buffers[base + 1][current_buffer] = v;

    // slot 2
    v = (adc_raw_value[6] * menu_map_max[base + 2]) >> 10;
    if (!menu_pickup_flag[base + 2] &&
        v == mode_storage_buffers[base + 2][current_buffer])
        menu_pickup_flag[base + 2] = true;
    else if (menu_pickup_flag[base + 2])
        mode_storage_buffers[base + 2][current_buffer] = v;

    // slot 3
    v = (adc_raw_value[7] * menu_map_max[base + 3]) >> 10;
    if (!menu_pickup_flag[base + 3] &&
        v == mode_storage_buffers[base + 3][current_buffer])
        menu_pickup_flag[base + 3] = true;
    else if (menu_pickup_flag[base + 3])
        mode_storage_buffers[base + 3][current_buffer] = v;
}

//      example calls:

//      menu_mode_assign_group(1, 0);   // CH0–CH3
//      menu_mode_assign_group(2, 4);   // CH4–CH7
//      menu_mode_assign_group(3, 8);   // LFO




/* per-channel, per-mode capability (boolean) */
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


void CKernel::apply_mode_to_channel(int channel)
{
if (!g_channel_mode_capability
        [channel]
        [mode_storage_buffers[channel][current_buffer]])
    return;


    switch (mode_storage_buffers[channel][current_buffer])
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

void CKernel::modeADC (int channel) 
{
            output_float_value[channel] = adc_float_value[channel];
            output_int_value[channel]   = adc_int_value[channel];

}

void CKernel::modeTRG (int channel)
{
            if (adc_int_value[channel] >= threshold_high[channel] &&
                !input_threshold_flag[channel])
            {
                output_float_value[channel] = random_float_value[channel];
                output_int_value[channel]   = random_int_value[channel];
                input_threshold_flag[channel] = true;
            }
            else if (adc_int_value[channel] < threshold_low[channel])
            {
                input_threshold_flag[channel] = false;
            }
}
void CKernel::modeBPM (int channel) 
{ 
// i just realised now that you piece of shit removed the bpm logic !
if (current_time >= next_beat_time[active_bpm_channel])
            {
                output_float_value[channel] = random_float_value[channel];
                output_int_value[channel]   = random_int_value[channel];
            }
}

void CKernel::modePOT ()
{
// lets see later ... wait, the pot is normalised or not? look into the circuit diagram first 
// we ahve this nifty litte d-switch routing the pots to the adc for input mapping .. and else?
}
void CKernel::modeLF1 ()
{
            output_float_value[channel] = LFO_float_output[0];
            output_int_value[channel]   = LFO_int_output[0];
}
void CKernel::modeLF2 ()
{
            output_float_value[channel] = LFO_float_output[1];
            output_int_value[channel]   = LFO_int_output[1];    
}


// add modes as you like but dont forget to past the into the cases above in the corect order until we found an automatic way to call the correct functions

// here are the "edgecases" from the original menu.cpp file in /sample/01-oculus_mendax but we resolve them differently

void CKernel::modeADC(int channel)
{
    switch (channel)
    {
        /* -------- CH4 : frame / time input -------- */
        case 4:
            if (g_menu_mode_new == 0)
            {
                output_float_value[4] = adc_float_value[4];
                output_int_value[4]   = adc_int_value[4];
            }
        break;

        /* -------- CH6 : pickup + single_tex_mode -------- */
        case 6:
            if (!menu_pickup_flag[12])
            {
                if (adc_raw_value[6] > menu_pickup_buffer[12] - TOLERANCE &&
                    adc_raw_value[6] < menu_pickup_buffer[12] + TOLERANCE)
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
                output_float_value[6] = adc_float_value[6];
                output_int_value[6]   = adc_int_value[6];

                if (single_tex_mode)
                    output_int_value[6] = adc_raw_value[6];
            }
        break;

        /* -------- CH7 : pickup + shader select -------- */
        case 7:
            if (!menu_pickup_flag[13])
            {
                if (adc_raw_value[7] > menu_pickup_buffer[13] - TOLERANCE &&
                    adc_raw_value[7] < menu_pickup_buffer[13] + TOLERANCE)
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
                output_float_value[7] = adc_float_value[7];
                output_int_value[7]   = adc_raw_value[7];
            }
        break;

        /* -------- all other channels : normal ADC -------- */
        default:
            output_float_value[channel] = adc_float_value[channel];
            output_int_value[channel]   = adc_int_value[channel];
        break;
    }
}

