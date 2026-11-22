#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "Investigation.h"

class ReportGenerator
{
public:
    ReportGenerator();

    void generateReport(Investigation investigation);
};

#endif