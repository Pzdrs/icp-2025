{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.cmake
    pkgs.vcpkg
    pkgs.opencv4
  ];
}
