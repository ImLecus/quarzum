# C++ Style Guide
## Table of contents

* Background
* Baseline 
* Quarzum compiler style guide
    * General naming rules
    * Files
    * Syntax
    * Static and global variables

### Background

### Baseline

### Quarzum compiler style guide

#### General naming rules

* Use `camelCase` for methods and variables, `PascalCase` for namespaces, classes and structs, and `UPPERCASE` for static and global variables.

#### Files

Always use `snake_case` for files and diretories.
Use `.cpp` in implementation files, `.hpp` in library files. The code documentation will be in the `.hpp` files.

#### Syntax

These are some rules for the syntax in the compiler files. All the contributions will need to follow these instructions in order of having a unique form to write code.
* Use `unique_ptr` always if it's possible. Otherwise, pointers will be placed adjacent to the type: `Type* name`.
* Write `inline` and `const` before the type: `inline const Type`.
* Use multi-line comments for the library files, and single-line comments in the implementation files.
```
int i = 42; // bad comment

// good comment
int i = 42;

/**
 * good comment in a library file
*/ 
void doSomething();
```
* Don't use the `using` directive outside the project namespaces. For example: `using std::deque` is not allowed while `using Quarzum::Lexer` is.
* Using logical comparators, don't use `||`,`&&` or `!`, use `or`,`and` or `not` instead.