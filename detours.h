#pragma once
#include "cdetour.h"
#include "module.h"
#include "addresses.h"
#include "tier1/netadr.h"
#include "tier1/utlbuffer.h"
#include "tier0/memdbgon.h"

#define SERVERDATA_AUTH 3
#define SERVERDATA_AUTH_RESPONSE 2
#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_RESPONSE_VALUE 2

struct RCONPacket 
{
	int32_t ID;
	int32_t type;
	char body[1];
	char pad0[0x2];
	
	const char* ReadBody(uint32_t dataSize) 
	{
		if (this == nullptr || dataSize < sizeof(RCONPacket) - 1) {
			Message("Invalid RCON packet: nullptr or size is too small.\n");
			return nullptr;
		}
		
		int32_t bodyLength = dataSize - (sizeof(this->type) + sizeof(this->pad0));
		if (bodyLength < 0 ) 
		{
			Message("Invalid body length.\n");
			return nullptr;
		}
		
		char* bodyStr = new char[bodyLength + 1];
		V_memcpy(bodyStr, this->body, bodyLength);
		bodyStr[bodyLength] = '\0';

		return bodyStr;
	}
};

void InitDetours();
void FASTCALL Detour_On_Rcon_Message(void *param_1, void *param_2, int param_3, RCONPacket *pPacket, uint32_t PacketSize, netadr_t *paddr);