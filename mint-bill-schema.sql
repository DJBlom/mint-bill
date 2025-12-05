-- ============================================================================================
--  Schema: Mint-Bill Core Domain
-- --------------------------------------------------------------------------------------------
--  This script defines the relational schema for the Mint-Bill application.
--
--  Entities:
--    • business_details : Master record for any business in the system.
--    • admin            : Administrative / banking details for exactly one business.
--    • client           : Client configuration (VAT, statement schedule) tied to a business.
--    • statement        : Billing statement periods for a client’s business.
--    • invoice          : Individual invoices optionally linked to a statement.
--    • labor            : Line items (description + material lines) for an invoice.
--
--  High-level relationships:
--    • business_details (1) ──(1:1)── admin
--          - Enforced via UNIQUE business_id in admin and `only_one` guard.
--
--    • business_details (1) ──(1:1)── client
--          - Each client row maps to exactly one business_details row.
--
--    • client (1) ──(1:N)── statement
--          - Each statement belongs to a client’s business_id.
--          - Uniqueness on (business_id, period_start, period_end) prevents duplicates for
--            the same period.
--
--    • client (1) ──(1:N)── invoice
--          - Invoices are tied to business_id from client.
--          - Optional link to statement via statement_id.
--
--    • invoice (1) ──(1:N)── labor
--          - labor lines are grouped by invoice_id.
--          - UNIQUE(invoice_id, line_number, is_description) enforces a stable layout.
--
--  Cascade / integrity behavior:
--    • Deleting a business_details row cascades to admin and client.
--    • Deleting a client cascades to its statements and invoices (via business_id).
--    • Deleting an invoice cascades to labor lines.
--    • Deleting a statement sets invoice.statement_id to NULL (invoice still exists).
-- ============================================================================================
PRAGMA foreign_keys = ON;

BEGIN;

-- --------------------------------------------------------------------------------------------
-- Drop existing tables to allow re-creation during development or migration.
-- Order matters due to foreign-key dependencies.
-- --------------------------------------------------------------------------------------------
DROP TABLE IF EXISTS business_details;
DROP TABLE IF EXISTS admin;
DROP TABLE IF EXISTS client;
DROP TABLE IF EXISTS statement;
DROP TABLE IF EXISTS invoice;
DROP TABLE IF EXISTS labor;

-- --------------------------------------------------------------------------------------------
-- Table: business_details
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Canonical record for each business in the system. Everything else (admin, client,
--    statements, invoices) is anchored to this table via business_id.
--
--  Constraints:
--    • business_id      : INTEGER PRIMARY KEY (row identifier).
--    • business_name    : UNIQUE business name.
--    • email_address    : UNIQUE contact email for the business.
-- --------------------------------------------------------------------------------------------
CREATE TABLE business_details (
	business_id		INTEGER PRIMARY KEY,
	business_name        	TEXT NOT NULL UNIQUE,
	email_address		TEXT NOT NULL UNIQUE,
	contact_number		TEXT NOT NULL,
	street			TEXT NOT NULL,
	area_code		TEXT NOT NULL,
	town_name		TEXT NOT NULL
);

-- --------------------------------------------------------------------------------------------
-- Table: admin
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Stores admin / payment configuration for exactly one business. Includes bank details,
--    app password, and a generic client message used in emails.
--
--  Relationships:
--    • business_id → business_details(business_id)
--        - 1:1 mapping (UNIQUE on business_id).
--        - ON DELETE CASCADE: removing the business removes admin row.
--        - ON UPDATE CASCADE: business_id changes propagate.
--
--  Special constraint:
--    • only_one = 1 and UNIQUE
--        - Ensures there can be at most one admin row total in the database, i.e. one
--          “active admin profile”.
-- --------------------------------------------------------------------------------------------
CREATE TABLE admin (
	only_one		INTEGER NOT NULL UNIQUE DEFAULT 1
					CHECK (only_one = 1),
	business_id     	INTEGER NOT NULL UNIQUE
					REFERENCES business_details(business_id)
					ON DELETE CASCADE ON UPDATE CASCADE,
	bank_name       	TEXT NOT NULL,
	branch_code     	TEXT NOT NULL,
	account_number  	TEXT NOT NULL,
	app_password    	TEXT NOT NULL,
	client_message  	TEXT NOT NULL
);

-- --------------------------------------------------------------------------------------------
-- Table: client
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Represents a client configuration tied to a business. Stores VAT number and the
--    statement scheduling rule (e.g. "N,D").
--
--  Relationships:
--    • business_id → business_details(business_id)
--        - 1:1 mapping (UNIQUE on business_id).
--        - ON DELETE CASCADE: deleting the business deletes the client row.
--        - ON UPDATE CASCADE: business_id changes propagate.
--
--  Constraints:
--    • vat_number : UNIQUE per client to avoid duplicate client tax IDs.
-- --------------------------------------------------------------------------------------------
CREATE TABLE client (
	client_id		INTEGER PRIMARY KEY,
	business_id		INTEGER NOT NULL UNIQUE
					REFERENCES business_details(business_id)
					ON DELETE CASCADE ON UPDATE CASCADE,
	vat_number         	TEXT NOT NULL UNIQUE,
	statement_schedule 	TEXT NOT NULL
);

-- --------------------------------------------------------------------------------------------
-- Table: statement
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Represents a billing statement for a client over a defined period. This defines a
--    summary window (period_start/period_end) into which multiple invoices may logically
--    belong.
--
--  Relationships:
--    • business_id → client(business_id)
--        - Links each statement to the client’s business entry.
--        - ON DELETE CASCADE: deleting client removes all their statements.
--        - ON UPDATE CASCADE: updated business_id cascades.
--
--  Constraints:
--    • UNIQUE (business_id, period_start, period_end)
--        - Prevents multiple statements for the same business and period.
-- --------------------------------------------------------------------------------------------
CREATE TABLE statement (
	statement_id   INTEGER PRIMARY KEY,
	business_id    INTEGER NOT NULL,
	period_start   TEXT    NOT NULL,  
	period_end     TEXT    NOT NULL,  
	statement_date TEXT    NOT NULL DEFAULT (date('now')),
	paid_status    TEXT    NOT NULL DEFAULT 'Not Paid', 

	FOREIGN KEY (business_id) REFERENCES client(business_id)
		ON DELETE CASCADE ON UPDATE CASCADE,

	UNIQUE (business_id, period_start, period_end)
);

-- --------------------------------------------------------------------------------------------
-- Table: invoice
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Represents a single invoice issued to a client. Optionally linked to a statement if
--    the invoice has been assigned to a specific billing period.
--
--  Relationships:
--    • business_id → client(business_id)
--        - Associates the invoice with the owning client/business.
--        - ON DELETE CASCADE: deleting the client removes the invoice.
--
--    • statement_id → statement(statement_id)
--        - Optional relation: an invoice may or may not belong to a statement.
--        - ON DELETE SET NULL: deleting a statement does *not* delete invoices, but clears
--          their statement_id, preserving history.
--        - ON UPDATE CASCADE: statement_id changes propagate.
--
--  Constraints:
--    • UNIQUE (business_id, order_number, job_card_number)
--        - Prevents duplicate invoices with the same business + order + job card combo.
-- --------------------------------------------------------------------------------------------
CREATE TABLE invoice (
	invoice_id		INTEGER PRIMARY KEY,
	business_id		INTEGER NOT NULL,
	statement_id		INTEGER,
	order_number		TEXT NOT NULL,
	job_card_number		TEXT NOT NULL,
	date_created		TEXT NOT NULL DEFAULT (date('now')),
	paid_status		TEXT NOT NULL DEFAULT 'Not Paid',
	material_total		TEXT NOT NULL,
	description_total	TEXT NOT NULL,
	grand_total		TEXT NOT NULL,
	FOREIGN KEY (business_id) REFERENCES client(business_id)
		ON DELETE CASCADE ON UPDATE CASCADE,
	FOREIGN KEY (statement_id) REFERENCES statement(statement_id)
		ON DELETE SET NULL ON UPDATE CASCADE,
	UNIQUE(business_id, order_number, job_card_number)
);

-- --------------------------------------------------------------------------------------------
-- Table: labor
-- --------------------------------------------------------------------------------------------
--  Purpose:
--    Stores individual line items for an invoice. Each row is either:
--      • a description line, or
--      • a material/quantity line
--    distinguished by is_description.
--
--  Relationships:
--    • invoice_id → invoice(invoice_id)
--        - Groups line items under a single invoice.
--        - ON DELETE CASCADE: deleting an invoice removes all associated labor rows.
--        - ON UPDATE CASCADE: invoice_id changes propagate.
--
--  Constraints:
--    • is_description IN (0,1)
--        - Boolean-like flag enforced via CHECK.
--
--    • UNIQUE (invoice_id, line_number, is_description)
--        - Prevents duplicate (line_number, is_description) combinations per invoice,
--          preserving a stable, ordered layout for rendering PDFs or UI tables.
-- --------------------------------------------------------------------------------------------
CREATE TABLE labor (
	labor_id		INTEGER PRIMARY KEY,
	invoice_id		INTEGER NOT NULL,
	line_number		INTEGER NOT NULL,
	quantity       		INTEGER NOT NULL,
	description      	TEXT NOT NULL,
	amount    		REAL NOT NULL,
	is_description		BOOLEAN NOT NULL
					CHECK (is_description IN (0,1))
					DEFAULT FALSE,
	FOREIGN KEY (invoice_id) REFERENCES invoice(invoice_id)
		ON DELETE CASCADE ON UPDATE CASCADE,
	UNIQUE(invoice_id, line_number, is_description)
);

COMMIT;
