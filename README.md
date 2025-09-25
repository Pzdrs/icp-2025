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