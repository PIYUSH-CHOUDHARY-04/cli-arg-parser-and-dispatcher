# CLI Argument Dispatcher Library (v3-dev)

### Modular Command-Line Argument Parsing and Dispatching for C Libraries

---

## 🧭 Introduction

The **CLI Argument Dispatcher (v3-dev)** is the next evolutionary step in the CLI argument handling framework written in pure C.  
While **v2** was designed for *end-user programs* (where the entire argument space was managed by a single dispatcher),  
**v3** focuses on **library integration**, **shared CLI handling**, and **layered ownership** of arguments.

In short, this version allows both a **library** and the **user program** to manage their own sets of command-line arguments — without collisions or redundant parsing.

---

## 🎯 Core Objective

v3 enables **modular and cooperative CLI handling**, where:
- The **library** handles its reserved arguments (`-lib-*`, `-engine-*`, etc.).
- The **user program** retains freedom over the rest of the CLI arguments.
- Both layers can co-exist without stepping over each other.

This creates a unified yet isolated interface for both developers and libraries.

---

## 🧩 Architectural Overview

### 1. Layered CLI Ownership

At the heart of v3 is the **argument ownership split**:

| Layer | Ownership | Example Arguments | Description |
|--------|------------|------------------|--------------|
| **Library layer** | Reserved arguments handled by the library | `-libinfo`, `-version`, `-verbose` | Library-defined switches and key-values |
| **User layer** | Application or program-defined arguments | `-start`, `-config=file.txt` | User-space options |

The library processes its subset first, removing or marking those entries before passing control to the user dispatcher.

---

### 2. Static Argument–Function Mapping

Each library argument is statically mapped to a handler function using three tables:

```c
const char* cli_arg_table[CLI_ARG_COUNT] = { ARG1, ARG2, ARG3 };
const int cli_args_size[CLI_ARG_COUNT]   = { SIZEOF_CSTR(ARG1), SIZEOF_CSTR(ARG2), SIZEOF_CSTR(ARG3) };
const int (*fptr_arr[CLI_ARG_COUNT])(void*) = { func_arg1, func_arg2, func_arg3 };

