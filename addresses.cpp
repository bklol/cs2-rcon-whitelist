#include "addresses.h"
#include "utils/module.h"

#include "tier0/memdbgon.h"
#define RESOLVE_SIG(module, sig, variable) variable = (decltype(variable))module->FindSignature((uint8*)sig)

void addresses::Initialize()
{
	modules::engine = new CModule(ROOTBIN, "engine2");
}