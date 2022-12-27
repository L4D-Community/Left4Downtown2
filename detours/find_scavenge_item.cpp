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

#include "find_scavenge_item.h"
#include "extension.h"

namespace Detours
{
	void* FindScavengeItem::OnFindScavengeItem(float fScavengeDistance)
	{
		int actualInvocationResult = (int)((this->*(GetTrampoline()))(fScavengeDistance));

		int overrideValue = actualInvocationResult;

		int client = 0, item = 0;

		cell_t result = Pl_Continue;

		edict_t *pEntity = gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(this));
		client = IndexOfEdict(pEntity);
		edict_t *pEntity2 = gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(actualInvocationResult));
		item = IndexOfEdict(pEntity2);

		if (!actualInvocationResult) {
			item = 0;
		}

		g_pFwdOnFindScavengeItem->PushCell(client);
		g_pFwdOnFindScavengeItem->PushCellByRef(&item);
		g_pFwdOnFindScavengeItem->Execute(&result);

		if (result == Pl_Handled) {
			return NULL;
		} else if (result == Pl_Changed) {
			edict_t *pEntity3 = PEntityOfEntIndex(item);
			return reinterpret_cast<void *>(gameents->EdictToBaseEntity(pEntity3));
		}

		return reinterpret_cast<void *>(actualInvocationResult);
	}
};
