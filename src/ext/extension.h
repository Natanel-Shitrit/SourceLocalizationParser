#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <filesystem>
#include <iostream>

#include "localization/parser.h"
#include "phrases/generator.h"

#include "smsdk_ext.h"


class SourceLocalizationParser:
    public SDKExtension
{
public: // SDKExtension
    /**
     * @brief This is called right before the extension is unloaded.
     */
    virtual void SDK_OnUnload();

    /**
     * @brief This is called once all known extensions have been loaded.
     * Note: It is is a good idea to add natives here, if any are provided.
     */
    virtual void SDK_OnAllLoaded();

private:
    void LoadLanguages();
    void ProcessLanguages();
    void ProcessLanguage(std::string language);

private:
    std::vector<std::string> m_Languages;
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
