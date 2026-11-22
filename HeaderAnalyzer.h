#ifndef HEADERANALYZER_H
#define HEADERANALYZER_H

#include "Analyzer.h"
#include "Email.h"

class HeaderAnalyzer : public Analyzer
{
private:
    Email email;

public:
    HeaderAnalyzer(Email email);

    void analyze() override;
};

#endif