#ifndef _INCLUDE_LOCALIZE_H_
#define _INCLUDE_LOCALIZE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>

#include "smsdk_ext.h"

// { Key, Value }
typedef std::pair<std::wstring, std::wstring> LangToken;
// { LangToken, LangToken, ... }
typedef std::map<std::wstring, std::wstring> LangTokens;
// { Language, LangTokens }
typedef std::pair<std::wstring, LangTokens> Language;
// { Language, Language, ... }
typedef std::map<std::wstring, LangTokens> Languages;

// TODO: Change to template?
class LocalizationParser
{
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
        std::wstring value;

        LexToken(LexTokenType type = LexTokenType::None, std::wstring value = L"")
            : type(type), value(std::move(value))
        {}
    };
    
    LexToken LexNext();
    void SkipLine();
    LexToken ReadValue();
    void Parse();

public:
    void ParseGameLocalizationFile();
    void ParseFile(const std::filesystem::path& filePath);
    void ParseString(std::wstring_view content);

private:
    std::wstring_view m_Content {};
    size_t m_LexPos { 0 };

public:
    Language m_Language;
};

#endif // _INCLUDE_LOCALIZE_H_
