# Password Bruteforcer

A simple password cracking utility for Windows that attempts to log in to a system using a list of passwords. It uses the net use command to check each password against the provided IP address and username. The tool splits the password list into batches and processes them recursively.

## Features
- Batch Processing: Divides the password list into manageable chunks (default size: 1000 passwords per batch).
- Recursive Processing: Splits larger batches into smaller sub-batches if needed.
- Command Execution: Uses CreateProcessW to run the net use command and check password validity.
- Thread Safety: Ensures safe operation with CriticalSection for concurrent tasks.
- Logging: Prints each password attempt and shows success when the correct password is found.
## Requirements
Windows OS: The program uses Windows-specific APIs (CreateProcessW, net use).
Text File with Password List: A file containing passwords to attempt.
Basic Knowledge: Familiarity with IP addresses and usernames.
** How to Use
Run the Program: After starting, provide:

1. IP address of the target system

2. Username to use for login

3. Path to the password list file
