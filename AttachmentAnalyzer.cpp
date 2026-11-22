#include "../include/AttachmentAnalyzer.h"

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

AttachmentAnalyzer::AttachmentAnalyzer(
    Email emailData)
{
    email = emailData;
}


// ========================================
// ANALYZE SINGLE ATTACHMENT
// ========================================

void AttachmentAnalyzer::analyzeAttachment(
    const string& attachmentPath)
{
    cout << "\n----------------------------------------"
         << endl;

    cout << "Attachment: "
         << attachmentPath
         << endl;

    // ====================================
    // CREATE CONTENT READER
    // ====================================

    AttachmentContentReader reader(
        attachmentPath
    );


    // ====================================
    // CHECK WHETHER FORMAT IS SUPPORTED
    // ====================================

    if (!reader.isSupported())
    {
        cout << "File Type: Unsupported"
             << endl;

        cout << "Content Analysis: Not available"
             << endl;

        cout << "Warning: Unsupported attachment "
                "format."
             << endl;

        result +=
            "\nUnsupported attachment: " +
            attachmentPath;

        return;
    }


    // ====================================
    // READ ATTACHMENT
    // ====================================

    bool readSuccessful =
        reader.readAttachment();


    cout << "File Type: "
         << reader.getFileType()
         << endl;


    // ====================================
    // DISPLAY EXTRACTED CONTENT
    // ====================================

    cout << "\nExtracted / Analyzed Content:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    string content =
        reader.getExtractedContent();


    if (content.empty())
    {
        cout << "No readable content found."
             << endl;
    }
    else
    {
        cout << content << endl;
    }


    cout << "----------------------------------------"
         << endl;


    // ====================================
    // CONTENT ANALYSIS
    // ====================================

    string lowerContent =
        content;

    for (char &c : lowerContent)
    {
        c = static_cast<char>(
            tolower(
                static_cast<unsigned char>(c)
            )
        );
    }


    int contentRisk = 0;


    // ====================================
    // PASSWORD INDICATOR
    // ====================================

    if (lowerContent.find(
            "password"
        ) != string::npos)
    {
        cout << "Warning: Password-related "
                "content detected."
             << endl;

        contentRisk += 5;
    }


    // ====================================
    // LOGIN INDICATOR
    // ====================================

    if (lowerContent.find(
            "login"
        ) != string::npos)
    {
        cout << "Warning: Login-related "
                "content detected."
             << endl;

        contentRisk += 5;
    }


    // ====================================
    // URGENCY INDICATOR
    // ====================================

    if (lowerContent.find(
            "urgent"
        ) != string::npos ||
        lowerContent.find(
            "immediately"
        ) != string::npos)
    {
        cout << "Warning: Urgency-related "
                "content detected."
             << endl;

        contentRisk += 5;
    }


    // ====================================
    // DOWNLOAD INDICATOR
    // ====================================

    if (lowerContent.find(
            "download"
        ) != string::npos)
    {
        cout << "Warning: Download-related "
                "content detected."
             << endl;

        contentRisk += 5;
    }


    // ====================================
    // HTTP / HTTPS INDICATOR
    // ====================================

    if (lowerContent.find(
            "http://"
        ) != string::npos ||
        lowerContent.find(
            "https://"
        ) != string::npos)
    {
        cout << "Warning: URL detected "
                "inside attachment."
             << endl;

        contentRisk += 5;
    }


    // ====================================
    // POWERSHELL INDICATOR
    // ====================================

    if (lowerContent.find(
            "powershell"
        ) != string::npos)
    {
        cout << "Warning: PowerShell-related "
                "content detected."
             << endl;

        contentRisk += 10;
    }


    // ====================================
    // CMD INDICATOR
    // ====================================

    if (lowerContent.find(
            "cmd.exe"
        ) != string::npos ||
        lowerContent.find(
            "command prompt"
        ) != string::npos)
    {
        cout << "Warning: Command execution "
                "indicator detected."
             << endl;

        contentRisk += 10;
    }


    // ====================================
    // RUNDLL32
    // ====================================

    if (lowerContent.find(
            "rundll32"
        ) != string::npos)
    {
        cout << "Warning: rundll32 indicator "
                "detected."
             << endl;

        contentRisk += 10;
    }


    // ====================================
    // REGSVR32
    // ====================================

    if (lowerContent.find(
            "regsvr32"
        ) != string::npos)
    {
        cout << "Warning: regsvr32 indicator "
                "detected."
             << endl;

        contentRisk += 10;
    }


    // ====================================
    // FILE EXTENSION ANALYSIS
    // ====================================

    string lowerName =
        attachmentPath;

    for (char &c : lowerName)
    {
        c = static_cast<char>(
            tolower(
                static_cast<unsigned char>(c)
            )
        );
    }


    // ====================================
    // DANGEROUS EXECUTABLE
    // ====================================

    if (lowerName.find(".exe")
            != string::npos)
    {
        cout << "Warning: Executable "
                "attachment detected."
             << endl;

        contentRisk += 25;
    }


    // ====================================
    // BATCH FILE
    // ====================================

    else if (lowerName.find(".bat")
                 != string::npos)
    {
        cout << "Warning: Batch script "
                "attachment detected."
             << endl;

        contentRisk += 20;
    }


    // ====================================
    // CMD FILE
    // ====================================

    else if (lowerName.find(".cmd")
                 != string::npos)
    {
        cout << "Warning: CMD script "
                "attachment detected."
             << endl;

        contentRisk += 20;
    }


    // ====================================
    // JAVASCRIPT
    // ====================================

    else if (lowerName.find(".js")
                 != string::npos)
    {
        cout << "Warning: JavaScript "
                "attachment detected."
             << endl;

        contentRisk += 20;
    }


    // ====================================
    // FINAL ATTACHMENT SCORE
    // ====================================

    riskScore += contentRisk;


    // ====================================
    // LIMIT ATTACHMENT SCORE
    // ====================================

    if (riskScore > 25)
    {
        riskScore = 25;
    }


    // ====================================
    // READ STATUS
    // ====================================

    if (!readSuccessful)
    {
        cout << "\nNote: Complete content analysis "
                "was not possible for this file."
             << endl;
    }


    // ====================================
    // STORE RESULT
    // ====================================

    result +=
        "\nAttachment analyzed: " +
        attachmentPath;

    result +=
        "\nFile type: " +
        reader.getFileType();

    result +=
        "\nAttachment content analysis completed.";


    cout << "\nCurrent Attachment Risk Score: "
         << riskScore
         << "/25"
         << endl;
}


// ========================================
// MAIN ANALYZE FUNCTION
// ========================================

void AttachmentAnalyzer::analyze()
{
    result = "";
    riskScore = 0;


    cout << "\n========== ATTACHMENT ANALYSIS =========="
         << endl;


    // ====================================
    // GET ATTACHMENTS
    // ====================================

    vector<string> attachments =
        email.getAttachments();


    if (attachments.empty())
    {
        cout << "No attachments found."
             << endl;

        result =
            "No attachments found.";

        return;
    }


    // ====================================
    // ANALYZE EACH ATTACHMENT
    // ====================================

    for (string attachment :
         attachments)
    {
        analyzeAttachment(
            attachment
        );
    }


    // ====================================
    // FINAL SCORE
    // ====================================

    if (riskScore > 25)
    {
        riskScore = 25;
    }


    cout << "\n========================================"
         << endl;

    cout << "FINAL ATTACHMENT RISK SCORE: "
         << riskScore
         << "/25"
         << endl;

    cout << "========================================"
         << endl;
}