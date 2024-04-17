/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  System.c
 *
 *  Description: This file contains the implementation of the functions of the banking system for user, admin, and
 *               services.
 *  ----------------------------------------------------------------
 *  License: Don't use this code without permission from the author.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "System.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

// Global Data 1: Admin Credentials
adminCredentials_t adminCredentials[MAX_ADMIN] = {{"admin", "0000"}};

// Global Data 2: User Credentials
userCredentials_t userCredentials[MAX_USERS] = {{"Enigma", "1234"}, {"Hosam", "4321"}, {"Mohamed", "2244"}};

// Global Data 3: User Activities
userActivities_t userActivities[MAX_USERS] = {
    {"Enigma", "1234", 1000.0}, {"Hosam", "4321", 2000.0}, {"Mohamed", "2244", 5000.0}};

// Numbers of Users
unsigned int numberOfUsers = 3;

// Current User
int currentUser = NO_CURRENT_USER;

// Current Login Status
int currentLoginStatus = INITIAL_PAGE; // 0: Not Logged In, 1: Logged In as Admin, 2: Logged In as User

// Current Page
int currentPage = INITIAL_PAGE;

// Current Timestamp
char currentTimestamp[20];

// Temporary Variables
char name[20];
char password[20];
double amount;

/**********************************************************************************************************************
 *  FUNCTIONS IMPLEMENTATION
 *********************************************************************************************************************/

/**************************************************************************
 * Get the current timestamp in the specified format (dd/mm/yyyy hh:mm:ss).
 **************************************************************************/
char *getTimestamp()
{
    time_t rawtime;
    struct tm timeinfo;
    char timestamp[20];

    time(&rawtime);                   // Get the current time
    localtime_s(&timeinfo, &rawtime); // Convert the current time to the local time
                                      // Convert the local time to the specified format
    strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", &timeinfo);

    // Convert to a static buffer to return
    static char timestampBuffer[20];
    strcpy(timestampBuffer, timestamp);

    return timestampBuffer;
}

/**************************************************************************
 * Check Admin Credentials, Checks if the admin credentials are correct.
 **************************************************************************/
int checkAdmin_Credentials(char *adminName, char *adminPassword)
{
    int status = INVALID_USER;
    for (unsigned int i = 0; i < numberOfUsers; i++)
    {
        // Admin found
        if (strcmp(adminName, adminCredentials[i].adminName) == 0 && strcmp(adminPassword, adminCredentials[i].adminPassword) == 0)
        {
            currentUser = i; // Set the current user (admin)
            status = VALID_USER;
            break;
        }
    }
    return status;
}

/**************************************************************************
 * Check User Credentials, Checks if the user credentials are correct.
 **************************************************************************/
int checkUser_Credentials(char *userName, char *userPassword)
{
    int status = INVALID_USER;
    for (unsigned int i = 0; i < numberOfUsers; i++)
    {
        // User found
        if (strcmp(userName, userCredentials[i].userName) == 0 && strcmp(userPassword, userCredentials[i].userPassword) == 0)
        {
            currentUser = i; // Set the current user
            status = VALID_USER;
            break;
        }
    }
    return status;
}

/**************************************************************************
 * Main Page, Displays initial page of the application and whether to
 * Login or Exit the application.
 **************************************************************************/
void mainPage()
{
    currentUser = NO_CURRENT_USER; // No current user is logged in

    // Get the current timestamp
    strcpy(currentTimestamp, getTimestamp());

    // Display the main page
    printf("\t--- WELCOME TO ENIGMA BANK ---\n");
    printf("\t      ------------------\n");
    printf("\t    /      Enigma      \\\n");
    printf("\t   | %s  |\n", currentTimestamp);
    printf("\t    /                    \\\n");
    printf("\t      ------------------\n");
    printf(" 1. Login\n");
    printf(" 2. Exit\n");
    printf(">>> ");
    scanf("%d", &currentPage);

    while (currentPage != 1 && currentPage != 2) // Check if the input is valid
    {
        printf("Please enter a number that is a valid input\n");
        printf(">>> ");
        scanf("%d", &currentPage);
    }

    if (currentPage == 1) // Login Page
    {
        currentPage = LOGIN_PAGE;
        loginPage();
    }
    else if (currentPage == 2) // Exit the application
    {
        exit(0);
    }
}

/**************************************************************************
 * Login Page, Displays login page of the application and whether to
 * login as an admin, a user or go back to the main page.
 **************************************************************************/
void loginPage()
{
    // Display the login page
    printf("What do you want to login as:\n");
    printf(" 1. Admin\n");
    printf(" 2. User\n");
    printf(" 3. Go Back\n");
    printf(">>> ");
    scanf("%d", &currentLoginStatus);

    // Check if the input is valid
    while (currentLoginStatus != 1 && currentLoginStatus != 2 && currentLoginStatus != 3)
    {
        printf("Please enter a number that is a valid input\n");
        printf(">>> ");
        scanf("%d", &currentLoginStatus);
    }

    if (currentLoginStatus == 1) // Admin Login
    {
        currentPage = LOGIN_ADMIN;
        currentLoginStatus = LOGIN_ADMIN;
        adminPage();
    }
    else if (currentLoginStatus == 2) // User Login
    {
        currentPage = LOGIN_USER;
        currentLoginStatus = LOGIN_USER;
        userPage();
    }
    else if (currentLoginStatus == 3) // Back to the Main Page
    {
        currentPage = INITIAL_PAGE;
        currentLoginStatus = INITIAL_PAGE;
        return; // return to the main page
    }
}

/**************************************************************************
 * Admin Login Page, Displays admin page of the application and whether to
 * Add a User, Delete a User, Display all Users, Exit Admin
 **************************************************************************/
void adminPage()
{
    currentLoginStatus = LOGIN_ADMIN;

    // Display the admin page
    printf("Admin Name: ");
    scanf("%s", name);

    printf("Password: ");
    scanf("%s", password);

    // Check if the admin credentials are correct
    currentLoginStatus = checkAdmin_Credentials(name, password);

    // Invalid Admin Credentials (Go back to the login page)
    if (currentLoginStatus == INVALID_USER)
    {
        printf("Invalid Admin Credentials\n");
        currentPage = LOGIN_PAGE;
        loginPage();
    }
    else if (currentLoginStatus == VALID_USER) // Valid Admin Credentials
    {
        printf(" Welcome %s\n", name);
        adminMenu();
    }
}

/**************************************************************************
 * Admin Menu, Displays admin page of the application and whether to Add a
 * User, Delete a User, Display all Users, Exit Admin Menu.
 **************************************************************************/
void adminMenu()
{
    // Display the admin menu
    printf("  - - - Admin Menu - - -\n");
    printf(" Please enter a number of the settings operations supported:\n");
    printf(" 1. Add User\n");
    printf(" 2. Remove User\n");
    printf(" 3. Display all Users\n");
    printf(" 4. Exit Admin Menu\n");
    printf(">>> ");
    scanf("%d", &currentPage);

    // Check if the input is valid
    while (currentPage != 1 && currentPage != 2 && currentPage != 3 && currentPage != 4)
    {
        printf("Please enter a number that is a valid input\n");
        printf(">>> ");
        scanf("%d", &currentPage);
    }

    if (currentPage == 1) // Add a User
    {
        printf("Access Granted\n");
        printf("Enter the new user name:\n");
        printf(">>> ");
        scanf("%s", name);

        printf("Enter the new user password:\n");
        printf(">>> ");
        scanf("%s", password);

        // Add the User
        strcpy(userCredentials[numberOfUsers].userName, name);
        strcpy(userCredentials[numberOfUsers].userPassword, password);
        strcpy(userActivities[numberOfUsers].userName, name);
        strcpy(userActivities[numberOfUsers].userPassword, password);
        userActivities[numberOfUsers].userBalance = 0.0;
        userActivities[numberOfUsers].deposit_LastIndex = 0;
        userActivities[numberOfUsers].withdrawl_LastIndex = 0;
        userActivities[numberOfUsers].transfer_LastIndex = 0;
        numberOfUsers++; // Increase the number of users

        printf("%s was added as an user\n\n", name);
        adminMenu(); // Go back to the admin menu
    }
    else if (currentPage == 2) // Remove a User
    {
        int tempStatus = 0; // 0: User not found, 1: User found
        printf("Access Granted\n");
        printf("Enter the user name:\n");
        printf(">>> ");
        scanf("%s", name);
        for (unsigned int i = 0; i < numberOfUsers; i++) // Search for the user
        {
            if (strcmp(name, userActivities[i].userName) == 0) // User found
            {
                tempStatus = 1; // User found
                // Remove the User
                for (unsigned int j = i; j < numberOfUsers - 1; j++) // Shift the users
                {
                    userCredentials[j] = userCredentials[j + 1];
                    userActivities[j] = userActivities[j + 1];
                }
                numberOfUsers--; // Decrease the number of users
                printf("%s was removed as an user to Medipol Bank!\n", name);
                break;
            }
        }
        if (tempStatus == 0) // User not found
        {
            printf("%s does not exist as an user to Medipol Bank\n\n", name);
        }
        adminMenu(); // Go back to the admin menu
    }
    else if (currentPage == 3) // Display all Users
    {
        printf("Access Granted\n");
        printf("There are %u users using Medipol Bank:\n", numberOfUsers);
        printf(" Name & Password:\n");
        for (unsigned int i = 0; i < numberOfUsers; i++)
        {
            printf(" %u. %s %s\n", i + 1, userActivities[i].userName, userActivities[i].userPassword);
        }
        printf("\n______________________\n\n");
        adminMenu(); // Go back to the admin menu
    }
    else if (currentPage == 4) // Exit Admin
    {
        currentLoginStatus = INITIAL_PAGE;
        currentPage = INITIAL_PAGE;
        return; // return to the main page
    }
}

/**************************************************************************
 * User Login Page, Displays user page of the application and whether to
 * Withdraw, Deposit, Transfer Money or Display Account Info. or Logout
 **************************************************************************/
void userPage()
{
    currentLoginStatus = LOGIN_USER;

    // Display the user page
    printf("User Name: ");
    scanf("%s", name);

    printf("Password: ");
    scanf("%s", password);

    // Check if the user credentials are correct
    currentLoginStatus = checkUser_Credentials(name, password);

    if (currentLoginStatus == INVALID_USER) // Invalid User Credentials
    {
        printf("Invalid User Credentials\n");
        currentPage = LOGIN_PAGE;
        loginPage(); // Go back to the login page
    }
    else if (currentLoginStatus == VALID_USER) // Valid User Credentials
    {
        printf("%s Welcome to Medipol Bank\n\n", name);
        printf(" Please enter the number of the service\n");
        printf(" 1. Withdraw Money\n");
        printf(" 2. Deposit Money\n");
        printf(" 3. Transfer Money\n");
        printf(" 4. My Account Information\n");
        printf(" 5. Logout\n");
        printf(">>> ");
        scanf("%d", &currentPage);

        // Check if the input is valid
        while (currentPage != 1 && currentPage != 2 && currentPage != 3 && currentPage != 4 && currentPage != 5)
        {
            printf("Please enter a number that is a valid input\n");
            printf(">>> ");
            scanf("%d", &currentPage);
        }

        if (currentPage == 1) // Withdraw Money
        {
            withdrawMoney();
        }
        else if (currentPage == 2) // Deposit Money
        {
            depositMoney();
        }
        else if (currentPage == 3) // Transfer Money
        {
            transferMoney();
        }
        else if (currentPage == 4) // Display Account Info.
        {
            displayAccountInfo();
        }
        else if (currentPage == 5) // Logout
        {
            currentLoginStatus = INITIAL_PAGE;
            currentPage = INITIAL_PAGE;
            return; // return to the main page
        }
    }
}

/*-------------------------------------------------------------------------
                                Services
 -------------------------------------------------------------------------*/
void withdrawMoney()
{
    printf("Please enter the amount you want to withdraw: ");
    scanf("%lf", &amount);

    // Check if the amount is valid
    if (amount < userActivities[currentUser].userBalance && amount > 0)
    {
        // Withdraw Money
        userActivities[currentUser].userBalance -= amount;
        strcpy(userActivities[currentUser].userWithdrawls[userActivities[currentUser].withdrawl_LastIndex].timestamp, getTimestamp());
        userActivities[currentUser].userWithdrawls[userActivities[currentUser].withdrawl_LastIndex].amount = amount;
        userActivities[currentUser].withdrawl_LastIndex++;

        printf("%.1lf TL withrawn from your account\n", amount);
        printf("Going back to main menu...\n");

        currentPage = INITIAL_PAGE;
        currentLoginStatus = INITIAL_PAGE;
        return; // return to the main page
    }
    else // Not enough money in the account
    {
        printf("Sorry! you don't have the entered amount\n\n");
        printf(" 1. Go back to main menu\n");
        printf(" 2. Withdraw again\n");
        printf(">>> ");
        scanf("%d", &currentPage);

        // Check if the input is valid
        while (currentPage != 1 && currentPage != 2)
        {
            printf("Please enter a number that is a valid input\n");
            printf(">>> ");
            scanf("%d", &currentPage);
        }

        if (currentPage == 1) // Go back to main menu
        {
            currentPage = INITIAL_PAGE;
            currentLoginStatus = INITIAL_PAGE;
            return; // return to the main page
        }
        else if (currentPage == 2) // Withdraw again
        {
            currentPage = LOGIN_USER;
            withdrawMoney(); // Go back to the withdraw money page
        }
    }
}

void depositMoney()
{
    printf("Please enter the amount you want to deposit: ");
    scanf("%lf", &amount);
    // Check if the amount is valid
    if (amount > 0)
    {
        // Deposit Money
        userActivities[currentUser].userBalance += amount;
        strcpy(userActivities[currentUser].userDeposits[userActivities[currentUser].deposit_LastIndex].timestamp, getTimestamp());
        userActivities[currentUser].userDeposits[userActivities[currentUser].deposit_LastIndex].amount = amount;
        userActivities[currentUser].deposit_LastIndex++;

        printf("%.1lf TL added to your account\n", amount);
        printf("Going back to main menu...\n");

        currentPage = INITIAL_PAGE;
        currentLoginStatus = INITIAL_PAGE;
        return; // return to the main page
    }
    else // Invalid amount
    {
        printf("Sorry! invalid amount\n\n");
        printf(" 1. Go back to main menu\n");
        printf(" 2. Deposit again\n");

        printf(">>> ");
        scanf("%d", &currentPage);

        // Check if the input is valid
        while (currentPage != 1 && currentPage != 2)
        {
            printf("Please enter a number that is a valid input\n");
            printf(">>> ");
            scanf("%d", &currentPage);
        }

        if (currentPage == 1) // Go back to main menu
        {
            currentPage = INITIAL_PAGE;
            currentLoginStatus = INITIAL_PAGE;
            return; // return to the main page
        }
        else if (currentPage == 2) // Withdraw again
        {
            currentPage = LOGIN_USER;
            depositMoney(); // Go back to the deposit money page
        }
    }
}

void transferMoney()
{
    printf("Warning: If you want to abort the transfer please enter abort\n");
    printf("Please enter the name of the user you want to transfer money to: ");
    scanf("%s", name);

    // Check if the user wants to abort the transfer
    if (strcmp(name, "abort") == 0)
    {
        printf("Going back to main menu...\n");

        currentPage = INITIAL_PAGE;
        currentLoginStatus = INITIAL_PAGE;
        return; // return to the main page
    }
    else // Continue the transfer
    {
        int userFound = 0;                               // 1: User found, 0: User not found
        for (unsigned int i = 0; i < numberOfUsers; i++) // Search for the user
        {
            if (strcmp(name, userActivities[i].userName) == 0) // User found
            {
                userFound = 1; // User found
                printf("Please enter the amount you want to transfer: ");
                scanf("%lf", &amount);
                // Check if the amount is valid
                if (amount < userActivities[currentUser].userBalance && amount > 0)
                {
                    // Transfer Money
                    userActivities[currentUser].userBalance -= amount;
                    strcpy(userActivities[currentUser].userTransfers[userActivities[currentUser].transfer_LastIndex].timestamp, getTimestamp());
                    userActivities[currentUser].userTransfers[userActivities[currentUser].transfer_LastIndex].amount = amount;
                    strcpy(userActivities[currentUser].userTransfers[userActivities[currentUser].transfer_LastIndex].userName, name);
                    userActivities[currentUser].userTransfers[userActivities[currentUser].transfer_LastIndex].status = 0; // Sent
                    userActivities[currentUser].transfer_LastIndex++;

                    // Add the money to the other user's account
                    userActivities[i].userBalance += amount;
                    strcpy(userActivities[i].userTransfers[userActivities[i].transfer_LastIndex].timestamp, getTimestamp());
                    userActivities[i].userTransfers[userActivities[i].transfer_LastIndex].amount = amount;
                    strcpy(userActivities[i].userTransfers[userActivities[i].transfer_LastIndex].userName, userActivities[currentUser].userName);
                    userActivities[i].userTransfers[userActivities[i].transfer_LastIndex].status = 1; // Received
                    userActivities[i].transfer_LastIndex++;

                    printf("Money Transferred Successfully!\n");
                    printf("Going back to main menu...\n\n");

                    currentPage = INITIAL_PAGE;
                    currentLoginStatus = INITIAL_PAGE;
                    return; // return to the main page
                }
                else // Not enough money in the account
                {
                    printf("Sorry! you don't have the entered amount\n\n");
                    printf(" 1. Go back to main menu\n");
                    printf(" 2. Transfer again\n");
                    printf(">>> ");
                    scanf("%d", &currentPage);

                    // Check if the input is valid
                    while (currentPage != 1 && currentPage != 2)
                    {
                        printf("Please enter a number that is a valid input\n");
                        printf(">>> ");
                        scanf("%d", &currentPage);
                    }

                    if (currentPage == 1) // Go back to main menu
                    {
                        currentPage = INITIAL_PAGE;
                        currentLoginStatus = INITIAL_PAGE;
                        return; // return to the main page
                    }
                    else if (currentPage == 2) // transfer again
                    {
                        currentPage = LOGIN_USER;
                        transferMoney(); // Go back to the transfer money page
                    }
                }
            }
        }
        if (userFound == 0) // User not found
        {
            printf("Transferring to user with the name %s is not possible!\n", name);
            printf("User does not exist!\n\n");
            transferMoney(); // Go back to the transfer money page
        }
    }
}

void displayAccountInfo()
{
    // Display the account info.
    printf("\n------- Enigma Bank -------\n");
    printf("----%s----\n", getTimestamp());
    printf("----------------------------\n");
    printf("Your Name: %s\n", userActivities[currentUser].userName);
    printf("Your Password: %s\n", userActivities[currentUser].userPassword);
    printf("Your Balance: %.1lf TL\n", userActivities[currentUser].userBalance);
    printf("----------------------------\n");

    printf("User Activities Report:\n\n");

    // Display the user's withdrawls
    printf("Your Withdrawals:\n");
    for (unsigned int i = 0; i < userActivities[currentUser].withdrawl_LastIndex; i++)
    {
        printf("    %s %.1lf TL\n", userActivities[currentUser].userWithdrawls[i].timestamp, userActivities[currentUser].userWithdrawls[i].amount);
    }

    // Display the user's deposits
    printf("\nYour Deposits:\n");
    for (unsigned int i = 0; i < userActivities[currentUser].deposit_LastIndex; i++)
    {
        printf("    %s %.1lf TL\n", userActivities[currentUser].userDeposits[i].timestamp, userActivities[currentUser].userDeposits[i].amount);
    }

    // Display the user's transfers
    printf("\nYour Transfers:\n");
    for (unsigned int i = 0; i < userActivities[currentUser].transfer_LastIndex; i++)
    {
        if (userActivities[currentUser].userTransfers[i].status == 1) // Received
        {
            printf("    %s Transferred to me from %s %.1lf TL\n", userActivities[currentUser].userTransfers[i].timestamp,
                   userActivities[currentUser].userTransfers[i].userName,
                   userActivities[currentUser].userTransfers[i].amount);
        }
        else // Sent
        {
            printf("    %s Transferred to %s %.1lf TL\n", userActivities[currentUser].userTransfers[i].timestamp,
                   userActivities[currentUser].userTransfers[i].userName,
                   userActivities[currentUser].userTransfers[i].amount);
        }
    }

    printf("\n----------------------------\n");
    printf("Going back to main menu...\n\n\n");
    currentPage = INITIAL_PAGE;
    currentLoginStatus = INITIAL_PAGE;
    return; // return to the main page
}