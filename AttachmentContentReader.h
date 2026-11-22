#ifndef ATTACHMENTCONTENTREADER_H
#define ATTACHMENTCONTENTREADER_H

#include <string>
#include <vector>

using namespace std;

class AttachmentContentReader
{
private:
    string filePath;
    string fileType;
    string extractedContent;

public:
    AttachmentContentReader(string path);

    bool readAttachment();

    string getFileType();
    string getExtractedContent();

    bool isSupported();

private:
    string getExtension();

    bool readTXT();
    bool readPDF();
    bool readOfficeFile();
    bool readZIP();
    bool readScriptFile();
    bool analyzeExecutable();
};

#endif