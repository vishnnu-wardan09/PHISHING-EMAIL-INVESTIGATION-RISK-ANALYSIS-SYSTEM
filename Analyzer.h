#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
using namespace std;

class Analyzer
{
protected:
    string result;
    int riskScore;

public:
    Analyzer();
    virtual ~Analyzer();

    virtual void analyze() = 0;

    string getResult();
    int getRiskScore();
};

#endif