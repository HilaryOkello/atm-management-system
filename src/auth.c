#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

int registerMenu(char name[50], char password[50], struct User *u)
{
    FILE *fp;
    struct User user;
    int id = 0;
    
    fp = fopen("./data/users.txt", "a+");
    
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    // Find the last used ID
    while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF)
    {
        id = user.id;
    }
    
    // Increment the ID for the new user
    id++;
    
    // Check if the username already exists
    rewind(fp);
    while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF)
    {
        if (strcmp(user.name, name) == 0)
        {
            fclose(fp);
            return 0; // Username already exists
        }
    }
    
    // Add the new user
    fprintf(fp, "%d %s %s\n", id, name, password);
    
    fclose(fp);

    // Update the current user struct with new user details
    u->id = id;
    strcpy(u->name, name);
    strcpy(u->password, password);
    
    return 1; // Registration successful
}

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%49s", a);

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
    scanf("%49s", pass);

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

int getUserDetails(struct User *u, const char *enteredPassword)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file\n");
        exit(1);
    }

    // Search for the user by name and check the password
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u->name) == 0) // Match username
        {
            if (strcmp(userChecker.password, enteredPassword) == 0) // Match password
            {
                fclose(fp);
                // Populate the User struct with ID and name
                u->id = userChecker.id;
                strcpy(u->name, userChecker.name);
                return 1; // Successful login
            }
            else
            {
                fclose(fp);
                return 0; // Password mismatch
            }
        }
    }

    fclose(fp);
    return 0; // User not found
}
