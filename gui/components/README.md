# Components Module

The **components** module provides a set of reusable GUI building blocks used
throughout the Mint-Bill application. These components wrap GTKMM widgets and
implement higher-level behaviors such as page-stack management, printing, and
common GUI interaction patterns. The module is structured as an **OBJECT
library**, intended to be linked into other GUI targets.

---

## Features

### ✔ GUI Stack Management (`stack`)
A thin abstraction around `Gtk::Stack`, providing:
- Subscription and notification callbacks when the visible page changes.
- Programmatic page switching.
- Validation of stack state before use.

### ✔ Printing Support (`printer`)
A GTKMM + Poppler-based printing subsystem that:
- Loads PDFs from byte buffers.
- Computes page ranges across multiple documents.
- Renders pages using Cairo.
- Runs synchronous or dialog-based print operations.

### ✔ GUI Parts (`gui_parts`)
Shared helper components (dialogs, buttons, search bars, and other
interactions). These parts implement observer-based or callback-based patterns
to keep pages decoupled from the UI widgets they manage.

---

## Directory Structure

---
gui/components/
├── CMakeLists.txt
├── include
│   ├── gui_parts.h
│   ├── part.h
│   ├── printer.h
│   └── stack.h
├── source
│   ├── gui_parts.cpp
│   ├── printer.cpp
│   └── stack.cpp
└── tests
    ├── gui_parts_tests.cpp
    └── stack_test.cpp
---



---

## Build Requirements

- **CMake ≥ 3.25**
- **GTKMM 4.x**
- **Poppler-glib + C++ bindings**
- C++20 or later (recommended)
- A working Mint-Bill top-level build environment (this module relies on
  include paths from parent directories)

---

## Building the Module

This module is **not a standalone project**. It is built as part of the main
Mint-Bill CMake tree.

However, the logic inside `CMakeLists.txt` works as follows:

```cmake
add_library(components OBJECT
    source/stack.cpp
    source/printer.cpp
    source/gui_parts.cpp
)
