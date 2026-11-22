#ifndef INVESTIGATION_H
#define INVESTIGATION_H

#include "User.h"
#include "Email.h"
#include "RiskCalculator.h"

class Investigation
{
private:
    int investigationId;
    User investigator;
    Email email;
    RiskCalculator riskCalculator;
    string status;

public:
    Investigation();
    Investigation(int id, User user, Email emailData);

    void setInvestigationId(int id);
    void setStatus(string investigationStatus);

    int getInvestigationId();
    User getInvestigator();
    Email getEmail();
    RiskCalculator getRiskCalculator();
    string getStatus();

    void startInvestigation();
};

#endif