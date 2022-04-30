#ifndef _INCLUDE_LOCALIZE_H_
#define _INCLUDE_LOCALIZE_H_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

class CLocalize
{
public:
    // LangToken = { Key, Value }
    typedef std::pair<std::u16string, std::u16string> LangToken;
    typedef std::map<std::u16string, std::u16string> LangTokens;

    // Language = { Name, Tokens }
    typedef std::pair<std::u16string, CLocalize::LangTokens> Language;
    typedef std::map<std::u16string, CLocalize::LangTokens> Languages;

private:
    enum class LexTokenType
    {
        None,
        Value,
        // Condition, 
        Comment,
        BraceOpen,
        BraceClose,
        End

        /// TODO: Implement conditionals.
    };

    struct LexToken
    {
        LexTokenType type;
        std::u16string value;

        LexToken(LexTokenType type = LexTokenType::None, std::u16string value = u"")
            : type(type), value(std::move(value))
        {}
    };

    LexToken LexNext();
    void Parse();
    void Clear()
    {
        m_Content = {};
        m_CurrentLanguage = {};
        m_LexPos = 0;
    }

public:
    CLocalize() {}
    ~CLocalize() {}

    void Parse(std::u16string_view content)
    {
        // Save content.
        m_Content = std::move(content);
        Parse();
        Clear();
    }

    void ParseFile(std::filesystem::path filePath)
    {
        std::ifstream file(filePath, std::ios::binary);

        // Check if file is open
        if (!file)
        {
            std::cout << "Unable to open file" << std::endl;
            return;
        }

        // Get file size
        auto size = std::filesystem::file_size(filePath);

        // Allocate buffer
        std::u16string fileContent;
        fileContent.resize(size / 2);

        // Read file
        file.read((char*)&fileContent[0], size);

        // Close file
        file.close();

        // Remove the BOM.
        fileContent.erase(0, 1);

        // Parse the content.
        Parse(std::move(fileContent));
    }

    void ParseGameLocalizationFile()
    {
        
    }

private:
    std::u16string_view m_Content { u"" };
    std::u16string m_CurrentLanguage { u"" };
    size_t m_LexPos { 0 };

public:
    Languages m_Languages;
};

#endif // _INCLUDE_LOCALIZE_H_
