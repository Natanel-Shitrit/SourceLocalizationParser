#include "localize.h"

CLocalize::LexToken CLocalize::LexNext()
{
    std::wstring value = L"";

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
                            // Adds '\' and then falls to default to add the escaped character.
                            value += m_Content[m_LexPos++];
                            [[fallthrough]];
                        
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
                    // Transform key string to lower:
                        // Valve's localization system performs a case-insensitive match for localization token lookups,
                        // so the behavior of the game and your plugin (or SourceMod's translation system) behaves differently in that way.
                    StringToLower(token.value);

                    // If the value is "language", this is the key that holds the file language.
                    if (token.value == L"language")
                    {
                        // Transform the language name to lowercase also,
                        // because for some reason VALVE capitalized only SOME language names. SMH...
                        StringToLower(nextToken.value);
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

void CLocalize::ParseFile(std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    // Check if file is open
    if (!file)
    {
        std::cout << "Unable to open file (" << filePath << ")" << std::endl;
        return;
    }

    // Get file size
    auto size = std::filesystem::file_size(filePath);

    // Allocate buffer
    std::wstring fileContent;
    fileContent.resize(size / 2);

    // Read file
    file.read((char *)fileContent.data(), size);

    // Close file
    file.close();

    // Remove the BOM.
    fileContent.erase(0, 1);

    // Parse the content.
    Parse(fileContent);
}

void CLocalize::ParseGameLocalizationFile(std::string_view language)
{
    // Get game folder name and build game resource folder base path.
    // Make the variables static because it's constant and will never change.
    static std::string_view gameFolderName = g_pSM->GetGameFolderName();
    static std::filesystem::path gameResourceFolderBasePath = std::filesystem::current_path() / gameFolderName / "resource";
    
    // Build the localization file name.
    std::ostringstream fileName;
    fileName << gameFolderName << "_" << language << ".txt";

    // Parse localization file.
    ParseFile((gameResourceFolderBasePath / fileName.str()));
}

void CLocalize::ParseGameLocalizationFiles(std::vector<std::string>& languages)
{
    for (auto& language : languages)
    {
        ParseGameLocalizationFile(language);
    }
}