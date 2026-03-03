# Integration Roadmap — `04-oculus_close_and_closer`

This document turns the notes in `README.cpp` into an execution plan:
- what is already present,
- what is missing,
- and what to do next (ordered in integration phases).

---

## 1) What we already have

## A. Pipeline skeleton is already split into wrappers
You already have a clean conceptual split for runtime flow:
- `wrapper_io()`
- `wrapper_modes()`
- `wrapper_render()`

And a boot/load side split:
- `wrapper_from_sd()` / `wrapper_load_usb()`
- `wrapper_init_gl_sd()` / `wrapper_init_gl_usb()`

This structure is exactly the right basis for the `load -> init -> io -> modes -> render` pipeline you described.

## B. Core mode/menu architecture exists
- `g_centralModeBuffer`-based mode handling is present in `menu_final.cpp`.
- `modeMenuAssignGroup()`, `applyModeToChannel()` and mode functions (`modeADC`, `modeTRG`, `modeBPM`, `modeLF1`, `modeLF2`) exist and reflect the intended control model.

## C. Unified input/output matrix concept exists
- The design is already moving toward `g_inOutMatrixInt[][]` and `g_inOutMatrixFlt[][]` as the central per-channel transport (raw/in/out/rnd/lfo/audio/threshold).

## D. Overlay menu shader prototype exists
- A concrete menu GPU state + tile-based overlay implementation exists in `the functions.cpp`:
  - menu state struct,
  - menu shader compilation/link helpers,
  - atlas upload,
  - init/update/draw/shutdown functions.

## E. File IO primitives for logging and asset loading exist
- `Mount/UnMount/openFile/loadFile/saveBuffer/closeFile/bulkLoad/scanRoot/updateUSB` are in place.
- `saveBuffer()` gives the exact hook needed for replacing ad-hoc logger output with buffer-backed logs.

---

## 2) What is currently missing / blocking integration

## A. Header and symbol consistency is not stabilized
`kernel.h` currently mixes declarations, globals, enums, and storage in a way that is not yet coherent as a contract surface.
Practical blockers:
- enum/symbol drift (`trH` vs `trH`, etc.),
- mixed old/new names (`g_centralModeBuffer` vs `g_centralModeBuffer`, `g_inOutMatrix*` vs `inOutMatrix*`),
- values and globals declared in header scope that should be centralized.

## B. Several source files are still in sketch state (compile blockers)
There are obvious syntax/consistency issues in key integration files (missing separators, comment fragments, inconsistent identifiers/includes), especially in:
- `kernel_run.cpp`,
- `wrappers.cpp`,
- `menu_final.cpp`,
- `features.cpp`.

These need a compile-pass stabilization before any behavioral integration can be trusted.

## C. Button/menu state machine is not fully encoded yet
You have the event intent (single/hold/double/layer switch) and timestamp structure, but the complete deterministic state machine that maps events to `menu_id`, layer selection, and assignment groups is incomplete.

## D. Overlay shader pass is implemented, but not fully wired into render lifecycle
Menu pass helpers exist, but integration points are still incomplete:
- guaranteed one-time menu asset load,
- dedicated menu shader + menu atlas initialization in the init path,
- clean call site in render order (main scene first, overlay second),
- cleanup and reload behavior under USB refresh/reset.

## E. Timing arrays are not yet unified into the same matrix philosophy
Your BPM/LFO code has much of the logic, but related timing state is still spread over dedicated arrays and static locals. The migration plan to a unified timing state table is not yet done.

## F. Logging migration is partial
`saveBuffer()` and screen-buffer drawing utilities exist, but the full replacement of `CString`/`screen/logger` call patterns with one unified logging buffer mechanism is not completed.

---

## 3) Recommended integration order (roadmap)

## Phase 0 — Contract freeze (do first)
Goal: stop naming drift and define one authoritative vocabulary.

1. Freeze a canonical naming table (old -> new):
   - `g_centralModeBuffer` -> `g_centralModeBuffer`
   - `g_inOutMatrix*` -> `inOutMatrix*` (or the opposite, but pick one)
   - threshold keys (`trL/trH/trF`) consistently everywhere.
2. Move global state ownership to a single place (prefer `.cpp` definitions with `extern` in header).
3. Keep `kernel.h` as API surface only (types, constants, class declarations).

Deliverable: builds parse cleanly and all symbols resolve consistently.

## Phase 1 — Make the wrappers executable end-to-end
Goal: ensure loop stages are real and callable.

1. Stabilize signatures and includes in `kernel_initialize.cpp`, `kernel_run.cpp`, and `wrappers.cpp`.
2. Enforce runtime order:
   - init: memory -> files -> parsers -> GL resources,
   - loop: io -> mode update -> render,
   - optional USB hot-load branch.
3. Add minimal status flags so each stage can fail loudly but safely.

Deliverable: one deterministic loop that can run without undefined calls.

## Phase 2 — Button-driven menu state machine
Goal: translate physical button behavior into deterministic menu layer/mode control.

1. Finalize `buttonPing()` events (`single`, `double`, `hold_tick`, `release`).
2. Implement one consumer/state machine that maps events to:
   - active menu layer A/B,
   - sublayers (LFO, attenuation, sensitivity, filesystem ops),
   - BPM tap capture.
3. Keep all outputs of this state machine in a compact struct (e.g., `MenuNavState`).

Deliverable: button behavior is deterministic and testable without rendering.

## Phase 3 — Central matrix + mode application hardening
Goal: make the matrix the only source of truth for channel processing.

1. Ensure ADC write path populates only canonical matrix fields (`raw`, `in`, etc.).
2. Keep `modeMenuAssignGroup()` and `applyModeToChannel()` pure and index-safe.
3. Add bounds checks for mode indices (`menu_map_max`, capability table).

Deliverable: mode engine works independently of UI and render.

## Phase 4 — Timing/BPM/LFO unification
Goal: make timing state structurally similar to channel matrix state.

1. Introduce a compact timing-state table/struct array (per clock source/lfo lane).
2. Migrate `calculateBPM()`, `predictedNextBeat()`, `sampleWaveTable()` to consume/write that state.
3. Keep exposed outputs mapped back into matrix slots (`lf1`, `lf2`, bpm-dependent outputs).

Deliverable: timing logic no longer scattered across unrelated globals/statics.

## Phase 5 — Overlay menu second pass integration
Goal: always render menu as explicit second pass.

1. Load menu `.fsh` + atlas once from SD in startup path.
2. Compile/link menu program and bind uniforms via dedicated init function.
3. In render loop:
   - draw scene pass,
   - update menu tile indices/layout,
   - draw menu overlay pass.
4. Add shutdown/reload hooks.

Deliverable: menu pass is independent, reproducible, and does not interfere with scene shaders.

## Phase 6 — Logging migration completion
Goal: remove hard dependency on `CString`/legacy logger flow.

1. Introduce one central log buffer writer API (`storeLog`, `storeMsg`, `nextline`).
2. Route debug outputs to:
   - `saveBuffer()` file sink,
   - optional `buffer_to_screen_*` sink.
3. Gate verbosity by build flag/menu setting.

Deliverable: unified logging with persistent capture + optional on-screen diagnostics.

---

## 4) Suggested immediate next sprint (high-value)

1. **Compile stabilization sprint** (no feature work):
   - resolve syntax/name mismatches in `kernel_run.cpp`, `wrappers.cpp`, `menu_final.cpp`, `features.cpp`.
2. **State-machine sprint**:
   - finalize button consumer and menu layer transitions.
3. **Render integration sprint**:
   - wire menu pass into render order with one-time menu asset init.

If you execute these 3 sprints first, all later work (filesystem actions from menu, firmware/load flags, parameter persistence) becomes much safer.

---

## 5) Definition of Done (integration)

You are “integrated” when all are true:
1. Build completes without symbol drift warnings/errors.
2. Runtime loop order is fixed and observable.
3. Buttons deterministically navigate layers and assign parameters.
4. `g_centralModeBuffer` is the single mode source across channels.
5. Menu overlay renders every frame as second pass.
6. Logging works to file and optional screen buffer without legacy logger dependence.
7. USB re-scan/reload path does not corrupt active render state.