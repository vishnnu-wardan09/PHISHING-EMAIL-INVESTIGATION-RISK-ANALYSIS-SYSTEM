#include "../include/SenderAnalyzer.h"

SenderAnalyzer::SenderAnalyzer(Email emailData)
{
    email = emailData;
}

void SenderAnalyzer::analyze()
{
    result = "";
    riskScore = 0;

    string sender = email.getSender();
    string replyTo = email.getReplyTo();

    if (sender.empty())
    {
        result = "Sender address is missing.";
        riskScore += 20;
    }
    else
    {
        result = "Sender address found: " + sender;
    }

    if (!replyTo.empty() && sender != replyTo)
    {
        result += "\nSender and Reply-To addresses are different.";
        riskScore += 20;
    }

    if (sender.find("@") == string::npos)
    {
        result += "\nSender address format appears suspicious.";
        riskScore += 20;
    }

    if (riskScore == 0)
    {
        result += "\nNo suspicious sender indicators detected.";
    }
}