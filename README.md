# CLI Argument Dispatcher Library

A robust, lightweight, and extensible command line argument dispatcher in C, designed to efficiently parse, validate, and handle CLI arguments with minimal overhead while supporting **switch-type** and **value-based arguments**.

> **Versioning Note:**  
> - **v2** is intended for direct use in user programs — all CLI arguments are fully handled by this library.  
> - **v3** (upcoming) is for library development — it allows the library to handle a subset of CLI arguments while leaving the rest available for the user program. This ensures that programs using the library always have some CLI arguments reserved for library functionality, while still providing flexibility for the user’s own argument handling.

---

## Overview

This library provides a structured framework to:

- Define and register CLI arguments (`-arg`, `-arg=X`) and their corresponding handler functions.
- Validate arguments for correctness and detect duplicates before execution.
- Dispatch the correct handler functions based on the passed arguments.
- Provide clear, descriptive error messages for invalid, unknown, or duplicate arguments.

---

## Features & Advantages

### 1. Efficient Argument Tracking

Uses a lightweight array-based “relation” system to:

- Track which arguments are present
- Detect duplicates
- Avoid repeated string comparisons

This ensures **fast argument validation and dispatch** even for multiple CLI arguments.

---

### 2. Switch and Value-Based Argument Support

Supports two types of arguments:

- **Switch arguments:** `-arg1`  
  → Executes the handler directly.

- **Value-based arguments:** `-arg2=X`  
  → Passes the value `X` to the handler for processing (`int`, `float`, `string`, etc.).

This allows flexible and dynamic argument handling without complicating the library.

---

### 3. Pre-Validation Before Execution

All arguments are validated before any handler runs:

- Detects unknown arguments
- Detects duplicates
- Provides comprehensive error messages

Prevents inconsistent program states and ensures robust operation.

---

### 4. Clear Error Codes and Messaging

Standardized return codes allow precise error detection:

| Code | Meaning                  |
|------|-------------------------|
| 0    | Success                 |
| -1   | Invalid argument        |
| -2   | Duplicate argument      |
| -3   | Handler function failed |

All errors are accompanied by **descriptive messages** to guide users.

---

### 5. Extensible and Maintainable

Adding new arguments requires only:

- Adding the new argument to the `cli_arg_table`.
- Adding the corresponding handler function in the `fptr_arr` array.

No external dependencies or complex parser logic — purely standard C (`stdio.h`, `string.h`).

---

### 6. Duplicate Detection and Validation

Tracks duplicates using the “relation” array. If a duplicate is detected:

- The offending argument is flagged.
- An error is returned, preventing handlers from executing in an inconsistent state.

---

## Usage Notes: v2 vs v3

- **v2:** For direct use in user programs. All CLI arguments passed to the program are handled by the library. Ideal when you want full argument management via this API.  
- **v3 (upcoming):** Designed for library development. Only a subset of CLI arguments is handled by the library; remaining arguments are left available for the user program. This guarantees that every user program using the library will always have some CLI arguments available for its own purposes.

---

## Example CLI Usage (v2)

```bash
./myapp -arg1
./myapp -arg2=42
./myapp -arg3=hello
