#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void registerMenu(char name[50], char password[50]) {
    FILE *fp;
    struct User user;
    int id = 0;
    
    fp = fopen("./data/users.txt", "a+");
    
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    // Find the last used ID
    while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF) {
        id = user.id;
    }
    
    // Increment the ID for the new user
    id++;
    
    // Check if the username already exists
    rewind(fp);
    while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF) {
        if (strcmp(user.name, name) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            fclose(fp);
            return;
        }
    }
    
    // Add the new user
    fprintf(fp, "%d %s %s\n", id, name, password);
    
    fclose(fp);
    
    printf("Registration successful!\n");
}

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            // Allocate memory for the password and return it
            char *password = malloc(strlen(userChecker.password) + 1);
            if (password == NULL)
            {
                printf("Memory allocation failed");
                exit(1);
            }
            strcpy(password, userChecker.password);
            return password;
        }
    }

    fclose(fp);
    return "no user found";
}
