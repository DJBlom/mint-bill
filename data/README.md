# Data Module

## Summary

The **data** module contains all foundational data structures used across the application.  
It provides lightweight, validation-aware C++ classes for representing business entities,
clients, invoices, statements, PDF-composite structures, and all related columnar data.
Each class includes strict validation rules, safe copy/move behavior, optional thread
synchronization, and predictable APIs for assigning and retrieving fields.  
A full suite of CppUTest unit tests ensures correctness, robustness, and long-term stability.

---

## Overview

The module centralizes all plain-data representations required by higher-level components.
This helps maintain consistency, reduce duplication, and ensure that all parts of the system
operate on well-validated, predictable data structures.

The design goals are:

- **Self-contained models** with no business logic.  
- **Strong validation** via `is_valid()` on every data class.  
- **Thread-safety** where needed via internal mutex protection.  
- **Clear ownership semantics** using copy/move constructors & assignment operators.  
- **Uniform field constraints** (max lengths, email format, schedule formats, etc.).  
- **Strict test coverage** using CppUTest suites for every model.

---

## Directory Structure
---
data
├── CMakeLists.txt
├── include
│   ├── admin_data.h
│   ├── billing_data.h
│   ├── business_data.h
│   ├── client_data.h
│   ├── column_data.h
│   ├── email_data.h
│   ├── invoice_data.h
│   ├── pdf_invoice_data.h
│   ├── pdf_statement_data.h
│   └── statement_data.h
├── mocks
│   ├── include
│   │   └── mock_pdf_invoice_data.h
│   └── source
│       └── mock_pdf_invoice_data.cpp
├── README.md
├── source
│   ├── admin_data.cpp
│   ├── billing_data.cpp
│   ├── business_data.cpp
│   ├── client_data.cpp
│   ├── column_data.cpp
│   ├── email_data.cpp
│   ├── invoice_data.cpp
│   ├── pdf_invoice_data.cpp
│   ├── pdf_statement_data.cpp
│   └── statement_data.cpp
└── tests
    ├── admin_data_test.cpp
    ├── billing_data_test.cpp
    ├── business_data_test.cpp
    ├── client_data_test.cpp
    ├── column_data_test.cpp
    ├── email_data_test.cpp
    ├── invoice_data_test.cpp
    ├── pdf_invoice_data_test.cpp
    ├── pdf_statement_data_test.cpp
    └── statement_data_test.cpp
---


---

## Purpose of Each Data Class

### `admin_data`
Holds administrative/business owner metadata such as contact info, banking details,
client messages, passwords, and identity fields. Provides strong validation of all inputs.

### `billing_data`
Represents a single billing entry—ID, business name, billing date, and paid status—along
with validation and safe copying/moving.

### `business_data`
Core address-and-contact information for a business entity.  
Used by invoices, statements, and email composition.

### `client_data`
Contains all client-side details: name, address, VAT number, contact information,
email, and statement schedule with strict formatting validation.

### `column_data`
Represents a single row of invoice/statement content, including:

- quantity  
- description  
- amount  
- metadata (row number, description-mode flag)

It enforces strict bounds on quantity (0–8 digits), description length (<200 chars),
and amount formatting.

### `invoice_data`
Aggregates all fields required for an invoice:

- job card number  
- order number  
- totals  
- description and material columns  
- dates and paid status  

Designed to support potentially large column datasets.

### `statement_data`
Summarizes a billing period for a client (start/end dates, billing date, paid status,
and identifiers). Ensures compliance with length constraints and validity rules.

### `email_data`
A composite that binds together:

- admin data  
- client data  
- invoice data  
- attachments (PDFs)

Used by the email-generation subsystem.

### `pdf_invoice_data`
A wrapper class that combines:

- client  
- admin  
- invoice

into a single unit for PDF rendering.

### `pdf_statement_data`
Represents a statement PDF, containing:

- A single statement  
- A collection of related invoice PDFs (many-to-one)

---

## Validation and Error Handling

Every data class implements an `is_valid()` method that ensures:

- Required fields are present and non-empty.  
- Email addresses follow proper formatting.  
- Character length constraints are never exceeded.  
- Numeric fields remain within allowed bounds.  
- Composite classes recursively validate contained objects.  

This design prevents malformed data from propagating through the system.

---

## Unit Testing

The test suite verifies:

- Field assignment & retrieval  
- Correct `is_valid()` behavior for good/bad cases  
- Copy and move constructors/assignments  
- Thread-safety behavior where applicable  
- Collection handling (e.g., invoice column vectors)  

`mock_pdf_invoice_data.*` provides reusable sample generators for clean, consistent tests.

---

## CMake Integration

The module is built as an **OBJECT library**:
