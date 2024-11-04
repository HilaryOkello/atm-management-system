# atm-management-system

### Overview

atm-management-system is a console-based application written in C that allows users to manage their accounts through functionalities such as account creation, updating account details, checking account information, making transactions, and transferring account ownership.

### Features

- User Registration/Login: Register a new user or log in with existing credentials.
- Create a New Account: Users can create unique accounts.
- Update Account Information: Update phone numbers or country information for existing accounts.
- Check Account Details: View details of a specific account, including interest calculations for certain account types: `savings`, `fixed01`, `fixed02`, and `fixed03`.
- List Owned Accounts: Display all accounts owned by the user.
- Make Transactions: Deposit or withdraw funds from accounts (with restrictions on fixed accounts).
- Remove Account: Delete existing accounts.
- Transfer Account Ownership: Transfer account ownership to another registered user.

## Setup

To build and run the application, follow these steps:

### Clone the Repository:

```bash
git clone https://learn.zone01kisumu.ke/git/hilaokello/atm-management-system
cd atm-management-system
```

### Compile the Program:

Use the provided Makefile to compile the project:
```bash
make
```

### Run the Executable:

 After compilation, an executable named atm will be generated. Run it with:
```bash
./atm
```

## Usage

Upon running the program, you will be presented with a login/register menu.
Choose an option to either log in, register a new account, or exit.
If you log in successfully, you will be directed to the main menu where you can manage your accounts.
Follow the prompts to perform various actions.

### Terminal Interface

The application provides a color-coded terminal interface for a better user experience. Options are clearly displayed, and feedback is provided for user actions.

### File Structure

```bash

.
├── data
│   ├── records.txt      # Stores account information for users
│   └── users.txt        # Stores user information
├── Makefile              # Build instructions
└── src
    ├── auth.c           # Authentication logic
    ├── header.h         # Header file with struct definitions and function 
    ├── main.c           # Main program logic and menu interface
    └── system.c         # Core system functionalities
```

## License
This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.