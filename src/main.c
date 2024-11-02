#include "header.h"

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccInfo(u);
        break;
    case 3:
        checkAccount(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferAccount(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
        break;
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    char name[50], password[50];
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, password); // Get username and password input

            // Attempt to log in and update `u` if successful
            if (getUserDetails(u, password))
            {
                printf("\n\nPassword Match! Logged in as user ID %d, Name: %s\n", u->id, u->name);
                r = 1;
            }
            else
            {
                printf("\nWrong password or username!\n");
                exit(1);
            }
            break;
        case 2:
            printf("Enter your name: ");
            scanf("%s", name);
            printf("Enter your password: ");
            scanf("%s", password);
            if (registerMenu(name, password, u) == 1)
            {
                printf("Registration successful!\n");
                r = 1; // Set r to 1 to exit the loop
            }
            else
            {
                printf("Invalid username.Please try a different username by selecting register or login.\n");
            }
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};

int main()
{
    struct User u;

    initMenu(&u);
    mainMenu(u);
    return 0;
}
