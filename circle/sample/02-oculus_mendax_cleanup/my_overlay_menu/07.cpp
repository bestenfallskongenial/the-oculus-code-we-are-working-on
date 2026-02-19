                if( filesystem_mount("emmc1-1", 
                    SCANED_FILES_VSH,  VSH_LOADED_BYTES, VSH_SD,    
                -> my new overlay shader loader variables    
                    SCANED_FILES_FSH,  FSH_LOADED_BYTES, FSH_SD,
                    SCANED_FILES_TEX,  TEX_LOADED_BYTES, TEX_SD,
                -> my new texture atlas variables!
                    SCANED_FILES_VID,  VID_LOADED_BYTES, VID_SD
                ))
                    {                   
                    // Flush CPU->RAM so the VPU sees the loaded bitstream
                    CleanAndInvalidateDataCacheRange((uintptr_t)m_videoBlockBase, (size_t)m_videoBlockSize); // we need to flush the cache after loading the video bitstream to make sure that the VPU can see the updated data in memory, otherwise it may cause a crash when the VPU tries to access the video data and if the video data is not updated in memory, it will cause a crash when the VPU tries to access the video data

                    gfx_init_v_buffer(&state);

                    parser_bmp(TEX_LOADED_OLD,TEX_LOADED_NEW);
                -> a parser call for my overlay texture atlas, by the way, as long as not forbitten by gl es 2.0 - we use the same format 24 bit rgb!     
                    parser_h264(VID_LOADED_OLD,VID_LOADED_NEW);
    
                    gfx_init_vshaders(&state, VSH_LOADED_OLD, VSH_LOADED_NEW);
                -> my new overlay shader init
                    gfx_init_fshaders(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                -> my new overlay shader init ( shader binding?! )    
                    gfx_init_programs(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                -> my new overlay shader uniform init    
                    gfx_init_uniforms(&state, FSH_LOADED_OLD, FSH_LOADED_NEW);
                -> my new overlay texture atlas init
                //  gfx_init_textures(&state, TEX_LOADED_OLD, TEX_LOADED_NEW); // we commented out the texture loading for the test of the h264 decoder to texture mechanism, but we need to load the textures at least one time for the brandlogo and the ui, so we load them here from the emmc, but we will not load them again from the usb later, so that we can test the h264 decoder to texture mechanism without interference of the usb loading

                    m_Watchdog.Start(TIMEOUT);

                    m_Timer.MsDelay(100);
 
                -> no update of indexes neccessary because we will never load our texture menu shader / atlas again!    
                    VSH_LOADED_OLD = VSH_LOADED_NEW;    // also 0 at bytesize calc beacuse i loaded only here
                    FSH_LOADED_OLD = FSH_LOADED_NEW;
                    TEX_LOADED_OLD = TEX_LOADED_NEW;    // we need to reset the couter casues issues later at byte size calculation
                    VID_LOADED_OLD = VID_LOADED_NEW;
                    }

means you have to engage in the files 

circle/sample/01-oculus_mendax/filesystem.cpp
and 
circle/sample/01-oculus_mendax/gfx.cpp

for reference 

also 

do we need to allocate the memory accordingly right means we need to look at 

circle/sample/01-oculus_mendax/memory.cpp/bool CKernel::memory_allocate too, right?

your job is to provide a NEW file circle/sample/02-oculus_mendax_cleanup/gpu_menu_3.cpp 

you will add here the refactored versions of the contained functions 
AND the refactored functions i imentioned above, understood?
you will not thange the files! you will put all fucnktions in this file, clearly marked by commentary,
its 100% my concern to integrate them lateer in my code.

do you have any questions?
