#include "rcon_whitelist.h"
#include "iserver.h"

#include "appframework/IAppSystem.h"
#include "common.h"
#include "interface.h"
#include "tier0/dbg.h"
#include "plat.h"
#include "tier0/memdbgon.h"
#include "detours.h"
#include "addresses.h"

void Message(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	char buf[1024] = {};
	V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

	ConColorMsg(Color(255, 0, 255, 255), "[NEKO] %s", buf);

	va_end(args);
}

void Panic(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	char buf[1024] = {};
	V_vsnprintf(buf, sizeof(buf) - 1, msg, args);

	if (CommandLine()->HasParm("-dedicated"))
		Warning("[NEKO] %s", buf);
#ifdef _WIN32
	else
		MessageBoxA(nullptr, buf, "Warning", 0);
#endif

	va_end(args);
}

rcon_whitelist g_rcon_whilelist;
PLUGIN_EXPOSE(rcon_whitelist, g_rcon_whilelist);

bool rcon_whitelist::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	META_CONPRINTF( "[NEKO RCON]Starting plugin.\n" );
	addresses::Initialize();
	InitDetours();
	return true;
}

bool rcon_whitelist::Unload(char *error, size_t maxlen)
{
	FlushAllDetours();
	return true;
}

void rcon_whitelist::AllPluginsLoaded()
{


}

bool rcon_whitelist::Pause(char *error, size_t maxlen)
{
	return true;
}

bool rcon_whitelist::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *rcon_whitelist::GetLicense()
{
	return "";
}

const char *rcon_whitelist::GetVersion()
{
	return "0.1";
}

const char *rcon_whitelist::GetDate()
{
	return __DATE__;
}

const char *rcon_whitelist::GetLogTag()
{
	return "rcon_whitelist";
}

const char *rcon_whitelist::GetAuthor()
{
	return "bklol";
}

const char *rcon_whitelist::GetDescription()
{
	return "neko rcon_whitelist";
}

const char *rcon_whitelist::GetName()
{
	return "neko rcon_whitelist";
}

const char *rcon_whitelist::GetURL()
{
	return "https://github.com/bklol";
}
