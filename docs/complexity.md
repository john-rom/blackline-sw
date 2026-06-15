# Complexity Notes

This document records the runtime and memory behavior of Blackline/SW algorithms
that are part of the portable software reference implementation.

## 1D Terrain Marker Lookup

Algorithm:
Linear scan over a caller-provided 1D terrain buffer.

Purpose:
Locate the first agent or target marker in a bounded terrain buffer.

Functions:
- `bl_terrain_locate_agent_1d`
- `bl_terrain_locate_target_1d`

Time complexity:
`O(n)`, where `n` is the supplied terrain length. In the worst case, every cell
is checked once.

Space complexity:
`O(1)`. The scan uses a fixed amount of local state and does not allocate memory.

Why this algorithm is used now:
The terrain buffers are small and bounded, and linear scan gives predictable
behavior with a simple C API.

What replaces or extends it later:
2D grid traversal will build on the same explicit bounds and status-code
behavior. More advanced route search should remain separate from the 1D terrain
primitive.

## 1D Blocked Cell Lookup

Algorithm:
Direct indexed lookup after validating the terrain buffer and requested index.

Purpose:
Check whether one caller-selected terrain cell is blocked.

Function:
- `bl_terrain_is_blocked_1d`

Time complexity:
`O(1)`. The function checks one cell after validating its inputs.

Space complexity:
`O(1)`. The lookup uses a fixed amount of local state and does not allocate
memory.

Why this algorithm is used now:
Blocked-cell lookup is a primitive operation for later grid traversal. Keeping it
indexed and bounded makes the behavior deterministic and easy to reuse.

What replaces or extends it later:
2D grid APIs will extend this idea with coordinate-to-index conversion and
grid-level bounds checks.
