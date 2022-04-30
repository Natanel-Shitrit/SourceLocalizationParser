#include "extension.h"

SourceLocalizationPhrasesGenerator g_Extension;	
SMEXT_LINK(&g_Extension);

void SourceLocalizationPhrasesGenerator::SDK_OnAllLoaded()
{
    // Generate phrases files.
    g_Extension.Generate();
}

void SourceLocalizationPhrasesGenerator::SDK_OnUnload()
{
    // check for running thread.
    CPhrasesGenerator::SDK_OnUnload();
}