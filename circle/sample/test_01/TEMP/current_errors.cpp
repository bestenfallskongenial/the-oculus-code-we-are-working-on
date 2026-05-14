root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01# make
  CPP   main.o
  CPP   CLogger.o
  CPP   kernel.o
In file included from kernel.h:434,
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
code_menu.cpp:9:40: error: 'modetablecount' was not declared in this scope; did you mean 'modetable'?
    9 |                     for(int i = 0; i < modetablecount; i++)
      |                                        ^~~~~~~~~~~~~~
      |                                        modetable
code_menu.cpp: In member function 'void CKernel::set_mode_length(uint8_t)':
code_menu.cpp:41:17: error: 'g_modeMap' was not declared in this scope
   41 |                 g_modeMap[base + 0][0] = length0;
      |                 ^~~~~~~~~
code_menu.cpp: At global scope:
code_menu.cpp:47:17: error: no declaration matches 'void CKernel::modeMenuAssignGroup(uint8_t, uint8_t)'
   47 | void            CKernel::modeMenuAssignGroup        (   uint8_t menu_id, uint8_t base)
      |                 ^~~~~~~
code_menu.cpp:47:17: note: no functions named 'void CKernel::modeMenuAssignGroup(uint8_t, uint8_t)'
In file included from code_menu.cpp:2:
kernel.h:17:7: note: 'class CKernel' defined here
   17 | class CKernel
      |       ^~~~~~~
code_menu.cpp: In member function 'void CKernel::getChannelModeB(int)':
code_menu.cpp:100:21: error: 'g_modeMap' was not declared in this scope
  100 |                     g_modeMap[p_channel]
      |                     ^~~~~~~~~
code_menu.cpp:103:17: error: 'ModeFunc' was not declared in this scope
  103 |                 ModeFunc fn = g_modeTable[mode];
      |                 ^~~~~~~~
code_menu.cpp:105:21: error: 'fn' was not declared in this scope
  105 |                 if (fn)
      |                     ^~
code_menu.cpp:99:21: warning: unused variable 'mode' [-Wunused-variable]
   99 |                 int mode =
      |                     ^~~~
code_menu.cpp: In member function 'void CKernel::modeBPM(int)':
code_menu.cpp:135:21: error: 'currentTime' was not declared in this scope; did you mean 'g_currentTime'?
  135 |                 if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
      |                     ^~~~~~~~~~~
      |                     g_currentTime
code_menu.cpp:135:36: error: 'g_nextBeatTime' was not declared in this scope
  135 |                 if (currentTime >= g_nextBeatTime[g_activeBpmChannel])
      |                                    ^~~~~~~~~~~~~~
code_menu.cpp: At global scope:
code_menu.cpp:174:17: error: no declaration matches 'void CKernel::updateOvlState(olg_state*, glsl_state*, tex_state*)'
  174 | void            CKernel::updateOvlState             (   olg_state*  o,
      |                 ^~~~~~~
code_menu.cpp:174:17: note: no functions named 'void CKernel::updateOvlState(olg_state*, glsl_state*, tex_state*)'
kernel.h:17:7: note: 'class CKernel' defined here
   17 | class CKernel
      |       ^~~~~~~
make: *** [../../Rules.mk:196: code_menu.o] Error 1
root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01#