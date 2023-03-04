#include "phrasesgen.h"

/*
void PhrasesFileGenerator::RunThread(IThreadHandle* pHandle)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    LoadLanguages();

    // Get only languages names from m_LanguageCodeNames.
    std::vector<std::string> languageNames;
    std::transform(
        m_LanguageCodeNames.begin(),
        m_LanguageCodeNames.end(),
        std::back_inserter(languageNames),
        [](const auto& pair) { return StringToUTF8(pair.first); }
    );

    std::cout << "Parsing " << languageNames.size() << " Languages" << std::endl;

    ParseGameLocalizationFiles(languageNames);
    GeneratePhrasesFromParsedFiles();

    // Notify about generation end.
    std::cout
        << "Generated translations in \"sourcemod/translations/\" (in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count()
        << "ms)" << std::endl;
}
*/

void PhrasesFileGenerator::Generate()
{
    
}

// Other
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