#include "../include/ContentAnalyzer.h"

ContentAnalyzer::ContentAnalyzer(Email emailData)
{
    email = emailData;
}

void ContentAnalyzer::analyze()
{
    result = "";
    riskScore = 0;

    string body = email.getBody();

    if (body.empty())
    {
        result = "Email body is empty.";
        return;
    }

    result = "Content Analysis:\n";

    if (body.find("urgent") != string::npos ||
        body.find("URGENT") != string::npos)
    {
        result += " - Urgency detected.\n";
        riskScore += 10;
    }

    if (body.find("password") != string::npos ||
        body.find("PASSWORD") != string::npos)
    {
        result += " - Password request detected.\n";
        riskScore += 20;
    }

    if (body.find("verify") != string::npos ||
        body.find("VERIFY") != string::npos)
    {
        result += " - Verification request detected.\n";
        riskScore += 10;
    }

    if (body.find("suspended") != string::npos ||
        body.find("SUSPENDED") != string::npos)
    {
        result += " - Account suspension language detected.\n";
        riskScore += 15;
    }

    if (body.find("payment") != string::npos ||
        body.find("PAYMENT") != string::npos)
    {
        result += " - Payment-related request detected.\n";
        riskScore += 15;
    }

    if (riskScore == 0)
    {
        result += "No suspicious content indicators detected.";
    }
}