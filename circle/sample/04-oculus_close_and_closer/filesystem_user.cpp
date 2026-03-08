// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// more filesystem.cpp stuff here, right?
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
#include "kernel.h"
#include "global.h"

void            CKernel::util_save_modes_file       ()    // whats up here??? we will need to rewrite this - also do we need a parser and a loader for this file!
{
                CString temp_string;

                const char* table_attenuator[3]= { "2.5", "3.3", "5.0" };
                const char* table_waveform[4] = { "Sin","Saw","Tr1","Tr2" };
                const char* table_intensity[5] = { "12.5%", "25%", "50%", "75%", "100%" };
                const char* table_menu[NUMBER_OF_MODES]={ "ADC","TRG","BPM","LF1","LF2","AUD"};
                const char* file_header =   "# --------------------------------------------------------------------------------\n"
                                            "# Mode Config File\n"
                                            "# --------------------------------------------------------------------------------\n"
                                            "# How to Edit the Config File:\n"
                                            "# Add/Remove '#' or whole lines\n"
                                            "# Valid Entries for CV XY are:\n"
                                            "#  ADC - Direct CV-In\n"
                                            "#  TRG - Trigger Synced Randomizer\n"
                                            "#  BPM - Beat Synced Randomizer\n"
                                            "#  LF1 - LFO 1\n"
                                            "#  LF2 - LFO 2\n"
                                            "#  AUD - 4 Band Audio Energy Analyzer\n"
                                            "#  ( is not availeble for CV BX! )\n\n"
                                            "# Valid Entries for LFO 1 & 2:\n"
                                            "#  Sin / Saw / TR1 / TR2 ( rising/falling Triangle )\n"
                                            "# Valid Entries for Divider 1 & 2:\n"
                                            "#  1 / 2 / 4 / 8 / 16 / 32 / 64\n"
                                            "# Valid Entries Frame/Texture Mode:\n"
                                            "#  ON / OFF\n\n";
                g_modes.Append(file_header);

                temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                    "#  Global Settings\n" 
                                    "# --------------------------------------------------------------------------------\n"
                                    "  BPM:                 %-3u\n"
                                    "  CLK:                 %-3u\n\n"
                                    "  CV-Attenuation:      %s Volt\n"
                                    "  Audio-Sensisitivity: %s\n",
                                    g_resultBPM[0], 
                                    g_resultBPM[1],
                                    table_attenuator[g_attenuation],
                                    table_intensity[g_sensitivityNew]);

                g_modes.Append(temp_string);

                for(int i = 0; i < g_linked_programs_counter; i++) 
                    {
                    temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                        "#  p_fileName: %s\n"
                                        "# --------------------------------------------------------------------------------\n", g_ScnFsh[i+1]);
                    g_modes.Append(temp_string);
                    g_modes.Append("\n");

                    temp_string.Format( "  CV A0: %s\n"
                                        "  CV A1: %s\n"
                                        "  CV A2: %s\n"
                                        "  CV A3: %s\n\n",table_menu[g_centralModeBuffer[CH0_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH1_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH2_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH3_MODE][i]]);
                    g_modes.Append(temp_string);

                    temp_string.Format( "  CV B0: %s\n"
                                        "  CV B1: %s\n"
                                        "  CV B2: %s\n"
                                        "  CV B3: %s\n\n",table_menu[g_centralModeBuffer[CH4_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH5_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH6_MODE][i]],
                                                        table_menu[g_centralModeBuffer[CH7_MODE][i]]);
                    g_modes.Append(temp_string);       

                    temp_string.Format( "  Wave    LFO 1: %s\n"
                                        "  Divider LFO 1: %d\n\n"
                                        "  Wave    LFO 2: %s\n"
                                        "  Divider LFO 2: %d\n\n",table_waveform[g_centralModeBuffer[LF1_WAVE][i]],
                                                                    g_lfoMultiplier[g_centralModeBuffer[LF1_MULT][i]],
                                                                table_waveform[g_centralModeBuffer[LF2_WAVE][i]],
                                                                    g_lfoMultiplier[g_centralModeBuffer[LF2_MULT][i]]);
                    g_modes.Append(temp_string);  
                    
                    temp_string.Format("  Frame   Mode: %s\n", (g_centralModeBuffer[FRM_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);
            
                    temp_string.Format("  Texture Mode: %s\n", (g_centralModeBuffer[TEX_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);

                    g_modes.Append("\n");
                    }
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
