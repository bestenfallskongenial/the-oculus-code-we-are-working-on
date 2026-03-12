void            CKernel::io_event_button_A          (   BUTTONS::TEvent Event, void *pParam) // Button Y ( lower )
{
                CKernel *pThis = static_cast<CKernel *>(pParam);
                switch (Event)
                    {
                    case BUTTONS::EventSwitchDown:                                  
                    
                    break;

                    case BUTTONS::EventSwitchUp:

                    if (!(g_menu_mode_new == 1 || g_menu_mode_new == 3))
                        {
                        g_menu_mode_new = 0;
                        is_hold_for_2_sec_a = false;
                        is_hold_for_4_sec_a = false;
                        g_opaque = 0.5f; // g_opaque = 1.0f;
                        adc_pot_routing = false;
                    //  m_ChipSelectPin.Write(false);
                        }
                        a_is_hold = false;

                        break;

                    case BUTTONS::EventSwitchClick:  

                    if (g_menu_mode_new == 1) 
                        {
                        g_menu_mode_new = 3;
                    //  g_menu_mode_old = 3;
                        }
                    else if (g_menu_mode_new == 3) 
                        {
                        g_menu_mode_new = 1;
                    //  g_menu_mode_old = 1;
                        }

                    break;

                    case BUTTONS::EventSwitchDoubleClick:
                //  if ( g_menu_mode_new == 0 ) g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer] = !g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer];;           // switch between internal time and adc time 
                    if ( g_menu_mode_new != 0 && g_audio_source_channel != -1 ) g_sensitivityNew = (g_sensitivityNew + 1) % 5; // activate mode for lfo 
                    if ( g_menu_mode_new != 0 && g_audio_source_channel == -1 ) g_attenuation = (g_attenuation + 1) % 3;
                   
                    break;
                    
                    case BUTTONS::EventSwitchTripleClick:
                    
                    if ( g_menu_mode_new != 0 ) g_centralModeBuffer[TEX_MODE][g_currentProgramBuffer] = !g_centralModeBuffer[TEX_MODE][g_currentProgramBuffer];
                    
                    
                    break;
                    
                    case BUTTONS::EventSwitchHold:
                    
                        a_is_hold = true;
                        g_menu_mode_new = 2;    
                        g_opaque = 0.5f;
                    //  g_menu_mode_old = 2;
                        g_show_bank = 1;
                        adc_pot_routing = true;
                    //  m_ChipSelectPin.Write(true);
                    
                    switch (pThis->m_Button_A.GetHoldQuarterSeconds())  // we added this to the button code ourself to enable tis quirked functionality
                        {
                        case 4:
                      is_hold_for_2_sec_a = true;
                        break;     
                        case 12:    
                    //  noTargetFPS =! noTargetFPS;  // we need to solve this, also in sync with our fps break
                        is_hold_for_4_sec_a = true;                     
                        
                        break;
                        
                        default:                                                    
                        
                        return;
                        }                                   										
                    
                    break;
                    
                    default:
                    
                    break;
                    }
}
void            CKernel::io_event_button_B          (   BUTTONS::TEvent Event, void *pParam) // Button X ( upper )
{
                CKernel *pThis = static_cast<CKernel *>(pParam);
                switch (Event)
                    {
                    case BUTTONS::EventSwitchDown:                                  
                    
                    break;

                    case BUTTONS::EventSwitchUp:

                    if (!(g_menu_mode_new == 2))
                        {     										
                        g_menu_mode_new = 0;
                        is_hold_for_2_sec_b = false;
                        is_hold_for_4_sec_b = false;
                        g_opaque = 0.5f; //  g_opaque = 1.0f;
                        adc_pot_routing = false;
                    //  m_ChipSelectPin.Write(false);
                        }
                    b_is_hold = false;    

                    break;

                    case BUTTONS::EventSwitchClick:                                 
                    
                    break;
                    case BUTTONS::EventSwitchDoubleClick:
                //  if ( g_menu_mode_new == 2 ) g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer] = !g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer];;           // switch between internal time and adc time 
                    break;
                    
                    case BUTTONS::EventSwitchTripleClick:                           

                    if ( g_menu_mode_new == 2 ) g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer] = !g_centralModeBuffer[FRM_MODE][g_currentProgramBuffer];;           // switch between internal time and adc time                    
                    
                    break;
                    
                    case BUTTONS::EventSwitchHold:
                    
                    b_is_hold = true;
                    if ( g_menu_mode_new != 3 )    g_menu_mode_new = 1;    
                //  if ( g_menu_mode_new != 3 )    g_menu_mode_old = 1;
                    g_opaque = 0.5f;
                    g_show_bank = 0;
                    adc_pot_routing = true;
                //  m_ChipSelectPin.Write(true);

                    switch (pThis->m_Button_B.GetHoldQuarterSeconds())
                        {
                        case 4:
                     is_hold_for_2_sec_b = true;
                        break;
                        case 12:    
                        
                        is_hold_for_4_sec_b = true;                     
                        
                        break;
                        
                        default:                                                    
                        
                        return;
                        }                                   										
                    
                    break;
                    
                    default:
                    
                    break;
                    }
}