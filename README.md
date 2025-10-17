# CLI Argument Dispatcher

A lightweight, efficient, and easily extensible command line argument dispatcher in C, designed to simplify and streamline the parsing and handling of CLI arguments in your applications.

## Overview

This project provides a modular framework to:

* Define and register CLI arguments of switch type only (--switch) alongside their handler functions
* Validate arguments at runtime for correctness and duplication
* Dispatch the correct handler based on the input arguments
* Provide clear, informative error handling with detailed messages

## Features & Advantages

### 1. **Bitmask-Based Argument Tracking**

The dispatcher uses bitmasking to efficiently track which CLI arguments have been passed and detect duplicates without repeated string comparisons. This ensures:

* **Fast lookups** even with many arguments
* **Minimal overhead** compared to iterative search methods

### 2. **Pre-Validation Before Execution**

All arguments are validated before any handler runs, preventing inconsistent states caused by invalid or duplicate arguments.

### 3. **Clear Error Codes and Messaging**

Standardized return codes and descriptive error messages allow easy integration with other systems and better user feedback.

### 4. **Extensible and Maintainable**

Adding new arguments and handlers requires only:

* Updating the `cli_arg_table` with the new argument string
* Adding the corresponding handler function to the function pointer array

No complex parser generators or external dependencies are needed.

### 5. **No Dependency on External Libraries**

Pure C implementation with standard library headers only (`stdio.h` and `string.h`), making it portable and easy to embed in any project.

### 6. **Controlled Argument Limits**

Supports defining a maximum number of allowed CLI arguments, preventing misuse or unexpected inputs.

---

## Comparison with Existing Approaches

| Aspect                | This Project                    | Typical Alternatives             |
| --------------------- | ------------------------------- | -------------------------------- |
| Parsing Method        | Bitmask + direct string match   | Loop with repeated strcmp calls  |
| Duplication Detection | Built-in bitmask flags          | Often overlooked or manual       |
| Extensibility         | Simple array updates            | May require complex parser rules |
| Dependencies          | None (standard C only)          | Often rely on getopt, argp, etc. |
| Error Handling        | Explicit error codes + messages | Sometimes silent or less clear   |
| Performance           | Low overhead, fast dispatch     | Higher overhead due to parsing   |

---

## Getting Started

### Build

```bash
gcc -o myapp test.c cli_arg_proc.c
```

### Usage

```bash
./myapp -addaccount
./myapp -opt1=value
```

If invalid or duplicate arguments are passed, the program will display informative messages explaining the issue.
Stick to the usage of passing arguments having key value pairs as given in second example i.e. -opt=value instead of -opt value, though both are true typically but parsing in both sense makes library heavier and needs some more functionalities to be added like in case two different key options are used but they have same values.

---

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

## Contributions & Feedback

Contributions, bug reports, and suggestions are welcome! Feel free to open issues or pull requests.

---
