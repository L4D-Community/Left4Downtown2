/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2019 Accelerator; 2021 A1m`, Accelerator;
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

#include "spawn_special.h"
#include "extension.h"

namespace Detours
{
	CTerrorPlayer* SpawnSpecial::OnSpawnSpecial(ZombieClassType zombieClassType, Vector *position, QAngle *angles)
	{
		cell_t result = Pl_Continue;
		cell_t overrideZombieClass = zombieClassType;
		CTerrorPlayer *pPlayer = NULL;

		g_pFwdOnSpawnSpecial->PushCellByRef(&overrideZombieClass);
		g_pFwdOnSpawnSpecial->PushArray(reinterpret_cast<cell_t *>(position), 3);
		g_pFwdOnSpawnSpecial->PushArray(reinterpret_cast<cell_t *>(angles), 3);
		g_pFwdOnSpawnSpecial->Execute(&result);

		if (result == Pl_Handled) {
			return NULL;
		} else if (result == Pl_Changed) {
			pPlayer = (this->*(GetTrampoline()))(*reinterpret_cast<ZombieClassType *>(&overrideZombieClass), position, angles);
		} else {
			pPlayer = (this->*(GetTrampoline()))(zombieClassType, position, angles);
		}

		int client = IndexOfEdict(gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(pPlayer)));

		g_pFwdOnSpawnSpecialPost->PushCell(client);
		g_pFwdOnSpawnSpecialPost->PushCell(overrideZombieClass);
		g_pFwdOnSpawnSpecialPost->PushArray(reinterpret_cast<cell_t *>(position), 3);
		g_pFwdOnSpawnSpecialPost->PushArray(reinterpret_cast<cell_t *>(angles), 3);
		g_pFwdOnSpawnSpecialPost->Execute(NULL);

		return pPlayer;
	}
};
