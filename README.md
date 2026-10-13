# N-Body Simulation in C

A real-time gravitational N-body physics engine and 3D visualizer built from scratch in C (C11) using [raylib](https://www.raylib.com/).

Designed with strict architectural decoupling between pure numerical simulation (`core/`) and real-time visualization (`render/`), enabling both headless batch benchmarking and interactive 3D rendering.

---

## Features & Current Progress

- **Decoupled Architecture:** Core physics (`include/core/`, `src/core/`) is pure ISO C11 with zero graphics dependencies, enabling automated headless testing and CLI benchmarks.
- **Inlined 3D Vector Math:** Custom `Vec3` operations with `static inline` functions for zero function-call overhead and auto-vectorization.
- **Symmetric Pairwise Gravity:** Softened Newtonian gravitational acceleration using Newton's Third Law symmetry ($F_{ji} = -F_{ij}$), cutting pairwise evaluations in half ($N(N-1)/2$).
- **Symplectic Leapfrog Integrator:** Kick-Drift-Kick (KDK) scheme preserving phase-space volume and bounding energy drift over tens of thousands of steps.
- **Physics Diagnostics:** Real-time Kinetic, Potential, and Total Mechanical Energy tracking ($KE + PE$) verifying physical conservation.
- **Automated Validation Suite:** 
  - Headless 2-body Kepler orbit test verifying $< 0.003\%$ relative energy drift over 10 full orbits (62,800+ steps).
  - Comparative validation demonstrating Leapfrog numerical stability versus Forward Euler orbital escape.
- **Interactive Scenarios & Visualizer:**
  - **Solar System:** Multi-planet system with Keplerian orbital velocities scattered in an orbital disk.
  - **Figure-8 3-Body Solution:** The periodic choreography discovered by Chenciner & Montgomery (2000) with zero momentum and locked face-on perspective.
  - Adaptive mass-based sphere sizing and live telemetry HUD.

---

## Build & Run

### Prerequisites
- GCC with C11 support (e.g. MinGW-w64 / MSYS2 UCRT64)
- GNU Make
- raylib

### Commands

```bash
# Build and run the interactive 3D simulation
make run

# Run the headless Kepler orbit verification test
make test

# Run Phase 0 sandboxes
make 2d
make 3d

# Clean build artifacts
make clean
```

---

## Controls

| Key | Action |
| :--- | :--- |
| **`1`** | Switch to Multi-Planet Solar System |
| **`2`** | Switch to Figure-8 3-Body Choreography |
| **`Space`** | Pause / Resume physics simulation |
| **`Up` / `Down`** | Speed up / slow down simulation time ($dt$) |
| **`R`** | Reset current scenario to $t = 0$ |
| **Mouse Drag** | Orbit camera (in Solar System view) |

---

## Roadmap

- [x] **Phase 0:** Environment setup, 2D/3D raylib mechanics, and camera sandboxes.
- [ ] **Phase 1: Core Physics & Naive $O(N^2)$ Engine** *(Current)*
  - [x] 3D Vector Math library
  - [x] Pairwise softened gravitational force & Leapfrog (KDK) integrator
  - [x] Energy conservation diagnostics & Kepler headless validation
  - [x] Multi-scenario interactive visualizer
  - [ ] Trajectory / orbital trails buffer
- [ ] **Phase 2:** Memory Architecture (Arena allocator & Struct-of-Arrays data layout).
- [ ] **Phase 3:** Barnes-Hut Octree ($O(N \log N)$ spatial partitioning).
- [ ] **Phase 4:** SIMD Vectorization (AVX/SSE) & Multi-threaded force evaluation.
- [ ] **Phase 5:** Scaled Visuals (100k+ particles with `rlgl`), binary trajectory recording, and resume packaging.
