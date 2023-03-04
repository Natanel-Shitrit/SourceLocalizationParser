#ifndef _INCLUDE_PHRASESGEN_H_
#define _INCLUDE_PHRASESGEN_H_

#include "localization/localize.h"
#include "smsdk_ext.h"

#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <codecvt>
#include <sstream>
#endif

class PhrasesFileGenerator
{
private:

public:
    enum class Result
    {
        Success = 0,

    };

    PhrasesFileGenerator() {}
    void Generate();
};

template <typename T>
static std::string StringToUTF8(const std::basic_string<T>& str)
{
    static std::wstring_convert<std::codecvt_utf8<T>, T> convertor;
    return convertor.to_bytes(str);
}

template <typename T>
static std::wstring StringToWide(const std::basic_string<T>& str)
{
    return std::wstring(str.begin(), str.end());
}

#endif // _INCLUDE_PHRASESGEN_H_
