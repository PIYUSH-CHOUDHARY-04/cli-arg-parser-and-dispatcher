CLI Argument Dispatcher Library

A robust, lightweight, and extensible command line argument dispatcher in C, designed to efficiently parse, validate, and handle CLI arguments with minimal overhead while supporting both switch-type and value-based arguments.

Overview

This library provides a structured framework to:

Define and register CLI arguments (-arg, -arg=X) and their corresponding handler functions.

Validate arguments for correctness and detect duplicates before execution.

Dispatch the correct handler functions based on the passed arguments.

Provide clear, descriptive error messages for invalid, unknown, or duplicate arguments.

Features & Advantages
1. Efficient Argument Tracking

Uses a lightweight array-based “relation” system to:

Track which arguments are present

Detect duplicates

Avoid repeated string comparisons

This ensures fast argument validation and dispatch even for multiple CLI arguments.

2. Switch and Value-Based Argument Support

Supports two types of arguments:

Switch arguments: -arg1
→ Executes the handler directly.

Value-based arguments: -arg2=X
→ Passes the value X to the handler for processing (supports int, float, string, etc.).

This allows flexible and dynamic argument handling without complicating the library.

3. Pre-Validation Before Execution

All arguments are validated before any handler runs:

Detects unknown arguments

Detects duplicates

Provides comprehensive error messages

This prevents inconsistent program states and ensures robust operation.

4. Clear Error Codes and Messaging

Standardized return codes allow precise error detection:

Code	Meaning
0	Success
-1	Extra arguments passed
-2	Invalid argument
-3	Duplicate argument
-4	Handler function failed
