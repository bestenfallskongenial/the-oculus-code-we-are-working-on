// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "kernel.h"
#include "global.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*

        my new memory.cpp with added dma memory allocation for the texture atlas and the overlay fragment shader - also i found some strange errors here.

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool            CKernel::memory_allocate            ()
{
                m_bufferVid         = memory_init_dma_buffer(   VID_FILES_ON_SD + VID_FILES_ON_USB, VID_FILE_SIZE,                      // Video and frame buffers need DMA memory
                                                                &m_videoBlockBase,
                                                                &m_videoRawBlock,
                                                                &m_videoBlockSize);
                m_bufferFrA         = memory_init_dma_buffer(   1, FRAME_SIZE,
                                                                &m_frameBlockBaseA,
                                                                &m_frameRawBlockA,
                                                                &m_frameBlockSizeA);
                m_bufferFrB         = memory_init_dma_buffer(   1, FRAME_SIZE,
                                                                &m_frameBlockBaseB,
                                                                &m_frameRawBlockB,
                                                                &m_frameBlockSizeB);
                m_BufferOmt         = memory_init_dma_buffer(   1, TEX_FILE_SIZE,                    // <- need to be adapted !!
                                                                &m_overlyBlockBase,             // <- need to be adapted !!
                                                                &m_overlayRawBlock,             // <- need to be adapted !!
                                                                &m_overlyBlockSize);            // <- need to be adapted !!                                                
                m_bufferTex         = memory_init_dma_buffer(   TEX_FILES_ON_SD + TEX_FILES_ON_USB, TEX_FILE_SIZE,
                                                                &m_textureBlockBase,
                                                                &m_textureRawBlock,
                                                                &m_textureBlockSize);
                m_bufferKnl         = memory_init_buffer    (   2                                 , KNL_FILE_SIZE );
                m_bufferVsh         = memory_init_buffer    (   VSH_FILES_ON_SD + VSH_FILES_ON_USB, VSH_FILE_SIZE );
                m_bufferOmf         = memory_init_buffer    (   1                                 , FSH_FILE_SIZE );         // <- new        
                m_bufferFsh         = memory_init_buffer    (   FSH_FILES_ON_SD + FSH_FILES_ON_USB, FSH_FILE_SIZE );

                if (!m_bufferVid || !m_bufferFrA || !m_bufferFrB || !m_BufferOmt || !m_bufferTex || !m_bufferKnl || !m_bufferVsh || !m_bufferOmf || !m_bufferFsh)
                    {
                    memory_clean_up();
                    return false;
                    }
                return true;
}

void            CKernel::memory_clean_up            ()
{
                    memory_clear_dma_buffer( m_bufferVid, m_videoRawBlock); 

                    memory_clear_dma_buffer( m_bufferFrA, m_frameRawBlockA); 

                    memory_clear_dma_buffer( m_bufferFrB, m_frameRawBlockB); 

                    memory_clear_dma_buffer( m_bufferTex, m_textureRawBlock); 

                    memory_clear_dma_buffer( m_BufferOmt, m_overlayRawBlock); 

                    memory_clear_buffer( m_bufferKnl, 2 ); 

                    memory_clear_buffer( m_bufferVsh, VSH_FILES_ON_SD + VSH_FILES_ON_USB); 

                    memory_clear_buffer( m_bufferOmf, 1 ); 

                    memory_clear_buffer( m_bufferFsh, FSH_FILES_ON_SD + FSH_FILES_ON_USB);    
}

char**          CKernel::memory_init_buffer         (   size_t count, size_t bufferSize) 
{
            //  #define LOG_NAME "ALLOC-STD"

                char** buffers = (char**)malloc(count * sizeof(char*));
                #ifdef ALLOC_DEBUG                   
                    CLogger::Get()->Write("ALLOC-STD", LogDebug, "buffers = 0x%p", buffers);
                #endif // ALLOC_DEBUG

                for (size_t i = 0; i < count; ++i) 
                {
                    buffers[i] = (char*)calloc(bufferSize, sizeof(char));
                #ifdef ALLOC_DEBUG                    
                    CLogger::Get()->Write("ALLOC-STD", LogDebug, "buffers[%u] = 0x%p", (unsigned)i, buffers[i]);
                #endif // ALLOC_DEBUG
                }
                #ifdef ALLOC_DEBUG   
                    CLogger::Get()->Write("ALLOC-STD", LogDebug, "final buffers = 0x%p count = %u bufferSize = %u", buffers, (unsigned)count, (unsigned)bufferSize);
                #endif // ALLOC_DEBUG                
                msleep(100);
                return buffers;
}

char**          CKernel::memory_init_dma_buffer     (   size_t count, 
                                                        size_t bufferSize,
                                                        char** blockBaseOut,
                                                        char** rawBlockOut,
                                                        size_t* alignedSizeOut)
{
            //  #define LOG_NAME "ALLOC-STD"

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

                msleep(100);    // for what reason?!
                return buffers;
}

void            CKernel::memory_clear_buffer        (   char** buffers, size_t count) 
{
                for (size_t i = 0; i < count; ++i) 
                    {
                    free(buffers[i]);
                    }
                free(buffers);

                buffers = nullptr;                
}

void            CKernel::memory_clear_dma_buffer    (   char** buffers, char* rawBlock)
{
                delete[] rawBlock;  // Raw block from new[]
                delete[] buffers;   // Slice table

                rawBlock = nullptr;
                buffers  = nullptr;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
