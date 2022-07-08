#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief Contains macros for configuring basic extension information.
 */

/* Basic information exposed publicly */
#define SMEXT_CONF_NAME			"SourceLocalizationParser"
#define SMEXT_CONF_DESCRIPTION	"Generates SourceMod translations phrases files from Source-Engine localization files."
#define SMEXT_CONF_VERSION		"2.0.0.0"
#define SMEXT_CONF_AUTHOR		"LuqS"
#define SMEXT_CONF_URL			"https://github.com/Natanel-Shitrit/"
#define SMEXT_CONF_LOGTAG		"SLP"
#define SMEXT_CONF_LICENSE		"GPL"
#define SMEXT_CONF_DATESTRING	__DATE__

/** 
 * @brief Exposes plugin's main interface.
 */
#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

/**
 * @brief Sets whether or not this plugin required Metamod.
 * NOTE: Uncomment to enable, comment to disable.
 */
#define SMEXT_CONF_METAMOD		

/** Enable interfaces you want to use here by uncommenting lines */
#define SMEXT_ENABLE_THREADER
#define SMEXT_ENABLE_TRANSLATOR

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
