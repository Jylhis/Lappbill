{
  stdenv,
  lib,
  qmake,
  qt5,
  wrapQtAppsHook,
  cmake, ninja,
}:
stdenv.mkDerivation {
  pname = "lappbill";
  version = "0.1";

  src = lib.sources.trace (lib.cleanSource ./.);

  buildInputs = [qt5.full];
  nativeBuildInputs = [cmake ninja qmake wrapQtAppsHook qt5.full];
}
