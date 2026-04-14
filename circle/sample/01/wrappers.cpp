// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::wrapperInitDMA()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferVid = allocBufferDMA  (  filecounter[FT_VID][FLD_MAXSD]+filecounter[FT_VID][FLD_MAXUSB], filecounter[FT_VID][FLD_SIZE], &m_videoBlockBase, &m_videoRawBlock, &m_videoBlockSize));
        }
    if (bOK)
        {
        bOK = (m_bufferFrA = allocBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB], filecounter[FRM_BF][FLD_SIZE], &m_frameBlockBaseA, &m_frameRawBlockA, &m_frameBlockSizeA));
        }
    if (bOK)
        {
        bOK = (m_bufferFrB = allocBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB], filecounter[FRM_BF][FLD_SIZE], &m_frameBlockBaseB, &m_frameRawBlockB, &m_frameBlockSizeB));
        }
    if (bOK)
        {
        bOK = (m_bufferOmt = allocBufferDMA  (  filecounter[FT_OMT][FLD_MAXSD]+filecounter[FT_OMT][FLD_MAXUSB], filecounter[FT_OMT][FLD_SIZE], &m_overlyBlockBase, &m_overlayRawBlock, &m_overlyBlockSize));
        }
    if (bOK)
        {
        bOK = (m_bufferTex = allocBufferDMA  (  filecounter[FT_TEX][FLD_MAXSD]+filecounter[FT_TEX][FLD_MAXUSB], filecounter[FT_TEX][FLD_SIZE], &m_textureBlockBase, &m_textureRawBlock, &m_textureBlockSize));
        }
    return bOK;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::wrapperInitMEM()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferKnl = allocBufferMEM( filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB], filecounter[FT_KLN][FLD_SIZE]));       // same as allocBufferMEM( KLN_SD + KLN_USB, filecounter[FT_KLN][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferLog = allocBufferMEM( filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB], filecounter[LOGGER][FLD_SIZE]));      // 1024 *64 <- the new buffer for log/text files <- saveFromBuffer() <-
        }                   
    if (bOK)
        {
        bOK = (m_bufferVsh = allocBufferMEM( filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB], filecounter[FT_VSH][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferOmf = allocBufferMEM( filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB], filecounter[FT_OMF][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferFsh = allocBufferMEM( filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB], filecounter[FT_FSH][FLD_SIZE]));
        }
    return bOK;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperDMAcleanUp            ()
{
                    clearBufferDMA( m_bufferVid, m_videoRawBlock); 

                    clearBufferDMA( m_bufferFrA, m_frameRawBlockA); 

                    clearBufferDMA( m_bufferFrB, m_frameRawBlockB); 

                    clearBufferDMA( m_bufferOmt, m_overlayRawBlock); 

                    clearBufferDMA( m_bufferTex, m_textureRawBlock); 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperMEMcleanUp            ()
{
                    clearBufferMEM( m_bufferKnl, filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferLog, filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferVsh, filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferOmf, filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferFsh, filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB] );        
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void                CKernel::wrapper_from_sd()
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_load_usb()
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_init_gl_sd()
{
            //  parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
            //    parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

                initVbuffer (   &m_ogl,
                                &m_vtx ) 

            //  initVshaders    (&m_glsl, filecounter[FT_VSH][FLD_PREV], filecounter[FT_VSH][FLD_LOADED]);

                initShader(     &m_vtx,         // init vertex shader 
                                &m_vsh, 
                                &m_tex,
                                m_bufferVsh,
                                filecounter[FT_VSH][FLD_PREV],      // should be 0
                                filecounter[FT_VSH][FLD_LOADED],    // should be 1 - BUT the code should also ensure that filecounter contains the correct values
                                GL_VERTEX_SHADER,
                                vsh_flags);

                initShader(     &m_vtx,         // init fragment shader ( default )
                                &m_fsh, 
                                &m_tex,
                                m_bufferFsh,
                                filecounter[FT_FSH][FLD_PREV],
                                filecounter[FT_FSH][FLD_LOADED],
                                GL_FRAGMENT_SHADER,
                                fsh_flags);          

                initShader(     &m_vtx,         // init Overlay shader
                                &m_osh, 
                                &m_omt,
                                m_bufferOmf,
                                filecounter[FT_OMF][FLD_PREV],
                                filecounter[FT_OMF][FLD_LOADED],
                                GL_FRAGMENT_SHADER,
                                omf_flags);

            //  initOshader     (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);  

            //  initFshaders    (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

            //  initOprogram    (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);

                initProgram(    &m_vtx,         // user fragment shaders
                                &m_vsh,
                                &m_fsh,
                                &m_tex,
                                filecounter[FT_FSH][FLD_PREV],
                                filecounter[FT_FSH][FLD_LOADED],
                                filecounter[FT_FSH][FLD_VALID],
                                vsh_flags,
                                fsh_flags);
                                
                initProgram(    &m_vtx,         // overlay fragment shader 
                                &m_vsh,
                                &m_osh,
                                &m_omt,
                                filecounter[FT_OMF][FLD_PREV],
                                filecounter[FT_OMF][FLD_LOADED],
                                filecounter[FT_OMF][FLD_VALID],
                                vsh_flags,
                                omf_flags);

            //  initFprograms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

                initUniform(    &m_vtx,
                                &m_fsh,
                                &m_tex,
                                0,                                  // why not simply change the call signature? 
                                filecounter[FT_FSH][FLD_VALID]);

                initUniform(    &m_vtx,
                                &m_osh,
                                &m_omt,
                                0,
                                filecounter[FT_OMF][FLD_VALID]);            

            //  initOuniforms   (&m_glsl, filecounter[FT_OMF][FLD_PREV], filecounter[FT_OMF][FLD_LOADED]);

            //  initFuniforms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

                initTexture(    &m_vtx,
                                &m_fsh,
                                &m_tex,
                                m_bufferTex,
                                filecounter[FT_TEX][FLD_PREV],
                                filecounter[FT_TEX][FLD_LOADED],
                                filecounter[FT_TEX][FLD_VALID],
                                tex_flags,
                                GL_REPEAT,
                                GL_REPEAT);

                initTexture(    &m_vtx,
                                &m_osh,
                                &m_omt,
                                m_bufferOmt,
                                filecounter[FT_OMT][FLD_PREV],
                                filecounter[FT_OMT][FLD_LOADED],
                                filecounter[FT_OMT][FLD_VALID],
                                omt_flags,
                                GL_CLAMP_TO_EDGE,
                                GL_CLAMP_TO_EDGE);

            //  initOtexture    (&m_glsl, filecounter[FT_OMT][FLD_PREV], filecounter[FT_OMT][FLD_LOADED]);

        //      initUtextures   (&m_glsl, filecounter[FT_TEX][FLD_PREV], filecounter[FT_TEX][FLD_LOADED]); 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_init_gl_usb()
{
            //    parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
            //    parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);

            //  initFshaders    (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

                initShader(     &m_vtx,         // init fragment shader ( default )
                                &m_fsh, 
                                &m_tex,
                                m_bufferFsh,
                                filecounter[FT_FSH][FLD_PREV],
                                filecounter[FT_FSH][FLD_LOADED],
                                GL_FRAGMENT_SHADER,
                                fsh_flags);    

            //  initFprograms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

                initProgram(    &m_vtx,         // user fragment shaders
                                &m_vsh,
                                &m_fsh,
                                &m_tex,
                                filecounter[FT_FSH][FLD_PREV],
                                filecounter[FT_FSH][FLD_LOADED],
                                filecounter[FT_FSH][FLD_VALID],
                                vsh_flags,
                                fsh_flags);            

            //  initFuniforms   (&m_glsl, filecounter[FT_FSH][FLD_PREV], filecounter[FT_FSH][FLD_LOADED]);

                initUniform(    &m_vtx,
                                &m_fsh,
                                &m_tex,
                                0,                                  // why not simply change the call signature? 
                                filecounter[FT_FSH][FLD_VALID]);            

            //  initUtextures   (&m_glsl, filecounter[FT_TEX][FLD_PREV], filecounter[FT_TEX][FLD_LOADED]); 

                initTexture(    &m_vtx,
                                &m_fsh,
                                &m_tex,
                                m_bufferTex,
                                filecounter[FT_TEX][FLD_PREV],
                                filecounter[FT_TEX][FLD_LOADED],
                                filecounter[FT_TEX][FLD_VALID],
                                tex_flags,
                                GL_REPEAT,
                                GL_REPEAT);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapper_io()
{
                readADC();                  //  we read and dampen the adc each loop

            //  chooseProgram(ADC_SELECT_PRG);          // determine the shader
            //  chooseTexture(ADC_SELECT_TEX);          // texture 
            //  chooseVideo(ADC_SELECT_VID);            // video each loop

                chooseIndexDense( ADC_SELECT_PRG, m_activePrg, filecounter[FT_FSH][FLD_VALID])
                chooseIndexDense( ADC_SELECT_TEX, m_activeTex, filecounter[FT_TEX][FLD_VALID])
                chooseIndexDense( ADC_SELECT_VID, m_activeVid, filecounter[FT_VID][FLD_VALID])

                buttonPing( 0, SW_PIN_A);                  // check button A
                buttonPing( 1, SW_PIN_B);                  // and B

                button_consumer(0);               // and transpose the button input in menulayer information each time
                button_consumer(1);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void    CKernel::wrapper_modes()
{
        resetPickUpFlags();                              // we need to reset the threshold flags each loop - why again?!

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
            case 3:
                modeMenuAssignGroup(4, 12);   // sensitivity layer A..D
                break;

            default:
                break;
            }
        setChannelMode(0);                               // than we apply the mapped modes for the 8 channels
        setChannelMode(1);                               // other menulayer modes are handled in the background
        setChannelMode(2);
        setChannelMode(3);
        setChannelMode(4);
        setChannelMode(5);
        setChannelMode(6);
        setChannelMode(7);

        apply_state_to_led();                                   // than we update the 4 leds depending on the modes - we have to write this function 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------