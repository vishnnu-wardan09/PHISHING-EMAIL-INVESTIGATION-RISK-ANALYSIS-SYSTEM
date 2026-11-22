#ifndef SENDER_ANALYZER_H
#define SENDER_ANALYZER_H

#include "Analyzer.h"
#include "Email.h"

class SenderAnalyzer : public Analyzer
{
private:
    Email email;

public:
    SenderAnalyzer(Email email);

    void analyze() override;
};

#endif