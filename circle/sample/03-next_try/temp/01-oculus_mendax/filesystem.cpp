#include "kernel.h"
#include "global.h"

bool            CKernel::filesystem_open_file       (   const char *p_fileName)		                        // Open File by passing p_fileName via pointer 
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);                               // !!! g_hFile has to be defined globally !!!
	            if (g_hFile == 0)
		            {
		            return false;							                                    // Cannot open file
		            }	
                return true;								                                    // file opened successful
}
unsigned        CKernel::filesystem_load_file       (   char *buffer, 
                                                        unsigned bufferSize, 
                                                        int mode)
{
                unsigned totalBytesRead = 0;
                unsigned bytesRead;
    
                while (totalBytesRead < bufferSize)
                    {
                    g_opaque = 0.5f;
                    unsigned currentChunkSize = (bufferSize - totalBytesRead < CHUNK_SIZE) ? 
                                                (bufferSize - totalBytesRead) : CHUNK_SIZE;
                    bytesRead = m_pFileSystem->FileRead(g_hFile, buffer + totalBytesRead, currentChunkSize);

                    if (bytesRead == FS_ERROR)
                        {
                        g_opaque = 1.0f;    
                        return 0;  // Read error
                        }
                    if (bytesRead == 0)
                        {
                        g_opaque = 1.0f;    
                        return totalBytesRead;  // EOF reached, return total bytes read
                        }
                    totalBytesRead += bytesRead;

                        display_LoadScreenTexVidShd(mode);

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
int             CKernel::filesystem_process_files   (   char* p_fileNameArray[], 
                                                        unsigned p_loadedBytes[], 
                                                        char** p_bufferArray, 
                                                        int p_maxFiles, 
                                                        int p_validFiles, 
                                                        unsigned p_fileSize, 
                                                        int mode)
{
                for (int i = 0; i < p_maxFiles; ++i) 
                    {
                    if (filesystem_open_file(p_fileNameArray[i]))
                        {
                        unsigned bytesRead = filesystem_load_file(p_bufferArray[p_validFiles], p_fileSize, mode);
                        if (bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = bytesRead;
                            p_validFiles++;   
                            }
                        filesystem_close_file();
                        }
                    }   
                return p_validFiles;
}
bool            CKernel::filesystem_mount           (   const char* p_deviceName, 
                                                        char* vshaderFileNames[], 
                                                        unsigned vStotalLoadedBytes[], 
                                                        int maxVshaderFiles,
                                                        char* fshaderFileNames[], 
                                                        unsigned fStotalLoadedBytes[], 
                                                        int maxFshaderFiles,
                                                        char* textureFileNames[], 
                                                        unsigned tXtotalLoadedBytes[], 
                                                        int maxTextureFiles,
                                                        char* videoFileNames[]  , 
                                                        unsigned vItotalLoadedBytes[], 
                                                        int maxVideoFiles)
{
                bool success = false;
                
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);      // Retrieve a partition device

                if (f_partitionName != 0
                    && (m_pFileSystem = new CFATFileSystem) != 0
                    && m_pFileSystem->Mount(f_partitionName))
                    {
                    g_scanned_vsh = filesystem_ScanRootDir(vshaderFileNames, g_vhsExtensions, VSH_VALID_SUFFIX_COUNT, maxVshaderFiles);
                    g_scanned_fsh = filesystem_ScanRootDir(fshaderFileNames, g_fhsExtensions, FSH_VALID_SUFFIX_COUNT, maxFshaderFiles);
                    g_scanned_tex = filesystem_ScanRootDir(textureFileNames, g_texExtensions, TEX_VALID_SUFFIX_COUNT, maxTextureFiles);
                    g_scanned_vid = filesystem_ScanRootDir(videoFileNames,   g_vidExtensions, VID_VALID_SUFFIX_COUNT, maxVideoFiles);

                    g_loaded_vsh_new = filesystem_process_files(  vshaderFileNames, vStotalLoadedBytes, m_bufferVshader, 
                                                                g_scanned_vsh, g_loaded_vsh_new, VSH_FILE_SIZE, 0);  // The file system was mounted successfully                 
                    m_Watchdog.Start(8);        
                    g_loaded_fsh_new = filesystem_process_files(  fshaderFileNames, fStotalLoadedBytes, m_bufferFshader, 
                                                                g_scanned_fsh, g_loaded_fsh_new, FSH_FILE_SIZE, 1);                               
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

bool            CKernel::filesystem_save_log_file   (   const char* p_deviceName, 
                                                        const char* p_fileName, 
                                                        const CString& p_str_to_save)
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

bool            CKernel::filesystem_IsValidFileType (   const char* p_fileName, const char* p_fileExtension)
{
                CString fileName(p_fileName);
                int dotPos = fileName.Find('.');
                if (dotPos == -1 || dotPos == 0 || dotPos > 8) 
                    {
                    return FALSE;
                    }
                CString suffix((const char*)fileName + dotPos + 1);
                return suffix.Compare(p_fileExtension) == 0;
}
unsigned        CKernel::filesystem_ScanRootDir     ( char** fileArray, const char* p_fileExtension[], int p_extentionCount, unsigned p_maxFiles )
{
                TDirentry f_directoryEntry;
                TFindCurrentEntry f_currentDirectoryEntry;
                unsigned count = 0;
                
                unsigned nEntry = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);
                if(nEntry == 0)  // Initial directory access failed
                    {
                    return 0;    // Return 0 to indicate failure/no files found
                    }

                while (nEntry != 0 && count < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                    {
                        for (int i = 0; i < p_extentionCount; ++i)
                        {
                            if (filesystem_IsValidFileType(f_directoryEntry.chTitle, p_fileExtension[i])) 
                            {
                                fileArray[count] = new char[strlen(f_directoryEntry.chTitle) + 1];
                                strcpy(fileArray[count], f_directoryEntry.chTitle);
                                count++;
                                break;
                            }
                        }
                    }
                    nEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return count;    // Return actual number of files found and loaded
}
/*
unsigned        CKernel::filesystem_ScanRootDir     (   char** fileArray, 
                                                        const char* p_fileExtension, 
                                                        unsigned p_maxFiles)
{
                TDirentry f_directoryEntry;
                TFindCurrentEntry f_currentDirectoryEntry;
                unsigned count = 0;
                
                unsigned nEntry = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);
                if(nEntry == 0)  // Initial directory access failed
                    {
                    return 0;    // Return 0 to indicate failure/no files found
                    }

                while (nEntry != 0 && count < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                    {
                        if (filesystem_IsValidFileType(f_directoryEntry.chTitle, p_fileExtension)) 
                        {
                            fileArray[count] = new char[strlen(f_directoryEntry.chTitle) + 1];
                            strcpy(fileArray[count], f_directoryEntry.chTitle);
                            count++;
                        }
                    }
                    nEntry = m_pFileSystem->RootFindNext(&f_directoryEntry, &f_currentDirectoryEntry);
                    }
                return count;    // Return actual number of files found and loaded
}
*/
bool            CKernel::filesystem_update_USB      (   const char* deviceType)
{
                if (m_USBHCI.UpdatePlugAndPlay())   // Update the tree of connected USB devices
                    {
                    CDevice *pDevice = m_DeviceNameService.GetDevice(deviceType, TRUE);
                    if (pDevice != nullptr)
                        {
                        m_bStorageAttached = true;

                        pDevice->RegisterRemovedHandler(filesystem_remove_USB, this);
                        return true;
                        }
                    }
                return false;
}
void            CKernel::filesystem_remove_USB      (   CDevice *pDevice, void *p_pContext)
{
	            CKernel *pThis = (CKernel *) p_pContext;
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}

bool            CKernel::filesystem_load_kernel     (   const char* p_deviceName, 
                                                        const char* p_fileName, 
                                                        unsigned p_fileIndex)
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
                bool success = false;   
                
                CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                
                if (f_partitionName != 0 && (m_pFileSystem = new CFATFileSystem) != 0 && m_pFileSystem->Mount(f_partitionName))
                {
                    unsigned g_hFile = m_pFileSystem->FileCreate(p_fileName);
                    if (g_hFile != 0)
                    {
                        if (m_pFileSystem->FileWrite(g_hFile, m_bufferKernel[p_fileIndex], loaded_bytes_kernel[p_fileIndex]) == loaded_bytes_kernel[p_fileIndex])
                        {
                            success = true;
                        }
                        m_pFileSystem->FileClose(g_hFile);
                    }
                    m_pFileSystem->UnMount();
                }
                delete m_pFileSystem;
                m_pFileSystem = 0;
                
                return success;
}

