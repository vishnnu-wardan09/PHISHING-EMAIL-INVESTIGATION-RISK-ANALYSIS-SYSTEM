#include "../include/ReportGenerator.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

ReportGenerator::ReportGenerator()
{
}

void ReportGenerator::generateReport(Investigation investigation)
{
    Email email = investigation.getEmail();
    RiskCalculator risk = investigation.getRiskCalculator();

    // ========================================
    // CREATE REPORT FILE
    // ========================================

    string fileName =
        "reports/investigation_" +
        to_string(investigation.getInvestigationId()) +
        ".txt";

    ofstream reportFile(fileName);

    if (!reportFile)
    {
        cout << "\nWarning: Could not create report file." << endl;
    }

    // ========================================
    // GET CURRENT DATE AND TIME
    // ========================================

    time_t currentTime = time(nullptr);
    tm *localTime = localtime(&currentTime);

    char dateTime[50];

    strftime(
        dateTime,
        sizeof(dateTime),
        "%Y-%m-%d %H:%M:%S",
        localTime
    );


    // ========================================
    // TERMINAL REPORT
    // ========================================

    cout << "\n==================================================" << endl;
    cout << "             INVESTIGATION REPORT" << endl;
    cout << "==================================================" << endl;

    cout << "\n--------------- INVESTIGATION DETAILS -----------" << endl;

    cout << "Investigation ID : "
         << investigation.getInvestigationId() << endl;

    cout << "Investigator     : "
         << investigation.getInvestigator().getUsername() << endl;

    cout << "Status           : "
         << investigation.getStatus() << endl;

    cout << "Investigation Date: "
         << dateTime << endl;


    // ========================================
    // EMAIL DETAILS
    // ========================================

    cout << "\n------------------- EMAIL DETAILS ----------------" << endl;

    cout << "Email ID         : "
         << email.getEmailId() << endl;

    cout << "Sender Email     : "
         << email.getSender() << endl;

    cout << "Reply-To Email   : "
         << email.getReplyTo() << endl;

    cout << "Subject          : "
         << email.getSubject() << endl;


    // ========================================
    // EMAIL BODY
    // ========================================

    cout << "\n-------------------- EMAIL BODY ------------------" << endl;

    cout << email.getBody() << endl;


    // ========================================
    // DETECTED URLS
    // ========================================

    cout << "\n------------------- DETECTED URLS ----------------" << endl;

    vector<string> urls = email.getUrls();

    if (urls.empty())
    {
        cout << "No URLs detected." << endl;
    }
    else
    {
        for (string url : urls)
        {
            cout << "- " << url << endl;
        }
    }


    // ========================================
    // ATTACHMENTS
    // ========================================

    cout << "\n------------------- ATTACHMENTS ------------------" << endl;

    vector<string> attachments = email.getAttachments();

    if (attachments.empty())
    {
        cout << "No attachments detected." << endl;
    }
    else
    {
        for (string attachment : attachments)
        {
            cout << "- " << attachment << endl;
        }
    }


    // ========================================
    // RISK ANALYSIS
    // ========================================

    cout << "\n------------------- RISK ANALYSIS ----------------" << endl;

    cout << "Sender Score     : "
         << risk.getSenderScore() << endl;

    cout << "URL Score        : "
         << risk.getURLScore() << endl;

    cout << "Content Score    : "
         << risk.getContentScore() << endl;

    cout << "Attachment Score : "
         << risk.getAttachmentScore() << endl;

    cout << "Header Score     : "
         << risk.getHeaderScore() << endl;

    cout << "--------------------------------------------------" << endl;

    cout << "Final Risk Score : "
         << risk.getTotalScore() << "/100" << endl;

    cout << "Risk Level       : "
         << risk.getRiskLevel() << endl;

    cout << "==================================================" << endl;

    cout << "\nReport generated successfully." << endl;


    // ========================================
    // SAVE REPORT TO TXT FILE
    // ========================================

    if (reportFile)
    {
        reportFile << "==================================================" << endl;
        reportFile << "             INVESTIGATION REPORT" << endl;
        reportFile << "==================================================" << endl;

        reportFile << "\n--------------- INVESTIGATION DETAILS -----------" << endl;

        reportFile << "Investigation ID : "
                   << investigation.getInvestigationId() << endl;

        reportFile << "Investigator     : "
                   << investigation.getInvestigator().getUsername() << endl;

        reportFile << "Status           : "
                   << investigation.getStatus() << endl;

        reportFile << "Investigation Date: "
                   << dateTime << endl;


        reportFile << "\n------------------- EMAIL DETAILS ----------------" << endl;

        reportFile << "Email ID         : "
                   << email.getEmailId() << endl;

        reportFile << "Sender Email     : "
                   << email.getSender() << endl;

        reportFile << "Reply-To Email   : "
                   << email.getReplyTo() << endl;

        reportFile << "Subject          : "
                   << email.getSubject() << endl;


        reportFile << "\n-------------------- EMAIL BODY ------------------" << endl;

        reportFile << email.getBody() << endl;


        reportFile << "\n------------------- DETECTED URLS ----------------" << endl;

        if (urls.empty())
        {
            reportFile << "No URLs detected." << endl;
        }
        else
        {
            for (string url : urls)
            {
                reportFile << "- " << url << endl;
            }
        }


        reportFile << "\n------------------- ATTACHMENTS ------------------" << endl;

        if (attachments.empty())
        {
            reportFile << "No attachments detected." << endl;
        }
        else
        {
            for (string attachment : attachments)
            {
                reportFile << "- " << attachment << endl;
            }
        }


        reportFile << "\n------------------- RISK ANALYSIS ----------------" << endl;

        reportFile << "Sender Score     : "
                   << risk.getSenderScore() << endl;

        reportFile << "URL Score        : "
                   << risk.getURLScore() << endl;

        reportFile << "Content Score    : "
                   << risk.getContentScore() << endl;

        reportFile << "Attachment Score : "
                   << risk.getAttachmentScore() << endl;

        reportFile << "Header Score     : "
                   << risk.getHeaderScore() << endl;

        reportFile << "--------------------------------------------------" << endl;

        reportFile << "Final Risk Score : "
                   << risk.getTotalScore() << "/100" << endl;

        reportFile << "Risk Level       : "
                   << risk.getRiskLevel() << endl;

        reportFile << "==================================================" << endl;

        reportFile << "\nReport generated successfully." << endl;

        reportFile.close();

        cout << "Report saved to: "
             << fileName << endl;
    }
}