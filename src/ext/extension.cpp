#include "extension.h"

SourceLocalizationParser g_Extension;	
SMEXT_LINK(&g_Extension);

void SourceLocalizationParser::SDK_OnAllLoaded()
{
    // Generate phrases files.
    g_Extension.Generate();
}

void SourceLocalizationParser::SDK_OnUnload()
{
    // check for running thread.
    CPhrasesGenerator::SDK_OnUnload();
}