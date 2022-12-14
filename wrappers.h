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

#ifndef _INCLUDE_L4D2_WRAPPERS_H_
#define _INCLUDE_L4D2_WRAPPERS_H_

#include "extension.h"
#include "l4d2sdk/constants.h"

extern IServerGameEnts* gameents;

extern IMatchFramework* g_pMatchFramework;
extern IMatchExtL4D* g_pMatchExtL4D;

extern ConVar* mp_gamemode;

class CBaseEntity
{
public:
	CBaseEntity* GetOwnerEntity()
	{
		edict_t* pEdict = gamehelpers->GetHandleEntity(*(CBaseHandle*)((byte*)(this) + sendprop_m_hOwnerEntity));
		if (pEdict == NULL) {
			return NULL;
		}

		// Make sure it's a player
		if (engine->GetPlayerUserId(pEdict) == -1) {
			return NULL;
		}

		return gameents->EdictToBaseEntity(pEdict);
	}

	edict_t* edict()
	{
		return gameents->BaseEntityToEdict(this);
	}

	bool IsPlayer()
	{
		edict_t* pEdict = this->edict();
		if (engine->GetPlayerUserId(pEdict) != -1) {
			return true;
		}

		return false;
	}

public:
	static int sendprop_m_hOwnerEntity;
};

class CBasePlayer:
	public CBaseEntity
{
public:
	L4DTeam GetTeamNumber()
	{
		int iTeam = *(int*)((unsigned char*)this + sendprop_m_iTeamNum);
		L4DTeam m_iTeamNum = static_cast<L4DTeam>(iTeam);

		return m_iTeamNum;
	}

public:
	static int sendprop_m_iTeamNum;
};

class CTerrorPlayer :
	public CBasePlayer
{
public:
	bool IsAttemptingToPounce()
	{
		int m_isAttemptingToPounce = *(int*)((unsigned char*)this + sendprop_m_isAttemptingToPounce);

		return (m_isAttemptingToPounce) ? true : false;
	}

	ZombieClassType GetZombieClass()
	{
		int iZClass = *(int*)((unsigned char*)this + sendprop_m_zombieClass);
		ZombieClassType m_zombieClass = static_cast<ZombieClassType>(iZClass);

		return m_zombieClass; 
	}

public:
	static int sendprop_m_isAttemptingToPounce;
	static int sendprop_m_zombieClass;
};

class CBaseCombatWeapon: //CBaseAnimating
	public CBaseEntity
{
public:
	CBaseEntity* GetOwnerEntity()
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

public:
	static int sendprop_m_hOwner;
};

class CWeaponSpawn: //CBaseAnimating
	public CBaseEntity
{
public:
	int GetWeaponIDNetProp()
	{
		int m_weaponID = *(int*)((unsigned char*)this + sendprop_m_weaponID);

		return m_weaponID;
	}

public:
	static int sendprop_m_weaponID;
};

class CTerrorGameRules
{
public:
	static bool IsGenericCooperativeMode()
	{
		return CTerrorGameRules::IsCoopMode() || CTerrorGameRules::IsRealismMode();
	}

	static bool IsCoopMode()
	{
		return IsSpecifiedMode("coop");
	}

	static bool IsRealismMode()
	{
		return IsSpecifiedMode("realism");
	}

	static bool IsSurvivalMode()
	{
		return IsSpecifiedMode("survival");
	}

	static bool IsScavengeMode()
	{
		return IsSpecifiedMode("scavenge");
	}

	static bool IsVersusMode()
	{
		return IsSpecifiedMode("versus");
	}

	static bool IsSpecifiedMode(const char* pszBaseMode)
	{
		if (!g_pMatchExtL4D) {
			return false;
		}

		const char* szGameMode = mp_gamemode->GetString();

		KeyValues* pkvMode = g_pMatchExtL4D->GetGameModeInfo(szGameMode);

		if (pkvMode != NULL) {
			return V_stricmp(pkvMode->GetString("base"), pszBaseMode ) == 0;
		}

		return false;
	}
};

bool L4D2_GetOffsets(char* error, size_t maxlength);

#endif // _INCLUDE_L4D2_WRAPPERS_H_
