#ifndef URLANALYZER_H
#define URLANALYZER_H

#include "Analyzer.h"
#include "Email.h"

class URLAnalyzer : public Analyzer
{
private:
    Email email;

public:
    URLAnalyzer(Email email);

    void analyze() override;
};

#endif