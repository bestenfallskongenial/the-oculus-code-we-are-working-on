#include "kernel.h"
#include "global.h"

static inline bool mode_allowed (int ch, int mode)
{
                return mode >= 0 && mode < NUMBER_OF_MODES && g_channel_mode_capability[ch][mode];
                // what the fuck is this? why not just have a single array of allowed modes per channel and check against that? 
                // this is so fucking convoluted and inefficient. 
                // very simple to apply - we will use the array for logic like && or || later
}


// same here, when mode_storage_buffers is our source of truth, why 
static void apply_mode_to_channel (int channel, int mode, unsigned long current_time)
{
                if (!mode_allowed(channel, mode))
                    {
                    mode = 0;
                    }

                switch (mode)
                    {
                    case 0:
                        output_float_value[channel] = adc_float_value[channel];
                        output_int_value[channel]   = adc_int_value[channel];
                    break;
// we should have some commentary here about the functionality. what /  why
// also is this the place where we can call functions from the switch cases, 
// like maybe we can have a function for each mode that handles the logic for that mode, 
// and then we can just call those functions from the switch cases, just for clarity and separation of concerns.
                    case 1:
                        if (adc_int_value[channel] >= threshold_high[channel] && !input_threshold_flag[channel])
                            {
                            output_float_value[channel] = random_float_value[channel];
                            output_int_value[channel]   = random_int_value[channel];
                            input_threshold_flag[channel] = true;
                            }
                        else if (adc_int_value[channel] < threshold_low[channel])
                            {
                            input_threshold_flag[channel] = false;
                            }
                    break;

                    case 2:
                        if (current_time >= next_beat_time[active_bpm_channel])
                            {
                            output_float_value[channel] = random_float_value[channel];
                            output_int_value[channel]   = random_int_value[channel];
                            }
                    break;

                    case 3:
                        output_float_value[channel] = LFO_float_output[0];
                        output_int_value[channel]   = LFO_int_output[0];
                    break;

                    case 4:
                        output_float_value[channel] = LFO_float_output[1];
                        output_int_value[channel]   = LFO_int_output[1];
                    break;
                    }
}

void            CKernel::apply_mode_logic           ()
{
                unsigned long current_time = m_Timer.GetClockTicks();
                // i explicitly stated that i want my loops unrolled, but you just had to go and make it more complicated than it needs to be.
                for (int channel = 0; channel < ADC_CHANNELS; ++channel)
                    {
                    int mode = mode_storage_buffers[CH0_MODE + channel][current_buffer];
                    apply_mode_to_channel(channel, mode, current_time);
                    }

                // keep current special clock-trigger side effect on channel 5
                // wtf is this? why is this here? this is so bad. this is a side effect that only applies to one channel and one mode, 
                // a mode that we removed btw and it's just randomly tacked on here in the middle of the function. this is so bad. 
                // this should be handled in a much more elegant way, like maybe a separate function that checks for clock triggers and updates the bpm accordingly, 
                // or maybe even better, we could have a more general event system that allows us to trigger events based on certain conditions, 
                // like clock triggers or threshold crossings, and then we can have different handlers 
                // for those events that can update the bpm or do whatever else we need to do. this is just so bad.
                // we simply can delete it, when i need the current time for bpm i will use current_time.
                if (mode_storage_buffers[CH5_MODE][current_buffer] == 1 && input_threshold_flag[5])
                    {
                    current_time_for_bpm = current_time;
                    }
}