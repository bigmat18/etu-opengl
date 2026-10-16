{
  description = "OpenGL framework library";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    cpp-core-utils = {
      url = "github:bigmat18/cpp-core-utils";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      cpp-core-utils,
    }:
    flake-utils.lib.eachSystem
      [
        "x86_64-linux"
        "i686-linux"
        "aarch64-linux"
        "x86_64-darwin"
      ]
      (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ ];
          };
          coreUtils = cpp-core-utils.packages.${system}.default;
          glad = pkgs.runCommand "glad" {
            nativeBuildInputs = [ pkgs.python3Packages.glad ];
          } ''
            mkdir -p $out
            glad --profile core --api gl=4.6 --generator c --spec gl --out-path $out --reproducible
          '';
          etu-opengl = pkgs.callPackage ./package.nix {
            cpp-core-utils = coreUtils;
            inherit glad;
          };
        in
        {
          packages = {
            default = etu-opengl;
            etu-opengl = etu-opengl;
            glad = glad;
          };

          devShells.default = pkgs.mkShell rec {
            name = "etu-opengl";

            packages = with pkgs; [
              gcc
              cmake
              ninja
              clang-tools
              pkg-config
            ];

            buildInputs = with pkgs; [
              glfw
              libGL
              glm
              tinyobjloader
              stb
              coreUtils
              glad
              nlohmann_json
            ];

            shellHook = ''
              mkdir -p external
              ln -sfn "${glad}" external/glad
              ln -sfn "${pkgs.glm}" external/glm
              ln -sfn "${pkgs.tinyobjloader}" external/tinyobjloader
              ln -sfn "${pkgs.stb}" external/stb
              ln -sfn "${coreUtils}" external/cpp-core-utils
              ln -sfn "${pkgs.glfw}" external/glfw
              ln -sfn "${pkgs.nlohmann_json}" external/nlohmann_json
              echo "etu-opengl env ready (external/ symlinks populated)"
              if [[ $- == *i* ]]; then
                exec zsh
              fi
            '';
          };
        }
      );
}
