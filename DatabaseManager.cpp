#include "../include/DatabaseManager.h"

#include <iostream>

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

DatabaseManager::DatabaseManager()
{
    host = "localhost";
    username = "root";

    // PUT YOUR MYSQL PASSWORD HERE
    password = "Vishnnuwardan@0987";

    databaseName = "phishing_investigation";

    connection = nullptr;
}


// ========================================
// CONNECT TO MYSQL
// ========================================

bool DatabaseManager::connect()
{
    connection = mysql_init(nullptr);

    if (connection == nullptr)
    {
        cout << "MySQL initialization failed." << endl;
        return false;
    }

    if (mysql_real_connect(
            connection,
            host.c_str(),
            username.c_str(),
            password.c_str(),
            databaseName.c_str(),
            3306,
            nullptr,
            0) == nullptr)
    {
        cout << "MySQL connection failed." << endl;

        cout << "Error: "
             << mysql_error(connection)
             << endl;

        mysql_close(connection);
        connection = nullptr;

        return false;
    }

    cout << "MySQL connected successfully." << endl;

    return true;
}


// ========================================
// DISCONNECT
// ========================================

void DatabaseManager::disconnect()
{
    if (connection != nullptr)
    {
        mysql_close(connection);
        connection = nullptr;
    }
}


// ========================================
// REGISTER USER
// ========================================

bool DatabaseManager::registerUser(
    string username,
    string password)
{
    if (connection == nullptr)
    {
        return false;
    }

    string query =
        "INSERT INTO Users (username, password) VALUES ('" +
        username + "', '" +
        password + "')";

    if (mysql_query(connection, query.c_str()) != 0)
    {
        cout << "Registration failed: "
             << mysql_error(connection)
             << endl;

        return false;
    }

    return true;
}


// ========================================
// LOGIN USER
// ========================================

bool DatabaseManager::loginUser(
    string username,
    string password)
{
    if (connection == nullptr)
    {
        return false;
    }

    string query =
        "SELECT user_id FROM Users "
        "WHERE username = '" +
        username +
        "' AND password = '" +
        password +
        "'";

    if (mysql_query(connection, query.c_str()) != 0)
    {
        cout << "Login query failed: "
             << mysql_error(connection)
             << endl;

        return false;
    }

    MYSQL_RES* result =
        mysql_store_result(connection);

    if (result == nullptr)
    {
        return false;
    }

    bool found =
        (mysql_num_rows(result) > 0);

    mysql_free_result(result);

    return found;
}


// ========================================
// GET USER ID
// ========================================

int DatabaseManager::getUserId(
    string username,
    string password)
{
    if (connection == nullptr)
    {
        return -1;
    }

    string query =
        "SELECT user_id FROM Users "
        "WHERE username = '" +
        username +
        "' AND password = '" +
        password +
        "'";

    if (mysql_query(connection, query.c_str()) != 0)
    {
        return -1;
    }

    MYSQL_RES* result =
        mysql_store_result(connection);

    if (result == nullptr)
    {
        return -1;
    }

    MYSQL_ROW row =
        mysql_fetch_row(result);

    int userId = -1;

    if (row != nullptr)
    {
        userId = stoi(row[0]);
    }

    mysql_free_result(result);

    return userId;
}


// ========================================
// GET CURRENT USER ID
// ========================================

int DatabaseManager::getCurrentUserId()
{
    return -1;
}


// ========================================
// SAVE EMAIL
// ========================================

int DatabaseManager::saveEmail(Email email)
{
    if (connection == nullptr)
    {
        return -1;
    }

    string sender =
        email.getSender();

    string replyTo =
        email.getReplyTo();

    string subject =
        email.getSubject();

    string body =
        email.getBody();


    // ========================================
    // ESCAPE SPECIAL CHARACTERS
    // ========================================

    char* escapedSender =
        new char[sender.length() * 2 + 1];

    char* escapedReplyTo =
        new char[replyTo.length() * 2 + 1];

    char* escapedSubject =
        new char[subject.length() * 2 + 1];

    char* escapedBody =
        new char[body.length() * 2 + 1];


    mysql_real_escape_string(
        connection,
        escapedSender,
        sender.c_str(),
        sender.length());


    mysql_real_escape_string(
        connection,
        escapedReplyTo,
        replyTo.c_str(),
        replyTo.length());


    mysql_real_escape_string(
        connection,
        escapedSubject,
        subject.c_str(),
        subject.length());


    mysql_real_escape_string(
        connection,
        escapedBody,
        body.c_str(),
        body.length());


    // ========================================
    // INSERT EMAIL
    // ========================================

    string query =
        "INSERT INTO Emails "
        "(sender_email, reply_to_email, subject, body) "
        "VALUES ('" +
        string(escapedSender) + "', '" +
        string(escapedReplyTo) + "', '" +
        string(escapedSubject) + "', '" +
        string(escapedBody) + "')";


    delete[] escapedSender;
    delete[] escapedReplyTo;
    delete[] escapedSubject;
    delete[] escapedBody;


    if (mysql_query(
            connection,
            query.c_str()) != 0)
    {
        cout << "Error saving email: "
             << mysql_error(connection)
             << endl;

        return -1;
    }


    return static_cast<int>(
        mysql_insert_id(connection));
}


// ========================================
// SAVE INVESTIGATION
// ========================================

int DatabaseManager::saveInvestigation(
    int investigatorId,
    int emailId,
    int riskScore,
    string riskLevel,
    int senderScore,
    int urlScore,
    int contentScore,
    int attachmentScore,
    int headerScore)
{
    if (connection == nullptr)
    {
        return -1;
    }


    // ========================================
    // INSERT INVESTIGATION
    // ========================================

    string query =
        "INSERT INTO Investigations "
        "(investigator_id, email_id, risk_score, risk_level, "
        "sender_score, url_score, content_score, "
        "attachment_score, header_score) "
        "VALUES (" +
        to_string(investigatorId) + ", " +
        to_string(emailId) + ", " +
        to_string(riskScore) + ", '" +
        riskLevel + "', " +
        to_string(senderScore) + ", " +
        to_string(urlScore) + ", " +
        to_string(contentScore) + ", " +
        to_string(attachmentScore) + ", " +
        to_string(headerScore) + ")";


    if (mysql_query(
            connection,
            query.c_str()) != 0)
    {
        cout << "Error saving investigation: "
             << mysql_error(connection)
             << endl;

        return -1;
    }


    // ========================================
    // GET GENERATED INVESTIGATION ID
    // ========================================

    int investigationId =
        static_cast<int>(
            mysql_insert_id(connection));


    return investigationId;
}


// ========================================
// VIEW INVESTIGATION HISTORY
// ========================================

void DatabaseManager::viewInvestigationHistory()
{
    if (connection == nullptr)
    {
        cout << "Database is not connected."
             << endl;

        return;
    }


    // ========================================
    // GET INVESTIGATION HISTORY
    // ========================================

    string query =
        "SELECT "
        "i.investigation_id, "
        "u.username, "
        "e.sender_email, "
        "e.subject, "
        "i.risk_score, "
        "i.risk_level, "
        "i.investigation_date "
        "FROM Investigations i "
        "JOIN Users u "
        "ON i.investigator_id = u.user_id "
        "JOIN Emails e "
        "ON i.email_id = e.email_id "
        "ORDER BY i.investigation_date DESC";


    if (mysql_query(
            connection,
            query.c_str()) != 0)
    {
        cout << "Error retrieving investigation history: "
             << mysql_error(connection)
             << endl;

        return;
    }


    MYSQL_RES* result =
        mysql_store_result(connection);


    if (result == nullptr)
    {
        cout << "No investigation history found."
             << endl;

        return;
    }


    MYSQL_ROW row;


    if (mysql_num_rows(result) == 0)
    {
        cout << "No investigation history found."
             << endl;

        mysql_free_result(result);

        return;
    }


    // ========================================
    // DISPLAY HISTORY
    // ========================================

    cout << "\n";

    cout << "=============================================================="
         << endl;

    cout << "              INVESTIGATION HISTORY"
         << endl;

    cout << "=============================================================="
         << endl;


    while ((row = mysql_fetch_row(result)) != nullptr)
    {
        cout << "\nInvestigation ID : "
             << row[0]
             << endl;

        cout << "Investigator     : "
             << row[1]
             << endl;

        cout << "Sender Email     : "
             << row[2]
             << endl;

        cout << "Subject          : "
             << row[3]
             << endl;

        cout << "Risk Score       : "
             << row[4]
             << endl;

        cout << "Risk Level       : "
             << row[5]
             << endl;

        cout << "Investigation Date: "
             << row[6]
             << endl;

        cout << "--------------------------------------------------------------"
             << endl;
    }


    mysql_free_result(result);


    // ========================================
    // VIEW FULL INVESTIGATION
    // ========================================

    int selectedId;

    cout << "\nEnter Investigation ID to view full details";
    cout << "\nEnter 0 to return to main menu: ";

    cin >> selectedId;


    if (selectedId == 0)
    {
        return;
    }


    // ========================================
    // QUERY FULL INVESTIGATION
    // ========================================

    string detailQuery =
        "SELECT "
        "i.investigation_id, "
        "u.username, "
        "e.email_id, "
        "e.sender_email, "
        "e.reply_to_email, "
        "e.subject, "
        "e.body, "
        "i.sender_score, "
        "i.url_score, "
        "i.content_score, "
        "i.attachment_score, "
        "i.header_score, "
        "i.risk_score, "
        "i.risk_level, "
        "i.investigation_date "
        "FROM Investigations i "
        "JOIN Users u "
        "ON i.investigator_id = u.user_id "
        "JOIN Emails e "
        "ON i.email_id = e.email_id "
        "WHERE i.investigation_id = " +
        to_string(selectedId);


    if (mysql_query(
            connection,
            detailQuery.c_str()) != 0)
    {
        cout << "\nError retrieving investigation details: "
             << mysql_error(connection)
             << endl;

        return;
    }


    MYSQL_RES* detailResult =
        mysql_store_result(connection);


    if (detailResult == nullptr)
    {
        cout << "\nInvestigation not found."
             << endl;

        return;
    }


    MYSQL_ROW detailRow =
        mysql_fetch_row(detailResult);


    if (detailRow == nullptr)
    {
        cout << "\nInvestigation ID "
             << selectedId
             << " not found."
             << endl;

        mysql_free_result(detailResult);

        return;
    }


    // ========================================
    // DISPLAY FULL REPORT
    // ========================================

    cout << "\n";
    cout << "=============================================================="
         << endl;

    cout << "             FULL INVESTIGATION DETAILS"
         << endl;

    cout << "=============================================================="
         << endl;


    cout << "\n---------------- INVESTIGATION ----------------"
         << endl;

    cout << "Investigation ID : "
         << detailRow[0]
         << endl;

    cout << "Investigator     : "
         << detailRow[1]
         << endl;

    cout << "Investigation Date: "
         << detailRow[14]
         << endl;


    cout << "\n---------------- EMAIL DETAILS ----------------"
         << endl;

    cout << "Email ID         : "
         << detailRow[2]
         << endl;

    cout << "Sender Email     : "
         << detailRow[3]
         << endl;

    cout << "Reply-To Email   : "
         << detailRow[4]
         << endl;

    cout << "Subject          : "
         << detailRow[5]
         << endl;


    cout << "\n---------------- EMAIL BODY -------------------"
         << endl;

    cout << detailRow[6]
         << endl;


    cout << "\n---------------- RISK ANALYSIS ----------------"
         << endl;

    cout << "Sender Score     : "
         << detailRow[7]
         << endl;

    cout << "URL Score        : "
         << detailRow[8]
         << endl;

    cout << "Content Score    : "
         << detailRow[9]
         << endl;

    cout << "Attachment Score : "
         << detailRow[10]
         << endl;

    cout << "Header Score     : "
         << detailRow[11]
         << endl;

    cout << "------------------------------------------------"
         << endl;

    cout << "Final Risk Score : "
         << detailRow[12]
         << "/100"
         << endl;

    cout << "Risk Level       : "
         << detailRow[13]
         << endl;

    cout << "=============================================================="
         << endl;


    mysql_free_result(detailResult);
}