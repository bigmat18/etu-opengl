{
  lib,
  stdenv,
  cmake,
  ninja,
  pkg-config,
  glfw,
  libGL,
  glm,
  tinyobjloader,
  stb,
  cpp-core-utils,
  glad,
}:

stdenv.mkDerivation {
  pname = "etu-opengl";
  version = "1.0.0";

  nativeBuildInputs = [
    cmake
    ninja
    pkg-config
  ];

  propagatedNativeBuildInputs = [
    pkg-config
  ];

  propagatedBuildInputs = [
    glfw
    libGL
    glm
    tinyobjloader
    stb
    cpp-core-utils
    glad
  ];

  src = lib.cleanSourceWith {
    src = ./.;
    filter = path: type:
      let base = baseNameOf (toString path);
      in !(base == "build" || base == ".cache" || base == "test_build" || base == "external" || base == "result");
  };

  cmakeFlags = [
    "-DETUGL_BUILD_EXAMPLES=OFF"
    "-DGLAD_SOURCE_DIR=${glad}"
  ];

  meta = with lib; {
    description = "OpenGL framework library";
    homepage = "https://github.com/bigmat18/etu-opengl";
    license = licenses.mit;
    maintainers = [ "bigmat18" ];
    platforms = platforms.unix;
  };
}
