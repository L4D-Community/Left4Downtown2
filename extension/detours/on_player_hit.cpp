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

#include "on_player_hit.h"
#include "extension.h"

namespace Detours
{
	void CPlayerHit::OnPlayerHit(CTerrorPlayer *pVictim, bool bIncapacitated)
	{
		cell_t result = Pl_Continue;
		CBaseEntity *pOwner = ((CBaseEntity *)this)->GetOwnerEntity();

		int iTankAttacker = IndexOfEdict(gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(pOwner)));
		int iClawWeapon = IndexOfEdict(gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(this)));
		int iPlayerVictim = IndexOfEdict(gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(pVictim)));

		g_pFwdOnPlayerHit->PushCell(iTankAttacker);
		g_pFwdOnPlayerHit->PushCell(iClawWeapon);
		g_pFwdOnPlayerHit->PushCell(iPlayerVictim);
		g_pFwdOnPlayerHit->PushCell(bIncapacitated);
		g_pFwdOnPlayerHit->Execute(&result);

		if (result == Pl_Handled) {
			g_pFwdOnPlayerHitPostHandled->PushCell(iTankAttacker);
			g_pFwdOnPlayerHitPostHandled->PushCell(iClawWeapon);
			g_pFwdOnPlayerHitPostHandled->PushCell(iPlayerVictim);
			g_pFwdOnPlayerHitPostHandled->PushCell(bIncapacitated);
			g_pFwdOnPlayerHitPostHandled->Execute(NULL);

			return;
		}

		(this->*(GetTrampoline()))(pVictim, bIncapacitated);

		g_pFwdOnPlayerHitPost->PushCell(iTankAttacker);
		g_pFwdOnPlayerHitPost->PushCell(iClawWeapon);
		g_pFwdOnPlayerHitPost->PushCell(iPlayerVictim);
		g_pFwdOnPlayerHitPost->PushCell(bIncapacitated);
		g_pFwdOnPlayerHitPost->Execute(NULL);
	}
};
