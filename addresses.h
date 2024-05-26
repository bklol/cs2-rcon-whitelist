#pragma once
#include "platform.h"
#include "stdint.h"
#include "utils/module.h"

namespace modules
{
	inline CModule *engine;
}

namespace addresses
{
	void Initialize();
}

namespace sigs
{
#ifdef _WIN32
	inline const byte *On_Rcon_Message = (byte *)"\x4C\x8B\xDC\x49\x89\x53\x2A\x55";
#else
	inline const byte *On_Rcon_Message = (byte *)"\x55\x48\x89\xE5\x41\x57\x49\x89\xFF\x41\x56\x45\x89\xC6\x41\x55\x49\x89\xCD";
#endif
}