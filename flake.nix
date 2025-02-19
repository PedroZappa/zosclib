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
    # utils.lib.eachDefaultSystem (system: let
    #   pkgs = import nixpkgs {inherit system;};
    utils.lib.eachDefaultSystem (system: let
    pkgs = nixpkgs.legacyPackages.${system}.extend (final: prev: {
      boost = prev.boost.override { version = "1.87.0"; }; # Specify a recent version
    });
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
          gcc
          boost
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
          gcc # Use GCC explicitly
          cmake
          pkg-config
          boost
        ];

        # Set environment variables to help the compiler find Boost
        shellHook = ''
          export CC=${pkgs.gcc}/bin/gcc
          export CXX=${pkgs.gcc}/bin/g++
          export LIBRARY_PATH="${pkgs.boost.out}/lib:${pkgs.gcc.cc}/lib:${pkgs.gcc.cc}/lib64"
          export NIX_CFLAGS_COMPILE="-I${pkgs.boost.dev}/include -I${pkgs.gcc.cc}/include"
          export NIX_LDFLAGS="-L${pkgs.boost.out}/lib"
          echo "Development environment loaded with GCC, Boost, CMake, and pkg-config."
        '';
      };
    })

    // {
      overlays.default = self: super: {
        hello = self.packages."${super.system}".hello;
      };
    };
}

