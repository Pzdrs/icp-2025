# MRDKIIII

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
- [ ]  multiple threads + synchronisation 
- [x]  3D GL Core profile + shaders version 4.6, enabled GL debug, used DSA (Direct State Access)
  - MacOS limited to 4.1 
- [ ]  high performance => at least 60 FPS
- [ ]  allow VSync control, fullscreen vs. windowed switching (restore window position & size), screenshot, antialiasing toggle
- [ ]  simple GUI: display (at least) GL version, profile, FPS
- [ ]  event processing: camera, object, app behaviour etc. controlled by mouse (both axes, wheel), keyboard, window resize, etc.
- [ ]  multiple different independently moving 3D models, at leats two loaded from file
- [ ]  free floating and first-person-view camera
- [ ]  at least three different textures (or subtextures from texture atlas etc.)
- [ ]  3D positional audio (multiple sources, background sound)

### EXTRAS

- [ ]  lighting model, at least 3 lights (1x ambient, 1x directional, 1x reflector: at least one is moving; + possible other lights)
- [ ]  correct full scale transparency (at least one transparent object; NOT if(alpha<0.1) {discard;} )
- [ ]  height map textured by height, proper player height coords
- [ ]  particles
- [ ]  scripting (useful)
- [ ]  correct collisions
- [ ]  some other nice complicated effect...