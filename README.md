
# C Preprocessor Implementation Using C

## Project Overview

This project implements a basic C preprocessor using the C programming language. It performs selected preprocessing operations on C source files before compilation.

The program accepts one or more C source files through command-line arguments and generates corresponding preprocessed files with the `.i` extension.

## Features

- Removes single-line (`//`) and multi-line (`/* ... */`) comments.
- Processes basic `#include` directives.
- Supports object-like macros using `#define`.
- Supports basic function-like macro expansion.
- Accepts multiple input files in one execution.
- Uses file handling and dynamic memory allocation.
- Uses modular programming with separate source and header files.
- Uses a Makefile to build the executable.

## Technologies Used

- **Language:** C
- **Operating System:** Ubuntu Linux
- **Compiler:** GCC
- **Build tool:** GNU Make
- **Editor:** vi

## Project Structure

```text
mypreprocessor/
├── main.c
├── comment_removal.c
├── macro_handler.c
├── file_inclusion.c
├── preprocessor.h
├── Makefile
├── sample_header.h
├── inputfile.c
└── README.md
```

## Module Description

| File | Responsibility |
|---|---|
| `main.c` | Reads command-line arguments and creates output filenames. |
| `comment_removal.c` | Removes comments from source code. |
| `macro_handler.c` | Defines and expands supported macros. |
| `file_inclusion.c` | Reads source files and processes header inclusions. |
| `preprocessor.h` | Contains shared function declarations. |
| `Makefile` | Compiles the project modules. |
| `sample_header.h` | Example user-defined header file. |
| `inputfile.c` | First sample input program. |
| `inputfile2.c` | Second sample input program. |
| `inputfile3.c` | Third sample input program containing binary search. |

## Installation

On Ubuntu, install GCC and Make:

```bash
sudo apt update
sudo apt install gcc make -y
```

## Build the Project

Open a terminal in the project directory and run:

```bash
make
```

To remove generated object files and the executable before rebuilding:

```bash
make clean
make
```

## Run the Project

Run the preprocessor with one input file:

```bash
./mypreprocessor inputfile.c
```

Run it with multiple input files:

```bash
./mypreprocessor inputfile.c inputfile2.c inputfile3.c
```

The program generates the corresponding output files:

```text
inputfile.i
inputfile2.i
inputfile3.i
```

View an output file:

```bash
cat inputfile.i
```

## Example

Input (`inputfile.c`):

```c
#include "sample_header.h"

#define VALUE 100
#define SQUARE(x) ((x) * (x))

int main(void)
{
    // Declare a variable
    int number = 5;
    int result = SQUARE(number);

    return result + VALUE;
}
```

The preprocessor removes comments, processes the supported header inclusion and macro definitions, and writes the processed source to `inputfile.i`.

## Workflow

1. Accept the input C filename or filenames through command-line arguments.
2. Open and read each input file.
3. Remove supported comments.
4. Process supported `#include` directives.
5. Register `#define` macros.
6. Expand supported macros in source lines.
7. Write the processed source code to a `.i` output file.

## Concepts Demonstrated

- C file handling (`fopen`, `fread`, `fclose`, etc.)
- Dynamic memory allocation (`malloc`, `free`)
- String handling and manipulation
- Preprocessor directives
- Object-like and function-like macros
- Recursive file processing
- Command-line arguments (`argc` and `argv`)
- Modular programming
- GCC compilation and Makefile usage

## Limitations

This is an educational implementation of selected C preprocessing features, not a complete replacement for the GCC preprocessor.

Features such as conditional compilation (`#if`, `#ifdef`, `#ifndef`), include guards, macro stringification (`#`), token pasting (`##`), variadic macros, and full standard system-header processing are not fully implemented. Complex system headers may therefore produce warnings or incomplete output.

## Future Enhancements

- Add conditional compilation directives.
- Improve nested and system-header inclusion support.
- Add include-guard handling.
- Support advanced macro operations.
- Improve diagnostics and error handling.
- Increase compatibility with standard C preprocessing behavior.

## Conclusion

This project demonstrates the basic working principles of a C preprocessor through modular C programming. It provides practical experience with file handling, dynamic memory allocation, string processing, macro expansion, command-line arguments, and automated compilation.

---

**Project:** C Preprocessor Implementation Using C  
**Language:** C  
**Platform:** Ubuntu Linux  
**Compiler:** GCC
