// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*


*/
// more filesystem.cpp stuff here, right?
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"

bool            CKernel::util_check_for_update      ()
{
                CString line;    

                // Read physical pin state - 0 means pressed, 1 means released
                unsigned pinStateA = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();

                // If button A is pressed (pinStateA == 0)  
                if (pinStateA == 0)  
                {
                    unsigned long startTime = m_Timer.GetTicks();
                    unsigned long duration = 5000000; // 5 seconds window for confirmation

                    // Wait for button B press for confirmation
                    while (m_Timer.GetTicks() - startTime < duration)
                    {
                        unsigned pinStateB = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();
                        
// we need a timing mechanism here - or we trigger the function via meny layer user interaction 

                        m_Screen.Write(CLEAR_SCREEN, strlen(CLEAR_SCREEN));
                        m_Screen.Write( "\n\n  do you want to update? press X\n"
                                        "  reset in 5 Seconds\n\n"
                                        "  prepare the USB Stick with the new Firmware File\n\n",
                                strlen( "\n\n  do you want to update? press X\n"
                                        "  reset in 5 Seconds\n\n"
                                        "  prepare the USB Stick with the new Firmware File\n\n" ));      

                        m_Timer.MsDelay(100);  // Update display every 100ms
                        
                        if (pinStateB == 0)  // If button B is pressed, start update process
                        {
                            m_Screen.Write("  okay, lets do it\n\n", strlen("okay, lets do it\n\n"));

                            
                            if (!filesystem_load_kernel(PARTITIONSD, FILENAME_KERNEL, 0))   // Load current firmware from SD to buffer 0
                            {
                                m_Screen.Write("  problem with the old firmware file / sd card - please try again\n", 
                                        strlen("  problem with the old firmware file / sd card - please try again\n"));
                                m_Timer.MsDelay(2000);  
                                return false;                      
                            }

                            line.Format ("  %u Byte loaded from SD\n\n",  loaded_bytes_kernel[0]);
                            m_Screen.Write(line, strlen(line));

                            m_Screen.Write("  insert USB stick\n\n", 
                                    strlen("  insert USB stick\n\n"));
                            
                            if (!filesystem_load_kernel(PARTITIONUSB, FILENAME_KERNEL, 1))  // Load new firmware from USB to buffer 1
                            {
                                m_Screen.Write("  problem with the new firmware file / USB stick - please try again\n", 
                                        strlen("  problem with the old firmware file / USB stick - please try again\n"));            
                                m_Timer.MsDelay(2000);
                                return false;                
                            }

                            line.Format ("  %u Byte loaded from USB\n\n",  loaded_bytes_kernel[1]);
                            m_Screen.Write(line, strlen(line));
                            
                            if (!filesystem_save_kernel(PARTITIONSD, FILENAME_KERNEL, 1))   // Write new firmware from buffer 1 to SD
                                {
                                m_Screen.Write("  Error ! Restoring the firmware\n", 
                                        strlen("  Error ! Restoring the firmware\n"));             
                                if (!filesystem_save_kernel(PARTITIONSD, FILENAME_KERNEL, 0))
                                    {
                                    m_Screen.Write("  Writing / Restoring the Firmware Failed - you need to restore it manually !!!\n", 
                                            strlen("  Writing / Restoring the Firmware Failed - you need to restore it manually !!!\n"));             
                                    m_Timer.MsDelay(2000);
                                    return false;    
                                    }
                                }
                            m_Screen.Write("  update successful - system will reboot now\n", 
                                    strlen("  update successful - system will reboot now\n"));      
                            m_Timer.MsDelay(2000);
                            return true;   
                        }
                    }
                    return false; 
                } 
                return false;      
}

void            CKernel::util_save_modes_file       ()    // whats up here??? we will need to rewite this - also do we need a parser and a loader for this file!
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
                                    resultBPM[0], 
                                    resultBPM[1],
                                    table_attenuator[attenuation],
                                    table_intensity[sensitivity_new]);

                g_modes.Append(temp_string);

                for(int i = 0; i < linked_programs; i++) 
                    {
                    temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                        "#  filename: %s\n"
                                        "# --------------------------------------------------------------------------------\n", SCANED_FILES_FSH[i+1]);
                    g_modes.Append(temp_string);
                    g_modes.Append("\n");

                    temp_string.Format( "  CV A0: %s\n"
                                        "  CV A1: %s\n"
                                        "  CV A2: %s\n"
                                        "  CV A3: %s\n\n",table_menu[mode_storage_buffers[CH0_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH1_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH2_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH3_MODE][i]]);
                    g_modes.Append(temp_string);

                    temp_string.Format( "  CV B0: %s\n"
                                        "  CV B1: %s\n"
                                        "  CV B2: %s\n"
                                        "  CV B3: %s\n\n",table_menu[mode_storage_buffers[CH4_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH5_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH6_MODE][i]],
                                                        table_menu[mode_storage_buffers[CH7_MODE][i]]);
                    g_modes.Append(temp_string);       

                    temp_string.Format( "  Wave    LFO 1: %s\n"
                                        "  Divider LFO 1: %d\n\n"
                                        "  Wave    LFO 2: %s\n"
                                        "  Divider LFO 2: %d\n\n",table_waveform[mode_storage_buffers[LF1_WAVE][i]],
                                                                    multiplier[mode_storage_buffers[LF1_MULT][i]],
                                                                table_waveform[mode_storage_buffers[LF2_WAVE][i]],
                                                                    multiplier[mode_storage_buffers[LF2_MULT][i]]);
                    g_modes.Append(temp_string);  
                    
                    temp_string.Format("  Frame   Mode: %s\n", (mode_storage_buffers[FRM_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);
            
                    temp_string.Format("  Texture Mode: %s\n", (mode_storage_buffers[TEX_MODE][i] ? "ON" : "OFF"));
                    g_modes.Append(temp_string);

                    g_modes.Append("\n");
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// helpers i didnt integrate yet, but they are here and working
void CKernel::GenerateH264ParserInfo( int file_index)
{
        CString bufferParser = m_H264Parser.m_DebugCharArray[file_index];
        filesystem_save_log_file( "emmc1-1", VID__LOG_NAMES[file_index], bufferParser);   
}
void CKernel::GenerateBmpParserInfo( int file_index)
{
        CString bufferParser = m_H264Parser.m_DebugCharArray[file_index];
        filesystem_save_log_file( "emmc1-1", BMP__LOG_NAMES[file_index], bufferParser);   
}

void            CKernel::parser_h264               (int fromFile, int toFile)
{
    for (int i = fromFile; i < toFile; i++) 
        {
                m_H264Parser.ParseVideoAuto(i, m_bufferVideo, VID_LOADED_BYTES );
                GenerateH264ParserInfo  (i);
        }
}

void            CKernel::parser_bmp               (int fromFile, int toFile)
{
    for (int i = fromFile; i < toFile; i++) 
        {
                m_H264Parser.ParseBPM(i, SCANED_FILES_TEX, m_bufferTexture, TEX_LOADED_BYTES );
                GenerateBmpParserInfo  (i);
        }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------