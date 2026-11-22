#include "../include/Analyzer.h"

Analyzer::Analyzer()
{
    result = "";
    riskScore = 0;
}

Analyzer::~Analyzer()
{
}

string Analyzer::getResult()
{
    return result;
}

int Analyzer::getRiskScore()
{
    return riskScore;
}