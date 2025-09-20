{
  outputs =
    { self, nixpkgs, ... }:
    let
      system = "aarch64-darwin";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
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
