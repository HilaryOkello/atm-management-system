#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, int *userId, struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  userId,
                  r->name,
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

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    int userId;
    int maxId = -1;
    FILE *pf = fopen(RECORDS, "a+");

    while (getAccountFromFile(pf, &userId, &cr))
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

    while (getAccountFromFile(pf, &userId, &cr))
    {
        if (userId == u.id && cr.accountNbr == r.accountNbr)
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
    while (getAccountFromFile(pf, &records[noOfRecords].userId, &records[noOfRecords]))
    {
        if (records[noOfRecords].accountNbr == targetAccountNbr && records[noOfRecords].userId == u.id)
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
        printf("\t\tEnter option 1 or 2\n");
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

void checkAllAccounts(struct User u)
{
    int userId;
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
    while (getAccountFromFile(pf, &userId, &r))
    {
        if (userId == u.id)
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
