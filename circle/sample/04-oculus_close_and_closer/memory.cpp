// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*

        my new memory.cpp with added dma memory allocation for the texture atlas and the overlay fragment shader - also i found some strange errors here.

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
bool            CKernel::wrapperDMAallocate            ()
{
                m_bufferVid         = initDMAbuffer(   VID_FILES_ON_SD + VID_FILES_ON_USB, VID_FILE_SIZE,                      // Video and frame buffers need DMA memory <- lets use the Bok style here too!
                                                                &m_videoBlockBase,
                                                                &m_videoRawBlock,
                                                                &m_videoBlockSize);
                m_bufferFrA         = initDMAbuffer(   1, FRAME_SIZE,
                                                                &m_frameBlockBaseA,
                                                                &m_frameRawBlockA,
                                                                &m_frameBlockSizeA);
                m_bufferFrB         = initDMAbuffer(   1, FRAME_SIZE,
                                                                &m_frameBlockBaseB,
                                                                &m_frameRawBlockB,
                                                                &m_frameBlockSizeB);
                m_bufferOmt         = initDMAbuffer(   1, TEX_FILE_SIZE,                    // <- need to be adapted !!
                                                                &m_overlyBlockBase,             // <- need to be adapted !!
                                                                &m_overlayRawBlock,             // <- need to be adapted !!
                                                                &m_overlyBlockSize);            // <- need to be adapted !!                                                
                m_bufferTex         = initDMAbuffer(   TEX_FILES_ON_SD + TEX_FILES_ON_USB, TEX_FILE_SIZE,
                                                                &m_textureBlockBase,
                                                                &m_textureRawBlock,
                                                                &m_textureBlockSize);

                if (!m_bufferVid || !m_bufferFrA || !m_bufferFrB || !m_bufferOmt || !m_bufferTex)
                    {
                    wrapperDMAcleanUp();
                    return false;
                    }
                return true;
}
*/
bool CKernel::wrapperDMAallocate()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferVid = initDMAbuffer(
                    VID_FILES_ON_SD + VID_FILES_ON_USB,
                    VID_FILE_SIZE,
                    &m_videoBlockBase,
                    &m_videoRawBlock,
                    &m_videoBlockSize));
        }

    if (bOK)
        {
        bOK = (m_bufferFrA = initDMAbuffer(
                    1,
                    FRAME_SIZE,
                    &m_frameBlockBaseA,
                    &m_frameRawBlockA,
                    &m_frameBlockSizeA));
        }

    if (bOK)
        {
        bOK = (m_bufferFrB = initDMAbuffer(
                    1,
                    FRAME_SIZE,
                    &m_frameBlockBaseB,
                    &m_frameRawBlockB,
                    &m_frameBlockSizeB));
        }

    if (bOK)
        {
        bOK = (m_bufferOmt = initDMAbuffer(
                    1,
                    TEX_FILE_SIZE,
                    &m_overlyBlockBase,
                    &m_overlayRawBlock,
                    &m_overlyBlockSize));
        }

    if (bOK)
        {
        bOK = (m_bufferTex = initDMAbuffer(
                    TEX_FILES_ON_SD + TEX_FILES_ON_USB,
                    TEX_FILE_SIZE,
                    &m_textureBlockBase,
                    &m_textureRawBlock,
                    &m_textureBlockSize));
        }

    return bOK;
}
/*
bool            CKernel::wrapperMEMallocate            ()
{
                m_bufferKnl         = initMEMbuffer    (   2                                 , KNL_FILE_SIZE );
                m_bufferVsh         = initMEMbuffer    (   VSH_FILES_ON_SD + VSH_FILES_ON_USB, VSH_FILE_SIZE );
                m_bufferOmf         = initMEMbuffer    (   1                                 , FSH_FILE_SIZE );         // <- new        
                m_bufferFsh         = initMEMbuffer    (   FSH_FILES_ON_SD + FSH_FILES_ON_USB, FSH_FILE_SIZE );

                if (!m_bufferKnl || !m_bufferVsh || !m_bufferOmf || !m_bufferFsh)
                    {
                    wrapperMEMcleanUp();
                    return false;
                    }
                return true;                
}
*/
bool CKernel::wrapperMEMallocate()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferKnl = initMEMbuffer( 2, KNL_FILE_SIZE));
        }
    if (bOK)
        {
        bOK = (m_bufferLog = initMEMbuffer( 8, LOG_SIZE));      // 1024 *64 <- the new buffer for log/text files <- saveBuffer() <-
        }                   
    if (bOK)
        {
        bOK = (m_bufferVsh = initMEMbuffer( VSH_FILES_ON_SD + VSH_FILES_ON_USB, VSH_FILE_SIZE));
        }
    if (bOK)
        {
        bOK = (m_bufferOmf = initMEMbuffer( 1, FSH_FILE_SIZE));
        }
    if (bOK)
        {
        bOK = (m_bufferFsh = initMEMbuffer( FSH_FILES_ON_SD + FSH_FILES_ON_USB, FSH_FILE_SIZE));
        }
    return bOK;
}

void            CKernel::wrapperDMAcleanUp            ()
{
                    clearDMAbuffer( m_bufferVid, m_videoRawBlock); 

                    clearDMAbuffer( m_bufferFrA, m_frameRawBlockA); 

                    clearDMAbuffer( m_bufferFrB, m_frameRawBlockB); 

                    clearDMAbuffer( m_bufferTex, m_textureRawBlock); 

                    clearDMAbuffer( m_bufferOmt, m_overlayRawBlock); 


}
void            CKernel::wrapperMEMcleanUp            ()
{
                    clearMEMbuffer( m_bufferKnl, 2 ); 

                    clearMEMbuffer( m_bufferLog, 8 ); 

                    clearMEMbuffer( m_bufferVsh, VSH_FILES_ON_SD + VSH_FILES_ON_USB); 

                    clearMEMbuffer( m_bufferOmf, 1 ); 

                    clearMEMbuffer( m_bufferFsh, FSH_FILES_ON_SD + FSH_FILES_ON_USB);        
}

char**          CKernel::initMEMbuffer         (   size_t count, size_t bufferSize) 
{
                char** buffers = (char**)malloc(count * sizeof(char*));
#ifdef ALLOC_DEBUG                   
CLogger::Get()->Write("ALLOC-DMA", LogDebug, "buffers = 0x%p", buffers);
#endif // ALLOC_DEBUG

                for (size_t i = 0; i < count; ++i) 
                {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
#ifdef ALLOC_DEBUG                    
CLogger::Get()->Write("ALLOC-DMA", LogDebug, "buffers[%u] = 0x%p", (unsigned)i, buffers[i]);
#endif // ALLOC_DEBUG
                }
#ifdef ALLOC_DEBUG   
CLogger::Get()->Write("ALLOC-DMA", LogDebug, "final buffers = 0x%p count = %u bufferSize = %u", buffers, (unsigned)count, (unsigned)bufferSize);
#endif // ALLOC_DEBUG                
                msleep(100);            // do i really need you here?
                return buffers;
}

char**          CKernel::initDMAbuffer     (   size_t count, 
                                                        size_t bufferSize,
                                                        char** blockBaseOut,
                                                        char** rawBlockOut,
                                                        size_t* alignedSizeOut)
{
                size_t total_size = count * bufferSize;
                size_t aligned_total_size = (total_size + 4095) & ~4095;

                // Allocate +4096 for manual alignment
                char* raw = new (HEAP_DMA30) char[aligned_total_size + 4096];
#ifdef ALLOC_DEBUG   
CLogger::Get()->Write("ALLOC-STD", LogDebug, "raw = 0x%p", raw);
#endif // ALLOC_DEBUG
                char* dma_block = (char*)(((uintptr_t)raw + 4095) & ~4095);  // 4K-aligned
#ifdef ALLOC_DEBUG   
CLogger::Get()->Write("ALLOC-STD", LogDebug, "dma_block (aligned) = 0x%p", dma_block);
#endif // ALLOC_DEBUG
                // Build slice table
                char** buffers = new char*[count];
                for (size_t i = 0; i < count; ++i)
                {
                    buffers[i] = dma_block + i * bufferSize;
#ifdef ALLOC_DEBUG   
CLogger::Get()->Write("ALLOC-STD", LogDebug, "buffers[%u] = 0x%p", (unsigned)i, buffers[i]);
#endif // ALLOC_DEBUG            
                    memset(buffers[i], 0, bufferSize);
                }
#ifdef ALLOC_DEBUG   
CLogger::Get()->Write("ALLOC-STD", LogDebug, "raw = 0x%p dma_block = 0x%p aligned_size = 0x%X", raw, dma_block, (unsigned)aligned_total_size);
#endif // ALLOC_DEBUG
                *blockBaseOut = dma_block;
                *rawBlockOut = raw;
                *alignedSizeOut = aligned_total_size;

                msleep(100);                                                    // for what reason?!
                return buffers;
}

void            CKernel::clearMEMbuffer        (   char** buffers, size_t count) 
{
                for (size_t i = 0; i < count; ++i)                              // why the elements and than all? why not just all?!
                    {
                    free(buffers[i]);
                    }
                free(buffers);                                                  // why this too?!

                buffers = nullptr;                
}

void            CKernel::clearDMAbuffer    (   char** buffers, char* rawBlock)
{
                delete[] rawBlock;  // Raw block from new[]
                delete[] buffers;   // Slice table

                rawBlock = nullptr;
                buffers  = nullptr;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
