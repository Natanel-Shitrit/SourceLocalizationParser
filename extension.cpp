#include "extension.h"

LanguagePhrasesParser g_Extension;	
SMEXT_LINK(&g_Extension);

IFileSystem* g_pFileSystem = NULL;
ILanguageFileParser* g_pLanguageFileParser = NULL;

bool LanguagePhrasesParser::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);
	return true;
}

void LanguagePhrasesParser::NotifyInterfaceDrop(SMInterface* pInterface)
{
	if (g_pLanguageFileParser == pInterface)
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
	SM_GET_LATE_IFACE(ILANGPARSER, g_pLanguageFileParser);
	g_Extension.LoadWhitelist();
	g_Extension.Generate();
}