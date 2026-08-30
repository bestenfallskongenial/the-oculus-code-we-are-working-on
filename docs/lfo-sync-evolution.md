# LFO/BPM synchronization: evolution and design notes

## Scope and evidence

This note compares the requested snapshots in their apparent old-to-new order:

1. `circle/sample/ORIGINAL_BUILD/util.cpp`
2. `circle/sample/WORKING_BUILD/code_util.cpp`
3. `circle/sample/A_the_rnd_wave_direction/code_util.cpp`
4. `circle/sample/B_clean_up_code_from_A/code_util.cpp`
5. `circle/sample/C_here_we_go_now_for_real/code_util.cpp`

It also follows declarations and call sites in each snapshot's `defs_enum.h`,
`defs_member.h`, `kernel.h`, `kernel_run.cpp`, and `code_menu.cpp`. The shared
ChatGPT URL could not be retrieved from this environment: the browser endpoint
returned HTTP 401 and a direct HTTPS request was rejected by the network proxy
with HTTP 403. Consequently, no claim below relies on that conversation.

Git history is weak evidence for the relative age of these copied snapshot
directories. All of `A` (including `LBCT`, `LLCB`, and the enum) first appears in
the repository's root/import commit `a7dd2fa` on 2026-07-21; this proves when it
became tracked here, not when the code was originally written. The old-to-new
ordering above therefore follows the directory names requested by the author,
not commit timestamps.

## Executive answer: `LLCB` versus `LBCT`

They are both edge-detection latches, but they watch different event streams
and drive different behavior.

| Field | Expanded meaning | Producer being watched | Consumer | Event detected | Effect |
|---|---|---|---|---|---|
| `LBCT` | last BPM calculation temporary/latch | `calculate1BPM*()` writes `LBC` after four sufficiently regular triggers | `predict1Beat()` | `LBC` changed, so a new valid BPM estimate was accepted | reset `NBT` to `LBC`, then copy `LBC` into `LBCT` so this happens once |
| `LLCB` | last latched circle buffer | `predict1Beat()` advances `LCB` when the LFO circle expires (and may reset it on multiplier change) | the random branch of `sample1WaveTable()` | `LCB` changed, so this LFO entered a new circle | copy one new random value to the held LFO output, then copy `LCB` into `LLCB` so this happens once |

In short:

```text
LBCT answers: "Have I already reacted to this newly accepted BPM calculation?"
LLCB answers: "Have I already emitted a random value for this LFO circle?"
```

`LLCB` is not a replacement for `LBCT`. Comparing random-wave state with
`LBCT` would couple sample-and-hold changes to BPM recalculations. A steady BPM
can have many LFO circles without another accepted calculation, especially with
multiply/divide settings, so that would hold the random output for the wrong
duration. Conversely, comparing `LCB` with `LLCB` provides one event per actual
LFO circle.

## State model

The current enum values (zero-based) are:

```text
 0 BPM    accepted beats/minute       10 ELP    elapsed time in LFO circle
 1 INTV   average beat interval       11 CYL    current LFO circle length
 2 NBT    predicted next beat         12 SMP    wavetable sample index
 3 LCB    current circle start        13 LTIME  last input trigger
 4 NCB    next circle boundary        14 TIDX   trigger ring write index
 5 LBC    accepted-BPM timestamp      15 TB0    source-0 trigger timestamps
 6 LBCT   consumed LBC latch          16 TB1    source-1 trigger timestamps
 7 LLCB   consumed LCB latch          17 DB0    source-0 interval deltas
 8 LMT    active LFO multiplier       18 DB1    source-1 interval deltas
 9 NLMT   proposed multiplier         19 IREG   audio irregularity
```

The displayed list above deliberately exposes a documentation hazard: the
actual sequence is `BPM=0, INTV, NBT, LCB, NCB, LBC, LBCT, LLCB, LMT, NLMT,
ELP, CYL, SMP, LTIME, TIDX, TB0, TB1, DB0, DB1, IREG`, so `IREG` is index 19
and `LFO_BPM_COUNT` evaluates to **20**, not 15. The `// 15` comment is stale. Code using `LFO_BPM_COUNT` still
allocates the correct width, but hard-coded widths or external serialization
would be unsafe.

Conceptually, the matrix mixes two dimensions:

```text
g_lfoBpmMatrix[row][field]
                   |
                   +-- per-source state when row is p_source (normally 0/1)
                   +-- four-slot ring storage when row is 0..3 and field is TB*/DB*
```

This works because `TB0/TB1` and `DB0/DB1` are separate columns, but it makes
the type system unable to distinguish an LFO/source row from a ring-buffer row.

## End-to-end control flow in the newest snapshot

```text
trigger (internal/frame or external clock)
  |
  v
calculate1BPMnew(source, TB0/TB1, DB0/DB1, triggerTime)
  |
  +-- triggerTime == LTIME? ------------------------------> ignore duplicate
  |
  `-- new trigger
       |-- put timestamp into circular four-sample buffer
       |-- calculate the three chronological deltas
       |-- all three pass ratio tolerance?
       |     `-- yes: average -> BPM + INTV; now -> LBC
       `-- update LTIME and advance TIDX

each render/run iteration, for LFO source 0 and 1
  |
  |-- sample1WaveTable(...)
  |     |-- selected wave >= WAVEFORMS_COUNT (random mode)?
  |     |     |-- LCB != LLCB: latch LCB and copy current RND to output
  |     |     `-- return (output is held otherwise)
  |     `-- ordinary wave: ELP=current-LCB; CYL=NCB-LCB;
  |           map phase to 0..255 (clamped); read wavetable
  |
  `-- predict1Beat(...)
        |-- frameStart >= NBT: NBT += own INTV
        |-- frameStart >= NCB: LCB=NCB; extend NCB using active BPM INTV * LMT
        |-- LBCT != LBC: NBT=LBC; LBCT=LBC
        `-- selected multiplier != LMT:
              re-anchor LCB/NCB to active source's LBC/INTV, then update LMT
```

Because `kernel_run.cpp` samples before it predicts, an `LCB` rollover is seen
by random mode on the following loop/frame, not the loop that advances it. This
is normally only one frame of latency, but it is relevant when describing exact
phase alignment.

## Progression by snapshot

| Snapshot | BPM measurement | Beat/LFO prediction | Sampling and random behavior | Important change or concern |
|---|---|---|---|---|
| `ORIGINAL_BUILD` | One duplicated function handles button and clock; each owns a 4-timestamp buffer and accepts three intervals within roughly 25% | One duplicated block per LFO. It advances `next_beat_time`, rolls circles, detects a new valid BPM with `last_valid_bpm_buffer`, and re-anchors on multiplier change | Two duplicated phase calculations and wavetable reads | Clear arrays, but duplicated source/LFO logic; active BPM selection is a separate function |
| `WORKING_BUILD` | Generalized to `calculate1BPM(source, trigger)`, and most arrays are packed into `g_lfoBpmMatrix` | Generalized `predict1Beat(source, multiplierField)`; `LBCT` is the matrix version of original `last_valid_bpm_buffer` | Generalized `sample1WaveTable()` | Initial packed implementation uses shared `TB`/`DB` columns incorrectly/ambiguously and indexes `g_lfoBpmMatrix[g_lfoBpmMatrix[source][TIDX]][TB]`; no `LLCB` or random sample-and-hold branch |
| `A` | Adds `TB0/TB1` and `DB0/DB1`; parameterizes buffer columns; orders deltas relative to the circular write index; replaces floating tolerance with `BPM_NUM/BPM_DEN` integer comparisons | Experiments with `NLMT` and immediate multiplier activation, while retaining the older predictor as an alternative | Adds random-wave detection and `LCB != LLCB` sample-and-hold | This is the first requested snapshot containing `LLCB`; it solves one-random-value-per-LFO-circle. File retains old/commented alternatives, so intended predictor is less obvious |
| `B` | Keeps the corrected parameterized circular calculation and removes old/commented BPM alternatives | Chooses the older/simple `LMT` predictor, removing the experimental active `NLMT` version | Keeps `LLCB` random sample-and-hold | Primarily cleanup; adds system defaults. `NLMT` remains in the enum although the active predictor does not use it |
| `C` | Same algorithm; internal calls use `g_frameStart` for a consistent frame timestamp | Replaces predictor comparisons against `g_currentTime` with `g_frameStart` | Same sampler, still obtains its own timer tick | Mostly integration/timebase cleanup rather than a synchronization redesign; sampling and prediction still do not use exactly the same captured timestamp |

## Branch-by-branch observations

### BPM acceptance

The three comparisons bound delta 1 and delta 2 relative to delta 0, but do not
perform all pairwise comparisons. With `BPM_NUM/BPM_DEN = 5/4`, for example,
the accepted region is approximately:

```text
d1 < 1.25*d0
d2 < 1.25*d0
d0 < 1.25*d2
```

There is no explicit lower bound on `d1` relative to `d0` or `d2`. A single very
short middle interval can therefore pass if the other conditions pass. This is
inherited from the original implementation.

### Catch-up behavior

Both `NBT` and `NCB` advance using one `if`, not a `while`. If execution stalls
for multiple beat/circle intervals, one call advances only one interval. The
system catches up over successive loops, and wavetable sampling clamps beyond
255 in the meantime. That avoids an out-of-range read, but it is not a true
phase modulo and can visibly park at the final sample.

### Multiplier transition

The active B/C predictor computes a new `NCB` using the **old** `LMT`, then
stores the requested multiplier. Therefore the immediate re-anchored cycle can
retain the previous length; the new multiplier becomes effective on a later
rollover. A's experimental `NLMT` version updates `LMT` first and then builds
`NCB`, explicitly choosing immediate application. This semantic decision should
be documented and tested rather than left accidental.

### Initialization and arithmetic safety

Before a valid circle exists, `CYL = NCB - LCB` can be zero, causing division by
zero in ordinary-wave sampling. Unsigned clock arithmetic also assumes the
timer type and wrap behavior are compatible across every stored field. The C
member declaration uses `unsigned long`, whereas WORKING used `long long`; the
intended clock width and wrap policy should be made explicit.

## Recommended next design (brainstorm)

1. Replace the overloaded matrix with named structures, for example
   `BpmTracker bpm[2]` and `LfoClock lfo[2]`, each containing its own typed ring
   buffer and latches. This makes `LBCT` and `LLCB` self-documenting members.
2. Rename latches by behavior: `consumedBpmTimestamp` and
   `sampleHoldCircleStart`. Rename `LBC` to `lastAcceptedBpmTime`.
3. Capture one `frameNow` and pass it to calculation, prediction, and sampling;
   choose and test whether prediction occurs before sampling.
4. Define multiplier-change policy explicitly: immediate re-phase, apply at the
   next circle, or preserve phase while changing frequency. Each is musically
   plausible but produces different output.
5. Use modulo/catch-up arithmetic for late frames, or a bounded `while`, so one
   long frame cannot leave beat and circle markers permanently behind.
6. Guard `INTV == 0` and `CYL == 0`, and add first-lock initialization that sets
   `LCB`, `NCB`, `NBT`, both latches, and the multiplier coherently.
7. Strengthen rhythm validation using min/max intervals or all pairwise bounds;
   protect the sum and multiplication from overflow before calculating BPM.
8. Separate random waveform identity from the `>= WAVEFORMS_COUNT` sentinel;
   an explicit enum mode prevents any invalid waveform index from silently
   becoming random mode.
9. Add deterministic host-side tests for: first four triggers, duplicate
   triggers, jitter boundary, clock wrap, missed frames, source switching,
   multiplier changes, and exactly one random latch per circle.

## Bottom line

The central evolution is from duplicated standalone arrays/functions to a
parameterized state machine stored in one matrix. `LBCT` came from the original
`last_valid_bpm_buffer` concept and acknowledges accepted tempo estimates.
`LLCB` appears in snapshot A alongside random-wave sample-and-hold and
acknowledges LFO circle transitions. B removes experiments and duplicate code;
C chiefly unifies prediction around `g_frameStart`. The two latches resemble
each other structurally, but represent different clocks and must remain
independent.
