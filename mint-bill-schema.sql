/*==============================================================================
  SQLite3 Schema — Billing/Business Domain
  ------------------------------------------------------------------------------
  Purpose:
    Creates (or re-creates) the database schema with enforced foreign keys.
    The script is *re-runnable*: it drops dependent tables first, then
    recreates all objects in a clean state.

  Entities:
    - BUSINESS_DETAILS (parent; PK: business_id)
    - ADMIN            (1:1 child of BUSINESS_DETAILS; PK/FK: business_id)
    - CLIENT           (1:1 child of BUSINESS_DETAILS; PK/FK: business_id)
    - INVOICE          (child of CLIENT;       PK: invoice_id, FK: business_id)
    - LABOR            (child of INVOICE;      PK: labor_id,   FK: invoice_id)

  Relationships / Cardinality:
    BUSINESS_DETAILS (1) ── (1) ADMIN
    BUSINESS_DETAILS (1) ── (1) CLIENT
    CLIENT          (1) ── (0..*) INVOICE   [ON DELETE/UPDATE CASCADE]
    INVOICE         (1) ── (0..*) LABOR     [ON DELETE/UPDATE CASCADE]

  Key Constraints & Notes:
    - PRAGMA foreign_keys = ON; is set to enforce referential integrity.
    - Unique constraints: CLIENT.vat_number, BUSINESS_DETAILS.email_address.
    - This script uses DROP TABLE IF EXISTS in child→parent order to allow
      repeated execution without errors.
    - SQLite has no DROP SCHEMA; to fully reset, delete the DB file.

  Usage:
    $ sqlite3 mydb.sqlite3 < schema.sql

  Revision:
    <v1.0> Initial version. Update this header when altering the schema.
==============================================================================*/
PRAGMA foreign_keys = ON;

BEGIN;

DROP TABLE IF EXISTS business_details;
DROP TABLE IF EXISTS admin;
DROP TABLE IF EXISTS client;
DROP TABLE IF EXISTS invoice;
DROP TABLE IF EXISTS labor;

CREATE TABLE business_details (
	business_id		INTEGER PRIMARY KEY,
	business_name        	TEXT NOT NULL UNIQUE,
	email_address		TEXT NOT NULL UNIQUE,
	contact_number		TEXT NOT NULL,
	street			TEXT NOT NULL,
	area_code		TEXT NOT NULL,
	town_name		TEXT NOT NULL
);

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

CREATE TABLE client (
	business_id		INTEGER PRIMARY KEY
					REFERENCES business_details(business_id)
					ON DELETE CASCADE ON UPDATE CASCADE,
	vat_number         	TEXT NOT NULL UNIQUE,
	statement_schedule 	TEXT NOT NULL
);

CREATE TABLE invoice (
	invoice_id		INTEGER PRIMARY KEY,
	business_id		INTEGER NOT NULL,
	order_number		TEXT NOT NULL,
	job_card_number		TEXT NOT NULL,
	date_created		TEXT NOT NULL DEFAULT (date('now')),
	paid_status		TEXT NOT NULL,
	material_total		TEXT NOT NULL,
	description_total	TEXT NOT NULL,
	grand_total		TEXT NOT NULL,
	FOREIGN KEY (business_id) REFERENCES client(business_id)
		ON DELETE CASCADE ON UPDATE CASCADE,
	UNIQUE(business_id, order_number, job_card_number)
);

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
