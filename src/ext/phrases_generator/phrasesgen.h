#ifndef _INCLUDE_PHRASESGEN_H_
#define _INCLUDE_PHRASESGEN_H_

#include "localization/localize.h"
#include "smsdk_ext.h"

#include <filesystem>
#include <iostream>

#ifdef _WIN32
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
    std::map<std::string, std::string> m_LanguageCodeNames;

    // Whitelist
    std::vector<std::string> m_LanguageWhitelist;

public: // Public methods
    CPhrasesGenerator() {}

    void Generate();
    void SDK_OnUnload();
    
private: // Private methods
    void LoadWhitelist();
    void LoadLanguages();
    bool IsLanguageWhitelisted(std::string language);
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

#endif // _INCLUDE_PHRASESGEN_H_
