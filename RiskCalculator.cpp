#include "../include/RiskCalculator.h"

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

RiskCalculator::RiskCalculator()
{
    senderScore = 0;
    urlScore = 0;
    contentScore = 0;
    attachmentScore = 0;
    headerScore = 0;

    totalScore = 0;
    riskLevel = "LOW";
}


// ========================================
// SET SENDER SCORE
// ========================================

void RiskCalculator::setSenderScore(int score)
{
    senderScore = score;
}


// ========================================
// SET URL SCORE
// ========================================

void RiskCalculator::setURLScore(int score)
{
    urlScore = score;
}


// ========================================
// SET CONTENT SCORE
// ========================================

void RiskCalculator::setContentScore(int score)
{
    contentScore = score;
}


// ========================================
// SET ATTACHMENT SCORE
// ========================================

void RiskCalculator::setAttachmentScore(int score)
{
    attachmentScore = score;
}


// ========================================
// SET HEADER SCORE
// ========================================

void RiskCalculator::setHeaderScore(int score)
{
    headerScore = score;
}


// ========================================
// GET SENDER SCORE
// ========================================

int RiskCalculator::getSenderScore()
{
    return senderScore;
}


// ========================================
// GET URL SCORE
// ========================================

int RiskCalculator::getURLScore()
{
    return urlScore;
}


// ========================================
// GET CONTENT SCORE
// ========================================

int RiskCalculator::getContentScore()
{
    return contentScore;
}


// ========================================
// GET ATTACHMENT SCORE
// ========================================

int RiskCalculator::getAttachmentScore()
{
    return attachmentScore;
}


// ========================================
// GET HEADER SCORE
// ========================================

int RiskCalculator::getHeaderScore()
{
    return headerScore;
}


// ========================================
// CALCULATE RISK
// ========================================

void RiskCalculator::calculateRisk()
{
    int rawScore =
        senderScore +
        urlScore +
        contentScore +
        attachmentScore +
        headerScore;

    // Maximum final score is 100
    if (rawScore > 100)
    {
        totalScore = 100;
    }
    else
    {
        totalScore = rawScore;
    }


    // ========================================
    // RISK LEVEL
    // ========================================

    if (totalScore >= 70)
    {
        riskLevel = "HIGH";
    }
    else if (totalScore >= 40)
    {
        riskLevel = "MEDIUM";
    }
    else
    {
        riskLevel = "LOW";
    }
}


// ========================================
// GET TOTAL SCORE
// ========================================

int RiskCalculator::getTotalScore()
{
    return totalScore;
}


// ========================================
// GET RISK LEVEL
// ========================================

string RiskCalculator::getRiskLevel()
{
    return riskLevel;
}