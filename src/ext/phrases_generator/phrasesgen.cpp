#include "phrasesgen.h"

// IThread implementation
void CPhrasesGenerator::RunThread(IThreadHandle* pHandle)
{
    // Save thread start time.
    auto startTime = std::chrono::high_resolution_clock::now();

    /// TODO:
    // Test if this compiles and runs as expected.
    std::vector<std::string> languageNames;

    // Get only languages names from m_Languages.
    std::transform(
        m_Languages.begin(),
        m_Languages.end(),
        std::back_inserter(languageNames),
        [](auto const& pair){ return pair.second; }
    );

    // Parse all languages. [DONE]
    m_Localizer.ParseGameLocalizationFiles(languageNames);

    // Generate phrases files. [WIP]
    m_Localizer.GeneratePhrasesFromParsedFiles();

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
    char whitelistPath[PLATFORM_MAX_PATH];
    smutils->BuildPath(Path_SM, whitelistPath, sizeof(whitelistPath), WHITELIST_FILE_PATH);

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

        // Check if language is whitelisted.
        if (!IsLanguageWhitelisted(std::string(language)))
        {
            std::cout << "Language \"" << language << "\" is not whitelisted" << std::endl;
            continue;
        }

        // Add language to whitelist.
        m_Languages.emplace_back(std::string(languageCode), std::string(language));
    }
}

bool CPhrasesGenerator::IsLanguageWhitelisted(std::string language)
{
    return \
        // Check if language is in whitelist (if whitelist is empty, all languages are whitelisted).
        std::find(
            m_LanguageWhitelist.begin(),
            m_LanguageWhitelist.end(),
            language
        ) != std::end(m_LanguageWhitelist)
        // If it's english, it's always whitelisted. (main phrases file should always exits to load it to SM)
        || language == "english";
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