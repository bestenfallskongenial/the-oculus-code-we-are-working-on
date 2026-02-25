// short clear technical additions/comments - i mean you bulshit emulator!

enum filecount 
{
	vsm = 0,
	omf,
	fsh,
	omt,
	tex,
	vid,
	filetypescount
}

enum fileindex
{
	maxSD = 0,
	maxUSB,
	oldcount,
	count,
	scanned,
	fileindexcount
}

int filecounter[fileindexcount][filetypescount] = { {0} };	// 2 for sd and usb, filetypescount for the different file types ( vsh, omf, fsh, omt, tex, vid ) 

bool            Ckernel::Mount                      (   const char* p_deviceName)
{
                CDevice* f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);

                if (f_partitionName != 0 && (m_pFileSystem = new CFATFileSystem) != 0 && m_pFileSystem->Mount(f_partitionName))
                {
                    return true;            // Successfully mounted the file system, return true to indicate success
                }
                else
                {
                    delete m_pFileSystem;   // Clean up the file system object if it was created
                    m_pFileSystem = 0;      // Ensure the file system pointer is set to null to avoid dangling pointer issues
                    return false;           // Failed to mount the file system, return false to indicate failure
                }
}

bool            CKernel::Mount                      (   const char*         p_deviceName)
{
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);

                if (f_partitionName == 0)
                    {
                    return false;
                    }

                m_pFileSystem = new CFATFileSystem;
                if (m_pFileSystem == 0)
                    {
                    return false;
                    }

                if (!m_pFileSystem->Mount(f_partitionName))
                    {
                    delete m_pFileSystem;
                    m_pFileSystem = 0;
                    return false;
                    }

                return true;
}


// here the approach to entangle the monsterous mount function - we will no call them all in sequesnce, not like here
/*
                if( filesystem_mount("emmc1-1", 
                    g_ScnVsh,  g_bytVsh, VSH_FILES_ON_SD,
                    g_ScnFsh,  g_bytFsh, FSH_FILES_ON_SD,
                    g_ScnTex,  g_bytTex, TEX_FILES_ON_SD,
                    g_ScnVid,  g_bytVid, VID_FILES_ON_SD))
                    {                   
                    // Flush CPU->RAM so the VPU sees the loaded bitstream
                    CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // we need to flush the cache after loading the video bitstream to make sure that the VPU can see the updated data in memory, otherwise it may cause a crash when the VPU tries to access the video data and if the video data is not updated in memory, it will cause a crash when the VPU tries to access the video data

                    gfx_init_v_buffer(&state);

                    parser_bmp(g_loaded_tex_old,g_loaded_tex_new);
                    parser_h264(g_loaded_vid_old,g_loaded_vid_new);
    
                    gfx_init_vshaders(&state, g_loaded_vsh_old, g_loaded_vsh_new);
                    gfx_init_fshaders(&state, g_loaded_fsh_old, g_loaded_fsh_new);
                    gfx_init_programs(&state, g_loaded_fsh_old, g_loaded_fsh_new);
                    gfx_init_uniforms(&state, g_loaded_fsh_old, g_loaded_fsh_new);
                //  gfx_init_textures(&state, g_loaded_tex_old, g_loaded_tex_new); // we commented out the texture loading for the test of the h264 decoder to texture mechanism, but we need to load the textures at least one time for the brandlogo and the ui, so we load them here from the emmc, but we will not load them again from the usb later, so that we can test the h264 decoder to texture mechanism without interference of the usb loading

                    m_Watchdog.Start(TIMEOUT);

                    m_Timer.MsDelay(100);
 
                    g_loaded_vsh_old = g_loaded_vsh_new;    // also 0 at bytesize calc beacuse i loaded only here
                    g_loaded_fsh_old = g_loaded_fsh_new;
                    g_loaded_tex_old = g_loaded_tex_new;    // we need to reset the couter casues issues later at byte size calculation
                    g_loaded_vid_old = g_loaded_vid_new;
                    }

*/
// okay, now i need to rework the filecounter array and the enums since i integrated some of the counters into the functions

                if(Mount("emmc1-1")) //
                {
                filesystem_ScanRootDir( g_ScnVsh, g_SufVsh, SUFFIX_VSH, filecounter[scanned][vsh], filecounter[maxSD][vsh]);
                filesystem_ScanRootDir( g_ScnOmf, g_SufOmf, SUFFIX_OMF, filecounter[scanned][omf], filecounter[maxSD][omf]);                
                filesystem_ScanRootDir( g_ScnFsh, g_SufFsh, SUFFIX_FSH, filecounter[scanned][fsh], filecounter[maxSD][fsh]);
                filesystem_ScanRootDir( g_ScnOmt, g_SufOmt, SUFFIX_OMT, filecounter[scanned][omt], filecounter[maxSD][omt]);                
                filesystem_ScanRootDir( g_ScnTex, g_SufTex, SUFFIX_TEX, filecounter[scanned][tex], filecounter[maxSD][tex]);
                filesystem_ScanRootDir( g_ScnVid, g_SufVid, SUFFIX_VID, filecounter[scanned][vid], filecounter[maxSD][vid]);

                m_Watchdog.Start(8);
                filesystem_process_files( g_ScnVsh, g_bytVsh, m_bufferVsh, filecounter[scanned][vsh], filecounter[count][vsh], VSH_FILE_SIZE, 0);              
                m_Watchdog.Start(8);                                                                                
                filesystem_process_files( g_ScnOmf, g_bytOmf, m_bufferOmf, filecounter[scanned][omf], filecounter[count][omf], FSH_FILE_SIZE, 1);
                m_Watchdog.Start(8);        
                filesystem_process_files( g_ScnFsh, g_bytFsh, m_bufferFsh, filecounter[scanned][fsh], filecounter[count][fsh], FSH_FILE_SIZE, 1);                           
                m_Watchdog.Start(8);    
                filesystem_process_files( g_ScnOmt, g_bytOmt, m_BufferOmt, filecounter[scanned][omt], filecounter[count][omt], TEX_FILE_SIZE, 2);                                                                                          
                m_Watchdog.Start(8);    
                filesystem_process_files( g_ScnTex, g_bytTex, m_bufferTex, filecounter[scanned][tex], filecounter[count][tex], TEX_FILE_SIZE, 2);                                   
                m_Watchdog.Start(8);    
                filesystem_process_files( g_ScnVid, g_bytVid, m_bufferVid, filecounter[scanned][vid], filecounter[count][vid], VID_FILE_SIZE, 3);   
                UnMount();   
                // Flush CPU->RAM so the VPU sees the loaded bitstream
                CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // do we actually flush the complete video dma buffer here? or just one block? and dont we need to do it for the output frame buffers to? 
                }            
// i wonder, shall i do the glsl stuff like gfx_init_v_buffer also in the same block or after it... 

// okay, i removed here the need for any wishi-washi tracking of loaded files,
// i refer & to my few needed counters inside the function, no return values needed

// why is this such a monster? 
void            CKernel::filesystem_process_files   (   char*               p_fileNameArray[],      // where we have stored the filenames 
                                                        unsigned            p_loadedBytes[],        // where we store the loaded bytes for each file 
                                                        char**              p_bufferArray,          // where we store the loaded file data for each file
                                                        int                 p_maxFiles,             // how many files we are allowed to process
                                                        int                &p_validFiles,           // <- is directly modified in the function, we dont need to return it
                                                        unsigned            p_fileSize,             // maximum size for each file
                                                        int                 p_display_load_mode)    // the display load mode is used to determine which message to display on the screen while loading  
{
                for (int i = 0; i < p_maxFiles; ++i) 
                    {
                    if (filesystem_open_file(p_fileNameArray[i])) // returns true if the file was opened successfully, so how we use this fact here?
                        {
                        unsigned f_bytesRead = filesystem_load_file(p_bufferArray[p_validFiles], p_fileSize, p_display_load_mode);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
                            p_validFiles++;   
                            }
                        filesystem_close_file();
                        }
                    // a false on open file will simply skip the file and move on to the next one, which is fine for our purposes
                    }   
}

bool            CKernel::filesystem_IsValidFileType (   const char*         p_fileName, 
                                                        const char*         p_fileExtension)
{
                CString             f_fileName(p_fileName);
                int                 f_dotPos = f_fileName.Find('.');

                if (f_dotPos == -1 || f_dotPos == 0 || f_dotPos > 8) 
                    {
                    return FALSE;
                    }
                CString             f_suffix((const char*)f_fileName + f_dotPos + 1);

                return f_suffix.Compare(p_fileExtension) == 0;
}
bool            CKernel::filesystem_ScanRootDir     (   char**              p_fileNameArray,    // where we store the valid filenames we find
                                                        const char*         p_fileExtension[],  // the array of valid file extensions for this type of file
                                                        int                 p_extentionCount,   // how many valid file extensions we have in the array above
                                                        int                &p_scannedFiles,     // <- has to be initialised with 0 before calling this function
                                                        unsigned            p_maxFiles )        // how many files are allowed to scan and stored in the array
{
                TDirentry           f_directoryEntry;
                TFindCurrentEntry   f_currentDirectoryEntry;
                
                unsigned            f_nextEntry                 = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);

                if(f_nextEntry == 0)  // Initial directory access failed
                        {
                    return false;    // Return false to indicate failure to access the directory
                        }

                while (f_nextEntry != 0 && p_scannedFiles < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                        {
                        for (int i = 0; i < p_extentionCount; ++i)
                            {
                            if (filesystem_IsValidFileType(f_directoryEntry.chTitle, p_fileExtension[i])) 
                                {
                                p_fileNameArray[p_scannedFiles] = new char[strlen(f_directoryEntry.chTitle) + 1];
                                strcpy(p_fileNameArray[p_scannedFiles], f_directoryEntry.chTitle);
                                p_scannedFiles++;
                                break;
                                }
                            }
                        }
                    f_nextEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return true;    // Return true to indicate successful scan (even if no valid files were found, as long as the directory was accessed successfully)               
}

// what matches more my stile here? what is really needed?
void CKernel::UnMount()
{
    m_pFileSystem->UnMount();
}

bool CKernel::UnMount()
{
    if (!m_pFileSystem)
        return false;

    m_pFileSystem->UnMount();
    return true;
}

void CKernel::UnMount()
{
    m_pFileSystem->UnMount();
    delete m_pFileSystem;
    m_pFileSystem = 0;
}

bool CKernel::UnMount()
{
    if (!m_pFileSystem)
        return false;

    m_pFileSystem->UnMount();
    delete m_pFileSystem;
    m_pFileSystem = 0;
    return true;
}

bool            CKernel::UnMount                    ()
{
                if (m_pFileSystem == 0)
                    {
                    return false;
                    }

                m_pFileSystem->UnMount();
                delete m_pFileSystem;
                m_pFileSystem = 0;
                return true;
}