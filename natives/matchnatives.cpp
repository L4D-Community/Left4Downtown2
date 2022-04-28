/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown 2 SourceMod Extension
 * Copyright (C) 2010 Michael "ProdigySim" Busby.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
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

// native bool L4D_IsCoopMode();
cell_t L4D_IsCoopMode(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorGameRules::IsCoopMode(void)
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(bool);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CTerrorGameRules::IsCoopMode", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_Cdecl, /*retInfo*/&retInfo, /*paramInfo*/NULL, /*numparams*/0));

		L4D_DEBUG_LOG("Built vcall wrapper CTerrorGameRules::IsCoopMode");
	}

	cell_t iRet = 0;

	L4D_DEBUG_LOG("Going to execute CTerrorGameRules::IsCoopMode");
	pWrapper->Execute(NULL, /*retbuffer*/&iRet);
	L4D_DEBUG_LOG("Invoked CTerrorGameRules::IsCoopMode, got back = %d", iRet);

	return iRet;
}

sp_nativeinfo_t g_L4DoMatchNatives[] =
{
	{"L4D_IsCoopMode",					L4D_IsCoopMode},
	{NULL,								NULL}
};
