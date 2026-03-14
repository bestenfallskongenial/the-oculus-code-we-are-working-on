// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"         //  my new memory.cpp with added dma memory allocation for the texture atlas and the overlay fragment shader - also i found some strange errors here.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char**          CKernel::alllocateBufferMEM         (   size_t count, size_t bufferSize) 
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char**          CKernel::alllocateBufferDMA         (   size_t count, 
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::clearBufferMEM        (   char** buffers, size_t count) 
{
                for (size_t i = 0; i < count; ++i)                              // why the elements and than all? why not just all?!
                    {
                    free(buffers[i]);
                    }
                free(buffers);                                                  // why this too?!

                buffers = nullptr;                
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::clearBufferDMA    (   char** buffers, char* rawBlock)
{
                delete[] rawBlock;  // Raw block from new[]
                delete[] buffers;   // Slice table

                rawBlock = nullptr;
                buffers  = nullptr;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::wrapperInitDMA()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferVid = alllocateBufferDMA  (  filecounter[FT_VID][FLD_MAXSD]+filecounter[FT_VID][FLD_MAXUSB],
                                                    filecounter[FT_VID][FLD_SIZE],
                                                   &m_videoBlockBase,
                                                   &m_videoRawBlock,
                                                   &m_videoBlockSize));
        }
    if (bOK)
        {
        bOK = (m_bufferFrA = alllocateBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB],
                                                    filecounter[FRM_BF][FLD_SIZE],
                                                   &m_frameBlockBaseA,
                                                   &m_frameRawBlockA,
                                                   &m_frameBlockSizeA));
        }
    if (bOK)
        {
        bOK = (m_bufferFrB = alllocateBufferDMA  (  filecounter[FRM_BF][FLD_MAXSD]+filecounter[FRM_BF][FLD_MAXUSB],
                                                    filecounter[FRM_BF][FLD_SIZE],
                                                   &m_frameBlockBaseB,
                                                   &m_frameRawBlockB,
                                                   &m_frameBlockSizeB));
        }
    if (bOK)
        {
        bOK = (m_bufferOmt = alllocateBufferDMA  (  filecounter[FT_OMT][FLD_MAXSD]+filecounter[FT_OMT][FLD_MAXUSB],
                                                    filecounter[FT_OMT][FLD_SIZE],
                                                   &m_overlyBlockBase,
                                                   &m_overlayRawBlock,
                                                   &m_overlyBlockSize));
        }
    if (bOK)
        {
        bOK = (m_bufferTex = alllocateBufferDMA  (  filecounter[FT_TEX][FLD_MAXSD]+filecounter[FT_TEX][FLD_MAXUSB],
                                                    filecounter[FT_TEX][FLD_SIZE],
                                                   &m_textureBlockBase,
                                                   &m_textureRawBlock,
                                                   &m_textureBlockSize));
        }
    return bOK;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CKernel::wrapperInitMEM()
{
    bool bOK = true;

    if (bOK)
        {
        bOK = (m_bufferKnl = alllocateBufferMEM( filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB], filecounter[FT_KLN][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferLog = alllocateBufferMEM( filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB], filecounter[LOGGER][FLD_SIZE]));      // 1024 *64 <- the new buffer for log/text files <- saveBuffer() <-
        }                   
    if (bOK)
        {
        bOK = (m_bufferVsh = alllocateBufferMEM( filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB], filecounter[FT_VSH][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferOmf = alllocateBufferMEM( filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB], filecounter[FT_OMF][FLD_SIZE]));
        }
    if (bOK)
        {
        bOK = (m_bufferFsh = alllocateBufferMEM( filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB], filecounter[FT_FSH][FLD_SIZE]));
        }
    return bOK;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperDMAcleanUp            ()
{
                    clearBufferDMA( m_bufferVid, m_videoRawBlock); 

                    clearBufferDMA( m_bufferFrA, m_frameRawBlockA); 

                    clearBufferDMA( m_bufferFrB, m_frameRawBlockB); 

                    clearBufferDMA( m_bufferOmt, m_overlayRawBlock); 

                    clearBufferDMA( m_bufferTex, m_textureRawBlock); 
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void            CKernel::wrapperMEMcleanUp            ()
{
                    clearBufferMEM( m_bufferKnl, filecounter[FT_KLN][FLD_MAXSD]+filecounter[FT_KLN][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferLog, filecounter[LOGGER][FLD_MAXSD]+filecounter[LOGGER][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferVsh, filecounter[FT_VSH][FLD_MAXSD]+filecounter[FT_VSH][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferOmf, filecounter[FT_OMF][FLD_MAXSD]+filecounter[FT_OMF][FLD_MAXUSB] ); 

                    clearBufferMEM( m_bufferFsh, filecounter[FT_FSH][FLD_MAXSD]+filecounter[FT_FSH][FLD_MAXUSB] );        
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------