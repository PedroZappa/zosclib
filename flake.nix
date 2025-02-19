{
  description = "Flake to manage zosclib Dependencies";

  # inputs.utils.url = "github:numtide/flake-utils";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable"; # Use unstable for latest packages
    utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    utils,
  }:
    utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {inherit system;};
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        pname = "zosclib_flake";
        version = "0.1.0";

        src = ./.;

        nativeBuildInputs = with pkgs; [ # for build derivation (build tools)
          cmake
          pkg-config
        ];

        buildInputs = with pkgs; [ # Libraries for development shell
          gcc # includes libatomic
          boost
          # readline
          # alsa-lib # For audio support
          # freetype # For font rendering
          # fontconfig # For font configuration
          # libGL # OpenGL support
          # curl # For libcurl
          # webkitgtk_4_0
        ];

        cmakeFlags = [];

        meta = with pkgs.lib; {
          homepage = "https://github.com/PedroZappa/zosclib";
          description = "OSC C++ Library";
          licencse = licenses.mit;
          platforms = platforms.all;
          maintainers = with maintainers; [zedro];
        };
      };

      devShells.default = pkgs.mkShell rec {
        buildInputs = with pkgs; [
          just
          gcc # Use GCC explicitly
          cmake
          pkg-config
          boost
          # readline
          # alsa-lib # Audio support
          # freetype # Font rendering
          # fontconfig # Font configuration
          # libGL # OpenGL
          # curl # libcurl
          # webkitgtk_4_0
        ];
      };
    })

    // {
      overlays.default = self: super: {
        hello = self.packages."${super.system}".hello;
      };
    };
}

