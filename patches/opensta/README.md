# OpenSTA changes

OpenSTA (`src/sta`) is a git submodule.  The changes this branch needs from it
are carried on the `circt-abc` branch of the OpenSTA fork that `.gitmodules`
points at, and `src/sta` is pinned to a commit of that branch.  No patch files
are needed.

| OpenSTA commit | what |
| --- | --- |
| `power: opt-in glitch-power estimate for combinational cells` | glitch-transition estimate added to the switching power of combinational cells with propagated activity; off unless `STA_POWER_GLITCH_ESTIMATE=1` |
| `power: glitch estimate: drop deprecated call ...; add regression test` | cleanup and the `power_glitch_estimate` test |

The branch is based on the OpenSTA commit that upstream OpenROAD pinned
(`f8e13cbf12`).  Upstream OpenSTA has no glitch-power model at that commit.
