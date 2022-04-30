#ifndef _INCLUDE_PHRASESGEN_H_
#define _INCLUDE_PHRASESGEN_H_

#include "localization/localize.h"
#include "smsdk_ext.h"

#include <filesystem>
#include <iostream>

#ifdef POSIX
#include <atomic>
#endif

#define WHITELIST_FILE_PATH "configs/slpg_whitelist.txt"

class CPhrasesGenerator :
    public IThread
{
private:
    // Threading
    IThreadHandle* m_pThread { nullptr };
    std::atomic<bool> m_bTerminateRequested { false };

    // Languages
    std::vector<std::pair<std::string, std::string>> m_Languages;

    // Whitelist
    std::vector<std::string> m_LanguageWhitelist;

    // Localizer
    CLocalize m_Localizer;

public: // Public methods
    CPhrasesGenerator()
    {
        // Load whitelist and languages.
        LoadWhitelist();
        LoadLanguages();
    }

    void Generate();
    void SDK_OnUnload();
    
private: // Private methods
    void LoadWhitelist();
    void LoadLanguages();
    bool IsLanguageWhitelisted(std::string language)
    {
        return \
            // Check if language is in whitelist (if whitelist is empty, all languages are whitelisted).
            std::find(m_LanguageWhitelist.begin(), m_LanguageWhitelist.end(), language) != std::end(m_LanguageWhitelist)
            // If it's english, it's always whitelisted. (main phrases file should always exits to load it to SM)
            || language == "english";
    }

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

#endif // _INCLUDE_PHRASESGEN_H_
