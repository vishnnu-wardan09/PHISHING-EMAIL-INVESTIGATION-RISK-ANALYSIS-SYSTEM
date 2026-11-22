#include "../include/Email.h"

using namespace std;


// ========================================
// CONSTRUCTOR
// ========================================

Email::Email()
{
    emailId = -1;

    sender = "";
    replyTo = "";
    subject = "";
    body = "";

    urls.clear();
    attachments.clear();
}


// ========================================
// EMAIL ID
// ========================================

void Email::setEmailId(int id)
{
    emailId = id;
}

int Email::getEmailId()
{
    return emailId;
}


// ========================================
// SENDER
// ========================================

void Email::setSender(string emailSender)
{
    sender = emailSender;
}

string Email::getSender()
{
    return sender;
}


// ========================================
// REPLY TO
// ========================================

void Email::setReplyTo(string emailReplyTo)
{
    replyTo = emailReplyTo;
}

string Email::getReplyTo()
{
    return replyTo;
}


// ========================================
// SUBJECT
// ========================================

void Email::setSubject(string emailSubject)
{
    subject = emailSubject;
}

string Email::getSubject()
{
    return subject;
}


// ========================================
// BODY
// ========================================

void Email::setBody(string emailBody)
{
    body = emailBody;
}

string Email::getBody()
{
    return body;
}


// ========================================
// URLS
// ========================================

void Email::setUrls(vector<string> emailUrls)
{
    urls = emailUrls;
}

vector<string> Email::getUrls()
{
    return urls;
}

vector<string> Email::getURLs()
{
    return urls;
}

vector<string> Email::getURLS()
{
    return urls;
}

void Email::addUrl(string url)
{
    urls.push_back(url);
}

void Email::addURL(string url)
{
    urls.push_back(url);
}


// ========================================
// ATTACHMENTS
// ========================================

void Email::setAttachments(vector<string> emailAttachments)
{
    attachments = emailAttachments;
}

vector<string> Email::getAttachments()
{
    return attachments;
}

void Email::addAttachment(string attachment)
{
    attachments.push_back(attachment);
}