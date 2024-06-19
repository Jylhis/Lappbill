{
  stdenv,
  lib,
  qmake,
  qt5,
  wrapQtAppsHook,
}:
stdenv.mkDerivation {
  pname = "lappbill";
  version = "0.1";

  src = ./.;

  buildInputs = [qt5.full];
  nativeBuildInputs = [qmake wrapQtAppsHook];
}
