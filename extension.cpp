#include "extension.h"

LanguagePhrasesParser g_Extension;	
SMEXT_LINK(&g_Extension);

ILanguageFileParser* g_pLanguageFileParser = NULL;
const char* g_pszGameFolderName = NULL;

void LanguagePhrasesParser::NotifyInterfaceDrop(SMInterface* interface)
{
	if (g_pLanguageFileParser == interface)
	{
		CPhrasesGenerator::SDK_OnUnload();
		g_pLanguageFileParser = NULL;
	}
}

void LanguagePhrasesParser::SDK_OnUnload()
{
	CPhrasesGenerator::SDK_OnUnload();
}

void LanguagePhrasesParser::SDK_OnAllLoaded()
{
	// Get language parser interface.
	SM_GET_LATE_IFACE(ILANGPARSER, g_pLanguageFileParser);

	// Save game folder name.
	g_pszGameFolderName = g_pSM->GetGameFolderName();

	// Load whitelist.
	g_Extension.LoadWhitelist();

	// Generate phrases files.
	g_Extension.Generate();
}