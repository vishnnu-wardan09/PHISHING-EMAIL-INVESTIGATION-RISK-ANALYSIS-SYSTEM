#include "../include/AttachmentContentReader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <vector>
#include <cstdint>

#include <zip.h>
#include <pugixml.hpp>

#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>

using namespace std;

namespace fs = std::filesystem;


// ========================================
// HELPER FUNCTION
// ========================================

string toLowerCase(string text)
{
    for (char &c : text)
    {
        c = static_cast<char>(
            tolower(
                static_cast<unsigned char>(c)
            )
        );
    }

    return text;
}


// ========================================
// CONSTRUCTOR
// ========================================

AttachmentContentReader::AttachmentContentReader(string path)
{
    filePath = path;
    fileType = "";
    extractedContent = "";
}


// ========================================
// GET FILE EXTENSION
// ========================================

string AttachmentContentReader::getExtension()
{
    fs::path path(filePath);

    string extension =
        path.extension().string();

    return toLowerCase(extension);
}


// ========================================
// CHECK WHETHER FORMAT IS SUPPORTED
// ========================================

bool AttachmentContentReader::isSupported()
{
    string extension =
        getExtension();

    return
        extension == ".txt"  ||
        extension == ".pdf"  ||
        extension == ".docx" ||
        extension == ".xlsx" ||
        extension == ".pptx" ||
        extension == ".zip"  ||
        extension == ".js"   ||
        extension == ".exe"  ||
        extension == ".bat"  ||
        extension == ".cmd";
}


// ========================================
// GET FILE TYPE
// ========================================

string AttachmentContentReader::getFileType()
{
    return fileType;
}


// ========================================
// GET EXTRACTED CONTENT
// ========================================

string AttachmentContentReader::getExtractedContent()
{
    return extractedContent;
}


// ========================================
// READ ATTACHMENT
// ========================================

bool AttachmentContentReader::readAttachment()
{
    extractedContent = "";
    fileType = "";

    if (!fs::exists(filePath))
    {
        fileType = "File Not Found";

        extractedContent =
            "Attachment file could not be found.";

        return false;
    }

    if (!fs::is_regular_file(filePath))
    {
        fileType = "Invalid File";

        extractedContent =
            "The provided attachment path is not a file.";

        return false;
    }

    string extension =
        getExtension();


    // ========================================
    // TXT
    // ========================================

    if (extension == ".txt")
    {
        fileType = "Text File";

        return readTXT();
    }


    // ========================================
    // PDF
    // ========================================

    if (extension == ".pdf")
    {
        fileType = "PDF";

        return readPDF();
    }


    // ========================================
    // DOCX / XLSX / PPTX
    // ========================================

    if (extension == ".docx" ||
        extension == ".xlsx" ||
        extension == ".pptx")
    {
        if (extension == ".docx")
        {
            fileType =
                "Microsoft Word Document";
        }
        else if (extension == ".xlsx")
        {
            fileType =
                "Microsoft Excel Spreadsheet";
        }
        else
        {
            fileType =
                "Microsoft PowerPoint Presentation";
        }

        return readOfficeFile();
    }


    // ========================================
    // ZIP
    // ========================================

    if (extension == ".zip")
    {
        fileType = "ZIP Archive";

        return readZIP();
    }


    // ========================================
    // JAVASCRIPT
    // ========================================

    if (extension == ".js")
    {
        fileType =
            "JavaScript File";

        return readScriptFile();
    }


    // ========================================
    // BAT
    // ========================================

    if (extension == ".bat")
    {
        fileType =
            "Windows Batch Script";

        return readScriptFile();
    }


    // ========================================
    // CMD
    // ========================================

    if (extension == ".cmd")
    {
        fileType =
            "Windows Command Script";

        return readScriptFile();
    }


    // ========================================
    // EXE
    // ========================================

    if (extension == ".exe")
    {
        fileType =
            "Windows Executable";

        return analyzeExecutable();
    }


    // ========================================
    // UNSUPPORTED
    // ========================================

    fileType =
        "Unsupported";

    extractedContent =
        "Unsupported attachment format.";

    return false;
}


// ========================================
// READ TXT
// ========================================

bool AttachmentContentReader::readTXT()
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        extractedContent =
            "Unable to open text file.";

        return false;
    }

    stringstream buffer;

    buffer << file.rdbuf();

    extractedContent =
        buffer.str();

    file.close();

    if (extractedContent.empty())
    {
        extractedContent =
            "Text file is empty.";

        return true;
    }

    return true;
}


// ========================================
// READ PDF
// ========================================

bool AttachmentContentReader::readPDF()
{
    poppler::document *document =
        poppler::document::load_from_file(
            filePath
        );

    if (document == nullptr)
    {
        extractedContent =
            "Unable to open PDF. "
            "The PDF may be corrupted, encrypted, "
            "or unsupported.";

        return false;
    }

    if (document->is_encrypted())
    {
        extractedContent =
            "PDF is encrypted. "
            "Content could not be extracted.";

        delete document;

        return false;
    }

    int pageCount =
        document->pages();

    extractedContent =
        "PDF Pages: " +
        to_string(pageCount) +
        "\n\n";


    for (int i = 0;
         i < pageCount;
         i++)
    {
        poppler::page *page =
            document->create_page(i);

        if (page == nullptr)
        {
            continue;
        }

        poppler::ustring pageText =
            page->text();

        poppler::byte_array utf8Text =
            pageText.to_utf8();

        string text(
            utf8Text.begin(),
            utf8Text.end()
        );

        extractedContent +=
            "----- PAGE " +
            to_string(i + 1) +
            " -----\n";

        extractedContent +=
            text;

        extractedContent +=
            "\n\n";

        delete page;
    }

    delete document;


    if (extractedContent.empty())
    {
        extractedContent =
            "PDF was opened but no readable "
            "text was extracted.";

        return false;
    }

    return true;
}


// ========================================
// COLLECT XML TEXT
// ========================================

void collectXMLText(
    const pugi::xml_node &node,
    string &text)
{
    for (pugi::xml_node child :
         node.children())
    {
        if (child.type() ==
                pugi::node_pcdata ||
            child.type() ==
                pugi::node_cdata)
        {
            string value =
                child.value();

            if (!value.empty())
            {
                text += value;
                text += "\n";
            }
        }

        collectXMLText(
            child,
            text
        );
    }
}


// ========================================
// EXTRACT XML TEXT
// ========================================

string extractXMLText(
    const string &xmlData)
{
    pugi::xml_document document;

    pugi::xml_parse_result result =
        document.load_string(
            xmlData.c_str()
        );

    if (!result)
    {
        return "";
    }

    string text;

    collectXMLText(
        document,
        text
    );

    return text;
}


// ========================================
// READ OFFICE FILE
// DOCX / XLSX / PPTX
// ========================================

bool AttachmentContentReader::readOfficeFile()
{
    int error = 0;

    zip_t *archive =
        zip_open(
            filePath.c_str(),
            ZIP_RDONLY,
            &error
        );

    if (archive == nullptr)
    {
        extractedContent =
            "Unable to open Office file "
            "as ZIP archive.";

        return false;
    }

    zip_int64_t numberOfFiles =
        zip_get_num_entries(
            archive,
            0
        );

    string extension =
        getExtension();

    bool extractedSomething =
        false;

    extractedContent = "";


    for (zip_uint64_t i = 0;
         i < static_cast<zip_uint64_t>(
                 numberOfFiles);
         i++)
    {
        const char *name =
            zip_get_name(
                archive,
                i,
                0
            );

        if (name == nullptr)
        {
            continue;
        }

        string entryName =
            name;

        bool relevant =
            false;


        // ====================================
        // DOCX
        // ====================================

        if (extension == ".docx")
        {
            if (entryName ==
                    "word/document.xml" ||
                entryName.find(
                    "word/header"
                ) != string::npos ||
                entryName.find(
                    "word/footer"
                ) != string::npos ||
                entryName.find(
                    "word/comments"
                ) != string::npos)
            {
                relevant = true;
            }
        }


        // ====================================
        // PPTX
        // ====================================

        else if (extension == ".pptx")
        {
            if (entryName.find(
                    "ppt/slides/slide"
                ) != string::npos &&
                entryName.find(
                    ".xml"
                ) != string::npos)
            {
                relevant = true;
            }

            if (entryName.find(
                    "ppt/notesSlides"
                ) != string::npos &&
                entryName.find(
                    ".xml"
                ) != string::npos)
            {
                relevant = true;
            }
        }


        // ====================================
        // XLSX
        // ====================================

        else if (extension == ".xlsx")
        {
            if (entryName ==
                    "xl/sharedStrings.xml" ||
                entryName.find(
                    "xl/worksheets/"
                ) != string::npos ||
                entryName.find(
                    "xl/comments"
                ) != string::npos)
            {
                relevant = true;
            }
        }


        if (!relevant)
        {
            continue;
        }


        zip_stat_t stat;

        if (zip_stat_index(
                archive,
                i,
                0,
                &stat) != 0)
        {
            continue;
        }


        zip_file_t *file =
            zip_fopen_index(
                archive,
                i,
                0
            );

        if (file == nullptr)
        {
            continue;
        }


        vector<char> buffer(
            static_cast<size_t>(
                stat.size
            ) + 1
        );


        zip_int64_t bytesRead =
            zip_fread(
                file,
                buffer.data(),
                stat.size
            );

        zip_fclose(file);


        if (bytesRead < 0)
        {
            continue;
        }


        buffer[
            static_cast<size_t>(
                bytesRead
            )
        ] = '\0';


        string xmlData(
            buffer.data(),
            static_cast<size_t>(
                bytesRead
            )
        );


        string text =
            extractXMLText(
                xmlData
            );


        if (!text.empty())
        {
            extractedContent +=
                "\n===== " +
                entryName +
                " =====\n";

            extractedContent +=
                text;

            extractedSomething =
                true;
        }
    }


    // ========================================
    // LIST PACKAGE FILES
    // ========================================

    extractedContent +=
        "\n===== PACKAGE FILES =====\n";


    for (zip_uint64_t i = 0;
         i < static_cast<zip_uint64_t>(
                 numberOfFiles);
         i++)
    {
        const char *name =
            zip_get_name(
                archive,
                i,
                0
            );

        if (name != nullptr)
        {
            extractedContent +=
                string(name) +
                "\n";
        }
    }


    zip_close(
        archive
    );


    if (!extractedSomething)
    {
        extractedContent +=
            "\nNo readable text was extracted.";

        return false;
    }

    return true;
}


// ========================================
// READ ZIP
// ========================================

bool AttachmentContentReader::readZIP()
{
    int error = 0;

    zip_t *archive =
        zip_open(
            filePath.c_str(),
            ZIP_RDONLY,
            &error
        );

    if (archive == nullptr)
    {
        extractedContent =
            "Unable to open ZIP archive.";

        return false;
    }


    zip_int64_t numberOfFiles =
        zip_get_num_entries(
            archive,
            0
        );


    extractedContent =
        "Files inside ZIP archive:\n\n";

    bool readTextFile =
        false;


    for (zip_uint64_t i = 0;
         i < static_cast<zip_uint64_t>(
                 numberOfFiles);
         i++)
    {
        const char *name =
            zip_get_name(
                archive,
                i,
                0
            );

        if (name == nullptr)
        {
            continue;
        }


        string entryName =
            name;

        extractedContent +=
            "- " +
            entryName +
            "\n";


        string lowerName =
            toLowerCase(
                entryName
            );


        // ====================================
        // TEXT-LIKE FILES
        // ====================================

        bool textLike =
            lowerName.find(
                ".txt"
            ) != string::npos ||

            lowerName.find(
                ".js"
            ) != string::npos ||

            lowerName.find(
                ".bat"
            ) != string::npos ||

            lowerName.find(
                ".cmd"
            ) != string::npos;


        if (!textLike)
        {
            continue;
        }


        zip_file_t *file =
            zip_fopen_index(
                archive,
                i,
                0
            );

        if (file == nullptr)
        {
            continue;
        }


        zip_stat_t stat;

        if (zip_stat_index(
                archive,
                i,
                0,
                &stat) != 0)
        {
            zip_fclose(file);

            continue;
        }


        // ====================================
        // LIMIT FILE SIZE
        // ====================================

        if (stat.size >
            5 * 1024 * 1024)
        {
            zip_fclose(file);

            extractedContent +=
                "  [File too large to inspect]\n";

            continue;
        }


        vector<char> buffer(
            static_cast<size_t>(
                stat.size
            ) + 1
        );


        zip_int64_t bytesRead =
            zip_fread(
                file,
                buffer.data(),
                stat.size
            );


        zip_fclose(file);


        if (bytesRead < 0)
        {
            continue;
        }


        buffer[
            static_cast<size_t>(
                bytesRead
            )
        ] = '\0';


        extractedContent +=
            "\n----- CONTENT: " +
            entryName +
            " -----\n";


        extractedContent +=
            string(
                buffer.data(),
                static_cast<size_t>(
                    bytesRead
                )
            );


        extractedContent +=
            "\n--------------------------------\n";


        readTextFile =
            true;
    }


    zip_close(
        archive
    );


    if (numberOfFiles == 0)
    {
        extractedContent +=
            "\nZIP archive is empty.";

        return true;
    }


    if (readTextFile)
    {
        return true;
    }


    extractedContent +=
        "\nNo directly readable text files "
        "were found inside the ZIP.";

    return true;
}


// ========================================
// READ SCRIPT FILE
// JS / BAT / CMD
// ========================================

bool AttachmentContentReader::readScriptFile()
{
    ifstream file(
        filePath
    );

    if (!file.is_open())
    {
        extractedContent =
            "Unable to open script file.";

        return false;
    }


    stringstream buffer;

    buffer << file.rdbuf();

    extractedContent =
        buffer.str();

    file.close();


    if (extractedContent.empty())
    {
        extractedContent =
            "Script file is empty.";

        return true;
    }


    return true;
}


// ========================================
// STATIC EXE ANALYSIS
// NEVER EXECUTES THE FILE
// ========================================

bool AttachmentContentReader::analyzeExecutable()
{
    ifstream file(
        filePath,
        ios::binary
    );

    if (!file.is_open())
    {
        extractedContent =
            "Unable to open executable.";

        return false;
    }


    // ========================================
    // FILE SIZE
    // ========================================

    file.seekg(
        0,
        ios::end
    );

    streamoff fileSize =
        file.tellg();

    file.seekg(
        0,
        ios::beg
    );


    extractedContent =
        "STATIC EXECUTABLE ANALYSIS\n\n";

    extractedContent +=
        "Execution: NOT PERFORMED\n";

    extractedContent +=
        "File Size: " +
        to_string(
            static_cast<long long>(
                fileSize
            )
        ) +
        " bytes\n";


    // ========================================
    // READ FIRST 16 MB
    // ========================================

    const streamsize MAX_READ =
        16 * 1024 * 1024;

    streamsize bytesToRead =
        static_cast<streamsize>(
            min<streamoff>(
                fileSize,
                MAX_READ
            )
        );


    vector<unsigned char> data(
        static_cast<size_t>(
            bytesToRead
        )
    );


    file.read(
        reinterpret_cast<char*>(
            data.data()
        ),
        bytesToRead
    );


    streamsize bytesRead =
        file.gcount();

    file.close();


    if (bytesRead < 2)
    {
        extractedContent +=
            "Unable to read enough "
            "binary data.\n";

        return false;
    }


    // ========================================
    // CHECK DOS HEADER
    // ========================================

    bool isMZ =
        data[0] == 'M' &&
        data[1] == 'Z';


    extractedContent +=
        "MZ Header: ";

    extractedContent +=
        isMZ
            ? "Detected\n"
            : "Not detected\n";


    if (!isMZ)
    {
        extractedContent +=
            "File does not appear to be a "
            "standard Windows PE executable.\n";

        return false;
    }


    // ========================================
    // CHECK PE HEADER
    // ========================================

    bool validPE =
        false;


    if (bytesRead >= 64)
    {
        uint32_t peOffset =
            static_cast<uint32_t>(
                data[0x3C]
            ) |

            (static_cast<uint32_t>(
                data[0x3D]
            ) << 8) |

            (static_cast<uint32_t>(
                data[0x3E]
            ) << 16) |

            (static_cast<uint32_t>(
                data[0x3F]
            ) << 24);


        if (peOffset + 4 <=
            static_cast<uint32_t>(
                bytesRead
            ))
        {
            if (data[peOffset] == 'P' &&
                data[peOffset + 1] == 'E' &&
                data[peOffset + 2] == 0 &&
                data[peOffset + 3] == 0)
            {
                validPE =
                    true;
            }
        }
    }


    extractedContent +=
        "PE Header: ";

    extractedContent +=
        validPE
            ? "Detected\n"
            : "Not detected\n";


    // ========================================
    // EXTRACT SUSPICIOUS READABLE STRINGS
    // ========================================

    extractedContent +=
        "\nSuspicious/Readable Strings:\n";


    string currentString;

    int suspiciousStringCount =
        0;

    const int MAX_STRINGS =
        40;


    for (streamsize i = 0;
         i < bytesRead &&
         suspiciousStringCount <
             MAX_STRINGS;
         i++)
    {
        unsigned char c =
            data[
                static_cast<size_t>(
                    i
                )
            ];


        if (c >= 32 &&
            c <= 126)
        {
            currentString +=
                static_cast<char>(c);
        }
        else
        {
            if (currentString.length() >= 5)
            {
                string lower =
                    toLowerCase(
                        currentString
                    );


                if (lower.find(
                        "powershell"
                    ) != string::npos ||

                    lower.find(
                        "cmd.exe"
                    ) != string::npos ||

                    lower.find(
                        "http://"
                    ) != string::npos ||

                    lower.find(
                        "https://"
                    ) != string::npos ||

                    lower.find(
                        "download"
                    ) != string::npos ||

                    lower.find(
                        "password"
                    ) != string::npos ||

                    lower.find(
                        "login"
                    ) != string::npos ||

                    lower.find(
                        "shell"
                    ) != string::npos ||

                    lower.find(
                        "rundll32"
                    ) != string::npos ||

                    lower.find(
                        "regsvr32"
                    ) != string::npos)
                {
                    extractedContent +=
                        "- " +
                        currentString +
                        "\n";

                    suspiciousStringCount++;
                }
            }

            currentString.clear();
        }
    }


    if (suspiciousStringCount == 0)
    {
        extractedContent +=
            "No predefined suspicious "
            "strings found.\n";
    }


    extractedContent +=
        "\nStatic analysis completed.\n";

    return true;
}