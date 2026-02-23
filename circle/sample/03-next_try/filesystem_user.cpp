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
                        
                        m_Screen.Write(CLEAR_SCREEN, strlen(CLEAR_SCREEN));                             // we need a timing mechanism here - or we trigger the function via meny layer user interaction 
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

                            
                            if (!filesystem_load_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 0))   // Load current firmware from SD to buffer 0
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
                            
                            if (!filesystem_load_kernel(PARTITION_NAME_USB, FILENAME_KERNEL, 1))  // Load new firmware from USB to buffer 1
                            {
                                m_Screen.Write("  problem with the new firmware file / USB stick - please try again\n", 
                                        strlen("  problem with the old firmware file / USB stick - please try again\n"));            
                                m_Timer.MsDelay(2000);
                                return false;                
                            }

                            line.Format ("  %u Byte loaded from USB\n\n",  loaded_bytes_kernel[1]);
                            m_Screen.Write(line, strlen(line));
                            
                            if (!filesystem_save_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 1))   // Write new firmware from buffer 1 to SD
                                {
                                m_Screen.Write("  Error ! Restoring the firmware\n", 
                                        strlen("  Error ! Restoring the firmware\n"));             
                                if (!filesystem_save_kernel(PARTITION_NAME_SD, FILENAME_KERNEL, 0))
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
                                    g_resultBPM[0], 
                                    g_resultBPM[1],
                                    table_attenuator[g_attenuation],
                                    table_intensity[g_sensitivityNew]);

                g_modes.Append(temp_string);

                for(int i = 0; i < g_linked_programs_counter; i++) 
                    {
                    temp_string.Format( "# --------------------------------------------------------------------------------\n"
                                        "#  p_fileName: %s\n"
                                        "# --------------------------------------------------------------------------------\n", g_fshScannedFileNames[i+1]);
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::filesystem_load_kernel     (   const char* p_deviceName, 
                                                        const char* p_fileName, 
                                                        unsigned    p_fileIndex)
{
                while(p_fileIndex == 1 && filesystem_update_USB("umsd1") == false)
                {
                    m_Timer.MsDelay(100);
                }

                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                        
                if (f_partitionName != 0 && (m_pFileSystem = new CFATFileSystem) != 0 && m_pFileSystem->Mount(f_partitionName))
                {
                    if (filesystem_open_file(p_fileName))
                    {

                        loaded_bytes_kernel[p_fileIndex] = filesystem_load_file(m_bufferKernel[p_fileIndex], KRL_FILE_SIZE, 4);
                        if (loaded_bytes_kernel[p_fileIndex] > 0)
                        {
                            filesystem_close_file();
                            m_pFileSystem->UnMount();
                            delete m_pFileSystem;
                            m_pFileSystem = 0;
                            return true;
                        }
                        filesystem_close_file();
                    }
                    m_pFileSystem->UnMount();
                }
                delete m_pFileSystem;
                m_pFileSystem = 0;
                return false;
}

bool            CKernel::filesystem_save_kernel     (   const char* p_deviceName, 
                                                        const char* p_fileName, 
                                                        unsigned p_fileIndex)
{
                bool f_write_success = false;   
                
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                
                if (f_partitionName != 0 && (m_pFileSystem = new CFATFileSystem) != 0 && m_pFileSystem->Mount(f_partitionName))
                {
                    unsigned g_hFile = m_pFileSystem->FileCreate(p_fileName);
                    if (g_hFile != 0)
                    {
                        if (m_pFileSystem->FileWrite(g_hFile, m_bufferKernel[p_fileIndex], loaded_bytes_kernel[p_fileIndex]) == loaded_bytes_kernel[p_fileIndex])
                        {
                            f_write_success = true;
                        }
                        m_pFileSystem->FileClose(g_hFile);
                    }
                    m_pFileSystem->UnMount();
                }
                delete m_pFileSystem;
                m_pFileSystem = 0;
                
                return f_write_success;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// helpers i didnt integrate yet, but they are here and working
void            CKernel::GenerateH264ParserInfo( int p_fileIndex)
{
                CString f_bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", g_vidLogNames[p_fileIndex], f_bufferParser);   
}
void            CKernel::GenerateBmpParserInfo( int p_fileIndex)
{
                CString f_bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", g_texLogNames[p_fileIndex], f_bufferParser);   
}

void            CKernel::GenerateBmpOverlayInfo( int p_fileIndex)           // new to store the log for the system textures, here the overlay atlas
{
                CString f_bufferParser = m_H264SystemParser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", OMT__LOG_NAMES[p_fileIndex], f_bufferParser);   // OMT__LOG_NAMES need to be created
}

void            CKernel::parser_h264               (int p_fromFile, int p_toFile)
{
    for (int i = p_fromFile; i < p_toFile; i++) 
        {
                m_H264Parser.ParseVideoAuto(i, m_bufferVideo, g_vidLoadedBytes );
                GenerateH264ParserInfo  (i);
        }
}

void            CKernel::parser_bmp               (int p_fromFile, int p_toFile)
{
    for (int i = p_fromFile; i < p_toFile; i++) 
        {
                m_H264Parser.ParseBPM(i, g_texScannedFileNames, m_bufferTexture, g_texLoadedBytes );
                GenerateBmpParserInfo  (i);
        }
}

void            CKernel::parser_overlay_bmp         (int p_fileIndex)
{        
                m_H264SystemParser.ParseBPM(p_fileIndex , g_omtScannedFileNames  /*"Overlay Atlas"*/ , m_BufferOverlayTexture, g_omtLoadedBytes );

                GenerateBmpOverlayInfo  (p_fileIndex); // we keep the indexing maybe there will be more 
        
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------