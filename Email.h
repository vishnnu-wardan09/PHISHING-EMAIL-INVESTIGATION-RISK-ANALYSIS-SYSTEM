#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <vector>

using namespace std;

class Email
{
private:
    int emailId;

    string sender;
    string replyTo;
    string subject;
    string body;

    vector<string> urls;
    vector<string> attachments;

public:

    Email();

    // Email ID
    void setEmailId(int id);
    int getEmailId();

    // Sender
    void setSender(string emailSender);
    string getSender();

    // Reply To
    void setReplyTo(string emailReplyTo);
    string getReplyTo();

    // Subject
    void setSubject(string emailSubject);
    string getSubject();

    // Body
    void setBody(string emailBody);
    string getBody();

    // URLs
    void setUrls(vector<string> emailUrls);
    vector<string> getUrls();
    vector<string> getURLs();
    vector<string> getURLS();

    void addUrl(string url);
    void addURL(string url);

    // Attachments
    void setAttachments(vector<string> emailAttachments);
    vector<string> getAttachments();
    void addAttachment(string attachment);
};

#endif