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

## Building
The current codebase was written on Windows 11. For Windows, I use [w64devkit](https://github.com/skeeto/w64devkit/releases) as my main compiler suite.