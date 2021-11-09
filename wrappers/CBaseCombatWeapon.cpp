/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2021 A1mDev (A1m`)
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

#include "CBaseCombatWeapon.h"

int CBaseCombatWeapon::sendprop_m_hOwner = 0;

bool CBaseCombatWeapon::OnLoad(char* error, size_t maxlength)
{
	sm_sendprop_info_t info;

	if (!gamehelpers->FindSendPropInfo("CBaseCombatWeapon", "m_hOwner", &info)) {
		snprintf(error, maxlength, "Unable to find SendProp \"CBaseCombatWeapon::m_hOwner\"");

		return false;
	}

	sendprop_m_hOwner = info.actual_offset;

	return true;
}

CBaseEntity *CBaseCombatWeapon::GetOwnerEntity()
{
	edict_t* pEdict = gamehelpers->GetHandleEntity(*(CBaseHandle*)((byte*)(this) + sendprop_m_hOwner));
	if (pEdict == NULL) {
		return NULL;
	}

	// Make sure it's a player
	if (engine->GetPlayerUserId(pEdict) == -1) {
		return NULL;
	}

	return gameents->EdictToBaseEntity(pEdict);
}