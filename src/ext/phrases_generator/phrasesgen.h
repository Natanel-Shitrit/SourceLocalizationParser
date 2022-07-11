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

class CPhrasesGenerator :
    public CLocalize,
    public IThread
{
private:
    // Threading
    IThreadHandle* m_pThread { nullptr };

    // Languages
    std::map<std::wstring, std::wstring> m_LanguageCodeNames;

    // Whitelist
    std::vector<std::string> m_LanguageWhitelist;

public: // Public methods
    CPhrasesGenerator() {}

    void Generate();
    void SDK_OnUnload();
    
private: // Private methods
    void LoadWhitelist();
    void LoadLanguages();
    bool IsLanguageWhitelisted(std::string_view language);
    void GeneratePhrasesFromParsedFiles();

public: // IThread
    /**
     * @brief Called when the thread runs (in its own thread).
     *
     * @param pHandle		Pointer to the thread's handle.
     */
    void RunThread(IThreadHandle* pHandle) override;

    /**
     * @brief Called when the thread terminates.  This occurs inside the thread as well.
     *
     * @param pHandle		Pointer to the thread's handle.
     * @param cancel		True if the thread did not finish, false otherwise.
     */
    void OnTerminate(IThreadHandle* pHandle, bool cancel) override;
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
