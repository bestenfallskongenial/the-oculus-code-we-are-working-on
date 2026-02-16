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
/*
void CKernel::modeADC (int channel) 
{
            output_float_value[channel] = adc_float_value[channel];
            output_int_value[channel]   = adc_int_value[channel];

}
*/
void CKernel::modeADC(int channel)
{
    switch (channel)
    {
        case 4:                                                 /* -------- CH4 : frame / time input -------- */
            if (g_menu_mode_new == 0)
            {
                output_float_value[4] = adc_float_value[4];
                output_int_value[4]   = adc_int_value[4];
            }
        break;

        case 6:                                                 /* -------- CH6 : pickup + single_tex_mode -------- */
            if (!menu_pickup_flag[12])
            {
                if (adc_raw_value[6] > menu_pickup_buffer[12] - TOLERANCE && adc_raw_value[6] < menu_pickup_buffer[12] + TOLERANCE)
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
                    {
                    output_int_value[6] = adc_raw_value[6];
                    }
            }
        break;

        case 7:                                                 /* -------- CH7 : pickup + shader select -------- */
            if (!menu_pickup_flag[13])
            {
                if (adc_raw_value[7] > menu_pickup_buffer[13] - TOLERANCE && adc_raw_value[7] < menu_pickup_buffer[13] + TOLERANCE)
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

        default:                                                /* -------- all other channels : normal ADC -------- */
            output_float_value[channel] = adc_float_value[channel];
            output_int_value[channel]   = adc_int_value[channel];
        break;
    }
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
// lets see later ... wait, the pot is normalized or not? look into the circuit diagram first we have this nifty little d-switch routing the pots to the adc for input mapping .. and else?
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


/*
// for kernel.h // global.* - but i like to move global back to kernel.h
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

//      this is how we do it calls:

    menu_reset_pickup_flags

    menu_mode_assign_group(1, 0);   // CH0–CH3
    menu_mode_assign_group(2, 4);   // CH4–CH7
    menu_mode_assign_group(3, 8);   // LFO

    apply_mode_to_channel(0);
    apply_mode_to_channel(1);
    apply_mode_to_channel(2);
    apply_mode_to_channel(3);
    apply_mode_to_channel(4);
    apply_mode_to_channel(5);
    apply_mode_to_channel(6);
    apply_mode_to_channel(7);

    apply_state_to_led();

    // works beca

*/