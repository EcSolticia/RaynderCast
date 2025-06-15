let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-25.05";
  pkgs = import nixpkgs { config = {
    allowUnfree = true;
  }; overlays = []; };

  vscode-with-extensions = pkgs.vscode-with-extensions.override {
    vscodeExtensions = with pkgs.vscode-extensions; [
      ms-vscode.cpptools
      ms-vscode.cmake-tools
    ];
  };
in

pkgs.mkShellNoCC {
  packages = with pkgs; [
    gcc
    libgcc
    gdb
    gnumake
    cmake
    SDL2
    vscode-with-extensions
  ];
}
