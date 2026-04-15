//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
        i assume here is all fine, for more details you need to look at wrappers.cpp
*/
//----------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
//----------------------------------------------------------------------------------------------------------------------------------------------------

// new
bool            CKernel::Mount                      (   const char* p_deviceName )   // this or the one above? are they actual the same?
{
                CDevice* f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                m_pFileSystem = new CFATFileSystem;

                if (f_partitionName == 0 || m_pFileSystem == 0 || !m_pFileSystem->Mount(f_partitionName))
                {
                    delete m_pFileSystem;
                    m_pFileSystem = 0;
                    return false;
                }
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

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
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::openFile                   (   const char* p_fileName)
{   
	            g_hFile = m_pFileSystem->FileOpen (p_fileName);                                                     // !!! g_hFile MUST BE GLOBAL !!!
	            if (g_hFile == 0)
		            {
		            return false;
		            }	
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

unsigned        CKernel::loadToBuffer               (   char*       p_buffer,                                                   // destination buffer for the file data
                                                        unsigned    p_bufferSize)                                               // maximum number of bytes to read into the buffer
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

                    m_Watchdog.Start(TIMEOUT);                                                                      // !!! new watchdog !!!
                    }
                return 0;                                                                                           // Buffer full, EOF not reached - this is NOT a success - 0 is equal to false !!!
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::saveFromBufferO          (   const char* p_fileName,
                                                        const char* p_buffer,
                                                        unsigned    p_bufferSize )
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::saveFromBuffer             (   const char* p_deviceName  // we should include also the mount / unmount stuff here
                                                        const char* p_fileName,             
                                                        const char* p_buffer,
                                                        unsigned    p_bufferSize )
{
                if(!Mount( p_deviceName ))
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Mount Device");
                    storeLog( MY_BUFFER, MY_INDEX, p_deviceName);    
#endif // __DEBUG_LOG__                    
                    return false;
                    }
                if (m_pFileSystem == 0 || p_fileName == 0 || p_buffer == 0 || p_bufferSize  == 0)
                    {
                    return false;
                    }
                g_hFile = m_pFileSystem->FileCreate(p_fileName);
                if (g_hFile == 0)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Create File");
                    storeLog( MY_BUFFER, MY_INDEX, p_fileName);    
#endif // __DEBUG_LOG__                    
                    return false;
                    }
                if (m_pFileSystem->FileWrite(g_hFile, p_buffer, p_bufferSize) != p_bufferSize)
                    {
#ifdef __DEBUG_LOG__                        
                    storeLog( MY_BUFFER, MY_INDEX, "Failed to Store File");
                    storeLog( MY_BUFFER, MY_INDEX, p_fileName);      
#endif // __DEBUG_LOG__                                      
                    return false;
                    }
                closeFile();
                UnMount();
#ifdef __DEBUG_LOG__
                storeLog( MY_BUFFER, MY_INDEX, "Successful Stored") // what if i refactor the storeLog to take ( "string", u32, "string", u32, "string", u32, "string", u32 ) or simliar?!
                storeLog( MY_BUFFER, MY_INDEX, p_fileName);
                storeLog( MY_BUFFER, MY_INDEX, "into Buffer");
                storeLog( MY_BUFFER, MY_INDEX, p_buffer, p_bufferSize);
                storeLog( MY_BUFFER, MY_INDEX, "on Device");
                storeLog( MY_BUFFER, MY_INDEX, p_deviceName);
#endif // __DEBUG_LOG__                                 
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::closeFile                  ()	                                                                        // close file ( release g_hFile handle ) 
{
	            if (!m_pFileSystem->FileClose (g_hFile))
		            {
		            return false;
		            }
                return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::bulkLoad                   (   char*       p_fileNameArray[],                                          // where we have stored the filenames from the root directory scan
                                                        unsigned    p_loadedBytes[],                                            // where we store the size in bytes for each file 
                                                        char**      p_bufferArray,                                              // where we store the loaded file data for each file ( or dma/non-dma buffers )
                                                        unsigned    p_maxFiles,                                                 // how many files we are allowed to process ( os limitations )
                                                        unsigned&   p_validFiles,                                               // counts successful loads - we need to keep track here <- MUST initialised with 0
                                                        unsigned&   p_prevFiles,                                                // number of loads from the last call - we need it to init the files correctly
                                                        unsigned    p_fileSize)                                                 // maximum size for each file
{
                p_prevFiles = p_validFiles;                                                                         // boundary before loading

#ifdef __DEBUG_LOG__
                storeLog (MY_BUFFER, MY_INDEX, "BULKLOAD begin max/valid/size", (u32) p_maxFiles, (u32) p_validFiles, (u32) p_fileSize);
#endif // __DEBUG_LOG__

                for (unsigned i = 0; i < p_maxFiles; ++i) 
                    {
                    if (openFile(p_fileNameArray[i]))                                                               // returns true if the file was opened successfully
                        {
                        unsigned f_bytesRead = loadToBuffer(p_bufferArray[p_validFiles], p_fileSize);
                        if (f_bytesRead)
                            {
                            p_loadedBytes[p_validFiles] = f_bytesRead;
#ifdef __DEBUG_LOG__
                            storeLog (MY_BUFFER, MY_INDEX, p_fileNameArray[i], (u32) f_bytesRead);
                            storeLog (MY_BUFFER, MY_INDEX, p_bufferArray, (u32) p_validFiles);
#endif // __DEBUG_LOG__                            
                            p_validFiles++;   
                            }
                        closeFile();
                        }                                                                                           // a false on open file will simply skip the file and move on to the next one
#ifdef __DEBUG_LOG__
                storeLog (MY_BUFFER, MY_INDEX, "BULKLOAD end prev/new/loaded", (u32) p_prevFiles, (u32) p_validFiles, (u32) (p_validFiles - p_prevFiles));
#endif // __DEBUG_LOG__
                    }   
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::IsValidFile                (   const char* pFileName,
                                                        const char* extension)
{
                if (!pFileName || !extension)
                    return false;
                const char* dot = 0;
                const char* p = pFileName;
                int index = 0;
                
                while (*p)                                                                                          // find last dot and track position
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
                
                while (*suffix && *extension)                                                                       // compare extension
                    {
                    if (*suffix != *extension)
                        return false;
                    suffix++;
                    extension++;
                    }
                
                return (*suffix == '\0' && *extension == '\0');                                                     // both must end at same time
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::scanRoot                   (   char**      p_fileNameArray,                                            // where we store the valid filenames we find
                                                        const char* p_fileExtension[],                                          // the array of valid file extensions for this type of file
                                                        unsigned    p_extentionCount,                                           // how many valid file extensions we have in the array above
                                                        unsigned&   p_scannedFiles,                                             // our counter of found files
                                                        unsigned    p_maxFiles )                                                // how many files are allowed to scan and stored in the array
{
                p_scannedFiles = 0;

                TDirentry           f_directoryEntry;
                TFindCurrentEntry   f_currentDirectoryEntry;
                
                unsigned            f_nextEntry                 = m_pFileSystem->RootFindFirst(&f_directoryEntry, &f_currentDirectoryEntry);

                if(f_nextEntry == 0)                                                                                // Initial directory access failed
                        {
                    return false;
                        }
                while (f_nextEntry != 0 && p_scannedFiles < p_maxFiles) 
                    {
                    if (!(f_directoryEntry.nAttributes & FS_ATTRIB_SYSTEM)) 
                        {
                        for (unsigned i = 0; i < p_extentionCount; ++i)
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
                return true;                                                                                        // Return true to indicate successful scan (even if no valid files were found)    
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

bool            CKernel::updateUSB                  (   const char* p_deviceName )                                               // get a plug and play event for the USB check if the device is already a connected devices
{
                if (m_USBHCI.UpdatePlugAndPlay())                                                                   // Update the tree of connected USB devices
                    {
                    CDevice* f_partitionName = m_DeviceNameService.GetDevice(p_deviceName, TRUE);
                    if (f_partitionName != nullptr)
                        {
                        m_bStorageAttached = true;                                                                  // !!! m_bStorageAttached is global, volatile variable !!! 

                        f_partitionName->RegisterRemovedHandler(removeUSB, this);
                        return true;
                        }
                    }
                return false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::removeUSB                  (   CDevice*    f_partitionName,                                            // this is the handler we register for when the USB gets removed
                                                        void*       p_pContext )
{
	            CKernel *pThis = (CKernel *) p_pContext;                                                            // we could also unmount the filesystem here if we wanted to be extra safe?
	            assert (pThis != 0);
	        //  assert (pThis->m_bStorageAttached);
	            pThis->m_bStorageAttached = FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------
#define __DEBUG_LOG__
#define MY_BUFFER m_bufferLog
#define MY_INDEX vc04_logIndex  // vc04_logIndex is a public member variable
//----------------------------------------------------------------------------------------------------------------------------------------------------

char**          CKernel::allocBufferMEM             (   size_t      count, 
                                                        size_t      bufferSize ) 
{
                char** buffers = (char**)malloc(count * sizeof(char*));
#ifdef ALLOC_DEBUG                   ␊
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA buffers", (u32) buffers);
#endif // ALLOC_DEBUG
                for (size_t i = 0; i < count; ++i) 
                {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
#ifdef ALLOC_DEBUG                    
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA buffers[i]", (u32) i, (u32) buffers[i]);
#endif // ALLOC_DEBUG
                }
#ifdef ALLOC_DEBUG   

                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA final", (u32) buffers, (u32) count, (u32) bufferSize);
#endif // ALLOC_DEBUG         
                msleep(100);            // do i really need you here?
                return buffers;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

char**          CKernel::allocBufferDMA             (   size_t      count, 
                                                        size_t      bufferSize,
                                                        char**      blockBaseOut,
                                                        char**      rawBlockOut,
                                                        size_t*     alignedSizeOut )
{
                size_t total_size = count * bufferSize;
                size_t aligned_total_size = (total_size + 4095) & ~4095;

                
                char* raw = new (HEAP_DMA30) char[aligned_total_size + 4096];   // Allocate +4096 for manual alignment - rather an artifact because it seems that the alignment happens by itself
#ifdef ALLOC_DEBUG   
            //  storeLog( MY_BUFFER, MY_INDEX, "ALLOC-STD raw", (u32) raw);
#endif // ALLOC_DEBUG
                char* dma_block = (char*)(((uintptr_t)raw + 4095) & ~4095);  // 4K-aligned
#ifdef ALLOC_DEBUG                  
            //  storeLog( MY_BUFFER, MY_INDEX, "ALLOC-STD dma_block", (u32) dma_block);
                storeLog( MY_BUFFER, MY_INDEX, "Alloc RAW:", (u32) raw, " / Alloc DMA Block:", (u32) dma_block )
#endif // ALLOC_DEBUG
                char** buffers = new char*[count];  // Build slice table
                for (size_t i = 0; i < count; ++i)
                    {
                    buffers[i] = dma_block + i * bufferSize;
#ifdef ALLOC_DEBUG   
                storeLog( MY_BUFFER, MY_INDEX, "ALLOC-DMA buffers[", (u32) i, "] -" (u32) sizeof(buffers[i]));  // !!!! MY NEW storeLog() FUNCTION and why all the logging above when i have all together below?
#endif // ALLOC_DEBUG  
                    memset(buffers[i], 0, bufferSize);
                    }
#ifdef ALLOC_DEBUG   
                storeLog( MY_BUFFER, MY_INDEX,  "ALLOC-DMA RAW", (u32) raw,
                                                "in Block", (u32) dma_block, 
                                                "Buffer Size", (u32) total_size, 
                                                "Alligned Size",  (u32) aligned_total_size); // correct?!?
#endif // ALLOC_DEBUG
                *blockBaseOut = dma_block;
                *rawBlockOut = raw;
                *alignedSizeOut = aligned_total_size;

                msleep(100);                                                    // for what reason?!
                return buffers;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::clearBufferMEM             (   char**      buffers, 
                                                        size_t      count) 
{
                for (size_t i = 0; i < count; ++i)                              // why the elements and than all? why not just all?!
                    {
                    free(buffers[i]);
                    }
                free(buffers);                                                  // why this too?!

                buffers = nullptr;                
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void            CKernel::clearBufferDMA             (   char**      buffers, 
                                                        char*       rawBlock )
{
                delete[] rawBlock;  // Raw block from new[]
                delete[] buffers;   // Slice table

                rawBlock = nullptr;
                buffers  = nullptr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
