/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2021-2024 A1m`
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

#include "jockey_ride.h"
#include "extension.h"

namespace Detours
{
	void CJockeyRide::OnLeptOnSurvivor(CTerrorPlayer *pTarget)
	{
		cell_t result = Pl_Continue;

		int iTarget = (pTarget == NULL) ? 0 : IndexOfEdict(gameents->BaseEntityToEdict(pTarget));
		int iAttacker = IndexOfEdict(gameents->BaseEntityToEdict((CBaseEntity*)this));

		g_pFwdOnLeptOnSurvivor->PushCell(iTarget);
		g_pFwdOnLeptOnSurvivor->PushCell(iAttacker);
		g_pFwdOnLeptOnSurvivor->Execute(&result);

		if (result == Pl_Handled) {
			g_pFwdOnLeptOnSurvivorPostHandled->PushCell(iTarget);
			g_pFwdOnLeptOnSurvivorPostHandled->PushCell(iAttacker);
			g_pFwdOnLeptOnSurvivorPostHandled->Execute(NULL);
			return;
		}

		(this->*(GetTrampoline()))(pTarget);

		g_pFwdOnLeptOnSurvivorPost->PushCell(iTarget);
		g_pFwdOnLeptOnSurvivorPost->PushCell(iAttacker);
		g_pFwdOnLeptOnSurvivorPost->Execute(NULL);
	}
};
