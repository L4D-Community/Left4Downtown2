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

#include "inferno_spread.h"
#include "extension.h"

namespace Detours
{
	void *InfernoSpread::OnInfernoSpread(/*const*/ Vector &spreadVelocity)
	{
		CBaseEntity *pEntity = (CBaseEntity *)this;
		CBaseEntity *pOwner = pEntity->GetOwnerEntity(); // spitter or other kind of fire-raiser (survivor that has thrown a molotov etc)

		// infected and spitter
		//if ((CTerrorPlayer *)pOwner->GetTeamNumber() == Team_Infected && (CTerrorPlayer *)pOwner->GetZombieClass() == ZombieClassType_Spitter) {}

		cell_t ctSpreadVelocity[3] = {sp_ftoc(spreadVelocity.x), sp_ftoc(spreadVelocity.y), sp_ftoc(spreadVelocity.z)};

		cell_t result = Pl_Continue;

		int thrower = (pOwner == NULL) ? 0 : IndexOfEdict(gameents->BaseEntityToEdict(pOwner));
		int projectile = IndexOfEdict(gameents->BaseEntityToEdict(pEntity));
		//int puddleID = *(int *)((unsigned char *)this + 2624);

		g_pFwdInfernoSpread->PushCell(thrower);
		g_pFwdInfernoSpread->PushCell(projectile);
		//g_pFwdInfernoSpread->PushCell(puddleID);
		g_pFwdInfernoSpread->PushArray(ctSpreadVelocity, 3, SM_PARAM_COPYBACK);
		g_pFwdInfernoSpread->Execute(&result);

		if (result == Pl_Changed) {
			for (int i = 0; i < 3; i++) {
				spreadVelocity[i] = sp_ctof(ctSpreadVelocity[i]);
			}
		} else if (result == Pl_Handled) {
			return NULL;
		}

		return (this->*(GetTrampoline()))(spreadVelocity);
	}
};
