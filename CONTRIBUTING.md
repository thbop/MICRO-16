# Contributing

Thanks for considering to contribute to this project. This document lays out a few guidelines for making an acceptable pull request.

## Style
Variables are in camel case, while types and functions are in pascal case. Enums and definitions are in uppercase snake case. Here are a few examples:
```c++

// Documentation
enum {
    COOL_THING,
    COOL_THING2,
};

// Document type
typedef struct {
    int x, y;
} CoolType;

// Documentation
typedef struct {
    char property[12];
    CoolType position;
    int counter;
} CoolLongType;

// Documentation
void CoolFunction( int args, int must, int be, int spaced ) {
    // ...
}

// Basic documentation
class MyClass {
public:
    int variable;
};

// ...

int coolVariableX = 1;
int coolVariableY = 5;
CoolType variable = { 0, 1 };
CoolLongType longVariable = {
    .property = "Cheese",     // Comments
    .position = variable,     // should be
    .counter = 45,            // spaced like so
};
```

Lastly, here are some other notes:
- Try to keep lines shorter than 80 characters
- Document your functions/structs
- Try to make functions only do one simple task
- Avoid globals
- Ensure that the main branch has working code

## Security
Since C/C++ are not memory-safe, do not use unsafe functions like: `gets`, `scanf`, `sprintf`, `system`, etc. In other words, when working with buffers, ensure that an overflow is impossible. Try to avoid using heap-allocated memory and always free/delete it.

## Git
Create a branch and develop within it. Do whatever you want, but when it is time to merge ensure that your code complies with the above standards and functions properly. Avoid committing to the main branch (like I am right now).

## Building
The current codebase was written on Windows 11. For Windows, I use [w64devkit](https://github.com/skeeto/w64devkit/releases) as my main compiler suite.

We are also using [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.16) which can be fairly difficult to setup for beginners.

### Windows
With [w64devkit](https://github.com/skeeto/w64devkit/releases) installed, download the `SDL3-devel-3.2.16-mingw.zip` option from the [SDL3 release page](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.16). Create a `build/` folder in the Micro-16 project. Extract the SDL release and drag the `SDL3.dll` from `bin/` into the `build/` folder. This is SDL3's dynamically linked library that is linked with the main program at runtime. Next, create a `lib/` folder and drag the `libSDL3.dll.a` static library into it.

### Linux
I cannot guide you through installing SDL3 on Linux because I've never done it. You probably need an archive library (possibly the same `libSDL3.dll.a` library from the [Windows section](#windows)) and a shared object library (.so). Check out the [official install instructions](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md) or maybe follow this [video](https://www.youtube.com/watch?v=1S5qlQ7U34M). Adjust the Makefile (in a Linux only section) if needed.