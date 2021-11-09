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

#include "vglobals.h"
#include "util.h"

CDirector **g_pDirector = NULL;
void *g_pZombieManager = NULL;
WeaponDatabase *g_pWeaponInfoDatabase = NULL;
CMeleeWeaponInfoStore *g_pMeleeWeaponInfoStore = NULL;

void InitializeValveGlobals()
{
	char *addr = NULL;

	/* g_pDirector */
	if (!g_pGameConf->GetAddress("CDirector", (void **)&addr) || !addr)
	{
		L4D_DEBUG_LOG("Couldn't find CDirector instance!");
		return;
	}
	g_pDirector = reinterpret_cast<CDirector **>(addr);
	L4D_DEBUG_LOG("TheDirector found at: %p", g_pDirector);

	/* g_pZombieManager */
	if (!g_pGameConf->GetAddress("ZombieManager", (void **)&addr))
	{
		L4D_DEBUG_LOG("TheZombieManager address not found.");
		return;
	}
	g_pZombieManager = addr;
	L4D_DEBUG_LOG("TheZombieManager found at: %p", g_pZombieManager);

	/* g_pWeaponInfoDatabase */
	if (!g_pGameConf->GetAddress("WeaponInfoDatabase", (void **)&addr))
	{
		L4D_DEBUG_LOG("WeaponInfoDatabase address not found.");
		return;
	}
	g_pWeaponInfoDatabase = reinterpret_cast<WeaponDatabase *>(addr);
	L4D_DEBUG_LOG("WeaponInfoDatabase found at: %p", g_pWeaponInfoDatabase);

	/* g_pMeleeWeaponInfoStore */
	if (!g_pGameConf->GetAddress("MeleeWeaponInfoStore", (void **)&addr))
	{
		L4D_DEBUG_LOG("MeleeWeaponInfoStore address not found.");
		return;
	}
	g_pMeleeWeaponInfoStore = reinterpret_cast<CMeleeWeaponInfoStore *>(addr);
	L4D_DEBUG_LOG("MeleeWeaponInfo Store: %p ", g_pMeleeWeaponInfoStore);
	L4D_DEBUG_LOG("MeleeWeaponInfo Store: %s ", g_pMeleeWeaponInfoStore->Name());
}
