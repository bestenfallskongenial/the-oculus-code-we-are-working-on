// helpers i didnt integrate yet, but they are here and working why not using the actual filename array? i mean the log is .txt
/*
void            CKernel::GenerateH264ParserInfo( int p_fileIndex)
{
                CString f_bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", g_vidLogNames[p_fileIndex], f_bufferParser);       // <---- is now saveBuffer we need to refactor!
}
void            CKernel::GenerateBmpParserInfo( int p_fileIndex)
{
                CString f_bufferParser = m_H264Parser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", g_texLogNames[p_fileIndex], f_bufferParser);       // <---- is now saveBuffer we need to refactor!
}

void            CKernel::GenerateBmpOverlayInfo( int p_fileIndex)                                       // new to store the log for the system textures, here the overlay atlas
{
                CString f_bufferParser = m_H264SystemParser.m_DebugCharArray[p_fileIndex];
                filesystem_save_log_file( "emmc1-1", OMT__LOG_NAMES[p_fileIndex], f_bufferParser);      // <---- is now saveBuffer we need to refactor!    
}
*/
void            CKernel::parser_h264    (   int         p_fromFile, 
                                            int         p_toFile)
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                            m_H264Parser.ParseVideoAuto(i, m_bufferVid, g_bytVid );
                            GenerateH264ParserInfo  (i);
                    }
}

void            CKernel::parser_bmp     (   int         p_fromFile, 
                                            int         p_toFile)
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                            m_H264Parser.ParseBPM(i, g_ScnTex, m_bufferTex, g_bytTex );
                            GenerateBmpParserInfo  (i);
                    }
}

void            CKernel::parser_o_bmp   (   int         p_fileIndex)
{        
                m_H264SystemParser.ParseBPM(p_fileIndex , g_ScnOmt  /*"Overlay Atlas"*/ , m_bufferOmt, g_bytOmt );

                GenerateBmpOverlayInfo  (p_fileIndex); // we keep the indexing maybe there will be more 
        
}

bool            CKernel::checkUpdate    () // aka is a new firmware present?!
{
                if (filecounter[FT_KLN][FLD_LOADED] == 2 ) // or is 1 correct? 
                    {
                    return true;    // there is the second ( usb ) firmware
                    }
                else
                    {
                    return false;    
                    }
}

bool            CKernel::Update         ()
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel
                // - filesystem is already mounted by caller

                if (filesystem_save_buffer_to_file(FILENAME_KERNEL, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                // fallback attempt (allowed to fail)
                filesystem_save_buffer_to_file(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]);

                return false;
}

void            set_pot_routing         (   bool        adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}