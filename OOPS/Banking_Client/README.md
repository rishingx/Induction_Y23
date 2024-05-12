# Introduction

This is a modular approach to the OOPS assignment of implementing a bank management system.

The program is broken down into multiple files, with each file containing only one class definition so as to keep the code clean and modular.

Caching is done in individual .bin files for each user so that the program only access a small chunk of memory at a time. Since, I couldn't implement a proper database, the system is pretty vulnerable and not secure at all. However it does serve the purpose that is required.

# File Descriptions

| File                | Description                                                                                                      |
| ------------------- | ---------------------------------------------------------------------------------------------------------------- |
| BankAccount.h       | Defines the class for BankAccount, and its derived classes CurrentAccount and SavingsAccount                     |
| BankAccountHolder.h | Defines the class for BankAccountHolder                                                                          |
| BranchManager.h     | Defines the class for the BranchManager aka the root user                                                        |
| Main.cpp            | Contains utility functions, wrapper functions for methods of the above classes, cli parser and the main function |
# Folder Descriptions

| Folder          | Description                                                                  |
| --------------- | ---------------------------------------------------------------------------- |
| AccountHolders/ | Contains .bin files for each account holder                                  |
| Accounts/       | Contains .bin files for each account                                         |
| Statements/     | Will contain .txt files for statements of each account (not implemented yet) |
# Command Descriptions

| Level  | Command    | Description                                   | Status       |
| ------ | ---------- | --------------------------------------------- | ------------ |
| Global | help       | Prints the default help message               | Incomplete   |
| Global | login      | Log in as a regular user                      | Complete<br> |
| Global | root-login | Log in as the root user (password: "root")    | Complete     |
| Global | signup     | Sign up a new user                            | Complete     |
| Global | exit       | Quits the cli                                 | Complete     |
|        |            |                                               |              |
| Root   | getacc     | Returns details of all account holders        | Incomplete   |
| Root   | getstat    | Returns statement of a specific account       | Incomplete   |
| Root   | ffwd       | Fast forwards the balance of savings accounts | Incomplete   |
| Root   | logout     | Log out of the root user account              | Complete     |
|        |            |                                               |              |
| User   | crtacc     | Create a new account (Savings/Current)        | Complete     |
| User   | getacc     | Get all accounts of the current user          | Complete     |
| User   | delacc     | Deletes an account                            | Incomplete   |
| User   | chpass     | Change the password of the user               | Complete     |
| User   | deposit    | Deposit money into an account                 | Complete     |
| User   | withdraw   | Withdraw money from an account                | Complete     |
| User   | transfer   | Transfer money across accounts                | Complete     |
| User   | logout     | Log out of the user account                   | Complete     |
# Things left to implement

1. Proper formatting of output to cli
2. "delacc" command for the regular user to delete an account
3. Statement feature to record every transaction
4. "getacc" command for the root user to retrieve all account holders
5. "getstat" command for the root user to retrieve bank statement of an account
6. "ffwd" command for the root user to fast forward the bank accounts