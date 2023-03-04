#ifndef _INCLUDE_LOCALIZE_H_
#define _INCLUDE_LOCALIZE_H_

#include "smsdk_ext.h"
#include "localization/lex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>


// { Key, Value }
using LangToken = std::pair<std::wstring, std::wstring>;
// { LangToken, LangToken, ... }
using LangTokens = std::map<std::wstring, std::wstring>;
// { Language, LangTokens }
using Language = std::pair<std::wstring, LangTokens>;
// { Language, Language, ... }
using Languages = std::map<std::wstring, LangTokens>;

// TODO: Change to template?
class LocalizationParser
{
private:
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
