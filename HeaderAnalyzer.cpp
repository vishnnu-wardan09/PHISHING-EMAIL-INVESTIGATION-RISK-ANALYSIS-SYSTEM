#include "../include/HeaderAnalyzer.h"

HeaderAnalyzer::HeaderAnalyzer(Email emailData)
{
    email = emailData;
}

void HeaderAnalyzer::analyze()
{
    result = "";
    riskScore = 0;

    string sender = email.getSender();
    string replyTo = email.getReplyTo();

    result = "Header Analysis:\n";

    if (sender.empty())
    {
        result += " - Sender information is missing.\n";
        riskScore += 10;
    }
    else
    {
        result += " - Sender information is available.\n";
    }

    if (replyTo.empty())
    {
        result += " - Reply-To information is missing.\n";
        riskScore += 5;
    }
    else
    {
        result += " - Reply-To information is available.\n";
    }

    if (!sender.empty() && !replyTo.empty() && sender != replyTo)
    {
        result += " - Sender and Reply-To addresses do not match.\n";
        riskScore += 15;
    }

    if (riskScore == 0)
    {
        result += "No suspicious header indicators detected.";
    }
}