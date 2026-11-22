#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <mysql/mysql.h>

#include "Email.h"

using namespace std;

class DatabaseManager
{
private:
    string host;
    string username;
    string password;
    string databaseName;

    MYSQL* connection;

public:

    DatabaseManager();

    // Database connection
    bool connect();
    void disconnect();


    // ========================================
    // USER FUNCTIONS
    // ========================================

    bool registerUser(string username, string password);

    bool loginUser(
        string username,
        string password
    );

    int getUserId(
        string username,
        string password
    );

    int getCurrentUserId();


    // ========================================
    // EMAIL FUNCTIONS
    // ========================================

    int saveEmail(Email email);


    // ========================================
    // INVESTIGATION FUNCTIONS
    // ========================================

    int saveInvestigation(
    int investigatorId,
    int emailId,
    int riskScore,
    string riskLevel,
    int senderScore,
    int urlScore,
    int contentScore,
    int attachmentScore,
    int headerScore
);

    void viewInvestigationHistory();
};

#endif