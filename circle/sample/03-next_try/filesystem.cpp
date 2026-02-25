// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::Mount                      (   const char*         p_deviceName)                           // its in the name. mount the filesystem ( sd / usb ) for further use
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

                if (!m_pFileSystem->Mount(f_partitionName))                                                         // error mounting the filesystem
                    {
                    delete m_pFileSystem;                                                                           // clean up
                    m_pFileSystem = 0;
                    return false;
                    }

                return true;
}

bool            CKernel::UnMount                    ()                                                              // unmount it after use
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
unsigned        CKernel::filesystem_load_file       (   char               *p_buffer,                               // destination buffer for the file data
                                                        unsigned            p_bufferSize,                           // maximum number of bytes to read into the buffer
                                                        int                 p_display_load_mode)                    // helper for the display load screen, used to determine which message to display on the screen while loading, presumably obsolete
{
                unsigned f_totalBytesRead = 0;
                unsigned f_bytesRead;
    
                while (f_totalBytesRead < p_bufferSize)
                    {
                    g_opaque = 0.5f;                                                                                // this is here to make the load display text visible its obsolete in the new version
                    unsigned f_currentChunkSize = (p_bufferSize - f_totalBytesRead < CHUNK_SIZE) ? 
                                                (p_bufferSize - f_totalBytesRead) : CHUNK_SIZE;
                    f_bytesRead = m_pFileSystem->FileRead(g_hFile, p_buffer + f_totalBytesRead, f_currentChunkSize);

                    if (f_bytesRead == FS_ERROR)
                        {
                        g_opaque = 1.0f;                                                                            // this is here to make the load display text visible its obsolete in the new version
                        return 0;                                                                                   // Read error
                        }
                    if (f_bytesRead == 0)
                        {
                        g_opaque = 1.0f;                                                                            // this is here to make the load display text visible its obsolete in the new version
                        return f_totalBytesRead;                                                                    // EOF reached, return total bytes read
                        }
                    f_totalBytesRead += f_bytesRead;

                        display_LoadScreenTexVidShd(p_display_load_mode);

                    m_Watchdog.Start(TIMEOUT);         // new watchdog    
                    }
                g_opaque = 1.0f;    
                return 0;  // Buffer full, EOF not reached - this is NOT a success - 0 is equal to false
}
bool            CKernel::filesystem_close_file      ()	                                                            // close file ( release g_hFile handle ) 
{
	            if (!m_pFileSystem->FileClose (g_hFile))
		            {
		            return false;							                                                        // error closing file
		            }
                return true;								                                                        // success closing file
}

void            CKernel::filesystem_process_files   (   char*               p_fileNameArray[],                      // where we have stored the filenames from the root directory scan
                                                        unsigned            p_loadedBytes[],                        // where we store the size in bytes for each file 
                                                        char**              p_bufferArray,                          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                        int                 p_maxFiles,                             // how many files we are allowed to process ( os limitations )
                                                        int                &p_validFiles,                           // counts successful loads - we need to keep track here <- directly modified in the function, we dont need to return it
                                                        unsigned            p_fileSize,                             // maximum size for each file
                                                        int                 p_display_load_mode)                    // the display load mode is used to determine which message to display on the screen while loading  
{
                for (int i = 0; i < p_maxFiles; ++i) 
                    {
                    if (filesystem_open_file(p_fileNameArray[i]))                                                   // returns true if the file was opened successfully
                        {
                        unsigned f_bytesRead = filesystem_load_file(p_bufferArray[p_validFiles], p_fileSize, p_display_load_mode);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
                            p_validFiles++;   
                            }
                        filesystem_close_file();
                        }                                                                                           // a false on open file will simply skip the file and move on to the next one, which is fine for our purposes
                    }   
}

bool            CKernel::filesystem_IsValidFileType (   const char*         p_fileName,                             // helper to check if the file we found in the root directory has a valid file extension, we pass the filename and the file extension we want to compare 
                                                        const char*         p_fileExtension)
{
                CString             f_fileName(p_fileName);
                int                 f_dotPos = f_fileName.Find('.');

                if (f_dotPos == -1 || f_dotPos == 0 || f_dotPos > 8) 
                    {
                    return FALSE;
                    }
                CString             f_suffix((const char*)f_fileName + f_dotPos + 1);

                return f_suffix.Compare(p_fileExtension) == 0;                                                      // Return true if the file extension matches, false otherwise
}
bool            CKernel::filesystem_ScanRootDir     (   char**              p_fileNameArray,                        // where we store the valid filenames we find
                                                        const char*         p_fileExtension[],                      // the array of valid file extensions for this type of file
                                                        int                 p_extentionCount,                       // how many valid file extensions we have in the array above
                                                        int                &p_scannedFiles,                         // our counter of found files, important - but we can reset it between calls / devices <- has to be initialised with 0 before calling this function
                                                        unsigned            p_maxFiles )                            // how many files are allowed to scan and stored in the array, not sure, the same as above? i had two parameters there, need to check the code
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
                return true;                                                                                        // Return true to indicate successful scan (even if no valid files were found, as long as the directory was accessed successfully)               
}

bool            CKernel::filesystem_update_USB      (   const char*         p_deviceName)                           // this is called when we get a plug and play event for the USB, we check if the device is already in our tree of connected devices,
{                                                                                                                   //  if not we update the tree and check again, if we find it this means we can now access the filesystem on the USB and we register a handler for when the USB gets removed so we can update our state accordingly 
                if (m_USBHCI.UpdatePlugAndPlay())                                                                   // Update the tree of connected USB devices
                    {
                    CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                    if (f_partitionName != nullptr)
                        {
                        m_bStorageAttached = true;                                                                  // this is a global, volatile variable! 

                        f_partitionName->RegisterRemovedHandler(filesystem_remove_USB, this);
                        return true;
                        }
                    }
                return false;
}
void            CKernel::filesystem_remove_USB      (   CDevice            *f_partitionName,                        // this is the handler we register for when the USB gets removed, we set our state to reflect that the storage is no longer attached, we could also unmount the filesystem here if we wanted to be extra safe?
                                                        void               *p_pContext)
{
	            CKernel *pThis = (CKernel *) p_pContext;
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------