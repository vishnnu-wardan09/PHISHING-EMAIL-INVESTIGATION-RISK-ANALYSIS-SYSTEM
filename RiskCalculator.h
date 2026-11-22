#ifndef RISKCALCULATOR_H
#define RISKCALCULATOR_H

#include <string>

using namespace std;

class RiskCalculator
{
private:
    int senderScore;
    int urlScore;
    int contentScore;
    int attachmentScore;
    int headerScore;

    int totalScore;
    string riskLevel;

public:
    RiskCalculator();

    // Set individual scores
    void setSenderScore(int score);
    void setURLScore(int score);
    void setContentScore(int score);
    void setAttachmentScore(int score);
    void setHeaderScore(int score);

    // Get individual scores
    int getSenderScore();
    int getURLScore();
    int getContentScore();
    int getAttachmentScore();
    int getHeaderScore();

    // Calculate final risk
    void calculateRisk();

    // Get final result
    int getTotalScore();
    string getRiskLevel();
};

#endif