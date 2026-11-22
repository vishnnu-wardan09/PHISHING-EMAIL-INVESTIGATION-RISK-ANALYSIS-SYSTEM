#include "../include/Investigation.h"

#include "../include/SenderAnalyzer.h"
#include "../include/URLAnalyzer.h"
#include "../include/ContentAnalyzer.h"
#include "../include/AttachmentAnalyzer.h"
#include "../include/HeaderAnalyzer.h"

#include <iostream>

using namespace std;


// ========================================
// DEFAULT CONSTRUCTOR
// ========================================

Investigation::Investigation()
{
    investigationId = 0;
    status = "Not Started";
}


// ========================================
// PARAMETERIZED CONSTRUCTOR
// ========================================

Investigation::Investigation(
    int id,
    User user,
    Email emailData)
{
    investigationId = id;
    investigator = user;
    email = emailData;
    status = "Not Started";
}


// ========================================
// SET INVESTIGATION ID
// ========================================

void Investigation::setInvestigationId(int id)
{
    investigationId = id;
}


// ========================================
// SET STATUS
// ========================================

void Investigation::setStatus(string investigationStatus)
{
    status = investigationStatus;
}


// ========================================
// GET INVESTIGATION ID
// ========================================

int Investigation::getInvestigationId()
{
    return investigationId;
}


// ========================================
// GET INVESTIGATOR
// ========================================

User Investigation::getInvestigator()
{
    return investigator;
}


// ========================================
// GET EMAIL
// ========================================

Email Investigation::getEmail()
{
    return email;
}


// ========================================
// GET RISK CALCULATOR
// ========================================

RiskCalculator Investigation::getRiskCalculator()
{
    return riskCalculator;
}


// ========================================
// GET STATUS
// ========================================

string Investigation::getStatus()
{
    return status;
}


// ========================================
// START INVESTIGATION
// ========================================

void Investigation::startInvestigation()
{
    cout << "\n";
    cout << "========================================"
         << endl;

    cout << "       STARTING EMAIL INVESTIGATION"
         << endl;

    cout << "========================================"
         << endl;

    status = "In Progress";


    // ========================================
    // 1. SENDER ANALYSIS
    // ========================================

    SenderAnalyzer senderAnalyzer(email);

    senderAnalyzer.analyze();

    int senderScore =
        senderAnalyzer.getRiskScore();

    cout << "\nSender Risk Score: "
         << senderScore
         << endl;


    // ========================================
    // 2. URL ANALYSIS
    // ========================================

    URLAnalyzer urlAnalyzer(email);

    urlAnalyzer.analyze();

    int urlScore =
        urlAnalyzer.getRiskScore();

    cout << "\nURL Risk Score: "
         << urlScore
         << endl;


    // ========================================
    // 3. CONTENT ANALYSIS
    // ========================================

    ContentAnalyzer contentAnalyzer(email);

    contentAnalyzer.analyze();

    int contentScore =
        contentAnalyzer.getRiskScore();

    cout << "\nContent Risk Score: "
         << contentScore
         << endl;


    // ========================================
    // 4. ATTACHMENT ANALYSIS
    // ========================================

    AttachmentAnalyzer attachmentAnalyzer(email);

    attachmentAnalyzer.analyze();

    int attachmentScore =
        attachmentAnalyzer.getRiskScore();

    cout << "\nAttachment Risk Score: "
         << attachmentScore
         << endl;


    // ========================================
    // 5. HEADER ANALYSIS
    // ========================================

    HeaderAnalyzer headerAnalyzer(email);

    headerAnalyzer.analyze();

    int headerScore =
        headerAnalyzer.getRiskScore();

    cout << "\nHeader Risk Score: "
         << headerScore
         << endl;


    // ========================================
    // 6. SEND SCORES TO RISK CALCULATOR
    // ========================================

    riskCalculator.setSenderScore(senderScore);

    riskCalculator.setURLScore(urlScore);

    riskCalculator.setContentScore(contentScore);

    riskCalculator.setAttachmentScore(
        attachmentScore);

    riskCalculator.setHeaderScore(headerScore);


    // ========================================
    // 7. CALCULATE RISK
    // ========================================

    riskCalculator.calculateRisk();


    // ========================================
    // 8. FINAL RISK ASSESSMENT
    // ========================================

    cout << "\n";
    cout << "========================================"
         << endl;

    cout << "         FINAL RISK ASSESSMENT"
         << endl;

    cout << "========================================"
         << endl;

    cout << "Sender Score     : "
         << senderScore
         << endl;

    cout << "URL Score        : "
         << urlScore
         << endl;

    cout << "Content Score    : "
         << contentScore
         << endl;

    cout << "Attachment Score : "
         << attachmentScore
         << endl;

    cout << "Header Score     : "
         << headerScore
         << endl;

    cout << "----------------------------------------"
         << endl;

    cout << "TOTAL RISK SCORE : "
         << riskCalculator.getTotalScore()
         << "/100"
         << endl;

    cout << "RISK LEVEL       : "
         << riskCalculator.getRiskLevel()
         << endl;

    cout << "========================================"
         << endl;


    status = "Completed";
}