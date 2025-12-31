{
  description = "";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    let
      outputsWithoutSystem = { };
      outputsWithSystem = flake-utils.lib.eachDefaultSystem (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };
          lib = pkgs.lib;
          nativeBuildInputs = with pkgs; [
            bash
            clang-tools
            pkg-config
            liburing
          ];
        in
        {
          packages = {
            default = pkgs.stdenv.mkDerivation {
              name = "kmemleak";
              src = ./.;

              buildPhase = "bash ./build.sh";
              installPhase = "mkdir -p $out/bin && mv kmemleak $out/bin";

              nativeBuildInputs = nativeBuildInputs;

            };

            docker = pkgs.dockerTools.buildImage {
              name = "negrel/kmemleak";
              tag = "dev";

              runAsRoot = ''
                #!${pkgs.runtimeShell}
                mkdir -p /app
                cp -r ${self.packages."${system}".default}/bin/kmemleak /kmemleak
              '';

              config = {
                Entrypoint = [ "/kmemleak" ];
              };
            };
          };
          devShells = {
            default = pkgs.mkShell {
              buildInputs =
                with pkgs;
                [
                  bpftrace
                ]
                ++ nativeBuildInputs;
            };
          };
        }
      );
    in
    outputsWithSystem // outputsWithoutSystem;
}
