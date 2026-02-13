#include "kernel.h"
#include "global.h"

namespace
{
void CKernel::led_set_scaled (int idx, int color, int brightness)
{
                led_col_new[idx][R] = rgb_tab[color][R] + ((rgb_tab[color][R] != 0) * brightness);
                led_col_new[idx][G] = rgb_tab[color][G] + ((rgb_tab[color][G] != 0) * brightness);
                led_col_new[idx][B] = rgb_tab[color][B] + ((rgb_tab[color][B] != 0) * brightness);
}

void CKernel::led_set_plain (int idx, int color)
{
                led_col_new[idx][R] = rgb_tab[color][R];
                led_col_new[idx][G] = rgb_tab[color][G];
                led_col_new[idx][B] = rgb_tab[color][B];
}

void CKernel::led_set_white (int idx)
{
                led_col_new[idx][R] = 159;
                led_col_new[idx][G] = 159;
                led_col_new[idx][B] = 159;
}
}

void            CKernel::menu_led_update            ()
{
                if (g_menu_mode_new == 0)
                    {
                    for (int chn = 0; chn < 8; ++chn)
                        {
                        int mode = mode_storage_buffers[CH0_MODE + chn][current_buffer];
                        int brightness = output_int_value[chn] >> 4;

                        if ((chn == 6 && !menu_pickup_flag[12] && mode == 0) ||
                            (chn == 7 && !menu_pickup_flag[13] && mode == 0))
                            {
                            led_set_plain(chn, M_1);
                            continue;
                            }

                        switch (mode)
                            {
                            case 0:
                                if (chn == 7)      led_set_scaled(chn, A_3 + gl_current_prg, brightness);
                                else if (chn == 6) led_set_scaled(chn, A_2 + gl_current_tex, brightness);
                                else               led_set_scaled(chn, A25 + attenuation, brightness);
                            break;

                            case 1:
                                if (chn == 5 && input_threshold_flag[5])
                                    {
                                    led_set_plain(chn, CLK);
                                    led_col_new[chn][R] = 223;
                                    led_col_new[chn][G] = 223;
                                    led_col_new[chn][B] = 223;
                                    }
                                else
                                    {
                                    led_set_scaled(chn, (chn == 5 ? CLK : TRG), brightness);
                                    }
                            break;

                            case 2:
                                if (next_beat_time[active_bpm_channel] <= current_time_for_bpm)
                                    {
                                    led_set_white(chn);
                                    }
                                else
                                    {
                                    led_set_scaled(chn, BPM, brightness);
                                    }
                            break;

                            case 3: led_set_scaled(chn, LF1, brightness); break;
                            case 4: led_set_scaled(chn, LF2, brightness); break;
                            default: led_set_scaled(chn, A25 + attenuation, brightness); break;
                            }
                        }
                    }
                else if (g_menu_mode_new == 1)
                    {
                    for (int i = 0; i < 4; ++i)
                        {
                        int mode = (adc_raw_value[4 + i] * NUMBER_OF_MODES) >> 10;
                        int brightness = output_int_value[i] >> 4;
                        if (!menu_pickup_flag[i]) led_set_plain(8 + i, M_1);
                        else                      led_set_scaled(8 + i, A50 + mode, brightness);
                        }
                    }
                else if (g_menu_mode_new == 2)
                    {
                    for (int i = 0; i < 4; ++i)
                        {
                        int mode = (adc_raw_value[4 + i] * (NUMBER_OF_MODES - 1)) >> 10;
                        int brightness = output_int_value[4 + i] >> 4;
                        if (!menu_pickup_flag[4 + i]) led_set_plain(12 + i, M_2);
                        else                          led_set_scaled(12 + i, A50 + mode, brightness);
                        }
                    }
}