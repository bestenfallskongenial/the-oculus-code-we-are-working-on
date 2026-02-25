#include kernel.h

boolean         CKernel::Initialize (void)
{
boolean bOK = TRUE;

                if (bOK)
                    {
                    bOK = m_Screen.Initialize(); // we need to initialize the screen before the logger, because the logger needs a target device and the screen is the default target device if no other is specified
                    }
                if (bOK)
                    {
                    bOK = m_Serial.Initialize(115200); // we need to initialize the serial port before the logger, because the logger needs a target device and the serial port is the default target device if no other is specified
                }   
                if (bOK)
                    {
                    CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
                    if (pTarget == 0)
                    {
                        pTarget = &m_Screen;
                    }
                    bOK = m_Logger.Initialize(pTarget);
                    }   
                if (bOK)
                {
                    bOK = m_Interrupt.Initialize();
                    }
                if (bOK)
                    {
                    bOK = display_startup_screen(&state); // we need to initialize the display before the timer, because the timer is used in the display initialization for the startup screen and if the timer is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_Timer.Initialize(); // we need to initialize the timer before the filesystem, because the filesystem uses the timer for delays and if the timer is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_EMMC.Initialize(); // we need to initialize the emmc before the filesystem, because the filesystem uses the emmc for storage and if the emmc is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    bOK = m_USBHCI.Initialize(); // we need to initialize the USBHCI before the filesystem, because the filesystem uses the USBHCI for storage and if the USBHCI is not initialized, it will cause a crash
                    }
                if (bOK)
                    {
                    m_USBHCI.UpdatePlugAndPlay(); // we need to call UpdatePlugAndPlay() after initializing the USBHCI to detect if a USB device is already connected at startup, because if we don't call it, the filesystem will not detect the USB device and if the user has a USB device connected at startup, it will cause a crash when the filesystem tries to access the USB device
                    m_Timer.MsDelay(1000);  // we need to delay after initializing the USBHCI to give it time to detect the USB device and if we don't delay, the filesystem will not detect the USB device and if the user has a USB device connected at startup, it will cause a crash when the filesystem tries to access the USB device
                    }
                if (bOK)
                    {
                    bOK = memory_allocate(); // we need to allocate memory before the VCHIQ, because the VCHIQ uses the memory for communication and if the memory is not allocated, it will cause a crash when the VCHIQ tries to access the memory
                    }
                if (bOK)
                    {
                    bOK = m_VCHIQ.Initialize(); // we need to initialize the VCHIQ before the shared memory, because the shared memory uses the VCHIQ for communication and if the VCHIQ is not initialized, it will cause a crash when the shared memory tries to access the VCHIQ
                    m_Timer.MsDelay(200);
                    }
                if (bOK)
                {
                    bcm_host_init();  // we need to initialize the bcm_host before the graphics, because the graphics uses the bcm_host for initialization and if the bcm_host is not initialized, it will cause a crash when the graphics tries to access the bcm_host
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    gfx_init_OGL(&state); // we need to initialize the graphics before the shared memory, because the shared memory uses the graphics for initialization and if the graphics is not initialized, it will cause a crash when the shared memory tries to access the graphics
                    m_Timer.MsDelay(200);
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMInitialize(); // we need to initialize the shared memory before importing the memory, because we need to have the shared memory initialized to import the memory and if the shared memory is not initialized, it will cause a crash when we try to import the memory
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMimportMemory(m_videoBlockBase, m_videoBlockSize, 0); // we can do all vcsm imports in one bOK block, dont you think?
                    m_SharedMemory.VCSMLockMemory(0);
                }
                if (bOK)
                {
                    bOK = m_SharedMemory.VCSMimportMemory(m_frameBlockBaseA, m_frameBlockSizeA, 1);
                    bOK = m_SharedMemory.VCSMLockMemory(1);
                }
                if (bOK)
                    {
                    bOK = m_SharedMemory.VCSMimportMemory(m_frameBlockBaseB, m_frameBlockSizeB, 2);
                    bOK = m_SharedMemory.VCSMLockMemory(2);
                    }
                if (bOK)// new only for the system texture, the overlay atlas, i guess i will rework the parser too, a dedicated bpm parser, one for h264
                    {
                    bOK = m_H264SystemParser.ParseInitialize(   TEX_FILE_SIZE,           // max_tex_size
                                                                8,                  // max_textures
                                                                
                                                                m_videoBlockBase,   // aka max vid_size ??
                                                                8,                  // max_videos   h264
                                                                MAX_FRAMES,         // max_frames   h264
                                                                VIDEO_WIDTH,        // max_width    h264
                                                                VIDEO_HEIGHT,       // max_height   h264
                                                                BASELINE_PROFILE,   // max_profile  h264
                                                                41                  // max_level    h264
                                                            );
                    }                    
                if (bOK)
                    {
                    bOK = m_H264Parser.ParseInitialize      (   TEX_FILE_SIZE,                  // we will rework the parser into a dedicated bpm and one for h264 ( i still dont know what codes )
                                                                8,
                                                                
                                                                m_videoBlockBase,
                                                                8,
                                                                MAX_FRAMES,
                                                                VIDEO_WIDTH,
                                                                VIDEO_HEIGHT,
                                                                BASELINE_PROFILE,
                                                                41
                                                            );
                    }
                if (bOK)
                    {
                    bOK = m_H264Decoder.MMALinitialize(     m_SharedMemory.m_vc_handle[0],      // means also that i will rework the decode,
                                                          /*m_SharedMemory.m_vc_pointer[0],*/   // still a blackbox,
                                                            m_videoBlockSize,                   // still about 90% finish
                                                            m_SharedMemory.m_vc_handle[1],
                                                          /*m_SharedMemory.m_vc_pointer[1],*/
                                                            m_SharedMemory.m_vc_handle[2],
                                                          /*m_SharedMemory.m_vc_pointer[2],*/
                                                            m_frameBlockSizeA,
                                                            VIDEO_WIDTH,
                                                            VIDEO_HEIGHT,
                                                            state.display,
                                                            state.context
                                                            );
                    }
                if (bOK)
                    {
                    m_Watchdog.Start(TIMEOUT);
                    }
                if (bOK)
                    {
                    bOK = m_SPIMaster.Initialize();
                    }
                if (bOK)
                    {
                    m_ChipSelectPin.Write(LOW);
                    }
                if (bOK)                                                                            // we will get rid of m_button in this version
                    {
                    bOK = m_Button_A.Initialize();
                    m_Button_A.RegisterEventHandler(io_event_button_A, this);
                    }
                if (bOK)
                    {
                    bOK = m_Button_B.Initialize();
                    m_Button_B.RegisterEventHandler(io_event_button_B, this);
                    }

                return bOK;

                }