root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01# make
  CPP   main.o
  CPP   CLogger.o
  CPP   kernel.o
In file included from kernel.h:438,
                 from kernel.cpp:1:
defs_member.h: In constructor 'CKernel::CKernel()':
defs_member.h:37:37: warning: 'CKernel::m_SMITxDMA' will be initialized after [-Wreorder]
   37 |                 CDMAChannel         m_SMITxDMA;
      |                                     ^~~~~~~~~~
defs_member.h:11:45: warning:   'CEMMCDevice CKernel::m_EMMC' [-Wreorder]
   11 |                 CEMMCDevice                 m_EMMC;
      |                                             ^~~~~~
kernel.cpp:3:33: warning:   when initialized here [-Wreorder]
    3 |                                 CKernel::CKernel                                                (       void )
      |                                 ^~~~~~~
defs_member.h:69:33: warning: 'CKernel::m_bStorageAttached' will be initialized after [-Wreorder]
   69 |     volatile    bool            m_bStorageAttached  = false;
      |                                 ^~~~~~~~~~~~~~~~~~
defs_member.h:14:37: warning:   'CFATFileSystem* CKernel::m_pFileSystem' [-Wreorder]
   14 |                 CFATFileSystem*     m_pFileSystem;
      |                                     ^~~~~~~~~~~~~
kernel.cpp:3:33: warning:   when initialized here [-Wreorder]
    3 |                                 CKernel::CKernel                                                (       void )
      |                                 ^~~~~~~
  CPP   kernel_initialize.o
  CPP   kernel_run.o
  CPP   code_data.o
  CPP   code_graphics.o
  CPP   code_hardware.o
  CPP   code_logging.o
  CPP   code_menu.o
code_menu.cpp: In member function 'void CKernel::resetPickUpFlags()':
code_menu.cpp:7:21: error: 'g_menu_mode_new' was not declared in this scope
    7 |                 if (g_menu_mode_new != g_menu_mode_old) /* or simply memset(menu_pickup_flag, 0, 16 * sizeof(bool)); ? **** */
      |                     ^~~~~~~~~~~~~~~
code_menu.cpp:7:40: error: 'g_menu_mode_old' was not declared in this scope
    7 |                 if (g_menu_mode_new != g_menu_mode_old) /* or simply memset(menu_pickup_flag, 0, 16 * sizeof(bool)); ? **** */
      |                                        ^~~~~~~~~~~~~~~
code_menu.cpp: In member function 'void CKernel::mapMenuGroup(uint8_t, uint8_t)':
code_menu.cpp:49:21: error: 'g_menu_mode_new' was not declared in this scope
   49 |                 if (g_menu_mode_new != menu_id)
      |                     ^~~~~~~~~~~~~~~
code_menu.cpp:57:54: warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]
   57 |                 if (!g_menuPickUpFlag[base + 0] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 0])
      |                                                    ~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
code_menu.cpp:67:54: warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]
   67 |                 if (!g_menuPickUpFlag[base + 1] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 1])
      |                                                    ~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
code_menu.cpp:77:54: warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]
   77 |                 if (!g_menuPickUpFlag[base + 2] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 2])
      |                                                    ~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
code_menu.cpp:87:54: warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]
   87 |                 if (!g_menuPickUpFlag[base + 3] && v == g_centralModeBuffer[g_currentProgramBuffer][base + 3])
      |                                                    ~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
code_menu.cpp: In member function 'void CKernel::modeBPM(int)':
code_menu.cpp:135:21: error: 'currentTime' was not declared in this scope; did you mean 'g_currentTime'?
  135 |                 if (currentTime >= g_nextBeatTime[g_activeBpmChannel])      // <- currentTime should be g_currentTime -> global member set during Run()
      |                     ^~~~~~~~~~~
      |                     g_currentTime
code_menu.cpp:135:36: error: 'g_nextBeatTime' was not declared in this scope
  135 |                 if (currentTime >= g_nextBeatTime[g_activeBpmChannel])      // <- currentTime should be g_currentTime -> global member set during Run()
      |                                    ^~~~~~~~~~~~~~
code_menu.cpp: In member function 'void CKernel::updateOvlState(olg_state*, glsl_state*, tex_state*)':
code_menu.cpp:215:48: error: 'g_resultBPM' was not declared in this scope
  215 |                     const unsigned long bpm0 = g_resultBPM[0] % 10000UL;
      |                                                ^~~~~~~~~~~
make: *** [../../Rules.mk:196: code_menu.o] Error 1
root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01#