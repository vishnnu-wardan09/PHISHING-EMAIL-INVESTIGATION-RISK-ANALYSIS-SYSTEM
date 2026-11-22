#ifndef CONTENTANALYZER_H
#define CONTENTANALYZER_H

#include "Analyzer.h"
#include "Email.h"

class ContentAnalyzer : public Analyzer
{
private:
    Email email;

public:
    ContentAnalyzer(Email email);

    void analyze() override;
};

#endif