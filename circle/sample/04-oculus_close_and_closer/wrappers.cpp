                CKernel::wrapper_from_sd()
{
                if(Mount( PARTITION_NAME_SD ))
                    {
                    scanRoot                (   g_ScnVsh,                           // where we store the valid filenames we find
                                                g_SufVsh,                           // the array of valid file extensions for this type of file
                                                filecounter[FT_VSH][FLD_EXTCNT],                         // how many valid file extensions we have in the array above also part of filecounter?
                                                filecounter[FT_VSH][FLD_SCANNED],         
                                                filecounter[FT_VSH][FLD_MAXSD]);           // how many files are allowed to scan and stored in the array
                    
                    scanRoot                (   g_ScnOmf, 
                                                g_SufOmf, 
                                                filecounter[FT_OMF][FLD_EXTCNT], 
                                                filecounter[FT_OMF][FLD_SCANNED], 
                                                filecounter[FT_OMF][FLD_MAXSD]);                
                    
                    scanRoot                (   g_ScnFsh, 
                                                g_SufFsh, 
                                                filecounter[FT_FSH][FLD_EXTCNT], 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_MAXSD]);
                    
                    scanRoot                (   g_ScnOmt, 
                                                g_SufOmt, 
                                                filecounter[FT_OMT][FLD_EXTCNT], 
                                                filecounter[FT_OMT][FLD_SCANNED], 
                                                filecounter[FT_OMT][FLD_MAXSD]);                
/*                    
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                filecounter[FT_TEX][FLD_EXTCNT], 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_MAXSD]);
                   
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                filecounter[FT_VID][FLD_EXTCNT], 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_MAXSD]);
*/ 
                    scanRoot                (   g_ScnKln, 
                                                g_SufKln, 
                                                filecounter[FT_KLN][FLD_EXTCNT], 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_MAXSD]);    

                    bulkLoad                (   g_ScnVsh,                           // where we have stored the filenames 
                                                g_bytVsh,                           // where we store the loaded bytes for each file 
                                                m_bufferVsh,                        // where we store the loaded file data for each file
                                                filecounter[FT_VSH][FLD_SCANNED],          // how many files we are allowed to process
                                                filecounter[FT_VSH][FLD_LOADED],            // <- is directly modified in the function, we dont need to return it
                                                filecounter[FT_VSH][FLD_PREV],
                                                filecounter[FT_VSH][FLD_SIZE]);                     // maximum size for each file
                                                                            
                    bulkLoad                (   g_ScnOmf, 
                                                g_bytOmf, 
                                                m_bufferOmf, 
                                                filecounter[FT_OMF][FLD_SCANNED], 
                                                filecounter[FT_OMF][FLD_LOADED], 
                                                filecounter[FT_OMF][FLD_PREV],
                                                filecounter[FT_OMF][FLD_SIZE]);
    
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_LOADED], 
                                                filecounter[FT_FSH][FLD_PREV],
                                                filecounter[FT_FSH][FLD_SIZE]);                           

                    bulkLoad                (   g_ScnOmt, 
                                                g_bytOmt, 
                                                m_bufferOmt, 
                                                filecounter[FT_OMT][FLD_SCANNED], 
                                                filecounter[FT_OMT][FLD_LOADED], 
                                                filecounter[FT_OMT][FLD_PREV],
                                                filecounter[FT_OMT][FLD_SIZE]);                                                                                          
 /*
                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_LOADED], 
                                                filecounter[FT_TEX][FLD_PREV],
                                                filecounter[FT_TEX][FLD_SIZE]);                                   
 
                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_LOADED], 
                                                filecounter[FT_VID][FLD_PREV],
                                                filecounter[FT_VID][FLD_SIZE]);   
*/
                    bulkLoad                (   g_ScnKln, 
                                                g_bytKln, 
                                                m_bufferKnl, 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_LOADED], 
                                                filecounter[FT_KLN][FLD_PREV],
                                                filecounter[FT_KLN][FLD_SIZE]); 

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
                                                filecounter[FT_FSH][FLD_EXTCNT], 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_MAXUSB]);   
                    scanRoot                (   g_ScnTex, 
                                                g_SufTex, 
                                                filecounter[FT_TEX][FLD_EXTCNT], 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_MAXUSB]);
                    scanRoot                (   g_ScnVid, 
                                                g_SufVid, 
                                                filecounter[FT_VID][FLD_EXTCNT], 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_MAXUSB]);
                    scanRoot                (   g_ScnKln, 
                                                g_SufKln, 
                                                filecounter[FT_KLN][FLD_EXTCNT], 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_MAXUSB]);                                                  
     
                    bulkLoad                (   g_ScnFsh, 
                                                g_bytFsh, 
                                                m_bufferFsh, 
                                                filecounter[FT_FSH][FLD_SCANNED], 
                                                filecounter[FT_FSH][FLD_LOADED], 
                                                filecounter[FT_FSH][FLD_PREV],
                                                filecounter[FT_FSH][FLD_SIZE]);                           
    
                    bulkLoad                (   g_ScnTex, 
                                                g_bytTex, 
                                                m_bufferTex, 
                                                filecounter[FT_TEX][FLD_SCANNED], 
                                                filecounter[FT_TEX][FLD_LOADED], 
                                                filecounter[FT_TEX][FLD_PREV],
                                                filecounter[FT_TEX][FLD_SIZE]);                                   
  
                    bulkLoad                (   g_ScnVid, 
                                                g_bytVid, 
                                                m_bufferVid, 
                                                filecounter[FT_VID][FLD_SCANNED], 
                                                filecounter[FT_VID][FLD_LOADED], 
                                                filecounter[FT_VID][FLD_PREV],
                                                filecounter[FT_VID][FLD_SIZE]);    
                    bulkLoad                (   g_ScnKln, 
                                                g_bytKln, 
                                                m_bufferKnl, 
                                                filecounter[FT_KLN][FLD_SCANNED], 
                                                filecounter[FT_KLN][FLD_LOADED], 
                                                filecounter[FT_KLN][FLD_PREV],
                                                filecounter[FT_KLN][FLD_SIZE]);                                                 
                    UnMount();   
                    }
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
}

CKernel::wrapper_init_gl_sd()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        initVbuffer(&m_glsl);

        initVshaders    (&m_glsl, filecounter[FT_VSH][FLD_PREV], filecounter[FT_VSH][FLD_LOADED]);

        initOshader     (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);  

        initFshaders    (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initOprogram    (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);

        initFprograms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initOuniforms   (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);

        initFuniforms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initOtexture    (&m_glsl, filecounter[FT_OMT][FLD_PREV], filecounter[FT_OMT][FLD_LOADED]);
/*
        initUtextures   (&m_glsl, filecounter[FT_TEX][FLD_PREV], filecounter[FT_TEX][FLD_LOADED]); 
*/
}
CKernel::wrapper_init_gl_usb()
{
        parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
        parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

        initFshaders    (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initFprograms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initFuniforms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

        initUtextures   (&m_glsl, filecounter[FT_TEX][FLD_PREV], filecounter[FT_TEX][FLD_LOADED]); 
}

CKernel::wrapper_io()
{
        readADC();                  //  we read and dampen the adc each loop

        chooseProgram(ADC_SELECT_PRG);          // determine the shader
        chooseTexture(ADC_SELECT_TEX);          // texture 
        chooseVideo(ADC_SELECT_VID);            // video each loop

        buttonPing( 0, SW_PIN_A);                  // check button A
        buttonPing( 1, SW_PIN_B);                  // and B

        button_consume(0);               // and transpose the button input in menulayer information each time
        button_consume(1);
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
/*
            case 3:
                modeMenuAssignGroup(4, 12);   // <- additional menu layers here -> brainstorm
                break;
*/
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