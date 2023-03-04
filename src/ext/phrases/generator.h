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

#endif // _INCLUDE_PHRASESGEN_H_
