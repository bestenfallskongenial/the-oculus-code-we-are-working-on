# 04-oculus_close_and_closer Integration Roadmap

This roadmap turns the notes in `README.cpp` into a staged implementation plan.

## 1) Current state (what exists)

- **Core intended pipeline is already sketched** in `CKernel::Run()` and wrappers:
  - setup (`prepParameters`)
  - media load (`wrapper_load_sd`, USB path)
  - GL init (`wrapper_init_gl_sd`, `wrapper_init_gl_usb`)
  - loop (`wrapper_io` → `wrapper_modes` → `wrapper_render`)
- **Filesystem primitives are present** (`Mount`, `scanRoot`, `bulkLoad`, `saveBuffer`, `updateUSB`).
- **Buffer logging/display path exists in pieces**:
  - `saveBuffer()` in filesystem layer
  - `storeLog` / `storeMsg` in util
  - framebuffer text drawing in `bufferToScreen.cpp`
- **Menu mode-state model exists conceptually**:
  - `g_centralModeBuffer`/mode table approach
  - per-channel mode application in `menu_final.cpp`
  - button timing function `buttonPing`
- **Overlay menu shader path exists as a prototype** in `the functions.cpp` with menu program/atlas helpers and a dedicated `menu.fsh`.
- **Timing/LFO/BPM feature logic exists** in `features.cpp` and can be adapted to matrix-based state.

## 2) Biggest integration gaps (what is missing)

1. **Build correctness first**
   - Multiple files currently contain syntax/integration breakages (missing semicolons, inconsistent names, mismatched enums, mixed old/new symbols).
   - `kernel.h` is not yet a coherent declaration surface for all globals/state used by cpp files.

2. **Single source of truth for state naming**
   - Old symbols (`g_centralModeBuffer`, `g_currentProgramBuffer`, `g_resultBPM`) and new symbols (`g_centralModeBuffer`, `g_currentProgramBuffer`, `g_resultBPM`) are mixed.
   - `g_inOutMatrix*` vs `inOutMatrix*` usage is inconsistent.

3. **Button/menu behavior contract is not centralized**
   - Desired UX (tap BPM, hold layer A/B, combo for deeper pages) is documented, but not yet encoded as a clean state machine with deterministic transitions.

4. **Wrapper API vs implementations drift**
   - Wrapper names/calls differ (`wrapper_load_sd` vs `wrapper_from_sd`, `wrapper_init_usb` vs `wrapper_load_usb`, missing args in `buttonPing` calls).

5. **Overlay menu pass not wired end-to-end**
   - Overlay compile/link/upload helpers exist, but render-loop integration and data feeding from central mode/bpm state is incomplete.

6. **Logging migration incomplete**
   - Goal is to remove CString/screen/logger dependency paths for runtime logs, but old CString-centric assumptions still appear in run path and helper signatures.

## 3) Recommended sequence (order matters)

### Phase A — Stabilize compile + naming (must do first)

- Freeze and publish canonical naming table:
  - mode store: `g_centralModeBuffer[row][slot]`
  - io matrices: `g_inOutMatrixInt[ch][io_type]`, `g_inOutMatrixFlt[ch][io_type]`
  - bpm/timing arrays: unified `g_timing[...]` or consistently-prefixed split arrays
- Fix all enum token consistency (`trL`/`trH`/`trF` casing, `io_types` entries, file index enums).
- Make wrapper declarations/definitions/call sites consistent.
- Remove/guard prototype-only fragments so project can compile incrementally.

**Exit criterion:** clean compile with stubs where needed.

### Phase B — Kernel contract + wrappers

- Refactor `CKernel::Run()` into strict steps:
  1. boot init
  2. SD load + GL init
  3. main loop: IO → menu/mode update → render
  4. hotplug update path
- Keep wrappers thin orchestrators only; move logic into cohesive subsystem functions.
- Ensure each wrapper has explicit preconditions/postconditions.

**Exit criterion:** deterministic run-order with no hidden side effects.

### Phase C — Menu input state machine

- Implement a small explicit menu controller with:
  - button event decode (`single`, `double`, `long-enter`, `hold-tick`, `release`)
  - layer state (`NONE`, `A`, `B`, `EXT_*`)
  - transition table for combo gestures
- Route ADC channels 4..7 to active menu group only while layer is active.
- Keep pickup behavior (`menu_pickup_flag`) but reset only on actual layer/group changes.

**Exit criterion:** reproducible button navigation behavior matching README intent.

### Phase D — Mode application and timing unification

- Keep `applyModeToChannel()` as the per-channel executor.
- Move BPM/LFO/timing into matrix-compatible structure and consume from there.
- Make one timing source per frame (single `currentTime` policy) to avoid drift.

**Exit criterion:** all modes read/write through unified matrices only.

### Phase E — Overlay menu second pass

- Treat overlay as independent render pass with its own:
  - program
  - uniforms
  - atlas texture
- Feed tile indices from `g_centralModeBuffer` + bpm outputs only (no legacy arrays).
- Define exact draw order: scene first, overlay second.

**Exit criterion:** menu atlas displays live values without affecting primary shader pipeline.

### Phase F — Logging and debug surfaces

- Standardize `storeLog/storeMsg -> ring/static buffer -> {saveBuffer | buffer_to_screen}`.
- Remove direct CString-only pathways from critical logging paths.
- Keep one runtime flag for output target (screen/file/both).

**Exit criterion:** no dependency on logger/screen CString path for required diagnostics.

## 4) Priority TODO checklist

- [ ] Normalize all symbol names (old/new merge pass).
- [ ] Repair `kernel.h` declarations and enum completeness.
- [ ] Align wrapper names/signatures/calls.
- [ ] Add menu controller state machine (single source of truth).
- [ ] Integrate button routing with ADC multiplexing behavior.
- [ ] Finish overlay pass wiring in render loop.
- [ ] Complete timing-array migration to matrix-compatible structure.
- [ ] Finalize log pipeline with file/screen sinks.
- [ ] Add validation hooks (build + smoke runtime logs).

## 5) Suggested immediate next sprint (practical)

1. **Compile recovery PR** (naming + syntax + wrapper alignment only).
2. **Menu controller PR** (buttons/layers/events + tests/log traces).
3. **Overlay integration PR** (menu atlas pass end-to-end).
4. **Timing+mode unification PR** (features to matrix contract).
5. **Logging migration PR** (CString pathway retirement and sink selection).

This keeps each PR reviewable and lowers the chance of regressions during integration.