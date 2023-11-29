{
  description = "AniLibria.Qt cross platform desktop client";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    nixGL.url = "github:nix-community/nixGL";
    nixGL.inputs.nixpkgs.follows = "nixpkgs";
    nixGL.inputs.flake-utils.follows = "flake-utils";
    flake-utils.url = "github:numtide/flake-utils";
    flake-compat.url = "github:edolstra/flake-compat";
    flake-compat.flake = false;
  };

  outputs = inputs @{ self, nixpkgs, nixGL, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ (nixGL.overlays.default) ];
            config = { allowUnfree = true; }; # needed for nvidia driver
          };
          wrapWithNixGL = anilibria-winmaclinux: nixGl: { nixGLCMD ? pkgs.lib.getExe nixGl }:
            let
              wrapper = pkgs.writeShellApplication
                {
                  name = "AniLibria";
                  runtimeInputs = [ anilibria-winmaclinux nixGl ];
                  text = ''
                    ${nixGLCMD} ${pkgs.lib.getExe anilibria-winmaclinux} "$@"
                  '';
                };
            in
            pkgs.stdenvNoCC.mkDerivation {
              pname = "anilibria-winmaclinux" + nixGl.name;
              version = anilibria-winmaclinux.version;
              inherit (anilibria-winmaclinux) meta;

              buildCommand = ''
                mkdir -p $out/bin
                ln -s ${anilibria-winmaclinux}/share $out/share
                cp ${pkgs.lib.getExe wrapper} $out/bin/${pkgs.lib.lists.last (pkgs.lib.strings.split "/" (pkgs.lib.getExe anilibria-winmaclinux))}
              '';
            };
          genPkgs = pkgs: pkg:
            rec {
              nixos = pkg;
              auto = wrapWithNixGL pkg pkgs.nixgl.auto.nixGLDefault { nixGLCMD = "nixGL"; };
              nvidia = wrapWithNixGL pkg pkgs.nixgl.auto.nixGLNvidia { };
              nvidia-hybrid = wrapWithNixGL pkg pkgs.nixgl.auto.nixGLNvidiaBumblebee { };
              intel = wrapWithNixGL pkg pkgs.nixgl.nixGLIntel { };
              amd = intel;
            };
        in
        rec {
          packages = rec {
            anilibria-winmaclinux-latest = throw "FIXME" genPkgs pkgs
              (
                pkgs.anilibria-winmaclinux.overrideAttrs (old: rec {
                  src = ./.;
                  sourceRoot = "src";
                })
              );
            anilibria-winmaclinux = genPkgs pkgs pkgs.anilibria-winmaclinux;
            default = anilibria-winmaclinux.auto;
          };
          defaultPackage = packages.anilibria-winmaclinux-auto;
          devShells.default = throw "TODO" pkgs.mkShell {
            nativeBuildInputs = with pkgs; [ ];
            buildInputs = with pkgs; [ ];
          };
        });
}
