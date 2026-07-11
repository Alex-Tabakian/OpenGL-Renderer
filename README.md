# Basic OpenGL Window Demo

This repository contains one minimal OpenGL/GLUT demo: `src/opengl-project`.

## Windows

Open `OpenGL-demos.sln` in Visual Studio and build the `opengl` project.
The project is configured for the bundled GLAD, FreeGLUT, and GLFW libraries, and copies the matching runtime DLLs into the build output folder after each build.

## Linux

Install the OpenGL/GLUT dependencies, then build the demo:

```sh
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev mesa-common-dev
make
```

The executable is written to `build/x64/Release/opengl-project`.

## License

The original demo source is MIT licensed. FreeGLUT is included for Windows builds under its own license in `libs/freeglut-MSVC-3.0.0-2.mp/freeglut/Copying.txt`.
