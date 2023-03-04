#include "localize.h"

LocalizationParser::LexToken LocalizationParser::LexNext()
{
    while (m_LexPos < m_Content.size())
    {
        switch (m_Content[m_LexPos])
        {
            // skip whitespace.
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                m_LexPos++; break;
            
            case '/': // skip comments.
            case '[': // For now, this will skip conditionals, until we implement them. 
                SkipLine(); break;
            
            // handle braces.
            case '{': return LexToken(LexTokenType::BraceOpen);
            case '}': return LexToken(LexTokenType::BraceClose);

            // handle values. (returns None if fails)
            case '"': return ReadValue();

            /// TODO:
            // handle conditionals.
            // case '[':
        }
    }

    return LexToken(LexTokenType::End);
}

void LocalizationParser::SkipLine()
{
    // Read until reaching a line breaker.
    while (m_LexPos < m_Content.size())
    {
        m_LexPos++;

        // When the line breaker was found, skip it and exit.
        if (m_Content[m_LexPos] == '\n')
        {
            m_LexPos++;
            break;
        }
    }
}

LocalizationParser::LexToken LocalizationParser::ReadValue()
{
    std::wstring value = L"";

    while (m_LexPos < m_Content.size())
    {
        switch (m_Content[++m_LexPos])
        {
            // Found matching quotation mark, this is the end of the value.
            case '"':
                m_LexPos++; // This skips the ending quotation mark.
                return LocalizationParser::LexToken(LexTokenType::Value, std::move(value));
            
            // Special characters, escape them.
            case '\n': value += L"\\n"; break;
            case '\t': value += L"\\t"; break;
            case '\r': value += L"\\r"; break;
            // Escape character means the next character should be treated as a literal.
            case '\\': value += L"\\";
                // After adding the backslash, skip it and fall to default to add the escaped character as is.
                m_LexPos++;
                [[fallthrough]];

            default:
                value += m_Content[m_LexPos];
                break;
        }
    }

    // If we ended up here the file ended before the value,
    // This means the file is not formatted correctly...
    return LocalizationParser::LexToken(LexTokenType::None);
}

bool LocalizationParser::Parse()
{
    int braceDepth = 0;
    bool markNextValueAsHandled = false;
    LexToken token, lastToken;

    while ((token = LexNext()).type != LexTokenType::End)
    {
        switch (token.type)
        {
            case LexTokenType::Value:
            {
                // Transform key string to lower:
                    // Valve's localization system performs a case-insensitive match for localization token lookups,
                    // so the behavior of the game and your plugin (or SourceMod's translation system) behaves differently in that way.
                StringToLower(token.value);

                switch (lastToken)
                {
                    case LexTokenType::Value:
                    {
                        if (markNextValueAsHandled)
                        {
                            markNextValueAsHandled = false;
                            break;
                        }
                            
                        // If the value is "language", this is the key that holds the file language.
                        // This should also be the first key-value pair in the localization file.
                        if (m_Language.first.empty() && lastToken.value == L"Language")
                        {
                            // Transform the language name to lowercase also,
                            // because for some reason VALVE capitalized only SOME language names. SMH...
                            m_Language.first = std::move(token.value);
                        }
                        else // This is a regular LangToken.
                        {
                            m_Language.second[lastToken.value] = std::move(token.value);
                        }

                        // Lex next token to skip the current token since it was handled here.
                        markNextValueAsHandled = true;

                        break;
                    }
                }

                break;
            }

            case LexTokenType::BraceOpen:
            {
                if (lastToken.type != LexTokenType::Value)
                {
                    std::cout << "File formatted incorrectly: Open brace should only exist after a value." << std::endl;
                    return false;
                }
                
                // Keep track on brace depth to later make sure there aren't too many closing braces.
                braceDepth++;
                break;
            } 
                
            case LexTokenType::BraceClose:
            {
                if (--braceDepth < 0)
                {
                    std::cout << "File formatted incorrectly: Brace depth underflow." << std::endl;
                    return false;
                }

                break;
            }

            case LexTokenType::None:
            {
                std::cout << "File formatted incorrectly: Unknown error." << std::endl;
                return false;
            }
        }

        lastToken = std::move(token);
    }
}

void LocalizationParser::ParseFile(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    
    // Throws an exception if the file doesn't exist. 
    file.exceptions(ifstream::badbit);

    auto size = std::filesystem::file_size(filePath);

    std::wstring fileContent;
    fileContent.resize(size / 2);

    file.read((char *)fileContent.data(), size);
    file.close();

    // Remove the BOM.
    fileContent.erase(0, 1);

    Parse(fileContent);
}

void LocalizationParser::ParseGameLocalizationFile(std::string_view language)
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
