void            CKernel::storeModes                 (   )
{
                if (g_gl_program_current != g_gl_program_last)
                    {    
                    g_currentProgramBuffer = g_centralModeBuffer[g_gl_program_current][IS_STORED] ? g_gl_program_current : DEFAULT_SLOT;
                    g_gl_program_last = g_gl_program_current;
                    }               
                if (g_centralModeBuffer[g_gl_program_current][IS_STORED] == true /* && g_currentProgramBuffer != g_gl_program_current */)
                    {  
                    memcpy(&g_centralModeBuffer[g_gl_program_current][0], &g_centralModeBuffer[DEFAULT_SLOT][0], sizeof(g_centralModeBuffer[g_gl_program_current])); // replaces 16 * sizeof(int)
                    g_currentProgramBuffer = g_gl_program_current;
                    }
                else if (g_centralModeBuffer[g_gl_program_current][IS_STORED] == false /* && g_currentProgramBuffer != DEFAULT_SLOT */)
                    {  
                    g_currentProgramBuffer = DEFAULT_SLOT;
                    }
}

void            CKernel::button_consumer()
{
                // read button state once
                const bool a_single = g_buttons_states[BTN_A][BTN_SINGLE]    != 0;
                const bool b_single = g_buttons_states[BTN_B][BTN_SINGLE]    != 0;
                const bool a_hold   = g_buttons_states[BTN_A][BTN_HOLD_TICK] != 0;
                const bool b_hold   = g_buttons_states[BTN_B][BTN_HOLD_TICK] != 0;

                // layer 0: normal runtime
                if (!a_hold && !b_hold)
                    {
                    g_menuLayer = 0;

                    if (a_single)
                        {
                        calculate1BPM(0, g_currentTime);            // button A: BPM tap timestamp
                        g_buttons_states[BTN_A][BTN_SINGLE] = 0;
                        }

                    if (b_single)
                        {
                        g_buttons_states[BTN_B][BTN_SINGLE] = 0;    // button B: store action
                        }
                    return;
                    }

                if (a_hold && !b_hold)                              // hold A -> layer 1 -> block 0
                    {
                    g_menuLayer = 1;
                    }

                if (b_hold && g_menuLayer < 2)                      // hold B -> layer 2 by default
                    {
                    g_menuLayer = 2;
                    }

                if (b_hold && a_single)                             // hold B + press A -> cycle layers 3..7..3
                    {
                    if (g_menuLayer < 3 || g_menuLayer >= 7)
                        {
                        g_menuLayer = 3;
                        }
                    else
                        {
                        g_menuLayer++;
                        }

                    g_buttons_states[BTN_A][BTN_SINGLE] = 0;
                    }

                switch (g_menuLayer)                                // dispatch active layer to block
                    {
                    case 1:                                         // block 0: MODE_CH0..MODE_CH3
                        set_mode_roof_map(0);
                        mapMenuGroup(0);
                        break;

                    case 2:                                         // block 1: MODE_CH4..MODE_CH7
                        set_mode_roof_map(1);
                        mapMenuGroup(1);
                        break;

                    case 3:                                         // block 2: LFO
                        set_mode_roof_map(2);
                        mapMenuGroup(2);
                        break;

                    case 4:                                         // block 3: sensitivity
                        set_mode_roof_map(3);
                        mapMenuGroup(3);
                        break;

                    case 5:                                         // block 4: target channel selectors
                        set_mode_roof_map(4);
                        mapMenuGroup(4);
                        break;

                    case 6:                                         // block 5: target flags
                        set_mode_roof_map(5);
                        mapMenuGroup(5);
                        break;

                    case 7:                                         // block 6: hw/sys toggles
                        set_mode_roof_map(7);
                        mapMenuGroup(7);
                        break;

                    default:
                        break;
                    }
}

void            CKernel::set_mode_roof_map(uint8_t block)
{
                const uint8_t f_first_flag = FLAG_AUDIO_A;
                const uint8_t base         = block << 2;

                for (uint8_t slot = 0; slot < 4; ++slot)
                {
                    const uint8_t row = base + slot;

                    if (g_mapType[block][slot] == MAP_VALUE)
                    {
                        g_modeRoof[row] = g_valueRoof[block][slot];
                        continue;
                    }

                    uint8_t dst = 0;

                    // base group length comes from g_valueRoof for MAP_MODE blocks (e.g. 5,5,5,5)
                    const uint8_t baseLen = g_valueRoof[block][slot];

                    for (uint8_t i = 0; i < baseLen; ++i)
                        g_modeMap[row][dst++] = g_groupModes[GROUP_BASE][i];

                    // optional groups (A/B)
                    for (uint8_t group = GROUP_FLAG1; group < GROUP_COUNT; ++group)
                    {
                        const uint8_t flag_pos = f_first_flag + (group - GROUP_FLAG1);
                        if (!g_centralModeBuffer[g_currentProgramBuffer][flag_pos]) continue;

                        for (uint8_t i = 0; i < g_groupLen[group]; ++i)
                            g_modeMap[row][dst++] = g_groupModes[group][i];
                    }

                    g_modeRoof[row] = dst;
                }
}

void            CKernel::mapMenuGroup(uint8_t block)
{
                const uint8_t base = block << 2;
                unsigned v;

                v = (g_inOutMatrixInt[4][RAW] * g_modeRoof[base + 0]) >> 10;

                if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
                {
                    g_menuPickUpFlag[base + 0] = true;
                }
                else if (g_menuPickUpFlag[base + 0])
                {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 0] = v;
                }

                v = (g_inOutMatrixInt[5][RAW] * g_modeRoof[base + 1]) >> 10;

                if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
                {
                    g_menuPickUpFlag[base + 1] = true;
                }
                else if (g_menuPickUpFlag[base + 1])
                {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 1] = v;
                }

                v = (g_inOutMatrixInt[6][RAW] * g_modeRoof[base + 2]) >> 10;

                if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
                {
                    g_menuPickUpFlag[base + 2] = true;
                }
                else if (g_menuPickUpFlag[base + 2])
                {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 2] = v;
                }

                v = (g_inOutMatrixInt[7][RAW] * g_modeRoof[base + 3]) >> 10;

                if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
                {
                    g_menuPickUpFlag[base + 3] = true;
                }
                else if (g_menuPickUpFlag[base + 3])
                {
                    g_centralModeBuffer[g_currentProgramBuffer][base + 3] = v;
                }
}

void            CKernel::getChannelModeB(uint8_t block )
{
                const uint8_t base = block << 2;



                int p_channel = base + 0;

                uint8_t mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                ModeFunc fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 1;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 2;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }



                p_channel = base + 3;

                mode = g_modeMap[p_channel][g_centralModeBuffer[g_currentProgramBuffer][p_channel]];

                fn = g_modeTable[mode];

                if (fn)
                {
                    (this->*fn)(p_channel);
                }
}

void            CKernel::modeADC                    (   int p_channel) 
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][VAL];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][VAL];       
                
           //  applyTargetModes(p_channel);
}

void            CKernel::modeTRG                    (   int p_channel) // current
{
                if (  g_inOutMatrixInt[p_channel][VAL] >= g_inOutMatrixInt[p_channel][TRH] &&
                    ! g_inOutMatrixInt[p_channel][TRF])
                    {
                    g_inOutMatrixFlt[p_channel][OUT]    = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT]    = g_inOutMatrixInt[p_channel][RND];
/*
                    if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT])
                        {
                        calculate1BPM(1, g_currentTime);
                        }
*/
                    g_extClockTime                      = g_currentTime;

                    g_inOutMatrixInt[p_channel][TRF]    = true;
                    }
                else if ( g_inOutMatrixInt[p_channel][VAL] <= g_inOutMatrixInt[p_channel][TRL])
                    {
                    g_inOutMatrixInt[p_channel][TRF] = false;
                    }
}

void            CKernel::modeBPM                    (   int p_channel)   // <- currentTime should be g_currentTime -> global member set during Run()  - or a call parameter!
{ 
                if ( g_currentTime >= g_lfoBpmMatrix[p_channel][NBT] )
                    {
                    g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[p_channel][RND];
                    g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[p_channel][RND];
                    }

           //  applyTargetModes(p_channel);
}

void            CKernel::modeLF1                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF1];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF1];

           //  applyTargetModes(p_channel);
}

void            CKernel::modeLF2                    (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][LF2];
                g_inOutMatrixInt[p_channel][OUT] = g_inOutMatrixInt[0][LF2]; 

           //  applyTargetModes(p_channel);
}

void            CKernel::modeAudioAb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU0];

           //  applyTargetModes(p_channel);
}

void            CKernel::modeAudioAb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU1];

           //  applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb0               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU2];

           //  applyTargetModes(p_channel);                
}

void            CKernel::modeAudioBb1               (   int p_channel)
{
                g_inOutMatrixFlt[p_channel][OUT] = g_inOutMatrixFlt[0][AU3];

           //  applyTargetModes(p_channel);
}

void            CKernel::applyTargetModes           (   )       // current!
{
                // hardwired target:
                // CH7 always selects the current program
                g_gl_program_current = g_inOutMatrixInt[ADC_SELECT_PRG][OUT];   // rather RAW?

                // optional target:
                // selected channel OUT controls active texture
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TEX])
                    {
                    m_activeTEX =   g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TEX]][OUT];
                    }

                // optional target:
                // selected channel OUT controls active video
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])
                    {
                    m_activeVideo = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_VID]][OUT];
                    }

                // optional target:
                // selected channel OUT controls active frame
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_VID])
                    {
                    m_activeFrame = g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_FRM]][OUT];
                    }

                // optional target:
                // selected channel OUT controls shader/program time
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])
                    {
                    gl_time =       g_inOutMatrixInt[g_centralModeBuffer[g_currentProgramBuffer][SEL_TIME]][OUT];
                    }

                if (!g_centralModeBuffer[g_currentProgramBuffer][FLAG_TIME])
                    {
                    gl_time =       g_currentTime / 1000000.0f;
                    }

                // optional target:
                // external BPM clock
                if (g_centralModeBuffer[g_currentProgramBuffer][FLAG_EXT])
                    {
                    calculate1BPM(1, g_extClockTime);
                    }
}