#include "header.h"

const char *RECORDS = "./data/records.txt";

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
    char name[50];
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
            printf("Insert a valid operation!\n");
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
    printf("\n✔ Success!\n\n");
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
        printf("Insert a valid operation!\n");
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
    }
    else if (strcmp(r.accountType, "fixed01") == 0)
    {
        interestRate = 0.04;
    }
    else if (strcmp(r.accountType, "fixed02") == 0)
    {
        interestRate = 0.05;
    }
    else if (strcmp(r.accountType, "fixed03") == 0)
    {
        interestRate = 0.08;
    }
    else if (strcmp(r.accountType, "current") == 0)
    {
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will not get interests because the account is of type current");
        return interestMessage;
    }

    // Calculate interest if applicable
    if (interestRate > 0.0)
    {
        interestAmount = r.amount * interestRate / 12;
        snprintf(interestMessage, sizeof(interestMessage),
                 "You will get $%.2f as interest on day %d of every month",
                 interestAmount, r.deposit.day);
    }

    return interestMessage;
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char name[50];
    int maxId = -1;
    FILE *pf = fopen(RECORDS, "a+");
    if (pf == NULL)
    {
        printf("Error opening file!\n");
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
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, name, &cr))
    {
        if (strcmp(name, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    printf("%d", u.id);
    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void updateAccInfo(struct User u)
{
    if (!userHasAtLeastOneAccount(u))
    {
        system("clear");
        printf("\t\t\t===== Update Account Information =====\n");
        printf("\t\t====== It seems like you do not have any accounts, %s =====\n\n", u.name);
        printf("\t\t====== Consider creating one to be able to perform this operation, %s =====\n\n", u.name);
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

    system("clear");
    printf("\t\t\t===== Update Account Information =====\n");
    printf("\nEnter the Account Number you want to update:\n");
    scanf("%d", &targetAccountNbr);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return;
    }
    while (getAccountFromFile(pf, records[noOfRecords].name, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && records[noOfRecords].name == u.name)
        {
            r = records[noOfRecords];
            accountExists = 1;
            recordToUpdateIndex = noOfRecords;
        }
        noOfRecords++;
    }
    fclose(pf);

    if (accountExists)
    {
        printf("_____________________\n");
        printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
               r.accountNbr,
               r.deposit.day,
               r.deposit.month,
               r.deposit.year,
               r.country,
               r.phone,
               r.amount,
               r.accountType);

        int option;
        printf("\nWhich information do you want to update?\n");
        printf("\t\t[1]-> phone number\n");
        printf("\t\t[2]-> country\n");
        printf("\t\tEnter your choice:");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("\nEnter the new phone number:");
            scanf("%d", &phoneNumber);
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
            printf("\nInvalid option. Returning to main menu.\n");
            mainMenu(u);
            return;
        }
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }

    fclose(pf);
    success(u);
}

void checkAccount(struct User u)
{
    if (!userHasAtLeastOneAccount(u))
    {
        system("clear");
        printf("\t\t====== It seems like you do not have any accounts, %s =====\n\n", u.name);
        printf("\t\t====== Consider creating one to be able to perform this operation, %s =====\n\n", u.name);
        stayOrReturn(1, checkAllAccounts, u);
        return;
    }

    int accountNbr;
    system("clear");
    printf("\t\t====== Check Account, %s =====\n\n", u.name);
    printf("\nEnter the account number:");
    scanf("%d", &accountNbr);

    char name[50];
    ;
    struct Record r;

    FILE *pf = fopen(RECORDS, "r+");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return;
    }

    while (getAccountFromFile(pf, name, &r))
    {
        if (strcmp(name, u.name) == 0 && accountNbr == r.accountNbr)
        {
            char *interestMessage = processInterest(r);

            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            printf("%s\n", interestMessage);
            fclose(pf);
            return;
        }
    }
    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    if (!userHasAtLeastOneAccount(u))
    {
        system("clear");
        printf("\t\t====== It seems like you do not have any accounts, %s =====\n\n", u.name);
        printf("\t\t====== Consider creating one to be able to perform this operation, %s =====\n\n", u.name);
        stayOrReturn(1, checkAllAccounts, u);
        return;
    }
    char name[50];
    ;
    struct Record r;

    FILE *pf = fopen(RECORDS, "r+");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, name, &r))
    {
        if (strcmp(name, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void makeTransaction(struct User u)
{
    if (!userHasAtLeastOneAccount(u))
    {
        system("clear");
        printf("\t\t\t===== Make a Transaction =====\n");
        printf("\t\t====== It seems like you do not have any accounts, %s =====\n\n", u.name);
        printf("\t\t====== Consider creating one to be able to perform this operation, %s =====\n\n", u.name);
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    struct Record r;
    struct Record records[100];
    int noOfRecords = 0;
    int accountExists = 0;
    int targetAccountNbr = 0;
    int recordToUpdateIndex = 0;

    system("clear");
    printf("\t\t\t===== Make a Transaction =====\n");
    printf("\nEnter the Account Number:");
    scanf("%d", &targetAccountNbr);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
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

    if (accountExists)
    {
        if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
        {
            printf("\nYou cannot withdraw or deposit from a %s account", r.accountType);
            stayOrReturn(1, makeTransaction, u);
            return;
        }

        int option;
        printf("\nDo you want to:\n");
        printf("\t\t[1]-> Deposit\n");
        printf("\t\t[2]-> Withdraw\n");
        printf("\t\tEnter your choice:");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            int depositAmount;
            printf("\nEnter the amount you want to deposit:");
            scanf("%d", &depositAmount);
            records[recordToUpdateIndex].amount = r.amount + depositAmount;
            printf("\nYou successfully deposited %d to account number: %d", depositAmount, r.accountNbr);
            break;
        case 2:
            int withdrawAmount;
            printf("\nEnter the amount you want to withdraw:");
            scanf("%d", &withdrawAmount);
            if (withdrawAmount > r.amount)
            {
                printf("\nFail. The withdraw amount is superior to your balance:");
                stayOrReturn(1, makeTransaction, u);
                return;
            }
            records[recordToUpdateIndex].amount = r.amount - withdrawAmount;
            printf("\nYou successfully withdrew %d from account number: %d", withdrawAmount, r.accountNbr);
            break;
        default:
            stayOrReturn(1, makeTransaction, u);
            return;
        }
    }
    else
    {
        printf("\nThat account does not exist. Please try again.\n");
        stayOrReturn(1, makeTransaction, u);
        return;
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        perror("\n\t\tFailed to open file");
        return;
    }
    for (int i = 0; i < noOfRecords; i++)
    {
        updateAccountInFile(pf, records[i]);
    }
    fclose(pf);

    success(u);
}