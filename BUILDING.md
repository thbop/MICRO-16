# Building
The current codebase was written on Windows 11. For Windows, I use [w64devkit](https://github.com/skeeto/w64devkit/releases) as my main compiler suite.

We are also using [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.16) which can be fairly difficult to setup for beginners.

## Windows
With [w64devkit](https://github.com/skeeto/w64devkit/releases) installed, download the `SDL3-devel-3.2.16-mingw.zip` option from the [SDL3 release page](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.16). Create a `build/` folder in the Micro-16 project. Extract the SDL release and drag the `SDL3.dll` from `bin/` into the `build/` folder. This is SDL3's dynamically linked library that is linked with the main program at runtime. Next, create a `lib/` folder and drag the `libSDL3.dll.a` static library into it. Follow the same process for the [SDL_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4) library. Ensure that the `bin/` folder of your w64devkit install is in your PATH and run `make` in the terminal.

## Linux
I cannot guide you through installing SDL3 on Linux because I've never done it. You probably need an archive library (possibly the same `libSDL3.dll.a` library from the [Windows section](#windows)) and a shared object library (.so). Check out the [official install instructions](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md) or maybe follow this [video](https://www.youtube.com/watch?v=1S5qlQ7U34M). Adjust the Makefile (in a Linux only section) if needed.