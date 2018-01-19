{pkgs ? import /etc/nixos/nixpkgs-master {} }:

with pkgs;
{
  it = stdenv.mkDerivation {
    name = "esp-project-builder";
    buildInputs = [ python27 python27Packages.pyserial ];
    hardeningDisable = ["all"];
    shellHook = ''
      export IDF_PATH=/home/binarin/personal-workspace/esp-idf/
    '';
  };
}
