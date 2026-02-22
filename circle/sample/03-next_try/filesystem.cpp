// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::filesystem_open_file       (   const char         *p_fileName)		                        // Open File by passing p_fileName via pointer 
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);
	            if (g_hFile == 0)
		            {
		            return false;
		            }	
                return true;
}
unsigned        CKernel::filesystem_load_file       (   char               *p_buffer, 
                                                        unsigned            p_bufferSize, 
                                                        int                 p_display_load_mode)
{
                unsigned f_totalBytesRead = 0;
                unsigned f_bytesRead;
    
                while (f_totalBytesRead < p_bufferSize)
                    {
                    g_opaque = 0.5f;                                                                    // this is here to make the load display text visible
                    unsigned f_currentChunkSize = (p_bufferSize - f_totalBytesRead < CHUNK_SIZE) ? 
                                                (p_bufferSize - f_totalBytesRead) : CHUNK_SIZE;
                    f_bytesRead = m_pFileSystem->FileRead(g_hFile, p_buffer + f_totalBytesRead, f_currentChunkSize);

                    if (f_bytesRead == FS_ERROR)
                        {
                        g_opaque = 1.0f;                                                                // this is here to make the load display text visible
                        return 0;                                                                       // Read error
                        }
                    if (f_bytesRead == 0)
                        {
                        g_opaque = 1.0f;                                                                // this is here to make the load display text visible
                        return f_totalBytesRead;                                                        // EOF reached, return total bytes read
                        }
                    f_totalBytesRead += f_bytesRead;

                        display_LoadScreenTexVidShd(p_display_load_mode);

                    m_Watchdog.Start(TIMEOUT);         // new watchdog    
                    }
                g_opaque = 1.0f;    
                return 0;  // Buffer full, EOF not reached - this is NOT a success - 0 is equal to false
}
bool            CKernel::filesystem_close_file      ()	                                                 // close file ( release g_hFile handle ) 
{
	            if (!m_pFileSystem->FileClose (g_hFile))		                                    // Close File
		            {
		            return false;							                                    // Cannot close file
		            }
                return true;								                                    // file has closed successful
}
int             CKernel::filesystem_process_files   (   char*               p_fileNameArray[], 
                                                        unsigned            p_loadedBytes[], 
                                                        char**              p_bufferArray, 
                                                        int                 p_maxFiles, 
                                                        int                 p_validFiles, 
                                                        unsigned            p_fileSize, 
                                                        int                 p_display_load_mode)
{
                for (int i = 0; i < p_maxFiles; ++i) 
                    {
                    if (filesystem_open_file(p_fileNameArray[i]))
                        {
                        unsigned f_bytesRead = filesystem_load_file(p_bufferArray[p_validFiles], p_fileSize, p_display_load_mode);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
                            p_validFiles++;   
                            }
                        filesystem_close_file();
                        }
                    }   
                return p_validFiles;
}

bool            CKernel::filesystem_mount           (   const char*         p_deviceName, 

                                                        char*               vshaderFileNames[], 
                                                        unsigned            vStotalLoadedBytes[], 
                                                        int                 maxVshaderFiles,

                                                        char*               fOverlayFileNames[],          // <- new, we need to pass a single name and skip somehow the scanfile mechanism
                                                        unsigned            foverlayLoadedBytes[], 
                                                        int                 maxFoverlayFiles,               // <- is one but i still want the new loaders here!

                                                        char*               fshaderFileNames[], 
                                                        unsigned            fStotalLoadedBytes[], 
                                                        int                 maxFshaderFiles,

                                                        char*               texOverlayFileNames[],       // <- same here, means also that we need the correct variables/arrays in global.h/cpp !
                                                        unsigned            tXoverlayLoadedBytes[], 
                                                        int                 maxTexOverlayFiles,

                                                        char*               textureFileNames[], 
                                                        unsigned            tXtotalLoadedBytes[], 
                                                        int                 maxTextureFiles,

                                                        char*               videoFileNames[]  , 
                                                        unsigned            vItotalLoadedBytes[], 
                                                        int                 maxVideoFiles)
{
                bool success = false;
                
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);      // Retrieve a partition device

                if (f_partitionName != 0
                    && (m_pFileSystem = new CFATFileSystem) != 0
                    && m_pFileSystem->Mount(f_partitionName))
                    {
                    g_scanned_vsh = filesystem_ScanRootDir(vshaderFileNames, g_vhsExtensions, VSH_VALID_SUFFIX_COUNT, maxVshaderFiles);
                // we dont scan for the filenames of the overlay shader, we have one and we prep the array with this single name    
                    g_scanned_fsh = filesystem_ScanRootDir(fshaderFileNames, g_fhsExtensions, FSH_VALID_SUFFIX_COUNT, maxFshaderFiles);
                // we dont scan for the filenames of the overlay texture atlas, we have one and we prep the array with this single name     
                    g_scanned_tex = filesystem_ScanRootDir(textureFileNames, g_texExtensions, TEX_VALID_SUFFIX_COUNT, maxTextureFiles);
                    g_scanned_vid = filesystem_ScanRootDir(videoFileNames,   g_vidExtensions, VID_VALID_SUFFIX_COUNT, maxVideoFiles);

                    m_Watchdog.Start(8);
                    g_loaded_vsh_new = filesystem_process_files(  vshaderFileNames, vStotalLoadedBytes, m_bufferVshader, 
                                                                g_scanned_vsh, g_loaded_vsh_new, VSH_FILE_SIZE, 0);  // The file system was mounted successfully                 
                //  we need to know if we have done this correct - correct and all arrays and variables?  
                    m_Watchdog.Start(8);                                                                                
                    OMS_LOADED_NEW = filesystem_process_files(  fOverlayFileNames, foverlayLoadedBytes, m_bufferFoverlay,          // <-the loader for the single overlay shader! - i could indeed use filesystem_load_file directly but... consistency, right?!
                                                                1/*or 0?*/, OMS_LOADED_NEW, FSH_FILE_SIZE, 1);                        // we need to define and assign the correct variables and arrays here and in global.* dont forget!
                    m_Watchdog.Start(8);        
                    g_loaded_fsh_new = filesystem_process_files(  fshaderFileNames, fStotalLoadedBytes, m_bufferFshader, 
                                                                g_scanned_fsh, g_loaded_fsh_new, FSH_FILE_SIZE, 1);                           
                               
                    m_Watchdog.Start(8);    
                    OMT_LOADED_NEW = filesystem_process_files(  texOverlayFileNames, tXoverlayLoadedBytes, m_BufferOverlayTexture,            // same shit, now for the overlay texture!!!
                                                                1/*or 0?*/, OMT_LOADED_NEW, TEX_FILE_SIZE, 2);                                                                                          
                    m_Watchdog.Start(8);    
                    g_loaded_tex_new = filesystem_process_files(  textureFileNames, tXtotalLoadedBytes, m_bufferTexture, 
                                                                g_scanned_tex, g_loaded_tex_new, TEX_FILE_SIZE, 2);                                   
                    m_Watchdog.Start(8);    
                    g_loaded_vid_new = filesystem_process_files(  videoFileNames  , vItotalLoadedBytes, m_bufferVideo  , 
                                                                g_scanned_vid  , g_loaded_vid_new , VID_FILE_SIZE  , 3);                                
                    m_Watchdog.Start(8);    
                    m_pFileSystem->UnMount();
                    success = true;
                    }
                delete m_pFileSystem;
                m_pFileSystem = 0;

                return success;   // Handle the case where no partition device was found or mounting failed
}

bool            CKernel::filesystem_save_log_file   (   const char*         p_deviceName, 
                                                        const char*         p_fileName, 
                                                        const CString&      p_str_to_save)
{
                bool success = false;   
                
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);  // Get partition device
                
                if (f_partitionName != 0 && (m_pFileSystem = new CFATFileSystem) != 0  && m_pFileSystem->Mount(f_partitionName))
                {
                    unsigned g_hFile = m_pFileSystem->FileCreate(p_fileName);   // Create and write shader log file
                    if (g_hFile != 0)
                    {
                        
                        if (m_pFileSystem->FileWrite(g_hFile, (const char*)p_str_to_save, p_str_to_save.GetLength()) == p_str_to_save.GetLength())   // Write the shader log
                        {
                            success = true;
                        }
                        m_pFileSystem->FileClose(g_hFile);
                    }
                    m_pFileSystem->UnMount();   // Unmount filesystem
                }
                delete m_pFileSystem;           // Cleanup filesystem object
                m_pFileSystem = 0;
                
                return success;
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
unsigned        CKernel::filesystem_ScanRootDir     (   char**              p_fileNameArray, 
                                                        const char*         p_fileExtension[], 
                                                        int                 p_extentionCount, 
                                                        unsigned            p_maxFiles )
{
                TDirentry           f_directoryEntry;
                TFindCurrentEntry   f_currentDirectoryEntry;

                unsigned            f_counter                   = 0;
                
                unsigned            f_nextEntry                 = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);

                if(f_nextEntry == 0)  // Initial directory access failed
                    {
                    return 0;    // Return 0 to indicate failure/no files found
                    }

                while (f_nextEntry != 0 && f_counter < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                    {
                        for (int i = 0; i < p_extentionCount; ++i)
                        {
                            if (filesystem_IsValidFileType(f_directoryEntry.chTitle, p_fileExtension[i])) 
                            {
                                p_fileNameArray[f_counter] = new char[strlen(f_directoryEntry.chTitle) + 1];
                                strcpy(p_fileNameArray[f_counter], f_directoryEntry.chTitle);
                                f_counter++;
                                break;
                            }
                        }
                    }
                    f_nextEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return f_counter;    // Return actual number of files found and loaded
}

bool            CKernel::filesystem_update_USB      (   const char*         p_deviceName)
{
                if (m_USBHCI.UpdatePlugAndPlay())   // Update the tree of connected USB devices
                    {
                    CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                    if (f_partitionName != nullptr)
                        {
                        m_bStorageAttached = true; // this is a global, volatile variable! 

                        f_partitionName->RegisterRemovedHandler(filesystem_remove_USB, this);
                        return true;
                        }
                    }
                return false;
}
void            CKernel::filesystem_remove_USB      (   CDevice            *f_partitionName, 
                                                        void               *p_pContext)
{
	            CKernel *pThis = (CKernel *) p_pContext;
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------