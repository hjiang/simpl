{
  description = "envel - snapshot shell env vars into an Emacs Lisp file";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system}; in {
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.llvmPackages.stdenv; } {
          nativeBuildInputs = [
	          pkgs.bazel
	          pkgs.expect
            pkgs.llvmPackages.clang
            pkgs.llvmPackages.clang-tools
          ];
        };
      });
}
