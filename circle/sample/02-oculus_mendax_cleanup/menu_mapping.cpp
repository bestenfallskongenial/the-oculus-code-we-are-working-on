#include "kernel.h"
#include "global.h"

// This file contains the code for the menu mode assignment, this is where we map the ADC values to the modes, we also have to take care of the pickup of the values, we don't want to change the mode while the user is picking up the value, that's why we have the pickup flags, we only change the mode after the pickup is done, this way we avoid any sudden changes in the behavior of the channels while the user is picking up the values.

// this array defines which modes are available for each channel, we have 8 channels and 6 modes, the last mode is the LFO mode which is not available for any channel, this way we can easily check if a mode is available for a channel or not.
const bool g_channel_mode_capability[ADC_CHANNELS][NUMBER_OF_MODES] =
{
    // i wonder if we use enum values here instead of hardcoding the indices, it would be more readable and less error prone, but for now we will keep it like this, maybe in the future we can refactor this to use enums instead of hardcoded indices.
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false },
    { true, true, true, true, true, false }
};
// this function checks if the requested mode is valid for the given channel, it returns the requested mode if it's valid, otherwise it returns 0 which is the default mode, this way we can ensure that we don't assign an invalid mode to a channel.
//nonono! we can simply apply the policy flag logical ( && / || ) at the function that applies the mode logic, NOT here!


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
