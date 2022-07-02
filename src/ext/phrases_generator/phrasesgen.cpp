#include "phrasesgen.h"

static std::string U16StringToUTF8(const std::u16string u16)
{
    return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);
}

// IThread implementation
void CPhrasesGenerator::RunThread(IThreadHandle* pHandle)
{
    // Save thread start time.
    auto startTime = std::chrono::high_resolution_clock::now();

    LoadLanguages();

    std::vector<std::string> languageNames;

    // Get only languages names from m_LanguageCodeNames.
    std::transform(
        m_LanguageCodeNames.begin(),
        m_LanguageCodeNames.end(),
        std::back_inserter(languageNames),
        [](const auto& pair) { return pair.first; }
    );

    std::cout << "Parsing " << languageNames.size() << " Languages" << std::endl;

    // Parse all languages. [DONE]
    ParseGameLocalizationFiles(languageNames);

    // Generate phrases files. [WIP]
    GeneratePhrasesFromParsedFiles();

    // Notify about generation end.
    std::cout
        << "Generated translations in \"sourcemod/translations/\" (in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count()
        << "ms)" << std::endl;
}

void CPhrasesGenerator::OnTerminate(IThreadHandle* pHandle, bool cancel)
{ 
    // Thread is no longer valid.
    m_pThread = nullptr;
}

// Private methods implementation
void CPhrasesGenerator::LoadWhitelist()
{
    std::string whitelistPath;
    whitelistPath.resize(PLATFORM_MAX_PATH);

    smutils->BuildPath(
        Path_SM,
        whitelistPath.data(),
        PLATFORM_MAX_PATH,
        "configs/slp_whitelist.txt"
    );

    std::cout << whitelistPath << std::endl;

    std::ifstream whitelist(whitelistPath);

    // Create whitelist file (and skip whitelist parsing) if not present.
    if (!whitelist)
    {
        std::ofstream { whitelistPath };
        return;
    }

    // Read file.
    std::string line;
    while (std::getline(whitelist, line))
    {
        // Skip empty lines and comments (# or //).
        if (line.empty() || line.front() == '#' || line.front() == '/')
            continue;

        // Add language to whitelist.
        m_LanguageWhitelist.push_back(std::move(line));
    }

    // Close file.
    whitelist.close();
}

void CPhrasesGenerator::LoadLanguages()
{
    LoadWhitelist();

    const char *languageCode, *language;
    auto count = translator->GetLanguageCount();

    // Iterate through all languages.
    for (auto currentLanguage = 0; currentLanguage < count; currentLanguage++)
    {
        // Get language code and name.
        if (!translator->GetLanguageInfo(currentLanguage, &languageCode, &language))
        {
            std::cout << "Couldn't 'GetLanguageInfo' for index " << currentLanguage << "." << std::endl;
            continue;
        }

        std::cout << "languageCode: " << languageCode << ", language: " << language << std::endl;

        // Check if language is whitelisted.
        if (!IsLanguageWhitelisted(std::string(language)))
        {
            std::cout << "Language \"" << language << "\" is not whitelisted" << std::endl;
            continue;
        }

        // Add language to whitelist.
        m_LanguageCodeNames[std::string(language)] = std::string(languageCode);
    }
}

bool CPhrasesGenerator::IsLanguageWhitelisted(std::string language)
{
    return \
        // If whitelist is empty, all languages are whitelisted
        m_LanguageWhitelist.empty() ||
        // Check if language is in whitelist.
        std::find(
            m_LanguageWhitelist.begin(),
            m_LanguageWhitelist.end(),
            language
        ) != std::end(m_LanguageWhitelist) ||
        // If it's english, it's always whitelisted. (main phrases file should always exits to load it to SM)
        language == "english";
}

void CPhrasesGenerator::GeneratePhrasesFromParsedFiles()
{
    // Create phrases file path.
    std::string translationsBasePath;
    translationsBasePath.resize(PLATFORM_MAX_PATH);

    smutils->BuildPath(
        Path_SM,
        translationsBasePath.data(),
        PLATFORM_MAX_PATH,
        "translations"
    );

    translationsBasePath.resize(translationsBasePath.find('\0'));

    for (auto const& [language, langTokens] : m_Languages)
    {
        std::string languageCode = m_LanguageCodeNames[U16StringToUTF8(language)];

        // Create phrases file path.
        std::ostringstream phrasesFilePath;

        phrasesFilePath << translationsBasePath;

        if (language != u"English")
        {
            if (languageCode.empty())
            {
                std::cout << "Language \"" << U16StringToUTF8(language) << "\" has no code" << std::endl;
                continue;
            }

            phrasesFilePath << "/" + languageCode;
        }

        phrasesFilePath << "/" << m_GameFolderName << ".phrases.txt";

        // Create phrases file.
        std::ofstream phrasesFile(phrasesFilePath.str());

        if (phrasesFile.fail())
        {
            std::cout << "Failed to open \"" << phrasesFilePath.str() << std::endl;
            continue;
        }

        // Write phrases file header.
        phrasesFile << "// " << U16StringToUTF8(language) << " phrases file" << "\n"
                    << "// Generated by 'Source Localization Parser'" << "\n"
                    << "\n";

        phrasesFile << "\"Phrases\"" << "\n"
                    << "{" << "\n";

        std::cout << "Writing " << langTokens.size() << " tokens!" << "\n";

        // Write phrases file content.
        for (auto const& [key, value] : langTokens)
        {
            phrasesFile << "\t" << "\"" << U16StringToUTF8(key) << "\"" << "\n"
                        << "\t" << "{" << "\n"
                        << "\t" << "\t" << "\"" << languageCode << "\"" << "\t" << "\t" << "\"" << U16StringToUTF8(value) << "\"" << "\n"
                        << "\t" << "}" << "\n"
                        << "\n";
        }

        // Write phrases file footer.
        phrasesFile << "}" << "\n";

        // Close file.
        phrasesFile.close();

        // Notify about phrases file generation.
        std::cout << "Generated phrases file \"" << phrasesFilePath.str() << "\"" << std::endl;
    }
}

// Public functions implementation
void CPhrasesGenerator::Generate()
{
    if (m_pThread != nullptr)
    {
        std::cout << "Operation in progress\n" << std::endl;
        return;
    }

    m_pThread = threader->MakeThread(this, NULL);
}

void CPhrasesGenerator::SDK_OnUnload()
{
    // check if thread is running.
    if (m_pThread != NULL)
    {
        // Wait for thread to terminate.
        m_pThread->WaitForThread();
    }
}