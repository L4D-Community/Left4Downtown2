/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2019 Accelerator, 2021 A1m`, Accelerator;
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

#ifndef _INCLUDE_L4DOWNTOWN_NATIVES_H_
#define _INCLUDE_L4DOWNTOWN_NATIVES_H_

// For the "L4D_GetPointer" native
enum PointerType
{
	POINTER_DIRECTOR		= 1,	// @TheDirector
	POINTER_SERVER			= 2,	// @sv
	POINTER_GAMERULES		= 3,	// @g_pGameRules
	POINTER_NAVMESH			= 4,	// @TheNavMesh
	POINTER_ZOMBIEMANAGER	= 5,	// @TheZombieManager
	POINTER_WEAPONINFO		= 6,	// @_ZL20m_WeaponInfoDatabase
	POINTER_MELEEINFO		= 7,	// @g_MeleeWeaponInfoStore
	POINTER_EVENTMANAGER	= 8,	// pScriptedEventManager
	POINTER_SCAVENGEMODE	= 9,	// pScavengeMode
	POINTER_VERSUSMODE		= 10	// pVersusMode
};

inline cell_t NativeValidateAddress(void *pAddress, const char *sName, IPluginContext* pContext)
{
	if (pAddress == NULL) {
		return pContext->ThrowNativeError("Failed to find \"%s\" address.", sName);
	}

	return reinterpret_cast<cell_t>(pAddress);
}


#endif //_INCLUDE_L4DOWNTOWN_NATIVES_H_
