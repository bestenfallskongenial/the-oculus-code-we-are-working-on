// STYLE: terse. no prose. 1 line. facts only. no filler.
// Goal: copy 01-oculus_mendax, cleanup incrementally, simplify menu + display path.

DONE
- Copied files: main.cpp, memory.cpp, gfx.cpp, io.cpp, filesystem.cpp, util.cpp (same as 01 baseline).
- Split old menu.cpp into focused units: menu_mode.cpp, menu_mapping.cpp, menu_led.cpp.
- Started display cleanup in display.cpp with menu tables and inline cleanup notes.

LEFT
- Copy/add missing core files required by current includes/build: kernel.h, kernel.cpp, global.h, global.cpp, LED.cpp.
- Align Makefile OBJS with actual cleanup layout (remove menu.o/global.o/kernel.o/LED.o or add replacements).
- Decide menu migration boundary: keep mode logic in menu_mode/menu_mapping or move more mode tables out of display.cpp.
- Replace placeholder comments in menu_mode.cpp and display.cpp with concrete task notes.
- Build + boot smoke test after each file migration step.