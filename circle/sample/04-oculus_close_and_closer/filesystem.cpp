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

bool            CKernel::openFile       (   const char         *p_fileName)		                        // Open File by passing p_fileName via pointer 
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);
	            if (g_hFile == 0)
		            {
		            return false;
		            }	
                return true;
}
unsigned        CKernel::loadFile       (   char               *p_buffer,                               // destination buffer for the file data
                                                        unsigned            p_bufferSize)                           // maximum number of bytes to read into the buffer
{
                unsigned f_totalBytesRead = 0;
                unsigned f_bytesRead;
    
                while (f_totalBytesRead < p_bufferSize)
                    {
                    unsigned f_currentChunkSize = (p_bufferSize - f_totalBytesRead < CHUNK_SIZE) ? (p_bufferSize - f_totalBytesRead) : CHUNK_SIZE;

                    f_bytesRead = m_pFileSystem->FileRead(g_hFile, p_buffer + f_totalBytesRead, f_currentChunkSize);

                    if (f_bytesRead == FS_ERROR)
                        {
                        return 0;                                                                                   // Read error
                        }
                    if (f_bytesRead == 0)
                        {
                        return f_totalBytesRead;                                                                    // EOF reached, return total bytes read
                        }
                    f_totalBytesRead += f_bytesRead;

                    m_Watchdog.Start(TIMEOUT);         // new watchdog    
                    }
                return 0;  // Buffer full, EOF not reached - this is NOT a success - 0 is equal to false
}

bool            CKernel::saveBuffer (   const char*     p_fileName,
                                                    const char*     p_buffer,
                                                    unsigned        p_bufferSize)
{
                if (m_pFileSystem == 0 || p_fileName == 0 || p_buffer == 0 || p_bufferSize  == 0)
                    {
                    return false;
                    }
                g_hFile = m_pFileSystem->FileCreate(p_fileName);
                if (g_hFile == 0)
                    {
                    return false;
                    }
                if (m_pFileSystem->FileWrite(g_hFile, p_buffer, p_bufferSize) != p_bufferSize)
                    {
                    return false;
                    }
                closeFile();
                return true;
}

bool            CKernel::closeFile      ()	                                                            // close file ( release g_hFile handle ) 
{
	            if (!m_pFileSystem->FileClose (g_hFile))
		            {
		            return false;							                                                        // error closing file
		            }
                return true;								                                                        // success closing file
}

void            CKernel::bulkLoad                   (   char*               p_fileNameArray[],                      // where we have stored the filenames from the root directory scan
                                                        unsigned            p_loadedBytes[],                        // where we store the size in bytes for each file 
                                                        char**              p_bufferArray,                          // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                        int                 p_maxFiles,                             // how many files we are allowed to process ( os limitations )
                                                        int                &p_validFiles,                           // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                        unsigned            p_fileSize)                             // maximum size for each file
{
                for (int i = 0; i < p_maxFiles; ++i) 
                    {
                    if (openFile(p_fileNameArray[i]))                                                   // returns true if the file was opened successfully
                        {
                        unsigned f_bytesRead = loadFile(p_bufferArray[p_validFiles], p_fileSize);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
                            p_validFiles++;   
                            }
                        closeFile();
                        }                                                                                           // a false on open file will simply skip the file and move on to the next one, which is fine for our purposes
                    }   
}

bool            CKernel::IsValidFile(                   const char*         pFileName,
                                                        const char*         extension)
{
                if (!pFileName || !extension)
                    return false;
                const char* dot = 0;
                const char* p = pFileName;
                int index = 0;
                
                while (*p)                                          // find last dot and track position
                    {
                    if (*p == '.')
                        dot = p;
                    p++;
                    index++;
                    }
                if (!dot)
                    return false;
                int dotPos = dot - pFileName;
                if (dotPos == 0 || dotPos > 8)
                    return false;
                const char* suffix = dot + 1;
                
                while (*suffix && *extension)                       // compare extension
                    {
                    if (*suffix != *extension)
                        return false;
                    suffix++;
                    extension++;
                    }
                
                return (*suffix == '\0' && *extension == '\0');     // both must end at same time
}

bool            CKernel::scanRoot     (   char**              p_fileNameArray,                        // where we store the valid filenames we find
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
                            if (IsValidFile(f_directoryEntry.chTitle, p_fileExtension[i])) 
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

bool            CKernel::updateUSB      (   const char*         p_deviceName)                           // this is called when we get a plug and play event for the USB, we check if the device is already in our tree of connected devices,
{                                                                                                                   //  if not we update the tree and check again, if we find it this means we can now access the filesystem on the USB and we register a handler for when the USB gets removed so we can update our state accordingly 
                if (m_USBHCI.UpdatePlugAndPlay())                                                                   // Update the tree of connected USB devices
                    {
                    CDevice *f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                    if (f_partitionName != nullptr)
                        {
                        m_bStorageAttached = true;                                                                  // this is a global, volatile variable! 

                        f_partitionName->RegisterRemovedHandler(removeUSB, this);
                        return true;
                        }
                    }
                return false;
}
void            CKernel::removeUSB      (   CDevice            *f_partitionName,                        // this is the handler we register for when the USB gets removed, we set our state to reflect that the storage is no longer attached, we could also unmount the filesystem here if we wanted to be extra safe?
                                                        void               *p_pContext)
{
	            CKernel *pThis = (CKernel *) p_pContext;
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// i need to create the array filecounter[][] containing the max on file, max to load, scanned ang so on values unsing an enum here!