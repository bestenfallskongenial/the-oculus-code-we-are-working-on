// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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