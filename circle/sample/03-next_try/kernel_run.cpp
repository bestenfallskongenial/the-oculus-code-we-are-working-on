#include kernel.h

TShutdownMode   CKernel::Run(void)
{
                util_prep_parameters();                                                     // we prep the parameters in the system arrays

                wrapper_load_from_sd();                                                     // we than load the files from sd

                wrapper_init_from_sd();                                                     // we than init the glsl system and parse the media files from sd

                while (m_resetFlag == false)                                                // the actual mainloop - we loop while the reset flag is false
                    {
                    if(filesystem_update_USB("umsd1") == true && m_USBhasLoadOnes == false) // usb attach detection
                        {
                        wrapper_init_from_usb();                                            // if true we run the load from usb function
                        wrapper_init_from_usb();                                            // we than init the glsl system and parse the media files from sd
                        }
                    start_time_fps_calculation = m_Timer.GetClockTicks();                   // for the "fps break" - shall we make this our main timestamp?

                    wrapper_io();
                    wrapper_modes();
                    wrapper_render();
                    }
}


                CKernel::wrapper_load_from_sd()
{
                if(Mount( PARTITION_NAME_SD ))
                    {
                    filesystem_ScanRootDir(     g_ScnVsh, 
                                                g_SufVsh, 
                                                SUFFIX_VSH, 
                                                filecounter[scanned][vsh], 
                                                filecounter[maxSD][vsh]);
                    filesystem_ScanRootDir(     g_ScnOmf, 
                                                g_SufOmf, 
                                                SUFFIX_OMF, 
                                                filecounter[scanned][omf], 
                                                filecounter[maxSD][omf]);                
                    filesystem_ScanRootDir(     g_ScnFsh, 
                                                g_SufFsh, 
                                                SUFFIX_FSH, 
                                                filecounter[scanned][fsh], 
                                                filecounter[maxSD][fsh]);
                    filesystem_ScanRootDir(     g_ScnOmt, 
                                                g_SufOmt, 
                                                SUFFIX_OMT, 
                                                filecounter[scanned][omt], 
                                                filecounter[maxSD][omt]);                
                    filesystem_ScanRootDir(     g_ScnTex, 
                                                g_SufTex, 
                                                SUFFIX_TEX, 
                                                filecounter[scanned][tex], 
                                                filecounter[maxSD][tex]);
                    filesystem_ScanRootDir(     g_ScnVid, 
                                                g_SufVid, 
                                                SUFFIX_VID, 
                                                filecounter[scanned][vid], 
                                                filecounter[maxSD][vid]);

                    m_Watchdog.Start(8);
                    filesystem_process_files(   g_ScnVsh, 
                                                g_bytVsh, 
                                                m_bufferVsh, 
                                                filecounter[scanned][vsh], 
                                                filecounter[count][vsh], 
                                                VSH_FILE_SIZE, 
                                                0);              
                    m_Watchdog.Start(8);                                                                                
                    filesystem_process_files(   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[scanned][omf], 
                                                filecounter[count][omf], 
                                                FSH_FILE_SIZE, 
                                                1);
                    m_Watchdog.Start(8);        
                    filesystem_process_files(   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE, 
                                                1);                           
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_BufferOmt, 
                                                filecounter[scanned][omt], 
                                                filecounter[count][omt], 
                                                TEX_FILE_SIZE, 
                                                2);                                                                                          
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE, 2);                                   
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[scanned][vid], 
                                                filecounter[count][vid], 
                                                VID_FILE_SIZE, 
                                                3);   
                    UnMount();   
                    // Flush CPU->RAM so the VPU sees the loaded bitstream
                    CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
                    }
}

                CKernel::wrapper_load_from_usb()
{
                if(Mount( PARTITION_NAME_USB ))
                    {
                    filesystem_ScanRootDir(     g_ScnVsh, 
                                                g_SufVsh, 
                                                SUFFIX_VSH, 
                                                filecounter[scanned][vsh], 
                                                filecounter[maxSD][vsh]);
                    filesystem_ScanRootDir(     g_ScnOmf, 
                                                g_SufOmf, 
                                                SUFFIX_OMF, 
                                                filecounter[scanned][omf], 
                                                filecounter[maxSD][omf]);                
                    filesystem_ScanRootDir(     g_ScnFsh, 
                                                g_SufFsh, 
                                                SUFFIX_FSH, 
                                                filecounter[scanned][fsh], 
                                                filecounter[maxSD][fsh]);
                    filesystem_ScanRootDir(     g_ScnOmt, 
                                                g_SufOmt, 
                                                SUFFIX_OMT, 
                                                filecounter[scanned][omt], 
                                                filecounter[maxSD][omt]);                
                    filesystem_ScanRootDir(     g_ScnTex, 
                                                g_SufTex, 
                                                SUFFIX_TEX, 
                                                filecounter[scanned][tex], 
                                                filecounter[maxSD][tex]);
                    filesystem_ScanRootDir(     g_ScnVid, 
                                                g_SufVid, 
                                                SUFFIX_VID, 
                                                filecounter[scanned][vid], 
                                                filecounter[maxSD][vid]);

                    m_Watchdog.Start(8);
                    filesystem_process_files(   g_ScnVsh, 
                                                g_bytVsh, 
                                                m_bufferVsh, 
                                                filecounter[scanned][vsh], 
                                                filecounter[count][vsh], 
                                                VSH_FILE_SIZE, 
                                                0);              
                    m_Watchdog.Start(8);                                                                                
                    filesystem_process_files(   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[scanned][omf], 
                                                filecounter[count][omf], 
                                                FSH_FILE_SIZE, 
                                                1);
                    m_Watchdog.Start(8);        
                    filesystem_process_files(   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE, 
                                                1);                           
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_BufferOmt, 
                                                filecounter[scanned][omt], 
                                                filecounter[count][omt], 
                                                TEX_FILE_SIZE, 
                                                2);                                                                                          
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE, 2);                                   
                    m_Watchdog.Start(8);    
                    filesystem_process_files(   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[scanned][vid], 
                                                filecounter[count][vid], 
                                                VID_FILE_SIZE, 
                                                3);    
                    UnMount();   
                    // Flush CPU->RAM so the VPU sees the loaded bitstream
                    CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
                    }
}