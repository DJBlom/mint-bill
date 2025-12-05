# Security Details
By utilizing SQLCipher the sqlite database is encrypted using a password.    
This will require the user to store the database password in the keyring   
--Passwords and Keys--. They will have to install this application on their    
host machine in order for the program to work. Alternatively, the program     
will prompt the user for the password on the first installation of the application.


# Test Database Passwords
All test databases have the same password **123456789**.
To open a database run these commands from the root of the project directory.
Commands:

- To enter the database: `sqlcipher storage/tests/<database_name>`
- To access the database: `PRAGMA key=123456789;`
- To list all tabels in the database: `.table` 

# Production Database 
The production database passwords are entered at the initial application launch time.   
Production passwords will never be required to be entered after the initial launch of    
the application.

## Production Database Testing
The following commands and tools can help speed up the system integration tests when      
utilizating the production database.
Since this application is using the secret service key ring to hold the database password   
you can use the **secret-tool** to add, remove, or lookup the database password.

Commands:

- To look up the password: `secret-tool lookup number 0`
- To remove the password: `secret-tool clear number 0`
