// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// globals and variables in used here:
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::resetMenuPickupFlags    ()
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

void            CKernel::modeMenuAssignGroup(uint8_t menu_id, uint8_t base)
{
                if (g_menu_mode_new != menu_id)
                    {
                    return;
                    }
                int v;

                v = (inOutMatrixInt[4][raw] * menu_map_max[base + 0]) >> 10;        // slot 0

                if (!menu_pickup_flag[base + 0] && v == g_centralModeBuffer[base + 0][g_currentProgramBuffer])
                    {
                    menu_pickup_flag[base + 0] = true;
                    }
                else if (menu_pickup_flag[base + 0])
                    {
                    g_centralModeBuffer[base + 0][g_currentProgramBuffer] = v;
                    }
                v = (inOutMatrixInt[5][raw] * menu_map_max[base + 1]) >> 10;        // slot 1

                if (!menu_pickup_flag[base + 1] && v == g_centralModeBuffer[base + 1][g_currentProgramBuffer])
                    {
                    menu_pickup_flag[base + 1] = true;
                    }
                else if (menu_pickup_flag[base + 1])
                    {
                    g_centralModeBuffer[base + 1][g_currentProgramBuffer] = v;
                    }
                v = (inOutMatrixInt[6][raw] * menu_map_max[base + 2]) >> 10;        // slot 2

                if (!menu_pickup_flag[base + 2] && v == g_centralModeBuffer[base + 2][g_currentProgramBuffer])
                    {
                    menu_pickup_flag[base + 2] = true;
                    }
                else if (menu_pickup_flag[base + 2])
                    {
                    g_centralModeBuffer[base + 2][g_currentProgramBuffer] = v;
                    }
                v = (inOutMatrixInt[7][raw] * menu_map_max[base + 3]) >> 10;        // slot 3

                if (!menu_pickup_flag[base + 3] && v == g_centralModeBuffer[base + 3][g_currentProgramBuffer])
                    {
                    menu_pickup_flag[base + 3] = true;
                    }
                else if (menu_pickup_flag[base + 3])
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
                inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][ in] // adc_float_value[channel];
                inOutMatrixInt[channel][out] = inOutMatrixInt[channel][ in] // adc_int_value[channel];            
}

void            CKernel::modeTRG (int channel)
{
                if (  inOutMatrixInt[channel][ in] >= inOutMatrixInt[channel][trh] &&
                    ! inOutMatrixInt[channel][trF])
                    {
                    inOutMatrixFlt[channel][out]    = inOutMatrixFlt[channel][rnd] // /* g_randomFloatValue */[channel];
                    inOutMatrixInt[channel][out]    = inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];

                    inOutMatrixInt[channel][trF]    = true;
                    }
                else if ( inOutMatrixInt[channel][ in] <= inOutMatrixInt[channel][trl])
                    {
                    inOutMatrixInt[channel][trF] = false;
                    }
}
void            CKernel::modeBPM (int channel) 
{ 
                if (current_time >= g_nextBeatTime[g_activeBpmChannel])
                    {
                    inOutMatrixFlt[channel][out] = inOutMatrixFlt[channel][rnd] // /* g_randomFloatValue */[channel];
                    inOutMatrixInt[channel][out] = inOutMatrixInt[channel][rnd] // g_randomIntegerValue[channel];
                    }
}

void            CKernel::modeLF1 (int channel)
{
                inOutMatrixFlt[channel][out] = inOutMatrixFlt[0][lf1] // g_lfoFltOut[0];
                inOutMatrixInt[channel][out] = inOutMatrixInt[0][lf1] // g_lfoIntOut[0];
}
void            CKernel::modeLF2 (int channel)
{
                inOutMatrixFlt[channel][out] = inOutMatrixFlt[1][lf2] // g_lfoFltOut[1];
                inOutMatrixInt[channel][out] = inOutMatrixInt[1][lf2] // g_lfoIntOut[1];    
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------