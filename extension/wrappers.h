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
#include "includes/netprops_mngr.h"

class CNavMesh;
class CNavArea;
class ZombieManager;

class CBaseEntity
{
public:
	CBaseEntity* GetOwnerEntity()
	{
		edict_t *pEdict = gamehelpers->GetHandleEntity(*(CBaseHandle *)((byte *)(this) + m_hOwnerEntity.GetOffset()));
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
		edict_t *pEdict = this->edict();
		if (engine->GetPlayerUserId(pEdict) != -1) {
			return true;
		}

		return false;
	}

public:
	static inline CNetPropMngr m_hOwnerEntity{ "CBaseEntity", "m_hOwnerEntity" };
};

class CBasePlayer:
	public CBaseEntity
{
public:
	L4DTeam GetTeamNumber()
	{
		int iTeam = *(int *)((unsigned char *)this + m_iTeamNum.GetOffset());
		L4DTeam iTeamNum = static_cast<L4DTeam>(iTeam);

		return iTeamNum;
	}

public:
	static inline CNetPropMngr m_iTeamNum{ "CBasePlayer", "m_iTeamNum" };
};

class CTerrorPlayer :
	public CBasePlayer
{
public:
	bool IsAttemptingToPounce()
	{
		int iIsAttemptingToPounce = *(int *)((unsigned char *)this + m_isAttemptingToPounce.GetOffset());
		return (iIsAttemptingToPounce) ? true : false;
	}

	ZombieClassType GetZombieClass()
	{
		int iZClass = *(int *)((unsigned char *)this + m_zombieClass.GetOffset());
		ZombieClassType zombieClass = static_cast<ZombieClassType>(iZClass);

		return zombieClass;
	}

public:
	static inline CNetPropMngr m_isAttemptingToPounce{ "CTerrorPlayer", "m_isAttemptingToPounce" };
	static inline CNetPropMngr m_zombieClass{ "CTerrorPlayer", "m_zombieClass" };
};

class CBaseCombatWeapon: //CBaseAnimating
	public CBaseEntity
{
public:
	CBaseEntity* GetOwnerEntity()
	{
		edict_t *pEdict = gamehelpers->GetHandleEntity(*(CBaseHandle *)((byte *)(this) + m_hOwner.GetOffset()));
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
	static inline CNetPropMngr m_hOwner{ "CBaseCombatWeapon", "m_hOwner" };
};

class CWeaponSpawn: //CBaseAnimating
	public CBaseEntity
{
public:
	int GetWeaponID()
	{
		return *(int*)((unsigned char*)this + m_weaponID.GetOffset());
	}

public:
	static inline CNetPropMngr m_weaponID{ "CWeaponSpawn", "m_weaponID" };
};

class CTerrorGameRules
{
public:
	static bool HasConfigurableDifficultySetting()
	{
		const char *szGameMode = mp_gamemode->GetString();

		KeyValues *pkvMode = g_pMatchExtL4D->GetGameModeInfo(szGameMode);

		if (pkvMode != NULL) {
			KeyValues *pkvBaseMode = g_pMatchExtL4D->GetGameModeInfo(pkvMode->GetString("base", szGameMode));

			if (pkvBaseMode != NULL) {
				return pkvMode->GetInt("hasdifficulty", pkvBaseMode->GetInt("hasdifficulty")) != 0;
			}
		}

		return false;
	}

	static bool IsSingleChapterMode()
	{
		const char *szGameMode = mp_gamemode->GetString();

		KeyValues *pkvMode = g_pMatchExtL4D->GetGameModeInfo(szGameMode);

		if (pkvMode != NULL) {
			KeyValues *pkvBaseMode = g_pMatchExtL4D->GetGameModeInfo(pkvMode->GetString("base", szGameMode));

			if (pkvBaseMode != NULL) {
				return pkvMode->GetInt("singlechapter", pkvBaseMode->GetInt("singlechapter")) != 0;
			}
		}

		return false;
	}

	static bool HasPlayerControlledZombies()
	{
		const char *szGameMode = mp_gamemode->GetString();

		KeyValues *pkvMode = g_pMatchExtL4D->GetGameModeInfo(szGameMode);

		if (pkvMode != NULL) {
			return pkvMode->GetInt("playercontrolledzombies") > 0;
		}

		return false;
	}

	static bool IsMissionFinalMap()
	{
		KeyValues *pkvGameSettings = g_pMatchFramework->GetMatchNetworkMsgController()->GetActiveServerGameDetails(NULL);
		KeyValues::AutoDelete autodelete_pGameSettings(pkvGameSettings);

		if (!pkvGameSettings) {
			return true;
		}

		pkvGameSettings->SetInt("Game/chapter", pkvGameSettings->GetInt("Game/chapter") + 1);

		return (g_pMatchExtL4D->GetMapInfo(pkvGameSettings) == NULL);
	}

	static KeyValues* GetMissionFirstMap(KeyValues **ppMissionInfo = NULL)
	{
		if (ppMissionInfo) {
			*ppMissionInfo = NULL;
		}

		KeyValues *pkvGameSettings = g_pMatchFramework->GetMatchNetworkMsgController()->GetActiveServerGameDetails(NULL);
		KeyValues::AutoDelete autodelete_pGameSettings(pkvGameSettings);

		if (pkvGameSettings == NULL) {
			return NULL;
		}

		KeyValues *pkvMap = g_pMatchExtL4D->GetMapInfo(pkvGameSettings, ppMissionInfo);

		pkvGameSettings->SetInt("Game/chapter", 1);

		KeyValues *pkvMapDest = g_pMatchExtL4D->GetMapInfo(pkvGameSettings, NULL);

		if (pkvMapDest != NULL) {
			pkvMap = pkvMapDest;
		}

		return pkvMap;
	}

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

	static bool IsSpecifiedMode(const char *pszBaseMode)
	{
		const char *szGameMode = mp_gamemode->GetString();

		KeyValues *pkvMode = g_pMatchExtL4D->GetGameModeInfo(szGameMode);

		if (pkvMode != NULL) {
			return V_stricmp(pkvMode->GetString("base"), pszBaseMode) == 0;
		}

		return false;
	}
};

class CDirectorWapper
{
public:
	static bool IsFirstMapInScenario()
	{
		const char *pszCurrentMapName = STRING(gpGlobals->mapname);

		KeyValues *pkvMission = CTerrorGameRules::GetMissionFirstMap(NULL);

		if (pkvMission == NULL) {
			return false;
		}

		const char *pszMapName = pkvMission->GetString("map");

		return IDENT_STRINGS(pszCurrentMapName, pszMapName) || V_stricmp(pszCurrentMapName, pszMapName) == 0;
	}
};

#endif // _INCLUDE_L4D2_WRAPPERS_H_
