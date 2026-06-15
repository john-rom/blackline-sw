# Blackline/SW

Blackline is a route and approach engine for obstacle-aware target selection and
path planning in constrained environments.

Blackline/SW is its portable software reference implementation.

The current code starts with terrain lookup, validation, and tests. As the solver
grows, this implementation will define the behavior that firmware and hardware
targets must match.

The core library exposes a C-compatible API. Unit tests are written in C++.

## Current Status

Blackline/SW currently provides the first terrain primitives:

- locate the first agent marker in a 1D terrain buffer
- locate the first target marker in a 1D terrain buffer
- check whether a requested 1D terrain cell is blocked
- validate null pointers, lengths, bounds, and output parameters

This is a foundation release. Route scoring, grid traversal, queues, and path
search are not implemented yet.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Test

```sh
ctest --test-dir build --output-on-failure
```

## Documentation

- [Complexity notes](docs/complexity.md)

## Scope

This repository contains the software reference backend only. Firmware, FPGA,
and simulation backends are deferred.
