# JSON Parser in C

![Build Status](https://img.shields.io/badge/Build-make%20lin%20%2F%20make%20win-brightgreen)
![Tests](https://img.shields.io/badge/Tests-bun%20test-brightgreen)

A lightweight, modular JSON parser written in **C**. It provides a clean API to tokenize, parse, and manage JSON objects, arrays, and values with safe memory handling.

## Table of Contents

1. [Project Structure](#project-structure)
2. [Features](#features)
3. [Build Instructions](#build-instructions)
4. [Running Tests](#running-tests)
5. [Usage Example](#usage-example)
6. [Future Improvements](#future-improvements)
7. [License](#license)

## Project Structure

```
.
├── include/
│   ├── lexer.h         # Lexical analyzer (tokenizer)
│   ├── parser.h        # Core parser and JSON data structures
│   ├── utility_func.h  # Memory management + debugging helpers
│   ├── json_parser.h # High-level parsing interface
|   └── stack.h
├── src/
│   ├── lexer.c
│   ├── parser.c
│   ├── utility_func.c
│   ├── json_parser.c
│   └── stack.c
├── main.c              # Example entry point / test runner
├── makefile            # Build script (Linux & Windows)
└── tests/              # JS tests (bun test)
```

## Features

- Tokenizes JSON strings into typed tokens.
- Parses into nested C data structures:
  - `Object`
  - `Array`
  - `Value` (`STRING`, `INTEGER`, `FLOAT`, `BOOLEAN`, `NULL`, etc.)
- Safe memory deallocation helpers.
- Simple API for parsing JSON from strings.

## Build Instructions

Controlled by the project `makefile`:

### Linux

```bash
make lin
# output: ./jp
```

### Windows (cross-compile)

```bash
make win
# output: ./jp-w.exe  (requires MinGW)
```

### Clean

```bash
make clean        # remove all builds
make clean-lin    # remove Linux build only
make clean-win    # remove Windows build only
```

## Running Tests

Tests are written with [bun test](https://bun.sh/docs/test).

```bash
bun test
```

> **Note:** Ensure Bun is installed: `curl -fsSL https://bun.sh/install | bash`

## Usage Example

```c
#include "json_parser.h"

int main() {
    char* json_input = "{ \"name\": \"Alice\", \"age\": 25, \"student\": false }";

    // Parse JSON string
    ResponseKV result = json_parser(json_input);

    // Work with result...
    // e.g. traverse values, print types, etc.

    // Free allocated memory
    free_mem(result);
    return 0;
}
```

### Compile & Run

```bash
make lin
./jp
```

## Future Improvements

- Better error reporting for malformed JSON.
- Support for streaming large JSON files.
- Helper functions for traversal/querying (like JSONPath).

## License

Add your preferred license (MIT, Apache-2.0, etc.).

---

*README generated for the C JSON Parser project.*
