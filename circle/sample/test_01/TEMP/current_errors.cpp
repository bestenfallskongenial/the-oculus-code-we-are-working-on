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
code_menu.cpp:11:45: warning: iteration 16 invokes undefined behavior [-Waggressive-loop-optimizations]
   11 |                         g_menuPickUpFlag[i] = false;
      |                         ~~~~~~~~~~~~~~~~~~~~^~~~~~~
code_menu.cpp:9:38: note: within this loop
    9 |                     for(int i = 0; i < MODETABLE_COUNT; i++)
      |                                    ~~^~~~~~~~~~~~~~~~~
code_menu.cpp: In member function 'void CKernel::updateOvlState(olg_state*, glsl_state*, tex_state*)':
code_menu.cpp:195:65: warning: iteration 8 invokes undefined behavior [-Waggressive-loop-optimizations]
  195 |                     s->tile_rect_x[i] = ox + s->kMenuRelPos[i][0] * tw;
      |                                              ~~~~~~~~~~~~~~~~~~~^
code_menu.cpp:184:35: note: within this loop
  184 |                 for (int i = 0; i < MENU_GPU_TILE_COUNT; i++)
code_menu.cpp:218:36: warning: array subscript 8 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  218 |                     s->tile_index[8]  = (GLfloat) ((bpm0 / 1000UL) % 10UL);
      |                     ~~~~~~~~~~~~~~~^
In file included from kernel.h:24,
                 from code_menu.cpp:2:
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:219:36: warning: array subscript 9 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  219 |                     s->tile_index[9]  = (GLfloat) ((bpm0 / 100UL) % 10UL);
      |                     ~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:220:37: warning: array subscript 10 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  220 |                     s->tile_index[10] = (GLfloat) ((bpm0 / 10UL) % 10UL);
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:221:37: warning: array subscript 11 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  221 |                     s->tile_index[11] = (GLfloat) (bpm0 % 10UL);
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:222:37: warning: array subscript 12 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  222 |                     s->tile_index[12] = 10.0f;
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:223:37: warning: array subscript 13 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  223 |                     s->tile_index[13] = (GLfloat) bpm1;
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:224:37: warning: array subscript 14 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  224 |                     s->tile_index[14] = 48.0f;
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
code_menu.cpp:225:37: warning: array subscript 15 is above array bounds of 'GLfloat [8]' {aka 'float [8]'} [-Warray-bounds=]
  225 |                     s->tile_index[15] = 49.0f;
      |                     ~~~~~~~~~~~~~~~~^
defs_struct.h:56:33: note: while referencing 'CKernel::glsl_state::tile_index'
   56 |     GLfloat                     tile_index[MAX_TILES];
      |                                 ^~~~~~~~~~
  CPP   code_parser.o
code_parser.cpp: In member function 'bool CKernel::parse264(h264_state*, char*, char**, char**, size_t*, int, int, u16, u16, u8, u8)':
code_parser.cpp:112:24: warning: variable 'idr_len' set but not used [-Wunused-but-set-variable]
  112 |                 size_t idr_len[MAX_VIDEOS][MAX_FRAMES]       = {0};
      |                        ^~~~~~~
  CPP   code_stubs.o
  CPP   code_util.o
code_util.cpp: In member function 'unsigned int CKernel::get_gl_time(unsigned int)':
code_util.cpp:208:21: warning: no return statement in function returning non-void [-Wreturn-type]
  208 |                     }
      |                     ^
  CPP   code_wrappers.o
code_wrappers.cpp: In member function 'void CKernel::wrapper_init_gl_usb()':
code_wrappers.cpp:394:37: error: 'MAX_VIDEO_WIDTH' was not declared in this scope
  394 |                                     MAX_VIDEO_WIDTH,
      |                                     ^~~~~~~~~~~~~~~
code_wrappers.cpp:395:37: error: 'MAX_VIDEO_HEIGHT' was not declared in this scope
  395 |                                     MAX_VIDEO_HEIGHT,
      |                                     ^~~~~~~~~~~~~~~~
code_wrappers.cpp:396:37: error: 'MAX_VIDEO_PROFILE' was not declared in this scope
  396 |                                     MAX_VIDEO_PROFILE,
      |                                     ^~~~~~~~~~~~~~~~~
code_wrappers.cpp:397:37: error: 'MAX_VIDEO_LEVEL' was not declared in this scope
  397 |                                     MAX_VIDEO_LEVEL);
      |                                     ^~~~~~~~~~~~~~~
code_wrappers.cpp:387:17: error: 'initH264parser' was not declared in this scope
  387 |                 initH264parser  (   &m_vid,
      |                 ^~~~~~~~~~~~~~
code_wrappers.cpp:421:33: error: no matching function for call to 'CKernel::initTexture(CKernel::vtx_state*, CKernel::glsl_state*, CKernel::tex_state*, char**&, unsigned int&, unsigned int&, unsigned int&, int, int)'
  421 |                 initTexture     (   &m_vtx,
      |                 ~~~~~~~~~~~~~~~~^~~~~~~~~~~
  422 |                                     &m_fsh,
      |                                     ~~~~~~~
  423 |                                     &m_tex,
      |                                     ~~~~~~~
  424 |                                     m_bufferTex,
      |                                     ~~~~~~~~~~~~
  425 |                                     filecounter[FT_TEX][FLD_PREV],
      |                                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  426 |                                     filecounter[FT_TEX][FLD_LOADED],
      |                                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  427 |                                     filecounter[FT_TEX][FLD_VALID],
      |                                     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  428 |                                     GL_REPEAT,
      |                                     ~~~~~~~~~~
  429 |                                     GL_REPEAT);
      |                                     ~~~~~~~~~~
In file included from code_wrappers.cpp:2:
kernel.h:110:29: note: candidate: 'void CKernel::initTexture(vtx_state*, glsl_state*, tex_state*, int, int, unsigned int&, GLint, GLint)'
  110 |                 void        initTexture                 (               vtx_state*                      v,
      |                             ^~~~~~~~~~~
kernel.h:110:29: note:   candidate expects 8 arguments, 9 provided
code_wrappers.cpp: In member function 'void CKernel::wrapper_io()':
code_wrappers.cpp:434:17: error: 'readADC' was not declared in this scope
  434 |                 readADC();                  //  we read and dampen the adc each loop
      |                 ^~~~~~~
code_wrappers.cpp:436:47: error: 'm_activePrg' was not declared in this scope
  436 |                 chooseIndexD( ADC_SELECT_PRG, m_activePrg, filecounter[FT_FSH][FLD_VALID])      // we derive the
      |                                               ^~~~~~~~~~~
code_wrappers.cpp: In member function 'void CKernel::wrapper_modes()':
code_wrappers.cpp:451:25: error: 'g_current_menu_layer' was not declared in this scope
  451 |                 switch (g_current_menu_layer)                           // use than the menulayer variable the
      |                         ^~~~~~~~~~~~~~~~~~~~
code_wrappers.cpp:457:25: error: 'modeMenuAssignGroup' was not declared in this scope
  457 |                         modeMenuAssignGroup(1,  0);     // CH0–CH3 parameters in our global array depending on the menulayer
      |                         ^~~~~~~~~~~~~~~~~~~
code_wrappers.cpp:473:17: error: 'setChannelMode' was not declared in this scope; did you mean 'getChannelModeA'?
  473 |                 setChannelMode(0);                               // than we apply the mapped modes for the 8 channels
      |                 ^~~~~~~~~~~~~~
      |                 getChannelModeA
code_wrappers.cpp:482:17: error: 'apply_state_to_led' was not declared in this scope
  482 |                 apply_state_to_led();                                   // than we update the 4 leds depending on the modes - we have to write this function
      |                 ^~~~~~~~~~~~~~~~~~
code_wrappers.cpp: At global scope:
code_wrappers.cpp:485:17: error: no declaration matches 'bool CKernel::wrapper_VCSM()'
  485 | bool            CKernel::wrapper_VCSM               (   )   // for CKernel::Initialize()
      |                 ^~~~~~~
code_wrappers.cpp:485:17: note: no functions named 'bool CKernel::wrapper_VCSM()'
kernel.h:17:7: note: 'class CKernel' defined here
   17 | class CKernel
      |       ^~~~~~~
code_wrappers.cpp:690:17: error: no declaration matches 'void CKernel::wrapperFreeVCSMstruct()'
  690 | void            CKernel::wrapperFreeVCSMstruct      (   ) // here i must check what structs are init/debug and what are runtime code!
      |                 ^~~~~~~
kernel.h:421:29: note: candidate is: 'bool CKernel::wrapperFreeVCSMstruct()'
  421 |                 bool        wrapperFreeVCSMstruct       ();             // for the cleanup of the vcsm structs
      |                             ^~~~~~~~~~~~~~~~~~~~~
kernel.h:17:7: note: 'class CKernel' defined here
   17 | class CKernel
      |       ^~~~~~~
code_wrappers.cpp:726:17: error: no declaration matches 'void CKernel::wrapperFreeMMALstruct()'
  726 | void            CKernel::wrapperFreeMMALstruct      (   ) // here i must check what structs are init/debug and what are runtime code!
      |                 ^~~~~~~
kernel.h:423:29: note: candidate is: 'bool CKernel::wrapperFreeMMALstruct()'
  423 |                 bool        wrapperFreeMMALstruct       ();             // for the cleanup of the mmal structs
      |                             ^~~~~~~~~~~~~~~~~~~~~
kernel.h:17:7: note: 'class CKernel' defined here
   17 | class CKernel
      |       ^~~~~~~
make: *** [../../Rules.mk:196: code_wrappers.o] Error 1
root@WIN-HS0BFGPCN4F:/home/bestenfalls/O_C_Rev_2.0/circle/sample/test_01#