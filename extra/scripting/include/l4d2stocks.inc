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

#if defined __l4do_stocks_included
	#endinput
#endif
#define __l4do_stocks_included

#include <sdktools>

/**
 * @brief Sets a players zombie class, special infected can be alive and change!
 * @remarks Valid values L4D1: 1-3. L4D2: 1-6
 * @remarks zombieClass: 1=Smoker, 2=Boomer, 3=Hunter, 4=Spitter, 5=Jockey, 6=Charger
 *
 * @param client			Client ID of the player to kill. Not common infected
 * @param zombieClass		Zombie class number to change to
 *
 * @noreturn
 */
stock void L4D_SetClass(int iClient, int iZombieClass)
{
	static bool bSDKLoad = false;
	static Handle hSDK_CTerrorPlayer_SetClass = null;
	static Handle hSDK_CBaseAbility_CreateForPlayer = null;

	if (!bSDKLoad) {
		Handle hGameData = LoadGameConfigFile("left4downtown.l4d2");
		if (hGameData == null) {
			SetFailState("Failed to load 'left4downtown.l4d2.txt' gamedata.");
		}

		// CTerrorPlayer::SetClass
		StartPrepSDKCall(SDKCall_Player);
		if (!PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CTerrorPlayer::SetClass")) {
			SetFailState("Failed to find signature: 'CTerrorPlayer::SetClass'");
		}

		PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
		hSDK_CTerrorPlayer_SetClass = EndPrepSDKCall();

		if (hSDK_CTerrorPlayer_SetClass == null) {
			SetFailState("Failed to create SDKCall 'CTerrorPlayer::SetClass'");
		}

		// CBaseAbility::CreateForPlayer
		StartPrepSDKCall(SDKCall_Static);
		if (!PrepSDKCall_SetFromConf(hGameData, SDKConf_Signature, "CBaseAbility::CreateForPlayer")) {
			SetFailState("Failed to find signature: 'CBaseAbility::CreateForPlayer'");
		}

		PrepSDKCall_AddParameter(SDKType_CBaseEntity, SDKPass_Pointer);
		PrepSDKCall_SetReturnInfo(SDKType_CBaseEntity, SDKPass_Pointer);
		hSDK_CBaseAbility_CreateForPlayer = EndPrepSDKCall();

		if (hSDK_CBaseAbility_CreateForPlayer == null) {
			SetFailState("Failed to create SDKCall 'CBaseAbility::CreateForPlayer'");
		}

		delete hGameData;
		bSDKLoad = true;
	}

	int iWeapon = GetPlayerWeaponSlot(iClient, 0);
	if (iWeapon != -1) {
		RemovePlayerItem(iClient, iWeapon);
		RemoveEntity(iWeapon);
	}

	int iAbility = GetEntPropEnt(iClient, Prop_Send, "m_customAbility");
	if (iAbility != -1) {
		RemoveEntity(iAbility);
	}

	//PrintToServer("#### CALL hSDK_CTerrorPlayer_SetClass");
	SDKCall(hSDK_CTerrorPlayer_SetClass, iClient, iZombieClass);

	//PrintToServer("#### CALL hSDK_CBaseAbility_CreateForPlayer");
	iAbility = SDKCall(hSDK_CBaseAbility_CreateForPlayer, iClient);
	if (iAbility == -1) {
		LogError("SDKCall 'CBaseAbility::CreateForPlayer' an entity pointer was expected, but the function returned NULL.");
		return;
	}

	SetEntPropEnt(iClient, Prop_Send, "m_customAbility", iAbility);
}
