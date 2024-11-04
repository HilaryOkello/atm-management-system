#include "header.h"

const char *RECORDS = "./data/records.txt";
#define MAX_NAME_SIZE 50
#define DATE_INPUT_SIZE 11
#define MAX_ACC_NUM_SIZE 10
#define MAX_AMOUNT_SIZE 20
#define MAX_PHONE_SIZE 20
#define MAX_ACC_TYPE_SIZE 10
#define MAX_AMOUNT 100000
#define MIN_AMOUNT 0

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}
void updateAccountInFile(FILE *ptr, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

int userHasAtLeastOneAccount(struct User u)
{
    char name[MAX_NAME_SIZE];
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    while (getAccountFromFile(pf, name, &r))
    {
        if (strcmp(name, u.name) == 0)
        {
            fclose(pf);
            return 1;
        }
    }
    fclose(pf);
    printf("\t\t====== It seems like you do not have any accounts, %s =====\n\n", u.name);
    printf("\t\t====== Consider creating one to be able to perform this operation, %s =====\n\n", u.name);
    return 0;
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf(COLOR_RED "Insert a valid operation!\n" COLOR_RESET);
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf(COLOR_GREEN "\n✔ Success!\n\n" COLOR_RESET);
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf(COLOR_RED "Insert a valid operation!\n" COLOR_RESET);
        goto invalid;
    }
}

char *processInterest(struct Record r)
{
    static char interestMessage[1000];
    float interestRate = 0.0;
    float interestAmount;

    if (strcmp(r.accountType, "saving") == 0)
    {
        interestRate = 0.07;
        interestAmount = r.amount * interestRate / 12;
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will get $%.2f as interest on day %d of every month",
                 interestAmount, r.deposit.day);
    }
    else if (strcmp(r.accountType, "fixed01") == 0)
    {
        interestRate = 0.04;
        interestAmount = r.amount * interestRate;
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will get $%.2f as interest on %d/%d/%d",
                 interestAmount, r.deposit.month, r.deposit.day, r.deposit.year + 1);
    }
    else if (strcmp(r.accountType, "fixed02") == 0)
    {
        interestRate = 0.05;
        interestAmount = r.amount * interestRate * 2;
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will get $%.2f as interest on %d/%d/%d",
                 interestAmount, r.deposit.month, r.deposit.day, r.deposit.year + 2);
    }
    else if (strcmp(r.accountType, "fixed03") == 0)
    {
        interestRate = 0.08;
        interestAmount = r.amount * interestRate * 3;
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will get $%.2f as interest on %d/%d/%d",
                 interestAmount, r.deposit.month, r.deposit.day, r.deposit.year + 3);
    }
    else if (strcmp(r.accountType, "current") == 0)
    {
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will not get interests because the account is of type current");
        return interestMessage;
    }
    else
    {
        // Handle unexpected account types
        snprintf(interestMessage, sizeof(interestMessage),
                 "Invalid account type specified");
    }

    return interestMessage;
}

void removeRecordAtIndex(struct Record records[], int *size, int index)
{
    if (index < 0 || index >= *size)
    {
        printf("Record does not exist. Index out of bounds.\n");
        return;
    }

    for (int i = index; i < *size - 1; i++)
    {
        records[i] = records[i + 1];
    }

    (*size)--;
}

// Function to display account information
void displayAccountInfo(struct Record r)
{
    printf("_____________________\n");
    printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \nCountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
           r.accountNbr,
           r.deposit.day,
           r.deposit.month,
           r.deposit.year,
           r.country,
           r.phone,
           r.amount,
           r.accountType);
}

// Validation Functions

// Check if a string is alphanumeric
bool isAlphanumeric(const char *str)
{
    while (*str)
    {
        if (!isalnum((unsigned char)*str))
        {
            return false;
        }
        str++;
    }
    return true;
}

// Check if a string is numeric and positive
bool isNumericAndPositive(const char *str)
{
    if (*str == '\0')
        return false; // Empty string
    if (*str == '-')
        return false; // Negative numbers
    while (*str)
    {
        if (!isdigit((unsigned char)*str))
        {
            return false;
        }
        str++;
    }
    return true;
}

// Validate amount is numeric
bool amountValidation(const char *str)
{
    if (*str == '\0' || *str == '-' || *str == '.')
        return false;

    bool decimalPointSeen = false;

    while (*str)
    {
        if (*str == '.')
        {
            if (decimalPointSeen)
                return false;
            decimalPointSeen = true;
        }
        else if (!isdigit((unsigned char)*str))
        {
            return false;
        }
        str++;
    }

    // Ensure the string does not end with a decimal point
    if (*(str - 1) == '.')
        return false;

    return true;
}

// Validate the date format MM/DD/YYYY
bool isValidDate(const char *date)
{
    int month, day, year;
    if (sscanf(date, "%d/%d/%d", &month, &day, &year) != 3)
    {
        return false;
    }
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900)
    {
        return false;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return false;
    }
    if (month == 2)
    {
        bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if ((leapYear && day > 29) || (!leapYear && day > 28))
        {
            return false;
        }
    }
    return true;
}

// Check if the account type is valid
bool isValidAccountType(const char *type)
{
    return (strcmp(type, "saving") == 0 ||
            strcmp(type, "current") == 0 ||
            strcmp(type, "fixed01") == 0 ||
            strcmp(type, "fixed02") == 0 ||
            strcmp(type, "fixed03") == 0);
}

// Creates a new account for user and updates the records file
void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char name[MAX_NAME_SIZE];
    int maxId = -1;
    FILE *pf = fopen(RECORDS, "a+");
    if (pf == NULL)
    {
        printf(COLOR_RED "Error opening file!\n" COLOR_RESET);
        exit(1);
    }

    while (getAccountFromFile(pf, name, &cr))
    {
        if (cr.id > maxId)
        {
            maxId = cr.id;
        }
    }

    // Set the new record id
    r.id = maxId + 1;

noAccount:
    system("clear");
    printf("%s\t\t\t===== New record =====%s\n", COLOR_GREEN, COLOR_RESET);

    // Input and Validation

    // Date && Validation
    char dateInput[DATE_INPUT_SIZE];
    while (true)
    {
        printf("\nEnter today's date (MM/DD/YYYY): ");
        scanf("%10s", dateInput);
        if (isValidDate(dateInput))
        {
            sscanf(dateInput, "%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid date format. Please use MM/DD/YYYY.\n" COLOR_RESET);
        }
    }
    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            r.accountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    while (getAccountFromFile(pf, name, &cr))
    {
        if (strcmp(name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf(COLOR_RED "✖ This Account already exists for this user\n\n" COLOR_RESET);
            goto noAccount;
        }
    }
    // Country and Validation
    printf("\nEnter the country: ");
    scanf("%99s", r.country);

    // Phone and Validation
    char phoneInput[MAX_PHONE_SIZE];
    while (true)
    {
        printf("\nEnter the phone number: ");
        scanf("%19s", phoneInput);
        if (isNumericAndPositive(phoneInput))
        {
            r.phone = atoi(phoneInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid phone number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    // Amount nand Validation
    char amountInput[MAX_AMOUNT_SIZE];
    while (true)
    {
        printf("\nEnter amount to deposit: $");
        scanf("%19s", amountInput);
        if (amountValidation(amountInput))
        {
            r.amount = atof(amountInput);
            if (r.amount > MAX_AMOUNT)
            {
                printf(COLOR_RED "Maximum($%d)transaction limit breached.\n" COLOR_RESET, MAX_AMOUNT);
            }
            else
            {
                break;
            }
        }
        else
        {
            printf(COLOR_RED "✖ Invalid amount. It must be a positive number.\n" COLOR_RESET);
        }
    }

    // Account Type and Validation
    char accountTypeInput[MAX_ACC_TYPE_SIZE];
    while (true)
    {
        printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice: ");
        scanf("%9s", accountTypeInput);
        if (isValidAccountType(accountTypeInput))
        {
            strcpy(r.accountType, accountTypeInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account type. Please select a valid option.\n" COLOR_RESET);
        }
    }
    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

// Creates a new account for user and updates the records file
void updateAccInfo(struct User u)
{
    system("clear");
    printf("%s\t\t\t===== Update Account Information =====%s\n", COLOR_GREEN, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    struct Record r;
    struct Record records[100];
    int noOfRecords = 0;
    int accountExists = 0;
    int phoneNumber;
    char country[100];
    int targetAccountNbr = 0;
    int recordToUpdateIndex = 0;

    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            targetAccountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    while (getAccountFromFile(pf, records[noOfRecords].name, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && strcmp(records[noOfRecords].name, u.name) == 0)
        {
            r = records[noOfRecords];
            accountExists = 1;
            recordToUpdateIndex = noOfRecords;
        }
        noOfRecords++;
    }
    fclose(pf);

    if (!accountExists)
    {
        printf(COLOR_YELLOW "Account number %d not found." COLOR_RESET, targetAccountNbr);
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    displayAccountInfo(r);

    int option;
    printf("\nWhich information do you want to update?\n");
    printf("\t\t[1]-> phone number\n");
    printf("\t\t[2]-> country\n");
    printf("\t\t[3]-> Option to go back to main menu\n");
    // Option Validation
    char optionInput[2];
    while (true)
    {
        printf("\t\tEnter your choice:");
        scanf("%19s", optionInput);
        if (isNumericAndPositive(optionInput))
        {
            option = atoi(optionInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid option number. It must 1 or 2 or 3.\n" COLOR_RESET);
        }
    }

    switch (option)
    {
    case 1:
        // Phone and Validation
        char phoneInput[MAX_PHONE_SIZE];
        while (true)
        {
            printf("\nEnter the phone number: ");
            scanf("%19s", phoneInput);
            if (isNumericAndPositive(phoneInput))
            {
                phoneNumber = atoi(phoneInput);
                break;
            }
            else
            {
                printf(COLOR_RED "✖ Invalid phone number. It must be a positive number.\n" COLOR_RESET);
            }
        }
        records[recordToUpdateIndex].phone = phoneNumber;
        break;
    case 2:
        printf("\nEnter the new country:");
        scanf("%s", country);
        strcpy(records[recordToUpdateIndex].country, country);
        break;
    case 3:
        stayOrReturn(1, updateAccInfo, u);
        break;
    default:
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }

    fclose(pf);
    success(u);
}

// Lists details of a users account number
void checkAccount(struct User u)
{
    system("clear");
    printf("%s\t\t====== Check Account, %s =====%s\n\n", COLOR_GREEN, u.name, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, checkAllAccounts, u);
        return;
    }

    int accountNbr;
    int accountFound = 0;

    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            accountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    char name[MAX_NAME_SIZE];
    ;
    struct Record r;

    FILE *pf = fopen(RECORDS, "r+");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }

    while (getAccountFromFile(pf, name, &r))
    {
        if (strcmp(name, u.name) == 0 && accountNbr == r.accountNbr)
        {
            char *interestMessage = processInterest(r);
            displayAccountInfo(r);
            printf("%s\n", interestMessage);
            fclose(pf);
            accountFound = 1;
        }
    }

    if (!accountFound)
    {
        fclose(pf);
        printf(COLOR_YELLOW "Account number %d not found." COLOR_RESET, accountNbr);
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    success(u);
}

// Lists all accounts belonging to a user
void checkAllAccounts(struct User u)
{
    system("clear");
    printf("%s\t\t====== All Accounts from User, %s =====%s\n\n", COLOR_GREEN, u.name, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, checkAllAccounts, u);
        return;
    }
    char name[50];
    ;
    struct Record r;

    FILE *pf = fopen(RECORDS, "r+");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }

    while (getAccountFromFile(pf, name, &r))
    {
        if (strcmp(name, u.name) == 0)
        {
            displayAccountInfo(r);
        }
    }
    fclose(pf);
    success(u);
}

// Makes deposit and withdraw transactions on a user's account and updates the records file
void makeTransaction(struct User u)
{
    system("clear");
    printf("%s\t\t====== Make a Transaction, %s =====%s\n\n", COLOR_GREEN, u.name, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    struct Record r;
    struct Record records[100];
    int noOfRecords = 0;
    int accountExists = 0;
    int targetAccountNbr = 0;
    int recordToUpdateIndex = 0;

    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number you want to make a transactio on: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            targetAccountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    while (getAccountFromFile(pf, records[noOfRecords].name, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && strcmp(records[noOfRecords].name, u.name) == 0)
        {
            r = records[noOfRecords];
            accountExists = 1;
            recordToUpdateIndex = noOfRecords;
        }
        noOfRecords++;
    }
    fclose(pf);

    if (!accountExists)
    {
        printf(COLOR_YELLOW "Account number %d not found." COLOR_RESET, targetAccountNbr);
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
    {
        printf(COLOR_RED "\nYou cannot withdraw or deposit from a %s account" COLOR_RESET, r.accountType);
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    int option;
    printf("\nDo you want to:\n");
    printf("\t\t[1]-> Deposit\n");
    printf("\t\t[2]-> Withdraw\n");
    printf("\t\t[3]-> Option to go back to main menu\n");

    // Option Validation
    char optionInput[2];
    while (true)
    {
        printf("\t\tEnter your choice:");
        scanf("%19s", optionInput);
        if (isNumericAndPositive(optionInput))
        {
            option = atoi(optionInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid option number. It must 1 or 2 or 3.\n" COLOR_RESET);
        }
    }

    char amountInput[MAX_AMOUNT_SIZE];
    switch (option)
    {
    case 1:
        int depositAmount;
        // Amount nand Validation

        while (true)
        {
            printf("\nEnter amount to deposit: $");
            scanf("%19s", amountInput);
            if (amountValidation(amountInput))
            {
                depositAmount = atof(amountInput);
                if (depositAmount > MAX_AMOUNT || depositAmount <= MIN_AMOUNT)
                {
                    printf(COLOR_RED "Minimum(>%d) or Maximum(<=%d) transaction limit breached.\n" COLOR_RESET, MIN_AMOUNT, MAX_AMOUNT);
                }
                else
                {
                    break;
                }
            }
            else
            {
                printf(COLOR_RED "✖ Invalid amount. It must be a positive number.\n" COLOR_RESET);
            }
        }
        records[recordToUpdateIndex].amount = r.amount + depositAmount;
        printf(COLOR_YELLOW "\nDepositing %d to account number: %d" COLOR_RESET, depositAmount, r.accountNbr);
        break;
    case 2:
        int withdrawAmount;
        // Amount nand Validation
        while (true)
        {
            printf("\nEnter amount to deposit: $");
            scanf("%19s", amountInput);
            if (amountValidation(amountInput))
            {
                withdrawAmount = atof(amountInput);
                if (withdrawAmount > MAX_AMOUNT || withdrawAmount <= MIN_AMOUNT)
                {
                    printf(COLOR_RED "Minimum(>%d) or Maximum(<=%d) transaction limit breached.\n" COLOR_RESET, MIN_AMOUNT, MAX_AMOUNT);
                }
                else
                {
                    break;
                }
            }
            else
            {
                printf(COLOR_RED "✖ Invalid amount. It must be a positive number.\n" COLOR_RESET);
            }
        }
        if (withdrawAmount > r.amount)
        {
            printf(COLOR_RED "\nFail. The withdraw amount is superior to your balance:" COLOR_RESET);
            stayOrReturn(1, makeTransaction, u);
            return;
        }
        records[recordToUpdateIndex].amount = r.amount - withdrawAmount;
        printf(COLOR_YELLOW "\nWithdrawing %d from account number: %d" COLOR_RESET, withdrawAmount, r.accountNbr);
        break;
    case 3:
        stayOrReturn(1, makeTransaction, u);
        return;
    default:
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }
    fclose(pf);

    success(u);
}

// Removes a specific account number from the records file
void removeAccount(struct User u)
{
    system("clear");
    printf("%s\t\t====== Remove an Account, %s =====%s\n\n", COLOR_GREEN, u.name, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    struct Record r;
    struct Record records[100];
    int noOfRecords = 0;
    int accountExists = 0;
    int targetAccountNbr = 0;
    int recordToRemoveIndex = 0;

    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number you want to remove: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            targetAccountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    while (getAccountFromFile(pf, records[noOfRecords].name, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && strcmp(records[noOfRecords].name, u.name) == 0)
        {
            r = records[noOfRecords];
            accountExists = 1;
            recordToRemoveIndex = noOfRecords;
        }
        noOfRecords++;
    }
    fclose(pf);

    if (!accountExists)
    {
        printf(COLOR_YELLOW "Account number %d not found." COLOR_RESET, targetAccountNbr);
        stayOrReturn(1, removeAccount, u);
        return;
    }

    displayAccountInfo(r);

    int option;
    printf("\nAre you sure you want to remove this account?\n");
    printf("\t\t[1]-> Yes\n");
    printf("\t\t[2]-> No\n");

    // Option Validation
    char optionInput[2];
    while (true)
    {
        printf("\t\tEnter your choice:");
        scanf("%19s", optionInput);
        if (isNumericAndPositive(optionInput))
        {
            option = atoi(optionInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid option number. It must 1 or 2.\n" COLOR_RESET);
        }
    }

    switch (option)
    {
    case 1:
        int size = sizeof(records) / sizeof(records[0]);
        removeRecordAtIndex(records, &size, recordToRemoveIndex);
        noOfRecords--;
        printf("\nRemoving account number: %d\n", r.accountNbr);
        break;
    case 2:
        stayOrReturn(1, removeAccount, u);
        return;
    default:
        stayOrReturn(1, removeAccount, u);
        return;
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }

    fclose(pf);
    success(u);
}

// Transfers an acoount from one user to another and updates records
void transferAccount(struct User u)
{
    system("clear");
    printf("%s\t\t====== Transfer Account, %s =====%s\n\n", COLOR_GREEN, u.name, COLOR_RESET);
    if (!userHasAtLeastOneAccount(u))
    {
        stayOrReturn(1, updateAccInfo, u);
        return;
    }

    struct Record r;
    struct Record records[100];
    int noOfRecords = 0;
    int accountExists = 0;
    int targetAccountNbr = 0;
    char receiverUsername[50];
    int receiverFound = 0;
    int recordToUpdateIndex = 0;

    // Acc Number and Validation
    char accountNbrInput[MAX_ACC_NUM_SIZE];
    while (true)
    {
        printf("\nEnter the account number you want to transfer: ");
        scanf("%19s", accountNbrInput);
        if (isNumericAndPositive(accountNbrInput))
        {
            targetAccountNbr = atoi(accountNbrInput);
            break;
        }
        else
        {
            printf(COLOR_RED "✖ Invalid account number. It must be a positive number.\n" COLOR_RESET);
        }
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    while (getAccountFromFile(pf, records[noOfRecords].name, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && strcmp(records[noOfRecords].name, u.name) == 0)
        {
            r = records[noOfRecords];
            accountExists = 1;
            recordToUpdateIndex = noOfRecords;
        }
        noOfRecords++;
    }
    fclose(pf);

    if (!accountExists)
    {
        printf(COLOR_YELLOW "Account number %d not found." COLOR_RESET, targetAccountNbr);
        stayOrReturn(1, transferAccount, u);
        return;
    }
    displayAccountInfo(r);

    printf("\nEnter the username of the person you are transfering the account to:");
    scanf("%s", receiverUsername);

    FILE *fp;
    struct User receiver;
    fp = fopen("./data/users.txt", "a+");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    while (fscanf(fp, "%d %s %s", &receiver.id, receiver.name, receiver.password) != EOF)
    {
        if (strcmp(receiver.name, receiverUsername) == 0)
        {
            fclose(fp);
            receiverFound = 1;
            records[recordToUpdateIndex].userId = receiver.id;
            strcpy(records[recordToUpdateIndex].name, receiver.name);
            printf(COLOR_YELLOW "\nTransfering account number %d to %s" COLOR_RESET, targetAccountNbr, receiverUsername);
        }
    }

    if (!receiverFound)
    {
        fclose(fp);
        printf(COLOR_RED "\nUser %s not found." COLOR_RESET, receiverUsername);
        stayOrReturn(1, transferAccount, u);
        return;
    }
    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror(COLOR_RED "\n\t\tFailed to open file" COLOR_RESET);
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }

    fclose(pf);
    success(u);
}