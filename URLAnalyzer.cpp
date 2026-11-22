#include "../include/URLAnalyzer.h"

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

URLAnalyzer::URLAnalyzer(Email emailData)
{
    email = emailData;
}

void URLAnalyzer::analyze()
{
    result = "";
    riskScore = 0;

    cout << "\n========== URL ANALYSIS =========="
         << endl;

    vector<string> urls = email.getURLs();

    if (urls.empty())
    {
        cout << "No URLs found." << endl;

        result = "No URLs found.";

        return;
    }

    for (string url : urls)
    {
        cout << "URL: " << url << endl;

        string lowerURL = url;

        for (char &c : lowerURL)
        {
            c = static_cast<char>(
                tolower(static_cast<unsigned char>(c))
            );
        }

        bool suspicious = false;

        // HTTP instead of HTTPS
        if (lowerURL.find("http://") != string::npos)
        {
            cout << "Warning: URL uses HTTP instead of HTTPS."
                 << endl;

            riskScore += 10;
            suspicious = true;
        }

        // Suspicious @ character
        if (lowerURL.find("@") != string::npos)
        {
            cout << "Warning: URL contains suspicious '@' character."
                 << endl;

            riskScore += 15;
            suspicious = true;
        }

        // Phishing-related keywords
        if (lowerURL.find("login") != string::npos ||
            lowerURL.find("verify") != string::npos ||
            lowerURL.find("account") != string::npos ||
            lowerURL.find("password") != string::npos)
        {
            cout << "Warning: URL contains phishing-related keywords."
                 << endl;

            riskScore += 15;
            suspicious = true;
        }

        if (!suspicious)
        {
            cout << "URL appears normal." << endl;
        }
    }

    // Maximum URL contribution = 25
    if (riskScore > 25)
    {
        riskScore = 25;
    }

    result = "URL analysis completed.";
}