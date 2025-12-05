# Mint-Bill GUI

The **gui/** directory contains the complete graphical user interface layer of the
Mint-Bill application. It defines the visual layout, page structure, navigation
flow, and all top-level GTKMM widgets used by the program. The GUI integrates
closely with the reusable components in `gui/components`, but represents a
distinct layer where the actual application windows, dialogs, and page logic
reside.

---

## Overview

The GUI layer is composed of:

- A **Gtk::Application**-driven window system  
- A large `.ui` file designed in **Cambalache**  
- A custom CSS theme  
- A library of reusable GUI components (stack manager, dialogs, printers, list
  views, column views, etc.)  
- Wrapper logic under `gui/source/` implementing page behaviors  
- A full CMake-based build system and unit tests

---

## Directory Structure

---
gui/
├── CMakeLists.txt
├── components
├── include
├── mint-bill.cmb
├── mint-bill.css
├── mint-bill.ui
├── mint-bill.ui.cmb
├── source
└── tests
---


---

## UI Assets

### **mint-bill.ui**
This is the final UI layout compiled by Cambalache.  
It defines:

- The primary window structure  
- All `Gtk::Stack` pages  
- Dialog widgets  
- Reusable layout containers  
- Naming/IDs used by components to bind themselves via `Gtk::Builder`

### **mint-bill.cmb**
Cambalache project source file. Open it in Cambalache to visually edit the GUI.

```bash
cambalache mint-bill.cmb
