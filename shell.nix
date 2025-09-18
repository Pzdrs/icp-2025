{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    cmake
    vcpkg
    opencv4
    pkg-config
  ];

  shellHook = ''
    export VCPKG_ROOT=${pkgs.vcpkg}/share/vcpkg
  '';
}
