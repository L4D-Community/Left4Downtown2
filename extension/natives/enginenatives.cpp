/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2021 Accelerator, 2021-2023 A1m`;
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "util.h"

 // native L4D_LobbyUnreserve()
cell_t L4D_LobbyUnreserve(IPluginContext* pContext, const cell_t* params)
{
	const uint64 cookieUnreserved = 0;

	if (g_pServer == NULL) {
		return pContext->ThrowNativeError("Failed to get pointer to CBaseServer(IServer), is SDKTools extension available?");
	}

	static ICallWrapper* pWrapper = NULL;

	/* CBaseServer::SetReservationCookie(
			uint64_t reservationCookie,
			const char *formatString,
			va_list ap)

		__cdecl on windows, __thiscall on linux
	*/
	if (!pWrapper) {
#ifdef PLATFORM_WINDOWS
		REGISTER_NATIVE_ADDR("CBaseServer::SetReservationCookie", \
			PassInfo pass[4]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(IServer*); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(uint64_t); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(char const*); \
			pass[2].type = PassType_Basic; \
			pass[3].flags = PASSFLAG_BYVAL; \
			pass[3].size = sizeof(void*); \
			pass[3].type = PassType_Basic; \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_Cdecl, /*retInfo*/NULL, /*paramInfo*/pass, /*numparams*/4));
#else
		REGISTER_NATIVE_ADDR("CBaseServer::SetReservationCookie", \
			PassInfo pass[3]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(uint64_t); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(char const*); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(void*); \
			pass[2].type = PassType_Basic; \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/pass, /*numparams*/3));
#endif
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CBaseServer*) + sizeof(uint64_t) + sizeof(char const*) + sizeof(void*)];
	unsigned char* vptr = vstk;

	*(void**)vptr = g_pServer;
	vptr += sizeof(CBaseServer*);

	*(uint64_t*)vptr = cookieUnreserved;
	vptr += sizeof(uint64_t);

	*(const char**)vptr = "Manually unreserved by Left 4 Downtown Extension";
	vptr += sizeof(char*);

	*(void**)vptr = NULL; // don't bother using a va_list ap :)

	pWrapper->Execute(vstk, /*retbuffer*/NULL);

	L4D_DEBUG_LOG("Invoked CBaseServer::SetReservationCookie(0,fmt,...)");

	return 1;
}

// native bool:L4D_LobbyIsReserved()
cell_t L4D_LobbyIsReserved(IPluginContext* pContext, const cell_t* params)
{
	if (g_pServer == NULL) {
		g_pSM->LogError(myself, "Failed to get pointer to CBaseServer(IServer), is SDKTools extension available?");

		return false;
	}

	L4D_DEBUG_LOG("Address of IServer is %p, m_nReservationCookie: %llx", g_pServer, g_pServer->m_nReservationCookie);

	g_pSM->LogError(myself, "Address of IServer is %p, m_nReservationCookie: %llx", g_pServer, g_pServer->m_nReservationCookie);

	return (g_pServer->m_nReservationCookie != 0) ? true : false;
}

sp_nativeinfo_t g_L4DoEngineNatives[] =
{
	{"L4D_LobbyUnreserve",				L4D_LobbyUnreserve},
	{"L4D_LobbyIsReserved",				L4D_LobbyIsReserved},
	{NULL,								NULL}
};
