#include "phrasesgen.h"

// IThread implementation
void CPhrasesGenerator::RunThread(IThreadHandle* pHandle)
{
    // Save thread start time.
    auto startTime = std::chrono::high_resolution_clock::now();

    /// TODO:
    // Parse all languages.
    // Generate phrases files.

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
        // We can't quit yet - notify a thread about unloading.
        m_bTerminateRequested = true;

        // Wait for thread to terminate.
        m_pThread->WaitForThread();

        // reset the terminate flag.
        m_bTerminateRequested = false;
    }
}