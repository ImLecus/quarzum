# Quarzum compiler
## Table of contents
* Organization
* Implementation and library files
* Toolchain modules

### Organization

Every folder has its own `tests/` folder. In addition, there is only one main file inside a folder, the others will cover its functionality.

### Implementation and library files

Inside each folder, a file must have two versions: the **library** file and the **implementation** file. Inside the library file (`.hpp`), all the content will be forward-declared and documented. The implementation file (`.cpp`) will declare the functionality of that code without documentation comments.

### Toolchain modules

* **Tokenizer**: converts a `std::string` with the source file content in a `std::deque<Token>`.
* **Parser**: converts the previous `std::deque<Token>` into an AST.
* **Sem-IR**: this module does the semantic analisys and generates the Intermediate Representation.
* **Backend**: translates the IR into assembly code, depending on the architecture of the computer.
* **Debug**: Manages the errors, warnings, logs, debug messages and all the relevant information for the programmer in debug mode.
* **CLI** (not implemented yet): Manages the command line interface, project template creation...