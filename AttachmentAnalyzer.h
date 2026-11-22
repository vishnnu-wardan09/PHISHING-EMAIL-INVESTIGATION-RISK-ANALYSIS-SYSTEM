#ifndef ATTACHMENTANALYZER_H
#define ATTACHMENTANALYZER_H

#include "Analyzer.h"
#include "Email.h"
#include "AttachmentContentReader.h"

#include <string>

using namespace std;

class AttachmentAnalyzer : public Analyzer
{
private:
    Email email;

    void analyzeAttachment(
        const string& attachmentPath
    );

public:
    AttachmentAnalyzer(Email email);

    void analyze() override;
};

#endif