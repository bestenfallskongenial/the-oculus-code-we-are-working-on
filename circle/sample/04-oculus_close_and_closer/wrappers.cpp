                CKernel::wrapper_from_sd()
{
                if(Mount( PARTITION_NAME_SD ))
                    {
                    scanRoot                (   g_ScnVsh,                           // where we store the valid filenames we find
                                                g_SufVsh,                           // the array of valid file extensions for this type of file
                                                SUFFIX_VSH,                         // how many valid file extensions we have in the array above also part of filecounter?
                                                filecounter[scanned][vsh],          // <- has to be initialised with 0 before calling this function
                                                filecounter[maxSD][vsh]);           // how many files are allowed to scan and stored in the array
                    scanRoot                (   g_ScnOmf, 
                                                g_SufOmf, 
                                                SUFFIX_OMF, 
                                                filecounter[scanned][omf], 
                                                filecounter[maxSD][omf]);                
                    scanRoot                (   g_ScnFsh, 
                                                g_SufFsh, 
                                                SUFFIX_FSH, 
                                                filecounter[scanned][fsh], 
                                                filecounter[maxSD][fsh]);
                    scanRoot                (   g_ScnOmt, 
                                                g_SufOmt, 
                                                SUFFIX_OMT, 
                                                filecounter[scanned][omt], 
                                                filecounter[maxSD][omt]);                
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                SUFFIX_TEX, 
                                                filecounter[scanned][tex], 
                                                filecounter[maxSD][tex]);
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                SUFFIX_VID, 
                                                filecounter[scanned][vid], 
                                                filecounter[maxSD][vid]);

                    bulkLoad                (   g_ScnVsh,                           // where we have stored the filenames 
                                                g_bytVsh,                           // where we store the loaded bytes for each file 
                                                m_bufferVsh,                        // where we store the loaded file data for each file
                                                filecounter[scanned][vsh],          // how many files we are allowed to process
                                                filecounter[count][vsh],            // <- is directly modified in the function, we dont need to return it
                                                VSH_FILE_SIZE);                     // maximum size for each file
                                                                            
                    bulkLoad                (   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[scanned][omf], 
                                                filecounter[count][omf], 
                                                FSH_FILE_SIZE);
    
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE);                           

                    bulkLoad                (   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_BufferOmt, 
                                                filecounter[scanned][omt], 
                                                filecounter[count][omt], 
                                                TEX_FILE_SIZE);                                                                                          
 
                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE);                                   
 
                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[scanned][vid], 
                                                filecounter[count][vid], 
                                                VID_FILE_SIZE);   
                    UnMount();   
                    }
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
}

                CKernel::wrapper_load_usb()
{
                if(Mount( PARTITION_NAME_USB ))
                    {
                    scanRoot                (   g_ScnFsh, 
                                                g_SufFsh, 
                                                SUFFIX_FSH, 
                                                filecounter[scanned][fsh], 
                                                filecounter[maxUsb][fsh]);   
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                SUFFIX_TEX, 
                                                filecounter[scanned][tex], 
                                                filecounter[maxUsb][tex]);
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                SUFFIX_VID, 
                                                filecounter[scanned][vid], 
                                                filecounter[maxUsb][vid]);
     
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[scanned][fsh], 
                                                filecounter[count][fsh], 
                                                FSH_FILE_SIZE);                           
    
                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[scanned][tex], 
                                                filecounter[count][tex], 
                                                TEX_FILE_SIZE);                                   
  
                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[scanned][vid], 
                                                filecounter[count][vid], 
                                                VID_FILE_SIZE);    
                    UnMount();   
                    }
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
}

CKernel::wrapper_init_gl_sd()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        initVbuffer(&state);

        initVshaders(&state, VSH_LOADED_OLD, VSH_LOADED_NEW);
        initOshader();             
        initFshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initOprogram();
        initFprograms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initOuniforms();
        initFuniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initOtexture();
        initUtextures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); 
}
CKernel::wrapper_init_gl_usb()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        initFshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initFprograms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initFuniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
        initUtextures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); 
}

CKernel::wrapper_io()
{
        readADC();                  //  we read and dampen the adc each loop

        chooseProgram(ADC_SELECT_PRG);          // determine the shader
        chooseTexture(ADC_SELECT_TEX);          // texture 
        chooseVideo(ADC_SELECT_VID);            // video each loop

        buttonPing();                  // check button A
        buttonPing();                  // and B
        button_consume();               // and transpose the button input in menulayer information each time
}

CKernel::wrapper_modes()
{
        resetMenuPickupFlags();                              // we need to reset the threshold flags each loop - why again?!

        switch (g_current_menu_layer)                           // use than the menulayer variable the 
            {
            case 0:
                modeMenuAssignGroup(1,  0);                  // CH0–CH3 parameters in our global array depending on the menulayer
                break;
            case 1:
                modeMenuAssignGroup(2,  4);   // CH4–CH7
                break;
            case 2:
                modeMenuAssignGroup(3,  8);   // LFO
                break;
            default:
                break;
            }
            
        applyModeToChannel(0);                               // than we apply the mapped modes for the 8 channels
        applyModeToChannel(1);                               // other menulayer modes are handled in the background
        applyModeToChannel(2);
        applyModeToChannel(3);
        applyModeToChannel(4);
        applyModeToChannel(5);
        applyModeToChannel(6);
        applyModeToChannel(7);

        apply_state_to_led();                                   // than we update the 4 leds depending on the modes - we have to write this function 
}