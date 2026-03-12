
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::parser_h264    (   int         p_fromFile, 
                                            int         p_toFile)
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                            m_H264Parser.ParseVideoAuto(i, m_bufferVid, g_bytVid );
                            GenerateH264ParserInfo  (i);
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::parser_bmp     (   int         p_fromFile, 
                                            int         p_toFile)
{
                for (int i = p_fromFile; i < p_toFile; i++) 
                    {
                            m_H264Parser.ParseBPM(i, g_ScnTex, m_bufferTex, g_bytTex );
                            GenerateBmpParserInfo  (i);
                    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::parser_o_bmp   (   int         p_fileIndex)
{        
                m_H264SystemParser.ParseBPM(p_fileIndex , g_ScnOmt  /*"Overlay Atlas"*/ , m_bufferOmt, g_bytOmt );

                GenerateBmpOverlayInfo  (p_fileIndex); // we keep the indexing maybe there will be more 
        
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::Update         ()
{
                // assumes:
                // - m_bufferKnl[1] + loaded_bytes_kernel[1] already contain the "new" kernel
                // - m_bufferKnl[0] + loaded_bytes_kernel[0] already contain the fallback kernel loaded from sd ( the running kernel )
                // - filesystem is already mounted by caller

                if (m_filesystem_save_buffer_to_file(FILENAME_KERNEL, m_bufferKnl[1], loaded_bytes_kernel[1]))
                    {
                    return true;
                    }

                // fallback attempt (allowed to fail)
                m_filesystem_save_buffer_to_file(FILENAME_KERNEL, m_bufferKnl[0], loaded_bytes_kernel[0]);

                return false;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            set_pot_routing         (   bool        adc_pot_routing)
{
                m_ChipSelectPin.Write(adc_pot_routing);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------