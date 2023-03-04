#include "extension.h"

SourceLocalizationParser g_Extension;	
SMEXT_LINK(&g_Extension);


void SourceLocalizationParser::SDK_OnAllLoaded()
{
    this->s_GameName = g_pSM->GetGameFolderName();
    this->s_GameResourceFolderPath = \
        std::filesystem::current_path() / gameFolderName / "resource";
    
    this->LoadLanguages();
    this->ProcessLanguages();
}

/*
    Process all languages.
*/
void SourceLocalizationParser::ProcessLanguages()
{
    std::vector<std::thread> threads;

    // Run 'ProcessLanguage' on all languages.
    std::transform(
        this->m_Languages.begin(),
        this->m_Languages.end(),
        std::back_inserter(threads),
        [](std::string language){ return std::thread(this->ProcessLanguage, language) }
    )

    // Wait for all threads to finish.
    std::for_each(threads.begin(), threads.end(), std::thread::join);
}

/*
    Process a single language.
    Parse the localization file with 'LocalizationParser'.
    Generate SM phrases from the parsed localizations.
*/
void SourceLocalizationParser::ProcessLanguage(std::string& language)
{
    LocalizationParser parser;

    // Try to parse the game localization file for the language.
    try {
        parser.ParseGameLocalizationFile(
            this->GetLanguageLocalizationFilePath(language)
        );
    }
    // If the file doesn't exist.
    catch (const ifstream::failure& e) {
        std::cout << "Couldn't find localization file for '" << language << "'.";
        return;
    }
    /// TODO: Any other exceptions?

    PhrasesFileGenerator generator;

    PhrasesFileGenerator.
}

void SourceLocalizationParser::LoadLanguages()
{
    std::filesystem::path languagesPath = smutils->GetSourceModPath();
    whitelistPath /= "configs" / "slp_whitelist.txt";

    std::ifstream whitelist(whitelistPath);
    if (!whitelist)
    {
        // If the whitelist file doesn't exist, create it.
        std::ofstream { whitelistPath };
        return;
    }

    std::string line;
    while (std::getline(whitelist, line))
    {
        // Skip empty lines and comments (# or //).
        if (line.empty() || line.front() == '#' || line.front() == '/')
            continue;
        
        // Adds the language to the whitelist class member.
        this->m_Languages.push_back(std::move(line));
    }
    
    whitelist.close();
}

/*
    Threading Stuff
*/

/*
void PhrasesFileGenerator::LoadLanguages()
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
        if (!IsLanguageWhitelisted(std::string_view(language)))
        {
            std::cout << "Language \"" << language << "\" is not whitelisted" << std::endl;
            continue;
        }

        // Add language to whitelist.
        m_LanguageCodeNames[StringToWide(std::string(language))] = StringToWide(std::string(languageCode));
    }
}

bool PhrasesFileGenerator::IsLanguageWhitelisted(std::string_view language)
{
    return \
        // If whitelist is empty, all languages are whitelisted.
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

void PhrasesFileGenerator::GeneratePhrasesFromParsedFiles()
{
    static std::filesystem::path translationsBasePath = smutils->GetSourceModPath();
    translationsBasePath /= "translations";

    for (auto const& [language, langTokens] : m_Languages)
    {
        std::wstring& languageCode = m_LanguageCodeNames[language];

        if (languageCode.empty())
        {
            // If the language code is empty, it means the language is not whitelisted.
            std::wcout << L"Skipping \"" << language << L"\"" << std::endl;
            continue;
        }
        
        std::ofstream phrasesFile(
            translationsBasePath / 
                StringToUTF8(language != L"english" ? languageCode : L"") /
                    "localization.phrases.txt"
        );

        if (phrasesFile.fail())
        {
            std::wcout << L"Failed to open " << language << L" phrases file." << std::endl;
            continue;
        }

        // Write phrases output stream.
        std::wstringstream output;
        output << L"// " << language << L" phrases file" L"\n"
                  L"// Generated by 'Source Localization Parser'" L"\n"
                  L"\n"
                  L"\"" L"Phrases" L"\"" L"\n"
                  L"{" L"\n";

        // Write phrases file content.
        for (auto const& [key, value] : langTokens)
        {
            output << L"\t" L"\"" << key << L"\"" L"\n"
                      L"\t" L"{" L"\n"
                      L"\t" L"\t" L"\"" << languageCode << L"\"" L"\t" L"\t" L"\"" << value << L"\"" L"\n"
                      L"\t" L"}" L"\n"
                      L"\n";
        }

        // Write phrases file footer.
        output << L"}" L"\n";

        phrasesFile << StringToUTF8(output.str());

        // Notify about phrases file generation.
        std::wcout << L"Generated " << langTokens.size() << L" " << language << L" phrases." << std::endl;

        phrasesFile.close();
    }
}
*/