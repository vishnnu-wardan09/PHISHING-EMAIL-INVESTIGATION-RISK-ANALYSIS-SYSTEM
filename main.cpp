
#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <vector>

#include "../include/User.h"
#include "../include/Email.h"
#include "../include/DatabaseManager.h"
#include "../include/Investigation.h"
#include "../include/RiskCalculator.h"
#include "../include/ReportGenerator.h"

using namespace std;

// ========================================
// EXTRACT URLS FROM EMAIL BODY
// ========================================

void extractURLs(string body, Email& email)
{
    regex urlPattern(R"((https?://[^\s<>"']+))");

    sregex_iterator begin(body.begin(), body.end(), urlPattern);
    sregex_iterator end;

    set<string> uniqueURLs;

    for (auto it = begin; it != end; ++it)
    {
        string url = (*it).str();

        while (!url.empty() &&
               (url.back() == '.' ||
                url.back() == ',' ||
                url.back() == ')' ||
                url.back() == ']' ||
                url.back() == ';'))
        {
            url.pop_back();
        }

        if (!url.empty())
        {
            uniqueURLs.insert(url);
        }
    }

    for (const string& url : uniqueURLs)
    {
        email.addURL(url);
    }
}

// ========================================
// MAIN
// ========================================

int main()
{
    cout << "========================================" << endl;
    cout << "   PHISHING EMAIL INVESTIGATION SYSTEM" << endl;
    cout << "========================================" << endl;

    DatabaseManager database;

    // ========================================
    // DATABASE CONNECTION
    // ========================================

    if (!database.connect())
    {
        cout << "\nUnable to connect to MySQL." << endl;
        return 1;
    }

    while (true)
    {
        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. View Investigation History" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;

        if (!(cin >> choice))
        {
            if (cin.eof())
            {
                database.disconnect();
                return 0;
            }

            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nInvalid input. Enter a number." << endl;
            continue;
        }

        // ========================================
        // REGISTER
        // ========================================

        if (choice == 1)
        {
            string username;
            string password;

            cout << "\n========== REGISTRATION ==========" << endl;

            cout << "Enter Username: ";
            cin >> username;

            cout << "Enter Password: ";
            cin >> password;

            if (database.registerUser(username, password))
            {
                cout << "\nRegistration successful!" << endl;

                int userId = database.getUserId(username, password);

                cout << "Your Investigator ID is: "
                     << userId << endl;
            }
            else
            {
                cout << "\nRegistration failed." << endl;
            }
        }

        // ========================================
        // LOGIN
        // ========================================

        else if (choice == 2)
        {
            string username;
            string password;

            cout << "\n========== LOGIN ==========" << endl;

            cout << "Enter Username: ";
            cin >> username;

            cout << "Enter Password: ";
            cin >> password;

            if (!database.loginUser(username, password))
            {
                cout << "\nInvalid username or password." << endl;
                continue;
            }

            int userId = database.getUserId(username, password);

            if (userId <= 0)
            {
                cout << "\nUnable to retrieve Investigator ID." << endl;
                continue;
            }

            User loggedInUser;
            loggedInUser.setUserId(userId);
            loggedInUser.setUsername(username);
            loggedInUser.setPassword(password);

            cout << "\nLogin successful!" << endl;
            cout << "Welcome, " << username << "!" << endl;
            cout << "Investigator ID: " << userId << endl;

            // ========================================
            // CREATE EMAIL OBJECT
            // ========================================

            Email email;

            string sender;
            string replyTo;
            string subject;
            string body;

            cout << "\n========== EMAIL DETAILS ==========" << endl;

            cout << "Enter Sender Email: ";
            cin >> sender;

            cout << "Enter Reply-To Email: ";
            cin >> replyTo;

            cin.ignore(10000, '\n');

            cout << "Enter Subject: ";
            getline(cin, subject);

            // ========================================
            // EMAIL BODY
            // ========================================

            cout << "\nEnter Email Body." << endl;
            cout << "You can paste the complete email." << endl;
            cout << "Type END on a separate line when finished." << endl;
            cout << "\n----------------------------------------" << endl;

            body = "";

            string line;

            while (true)
            {
                if (!getline(cin, line))
                {
                    database.disconnect();
                    return 0;
                }

                if (line == "END")
                {
                    break;
                }

                body += line;
                body += "\n";
            }

            // ========================================
            // STORE EMAIL DATA
            // ========================================

            email.setSender(sender);
            email.setReplyTo(replyTo);
            email.setSubject(subject);
            email.setBody(body);

            // ========================================
            // AUTOMATIC URL EXTRACTION
            // ========================================

            extractURLs(body, email);

            // ========================================
            // ATTACHMENT INPUT
            // ========================================

            cout << "\n========================================" << endl;
            cout << "          ATTACHMENT INPUT" << endl;
            cout << "========================================" << endl;

            cout << "Enter the actual attachment file path." << endl;
            cout << "Example: samples/test_attachment.txt" << endl;
            cout << "Windows path example: C:/Users/YourName/Documents/test.pdf" << endl;
            cout << "Enter one attachment path per line." << endl;
            cout << "Type END on a separate line when finished." << endl;

            // ========================================
            // READ ATTACHMENT PATHS
            // ========================================

            while (true)
            {
                cout << "\nAttachment Path: ";

                if (!getline(cin, line))
                {
                    database.disconnect();
                    return 0;
                }

                if (line == "END" || line == "end")
                {
                    break;
                }

                if (line.empty())
                {
                    cout << "Please enter a valid file path." << endl;
                    continue;
                }

                email.addAttachment(line);
                cout << "Attachment added successfully." << endl;
            }

            // ========================================
            // DISPLAY DETECTED INFORMATION
            // ========================================

            cout << "\n========================================" << endl;
            cout << "Email details stored successfully." << endl;

            vector<string> detectedURLs = email.getURLs();

            cout << "URLs detected: " << detectedURLs.size() << endl;

            if (!detectedURLs.empty())
            {
                cout << "\nDetected URLs:" << endl;

                for (const string& url : detectedURLs)
                {
                    cout << " - " << url << endl;
                }
            }

            vector<string> attachments = email.getAttachments();

            cout << "\nAttachments added: " << attachments.size() << endl;

            for (const string& attachment : attachments)
            {
                cout << " - " << attachment << endl;
            }

            cout << "========================================" << endl;

            // ========================================
            // SAVE EMAIL
            // ========================================

            int emailId = database.saveEmail(email);

            if (emailId <= 0)
            {
                cout << "\nFailed to save email." << endl;
                continue;
            }

            email.setEmailId(emailId);

            cout << "\nEmail saved to database successfully." << endl;
            cout << "Email ID: " << emailId << endl;

            // ========================================
            // CREATE INVESTIGATION
            // ========================================

            Investigation investigation(
                0,
                loggedInUser,
                email
            );

            // ========================================
            // RUN INVESTIGATION
            // ========================================

            investigation.startInvestigation();

            // ========================================
            // GET FINAL RISK SCORE
            // ========================================

            RiskCalculator calculator =
                investigation.getRiskCalculator();

            int finalRiskScore = calculator.getTotalScore();
            string finalRiskLevel = calculator.getRiskLevel();

            // ========================================
            // GET INDIVIDUAL SCORES
            // ========================================

            int senderScore = calculator.getSenderScore();
            int urlScore = calculator.getURLScore();
            int contentScore = calculator.getContentScore();
            int attachmentScore = calculator.getAttachmentScore();
            int headerScore = calculator.getHeaderScore();

            // ========================================
            // SAVE INVESTIGATION AND GET ITS ID
            // ========================================

            int investigationId = database.saveInvestigation(
                userId,
                emailId,
                finalRiskScore,
                finalRiskLevel,
                senderScore,
                urlScore,
                contentScore,
                attachmentScore,
                headerScore
            );

            if (investigationId > 0)
            {
                investigation.setInvestigationId(investigationId);

                cout << "\nInvestigation saved successfully." << endl;
                cout << "Investigation ID: "
                     << investigation.getInvestigationId()
                     << endl;
            }
            else
            {
                cout << "\nFailed to save investigation." << endl;
                cout << "The report will not be generated because the investigation was not saved." << endl;
                continue;
            }

            // ========================================
            // GENERATE INVESTIGATION REPORT
            // ========================================

            ReportGenerator reportGenerator;

            reportGenerator.generateReport(investigation);

            // ========================================
            // FINAL RESULT
            // ========================================

            cout << "\n========================================" << endl;
            cout << "Investigation completed." << endl;
            cout << "Email ID: " << emailId << endl;
            cout << "Investigation ID: "
                 << investigation.getInvestigationId()
                 << endl;
            cout << "Risk Score: " << finalRiskScore << "/100" << endl;
            cout << "Risk Level: " << finalRiskLevel << endl;
            cout << "========================================" << endl;
        }

        // ========================================
        // VIEW HISTORY
        // ========================================

        else if (choice == 3)
        {
            database.viewInvestigationHistory();
        }

        // ========================================
        // EXIT
        // ========================================

        else if (choice == 4)
        {
            cout << "\nExiting program..." << endl;
            database.disconnect();
            break;
        }

        // ========================================
        // INVALID CHOICE
        // ========================================

        else
        {
            cout << "\nInvalid choice. Please try again." << endl;
        }
    }

    return 0;
}
