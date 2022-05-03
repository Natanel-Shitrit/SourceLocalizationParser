#include "localize.h"

CLocalize::LexToken CLocalize::LexNext()
{
    std::u16string value = u"";

    while (m_LexPos < m_Content.size())
    {
        switch (m_Content[m_LexPos])
        {
            // skip whitespace.
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                m_LexPos++;
                break;
            
            // skip comments.
            case '/':
            case '[': // For now, this will skip conditionals, until we implement them.
                while (m_LexPos < m_Content.size() && m_Content[m_LexPos] != '\n')
                {
                    m_LexPos++;
                }
                break;
            
            // handle braces.
            case '{':
            case '}':
                return CLocalize::LexToken(m_Content[m_LexPos++] == '{' ? LexTokenType::BraceOpen : LexTokenType::BraceClose);

            // handle values.
            case '"':
                m_LexPos++;
                while (m_LexPos < m_Content.size())
                {
                    switch (m_Content[m_LexPos])
                    {
                        case '"':
                            m_LexPos++;
                            return CLocalize::LexToken(LexTokenType::Value, std::move(value));
                        
                        case '\\':
                            m_LexPos++;
                            if (m_LexPos < m_Content.size())
                            {
                                switch (m_Content[m_LexPos])
                                {
                                    case 'n': value += '\n'; break;
                                    case 't': value += '\t'; break;
                                    case 'r': value += '\r'; break;
                                    default:
                                        value += '\\';
                                        [[fallthrough]];
                                    case '"':
                                    case '\'':
                                    case '\\':
                                        value += m_Content[m_LexPos];
                                        break;
                                }
                            }
                            break;
                        
                        default:
                            value += m_Content[m_LexPos];
                            break;
                    }
                    m_LexPos++;
                }
                break;

            /// TODO:
            // handle conditionals.
        }
    }

    return LexToken(LexTokenType::End);
}

void CLocalize::Parse()
{
    int braceDepth = 0;
    LexToken token, nextToken;

    while (token.type != LexTokenType::End)
    {
        switch ((token = LexNext()).type)
        {
            case LexTokenType::Value:
                // If next token is also a value, it's a LangToken or the file language.
                if ((nextToken = LexNext()).type == LexTokenType::Value)
                {
                    // If the value is "Language", this is the key that holds the file language.
                    if (token.value == u"Language")
                    {
                        m_CurrentLanguage = std::move(nextToken.value);
                        break;
                    }

                    // Otherwise, It's just a normal LangToken.
                    m_Languages[m_CurrentLanguage][token.value] = std::move(nextToken.value);
                    break;
                }

                // If the next token is not a value, it has to be 'LexTokenType::BraceOpen'.
                braceDepth++;
                break;
                
            case LexTokenType::BraceClose:
                if (--braceDepth < 0)
                {
                    // WTF?
                    std::cout << "Error: braceDepth went below 0." << std::endl;
                }
                break;
        }
    }
}

void CLocalize::ParseFile(std::filesystem::path filePath)
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
    file.read(fileContent.data(), size);

    // Close file
    file.close();

    // Remove the BOM.
    fileContent.erase(0, 1);

    // Parse the content.
    Parse(std::move(fileContent));
}

void CLocalize::ParseGameLocalizationFile(std::string language)
{
    if (m_GameFolderName.empty())
    {
        m_GameFolderName = std::string(g_pSM->GetGameFolderName());
    }

    std::string localizationFilePath;
    localizationFilePath.resize(PLATFORM_MAX_PATH);
    
    // Build path to localization file.
    g_pSM->BuildPath(
        Path_Game,
        localizationFilePath.data(),
        PLATFORM_MAX_PATH,
        "resource/%s_%s.txt",
        m_GameFolderName.c_str(),
        language.c_str()
    );

    // Parse localization file.
    ParseFile(localizationFilePath);
}

void CLocalize::ParseGameLocalizationFiles(std::vector<std::string> languages)
{
    for (auto& language : languages)
    {
        ParseGameLocalizationFile(language);
    }
}

/*
void CLocalize::GeneratePhrasesFromParsedFiles()
{
    for (auto const& [language, langTokens] : m_Languages)
    {
        /// TODO: Implement generation.
    }
}
*/