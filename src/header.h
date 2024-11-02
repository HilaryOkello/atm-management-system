#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
int registerMenu(char name[50], char password[50], struct User *u);
int getUserDetails(struct User *u, const char *enteredPassword);

// system function
void createNewAcc(struct User u);
void updateAccInfo(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void checkAccount(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferAccount(struct User u);
