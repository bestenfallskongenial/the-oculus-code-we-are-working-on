                CKernel::wrapper_from_sd()
{
                if(Mount( PARTITION_NAME_SD ))
                    {
                    filesystem_ScanRootDir(     g_ScnVsh,                           // where we store the valid filenames we find
                                                g_SufVsh,                           // the array of valid file extensions for this type of file
                                                SUFFIX_VSH,                         // how many valid file extensions we have in the array above also part of filecounter?
                                                filecounter[scanned][vsh],          // <- has to be initialised with 0 before calling this function
                                                filecounter[maxSD][vsh]);           // how many files are allowed to scan and stored in the array
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
                    filesystem_bulk_load    (   g_ScnVsh,                           // where we have stored the filenames 
                                                g_bytVsh,                           // where we store the loaded bytes for each file 
                                                m_bufferVsh,                        // where we store the loaded file data for each file
                                                filecounter[scanned][vsh],          // how many files we are allowed to process
                                                filecounter[count][vsh],            // <- is directly modified in the function, we dont need to return it
                                                VSH_FILE_SIZE,                      // maximum size for each file
                                                0);              
                    m_Watchdog.Start(8);                                                                                
                    filesystem_bulk_load    (   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[scanned][omf], 
                                                filecounter[count][omf], 
                                                FSH_FILE_SIZE, 
                                                1);
                    m_Watchdog.Start(8);        
                    filesystem_bulk_load    (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE, 
                                                1);                           
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_BufferOmt, 
                                                filecounter[scanned][omt], 
                                                filecounter[count][omt], 
                                                TEX_FILE_SIZE, 
                                                2);                                                                                          
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE, 2);                                   
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnVid, 
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

                CKernel::wrapper_load_usb()
{
                if(Mount( PARTITION_NAME_USB ))
                    {
                    filesystem_ScanRootDir(     g_ScnVsh, 
                                                g_SufVsh, 
                                                SUFFIX_VSH, 
                                                filecounter[scanned][vsh], 
                                                filecounter[maxUsb][vsh]);
                    filesystem_ScanRootDir(     g_ScnOmf, 
                                                g_SufOmf, 
                                                SUFFIX_OMF, 
                                                filecounter[scanned][omf], 
                                                filecounter[maxUsb][omf]);                
                    filesystem_ScanRootDir(     g_ScnFsh, 
                                                g_SufFsh, 
                                                SUFFIX_FSH, 
                                                filecounter[scanned][fsh], 
                                                filecounter[maxUsb][fsh]);
                    filesystem_ScanRootDir(     g_ScnOmt, 
                                                g_SufOmt, 
                                                SUFFIX_OMT, 
                                                filecounter[scanned][omt], 
                                                filecounter[maxUsb][omt]);                
                    filesystem_ScanRootDir(     g_ScnTex, 
                                                g_SufTex, 
                                                SUFFIX_TEX, 
                                                filecounter[scanned][tex], 
                                                filecounter[maxUsb][tex]);
                    filesystem_ScanRootDir(     g_ScnVid, 
                                                g_SufVid, 
                                                SUFFIX_VID, 
                                                filecounter[scanned][vid], 
                                                filecounter[maxUsb][vid]);

                    m_Watchdog.Start(8);
                    filesystem_bulk_load    (   g_ScnVsh, 
                                                g_bytVsh, 
                                                m_bufferVsh, 
                                                filecounter[scanned][vsh], 
                                                filecounter[count][vsh], 
                                                VSH_FILE_SIZE, 
                                                0);              
                    m_Watchdog.Start(8);                                                                                
                    filesystem_bulk_load    (   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[scanned][omf], 
                                                filecounter[count][omf], 
                                                FSH_FILE_SIZE, 
                                                1);
                    m_Watchdog.Start(8);        
                    filesystem_bulk_load    (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE, 
                                                1);                           
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_BufferOmt, 
                                                filecounter[scanned][omt], 
                                                filecounter[count][omt], 
                                                TEX_FILE_SIZE, 
                                                2);                                                                                          
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE, 2);                                   
                    m_Watchdog.Start(8);    
                    filesystem_bulk_load    (   g_ScnVid, 
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

CKernel::wrapper_init_gl_sd()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        gfx_init_v_buffer(&state);

        gfx_init_vshaders(&state, VSH_LOADED_OLD, VSH_LOADED_NEW);
        gfx_init_overlay_fshader();             
        gfx_init_fshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_overlay_program();
        gfx_init_programs(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_overlay_uniforms();
        gfx_init_uniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_overlay_texture();
        gfx_init_textures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); 
}
CKernel::wrapper_init_gl_usb()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        gfx_init_fshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_programs(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_uniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        gfx_init_textures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); 
}

CKernel::wrapper_io()
{
        io_read_ADC();                  //  we read and dampen the adc each loop

        util_choose_program();          // determine the shader
        util_choose_texture();          // texture 
        util_choose_video();            // video each loop

        button_ping();                  // check button A
        button_ping();                  // and B
        button_consume();               // and transpose the button input in menulayer information each time
}

CKernel::wrapper_modes()
{
        menu_reset_pickup_flags();                              // we need to reset the threshold flags each loop - why again?!

        switch (g_current_menu_layer)                           // use than the menylayer variable the 
            {
            case 0:
                menu_mode_assign_group(1,  0);                  // CH0–CH3 parameters in our global array depending on the menulayer
                break;
            case 1:
                menu_mode_assign_group(2,  4);   // CH4–CH7
                break;
            case 2:
                menu_mode_assign_group(3,  8);   // LFO
                break;
            default:
                break;
            }
            
        apply_mode_to_channel(0);                               // than we apply the mapped modes for the 8 channels
        apply_mode_to_channel(1);                               // other menulayer modes are handled in the background
        apply_mode_to_channel(2);
        apply_mode_to_channel(3);
        apply_mode_to_channel(4);
        apply_mode_to_channel(5);
        apply_mode_to_channel(6);
        apply_mode_to_channel(7);

        apply_state_to_led();                                   // than we update the 4 leds depending on the modes - we have to write this function 
}