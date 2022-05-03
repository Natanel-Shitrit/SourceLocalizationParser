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
    bool IsLanguageWhitelisted(std::string language);

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
