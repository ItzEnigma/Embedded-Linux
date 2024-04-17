/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  System.h
 *
 *  Description: This file contains the main system functions declaration and data structures used in the banking
 *               system.
 *  ----------------------------------------------------------------
 *  License: Don't use this code without permission from the author.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CONSTANT MACROS
 *********************************************************************************************************************/

#define MAX_USERS 100              // Maximum number of users
#define MAX_ADMIN 10               // Maximum number of admins
#define MAX_ACTIVITIES_HISTORY 100 // Maximum number of activities history

#define NO_CURRENT_USER -1 // No current user is logged in
#define INITIAL_PAGE 0     // Initial page of the application

#define VALID_USER 10    // Valid user
#define INVALID_USER -10 // Invalid user

/**********************************************************************************************************************
 *  DATA STRUCTURES
 *********************************************************************************************************************/

// Data Structure 1: Admin Credentials
typedef struct
{
    char adminName[20];
    char adminPassword[20];
} adminCredentials_t;

// Data Structure 2: User Credentials
typedef struct
{
    char userName[20];
    char userPassword[20];
} userCredentials_t;

/*
    Process Data Structure:
    - amount: amount of money deposited/withdrawn
    - timestamp: time of deposit/withdrawl
*/
typedef struct
{
    double amount;
    char timestamp[20];
} processActivity_t;

/* Transfer Data Structure:
    - amount: amount of money transferred
    - timestamp: time of transfer
    - userName: name of the sender/receiver
*/
typedef struct
{
    double amount;
    char timestamp[20];
    char userName[20]; // Sender/Receiver
    int status;        // 0: Sent, 1: Received
} transferActivity_t;

// Data Structure 3: User Activities
typedef struct
{
    char userName[20];
    char userPassword[20];
    double userBalance;

    // Activities (deposits, withdrawls, transfers)
    processActivity_t userDeposits[MAX_ACTIVITIES_HISTORY];
    processActivity_t userWithdrawls[MAX_ACTIVITIES_HISTORY];
    transferActivity_t userTransfers[MAX_ACTIVITIES_HISTORY];

    // Indexes for the last activity of each type (deposit, withdrawl, transfer)
    unsigned int deposit_LastIndex;
    unsigned int withdrawl_LastIndex;
    unsigned int transfer_LastIndex;
} userActivities_t;

typedef enum
{
    LOGIN_PAGE = 1,
    LOGIN_ADMIN,
    LOGIN_USER
} loginStatus_t;

/**********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**************************************************************************
 * Get the current timestamp in the specified format (dd/mm/yyyy hh:mm:ss).
 **************************************************************************/
char *getTimestamp();

/**************************************************************************
 * Check Admin Credentials, Checks if the admin credentials are correct.
 **************************************************************************/
int checkAdmin_Credentials(char *adminName, char *adminPassword);

/**************************************************************************
 * Check User Credentials, Checks if the user credentials are correct.
 **************************************************************************/
int checkUser_Credentials(char *userName, char *userPassword);

/**************************************************************************
 * Main Page, Displays initial page of the application and whether to
 * Login or Exit the application.
 **************************************************************************/
void mainPage();

/**************************************************************************
 * Login Page, Displays login page of the application and whether to
 * login as an admin, a user or go back to the main page.
 **************************************************************************/
void loginPage();

/**************************************************************************
 * Admin Login Page, login page for the admin (admin name & password).
 **************************************************************************/
void adminPage();

/**************************************************************************
 * Admin Menu, Displays admin page of the application and whether to Add a
 * User, Delete a User, Display all Users, Exit Admin Menu.
 **************************************************************************/
void adminMenu();

/**************************************************************************
 * User Login Page, Displays user page of the application and whether to
 * Withdraw, Deposit, Transfer Money or Display Account Info. or Logout
 **************************************************************************/
void userPage();

/*-------------------------------------------------------------------------
                                Services
 -------------------------------------------------------------------------*/
void withdrawMoney();

void depositMoney();

void transferMoney();

void displayAccountInfo();