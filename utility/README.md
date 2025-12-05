# Utility Module

The `utility` module provides a collection of reusable, low-level helper components used throughout the project. These utilities handle common concerns such as string slicing, date computation, file reading, and interface abstraction. The module is designed to be lightweight, thread-safe where required, and highly modular so individual utilities can be used independently.

## Summary

The `utility` module provides foundational services that the broader system relies on:

- **String slicing utilities** for formatting output.
- **Date computations** for business logic.
- **Thread-safe file handling**.
- **Lightweight abstractions** to maintain consistent design.

These utilities are intentionally small, focused, and extensively unit-tested to ensure safety and correctness.

---

## Directory Structure

---
utility/
├── CMakeLists.txt
├── include
│   ├── boundary_slicer.h
│   ├── date_manager.h
│   ├── file.h
│   ├── slicer.h
│   └── word_slicer.h
├── mocks
│   ├── include
│   └── source
├── source
│   ├── boundary_slicer.cpp
│   ├── date_manager.cpp
│   ├── file.cpp
│   └── word_slicer.cpp
└── tests
    ├── boundary_slicer_test.cpp
    ├── date_manager_test.cpp
    ├── file_test.cpp
    └── word_slicer_test.cpp
---

---

## Components

### **1. `slicer` Interface**
`include/slicer.h`

Defines a minimal interface for slicing string data into multiple segments.  
Classes implementing this interface must provide:

- `std::vector<std::string> slice(const std::string&)`

All string slicing utilities derive from this interface.

---

### **2. `boundary_slicer`**
`include/boundary_slicer.h` / `source/boundary_slicer.cpp`

A word-aware text slicer that:

- Breaks long strings into multiple lines.
- Ensures words are not split unless they exceed the maximum line width.
- Handles oversized words by chunking them safely.
- Is thread-safe using internal mutex protection.
- Supports configuration of maximum line length.

Used for formatting descriptions so they remain readable when printed or displayed.

---

### **3. `word_slicer`**
`include/word_slicer.h` / `source/word_slicer.cpp`

A simple whitespace tokenizer that:

- Splits strings into individual words.
- Enforces a maximum number of tokens (50).
- Is thread-safe.
- Ignores input beyond the maximum capacity.

Useful for labels, tags, or UI summaries where data must be bounded.

---

### **4. `date_manager`**
`include/date_manager.h` / `source/date_manager.cpp`

Responsible for:

- Retrieving the current date in `MM-DD-YYYY` format.
- Computing date ranges based on business rules for statement periods.
- Handling both weekly and monthly bound calculations.
- Using C++20 `<chrono>` for accurate calendar operations.

This utility centralizes all date computation logic.

---

### **5. `file`**
`include/file.h` / `source/file.cpp`

A RAII file wrapper providing:

- Safe, exception-based construction (throws when file cannot be opened).
- Thread-safe reading using mutex locking.
- Simple text extraction via `read()`.

Used for loading email templates, embedded content, or static data.

---

## Tests

Located under `utility/tests/` and made with CppUTest:

### **boundary_slicer_test.cpp**
Validates:

- Correct slicing into 40-character segments.
- Handling of oversized words.
- Preservation of word boundaries.
- Thread safety.

### **date_manager_test.cpp**
Validates:

- Computation of all statement period combinations.
- Handling of empty or invalid schedule strings.
- Formatting of current date.

### **file_test.cpp**
Validates:

- Reading plain text and HTML files.
- RAII safety.
- Thread safety.
- Non-copy/move behavior.

### **word_slicer_test.cpp**
Validates:

- Tokenization into words.
- Maximum word count enforcement.
- Handling of empty input.
- Thread safety.

---

## CMake Integration

The module builds as part of the overall project via `utility/CMakeLists.txt`.  
It compiles the utility sources into an object library so other modules can freely link against it.

---
