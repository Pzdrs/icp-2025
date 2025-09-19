{
  outputs =
    { self, nixpkgs, ... }:
    let
      pkgs = import nixpkgs { system = "aarch64-darwin"; };
    in
    {
      devShells.aarch64-darwin.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          cmake
          vcpkg
          pkg-config-unwrapped
        ];

        shellHook = ''
          export VCPKG_ROOT=${pkgs.vcpkg}/share/vcpkg
        '';
      };
    };
}
