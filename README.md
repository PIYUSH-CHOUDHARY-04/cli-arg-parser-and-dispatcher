
---

```markdown
# CLI Argument Dispatcher for C

A *lightweight, modular, and extensible command-line argument parser* for C programs, supporting both **switch-style arguments** (`-arg`) and **key-value arguments** (`-arg=value`). It also provides **argument reuse control, validation, and optional logging**.

---

## Features

- **Dual Argument Types**: Handle both switches (`-arg`) and key-value pairs (`-arg=value`).
- **Reusability Control**: Decide per argument whether it can be reused, and how many times.
- **Robust Validation**: Detect unknown arguments, duplication, or misuse before any handler executes.
- **Extensible Handlers**: Separate file for implementing argument handlers with private macros/variables.
- **Efficient Memory Usage**: Uses **bitfields** in structs to reduce memory footprint.
- **Optional Logging**: Toggle debug logs with a compile-time macro `LOG_ENABLE`.
- **Zero Dependencies**: Uses only standard C library functions.

---

## Project Structure

```

cli_arg_proc.h      # Public API and documentation
cli_arg_proc.c      # Dispatcher implementation
handlers.h          # Handler prototypes, shared macros, and error codes
handlers.c          # Handler function implementations and private definitions

````

### Responsibilities

| File             | Responsibility                                                                     | Notes                                   |
|------------------|------------------------------------------------------------------------------------|-----------------------------------------|
| `cli_arg_proc.h` | Declares `cli_arg_dispatcher()`; guides users on customization                     |               Public API                |
| `cli_arg_proc.c` | Implements parsing, validation, and dispatch; contains private structs and macros  |              Internal logic             |
| `handlers.h`     | Exposes handler prototypes, shared macros (`LOG_ENABLE`) and generic error codes   | Shared between dispatcher and handlers  |
| `handlers.c`     | Implements handler functions and private macros/variables                          |          Private to handler logic       |

---

## How It Works

The dispatcher works in **four stages**:

1. **Parsing**  
   CLI arguments are read from `argv[1]..argv[argc-1]` and compared with the `ARGS[]` table.

2. **Validation**  
   - Unknown arguments are flagged.
   - Duplicate arguments are checked against reuse rules.
   - Misuse is detected if argument usage exceeds the allowed count.

3. **Relation Mapping**  
   Creates a `relation[]` array that maps each valid CLI argument to its corresponding `ARGS[]` entry.

4. **Dispatch**  
   Each validated argument calls its handler function:
   - **Switch argument** → `NULL` pointer.
   - **Value argument** → Pointer to the character after `=`.

---

## Argument Format

| Type       | Example        | Passed to Handler    |
|------------|----------------|----------------------|
| Switch     |    `-arg1`     |        `NULL`        |
| Key-Value  | `-arg2=value`  | Pointer to `"value"` |

---

## Usage Example

```c
#include "cli_arg_proc.h"

int main(int argc, char** argv) {
    int status = cli_arg_dispatcher(argc, argv);
    
    if (status < 0) {
        // Handle errors here
        return status;
    }
    
    return 0;
}
````

### Command-Line Examples

```bash
# Single switch
./program -arg1

# Single key-value
./program -arg2=myvalue

# Multiple arguments
./program -arg1 -arg2=value -arg3

# Reusable argument (if allowed in ARGS)
./program -arg2=val1 -arg2=val2 -arg2=val3
```

---

## Customization

All **modifiable sections** are marked with `[---MODIFIABLE---]` in the source files.

### 1. Implement a New Handler

**In `handlers.c`:**

```c
int func_argX(void* ptr) {
    if (ptr == NULL) {} // add if you are implementing value type argument else can skip
    return 0;
}
```

### 2. Declare Handler Prototype

**In `handlers.h`:**

```c
int func_argX(void* ptr);
```

### 3. Add Argument Macro

**In `cli_arg_proc.c`:**

```c
#define ARG4 "-arg4"  /* [---MODIFIABLE---] */
```

### 4. Add Argument Entry to `ARGS[]`

```c
static struct argbox ARGS[CLI_ARG_COUNT] = {
    { .arg_str=ARG1, .arg_str_size=SIZEOF_CSTR(ARG1), .arg_fptr=func_arg1,
      .arg_reuse=!REUSE_ENABLE, .arg_reuse_count=0, .arg_use_check=0, .arg_misuse=0 },

    { .arg_str=ARG2, .arg_str_size=SIZEOF_CSTR(ARG2), .arg_fptr=func_arg2,
      .arg_reuse=REUSE_ENABLE, .arg_reuse_count=5, .arg_use_check=0, .arg_misuse=0 },

    { .arg_str=ARG4, .arg_str_size=SIZEOF_CSTR(ARG4), .arg_fptr=func_argX,
      .arg_reuse=!REUSE_ENABLE, .arg_reuse_count=0, .arg_use_check=0, .arg_misuse=0 },
};
```

### 5. Update Argument Count

```c
#define CLI_ARG_COUNT 4  /* [---MODIFIABLE---] */
```

### 6. Adjust `MAX_IDX_COUNT` (Optional)

```c
#define MAX_IDX_COUNT 15  /* [---MODIFIABLE---] */
// Options: 7 → 128 args, 15 → 32K args, 23, 31... (8n-1 sequence)
```

### 7. Enable/Disable Logging

```c
#define LOG_ENABLE TRUE  /* [---MODIFIABLE---] */
```

---

## Error Handling

| Code           | Macro         | Meaning                                                              |
| -------------- | ------------- | -------------------------------------------------------------------- |
|  0             | Success       | All arguments processed successfully                                 |
| -1             | `EINVALARG`   | Unknown/invalid argument                                             |
| -2             | `EARGMISUSED` | Argument used more than allowed                                      |
| -64 to INT_MIN | `EHANDLER`    | Handler function returned an error (multiplied by argument position) |

---

## Core Structures

### `struct argbox`

Represents an argument specification:

```c
struct argbox {
    const char* arg_str;              // Argument string
    const short int arg_str_size;     // Length (excluding null terminator)
    const int (*arg_fptr)(void*);     // Handler function
    const unsigned int arg_reuse :1;  // Allow reuse
    const unsigned int arg_reuse_count :7;  // Max reuse (0-127)
    int arg_use_check :7;             // Tracks usage
    unsigned int arg_misuse :1;       // Misuse flag
};
```

### `struct relation`

Maps CLI arguments to `ARGS[]` indices:

```c
struct relation {
    unsigned int rel_err :1;          // Error flag
    unsigned int rel_idx :MAX_IDX_COUNT;  // Index in ARGS[]
};
```

---

## Design Highlights

* **No Invalid States**: Arguments validated before handler dispatch.
* **Memory Efficient**: Uses bitfields.
* **Flexible Logging**: Can enable/disable debug messages at compile time.
* **Fine-Grained Reuse Control**: Per-argument reuse limits.
* **Extensible**: Easy to add new handlers and arguments.

---

## Building

```bash
gcc -o program cli_arg_proc.c handlers.c main.c
```

Disable logging for production:

```bash
# Edit handlers.h: #define LOG_ENABLE FALSE
gcc -O2 -o program cli_arg_proc.c handlers.c main.c
```

---

## Example Handler

```c
int func_arg2(void* ptr2) {
    if (ptr2 == NULL) {
        fprintf(stderr, "Error: arg2 requires a value\n");
        return -1;
    }

    char* value = (char*)ptr2;
    LOG_MSG("Processing arg2: %s\n", value);

    // Your custom logic here
    return 0;
}
```

---

## Important Notes

* Always initialize `arg_use_check` and `arg_misuse` to `0`.
* `argv[0]` (program name) is ignored.
* Handlers must return `0` on success, negative values on failure.
* Value arguments receive a pointer **after `=`**.
* Argument strings must include the leading `-`.

---


---

## Author

[PIYUSH CHOUDHARY/mercmerc961@gmail.com]

```

---




