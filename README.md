# Scuffcraft

A __very__ simple Minecraft-like 3D engine demo written in C++ using OpenGL.

## Installation

### 1. Install required packages (vcpkg, cmake, gcc, ...)

```bash
# with Nix
nix develop

# without Nix (untested)

# 1) Clone vcpkg and bootstrp the binary
git clone https://github.com/microsoft/vcpkg.git --depth 1
cd vcpkg && ./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)
```

## Requirements

- [ ]  realtime 2D raster processing = tracker, constant quality video encoder (can be separate project) 
- [x]  multiple threads + synchronisation 
  - [x]  procedural world generation (infinite world, chunk-based, multithreaded)
- [x]  3D GL Core profile + shaders version 4.6, enabled GL debug, used DSA (Direct State Access)
  - MacOS limited to 4.1 
- [x]  high performance => at least 60 FPS
- [x]  allow VSync control, fullscreen vs. windowed switching (restore window position & size), screenshot, antialiasing toggle
  - [x] vsync toggle
  - [x] fullscreen toggle (with alt+enter)
  - [x] screenshot (with alt+c)
  - [x] antialiasing toggle (with debug menu) - disabled by default, looks better that way
- [x]  simple GUI: display (at least) GL version, profile, FPS
- [x]  event processing: camera, object, app behaviour etc. controlled by mouse (both axes, wheel), keyboard, window resize, etc.
- [x]  multiple different independently moving 3D models, at leats two loaded from file
- [x]  free floating and first-person-view camera  
  - [x]  Minecraft creative-like camera
  - [x]  Freecam (WSAD use direction vectors)
- [x]  at least three different textures (or subtextures from texture atlas etc.)
- [x]  3D positional audio (multiple sources, background sound)
  - [x] Background music
  - [x] Positional sound

### EXTRAS

- [ ]  lighting model, at least 3 lights (1x ambient, 1x directional, 1x reflector: at least one is moving; + possible other lights)
- [ ]  correct full scale transparency (at least one transparent object; NOT if(alpha<0.1) {discard;} )
- [x]  height map textured by height, proper player height coords
- [ ]  particles
- [ ]  scripting (useful)
- [ ]  correct collisions
- [ ]  some other nice complicated effect...

## Credits

*Heavily* inspired by the early stages of [Hazel](https://github.com/TheCherno/Hazel). vzivote bych to sam takhle scalable nenapsal jebe?