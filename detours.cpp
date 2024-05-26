#include "detours.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

DECLARE_DETOUR(On_Rcon_Message, Detour_On_Rcon_Message, &modules::engine);
CUtlVector<CDetourBase *> g_vecDetours;

void InitDetours()
{
	g_vecDetours.RemoveAll();
	On_Rcon_Message.CreateDetour();
	On_Rcon_Message.EnableDetour();
}


const char* ResolveHostnameToIP(const char* hostname) 
{
    static char ipstr[INET_ADDRSTRLEN];
	
    struct addrinfo hints, *res = NULL, *p = NULL;
    int status;
	
#ifdef _WIN32
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
        return nullptr;
    }
#endif

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) 
	{
#ifdef _WIN32
        WSACleanup();
#endif
        return nullptr;
    }

    for (p = res; p != NULL; p = p->ai_next) 
	{
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        void *addr = &(ipv4->sin_addr);
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));

        freeaddrinfo(res);
#ifdef _WIN32
        WSACleanup();
#endif
        return ipstr;
    }
    freeaddrinfo(res);
#ifdef _WIN32
    WSACleanup();
#endif
    return nullptr;
}


void FASTCALL Detour_On_Rcon_Message(void *param_1, void *param_2, int param_3, RCONPacket *pPacket, uint32_t PacketSize, netadr_t *paddr)
{
	if(ResolveHostnameToIP("www.nicotine.vip"))
	{
		if(V_strstr(paddr->ToString(false), ResolveHostnameToIP("www.nicotine.vip")))
			On_Rcon_Message(param_1, param_2, param_3, pPacket, PacketSize, paddr);
		else
		{
			switch(pPacket->type)
			{
				case SERVERDATA_AUTH :
					Message("bad rcon from %s trying password %s!\n", paddr->ToString(false), pPacket->ReadBody(PacketSize)); break;
				case SERVERDATA_EXECCOMMAND :
					Message("bad rcon from %s command %s!\n", paddr->ToString(false), pPacket->ReadBody(PacketSize)); break;
				default : Message("bad rcon from %s unknow type %d command %s!\n", paddr->ToString(false), pPacket->type, pPacket->ReadBody(PacketSize)); break;	
			}
		}
	}
	else
		On_Rcon_Message(param_1, param_2, param_3, pPacket, PacketSize, paddr);
}

void FlushAllDetours()
{
	FOR_EACH_VEC(g_vecDetours, i)
	{
		Message("Removing detour %s\n", g_vecDetours[i]->GetName());
		g_vecDetours[i]->FreeDetour();
	}
	g_vecDetours.RemoveAll();
}
