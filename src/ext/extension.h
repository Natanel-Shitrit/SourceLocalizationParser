#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "phrases_generator/phrasesgen.h"
#include <string>

#include "smsdk_ext.h"

class SourceLocalizationPhrasesGenerator :
    public CPhrasesGenerator,
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
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
