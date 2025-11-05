# CTok — C Regex Tokenizer Library

**CTok** is a lightweight, cross-platform tokenizer library written in C that uses **PCRE2** for regex-based tokenization.
It is designed to be embedded in your C projects as a reusable library.

---

## Features

- Cross-platform (Windows, Linux, macOS)
- Uses PCRE2 for full-featured regex token matching
- Data-driven: define token types with names and regex patterns
- Safe memory handling (`strncpy_s` on MSVC, fallback for GCC/Clang)
- Compatible with unit testing via CTest

---

## Directory Structure

```
ctok/
  include/         # Public headers
    ctok/
      tokenizer.h
  src/             # Library source code
    tokenizer.c
    CMakeLists.txt
  tests/           # Unit tests
    test_tokenizer.c
    CMakeLists.txt
  CMakeLists.txt   # Top-level CMake file
  README.md
```

---

## Installation

### Requirements

- CMake ≥ 3.15
- C compiler supporting C17

### Build

```bash
mkdir build
cd build
cmake -DENABLE_TESTS=ON ..
cmake --build .
```

### Run Tests

```bash
ctest --verbose
```

---

## Usage

### Include the header

```c
#include <ctok/tokenizer.h>
```

### Example

```c
#include <ctok/tokenizer.h>
#include <stdio.h>

int main(void) {
  TokenType types[] = {
      {"NUMBER", "^[0-9]+"},
      {"IDENTIFIER", "^[a-zA-Z_][a-zA-Z0-9_]*"},
      {"SPACE", "^[ \t\n]+"},
  };

  const char *input = "foo 123 bar42";

  Tokenizer *tokenizer = ctok_create_tokenizer(input, types, 3);

  Token token;
  while ((token = ctok_tokenizer_next(tokenizer)).type_index != -1) {
    printf("%s: '%s'\n", types[token.type_index].name, token.text);
    ctok_token_free(&token);
  }

  return 0;
}
```

**Output:**

```
IDENT: 'foo'
NUMBER: '123'
IDENT: 'bar42'
```

---

## API

```c
ctok_Tokenizer *ctok_create_tokenizer(const char *src, const ctok_TokenType *types, size_t type_count);
ctok_Token ctok_tokenizer_next(ctok_Tokenizer *tokenizer);
void ctok_token_free(ctok_Token *token);
```

- `ctok_create_tokenizer` — creates a tokenizer instance
- `ctok_tokenizer_next` — returns the next token; `type_index == -1` indicates EOF
- `ctok_token_free` — frees the memory allocated for a token

---

## CMake Integration

Add your library as a subdirectory:

```cmake
add_subdirectory(src)
target_link_libraries(my_project PRIVATE ctok)
```

Enable tests if desired:

```cmake
option(ENABLE_TESTS \Enable tests\ ON)
```

---

## License

MIT License — see LICENSE file.;
