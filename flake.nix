{
  description = "envel - snapshot shell env vars into an Emacs Lisp file";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        gcovWrapper = pkgs.writeShellScriptBin "gcov" ''
          exec ${pkgs.llvmPackages.llvm}/bin/llvm-cov gcov "$@"
        '';
      in {
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.llvmPackages.stdenv; } {
          nativeBuildInputs = [
            pkgs.bazel
            pkgs.cpplint
            pkgs.expect
            gcovWrapper
            pkgs.jdk
            pkgs.lcov
            pkgs.llvmPackages.clang
            pkgs.llvmPackages.clang-tools
            pkgs.llvmPackages.llvm
          ];
          shellHook = ''
            export GCOV="${gcovWrapper}/bin/gcov"
          '';
        };
      });
}
