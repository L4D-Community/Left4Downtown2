/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2021 Accelerator, 2021-2022 A1m`;
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
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

#include "extension.h"
#include "wrappers.h"
#include "util.h"

class CBaseCombatCharacter;

// native void L4D2_CTerrorPlayer_Fling(int iTarget, int iAttacker, const float vecImpulse[3], int iAnimEvent = 76, float fTimeExternalView = 0.0);
cell_t L4D2_CTerrorPlayer_Fling(IPluginContext* pContext, const cell_t* params)
{
	static ICallWrapper* pWrapper = NULL;

	// void CTerrorPlayer::Fling( const Vector &vecImpulse, PlayerAnimEvent_t animEvent, CBaseCombatCharacter *pAirMovementCause, float fTimeExternalView )
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CTerrorPlayer::Fling", \
			PassInfo pass[4]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(const Vector*); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(int); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(CBaseCombatCharacter *); \
			pass[2].type = PassType_Basic; \
			pass[3].flags = PASSFLAG_BYVAL; \
			pass[3].size = sizeof(float); \
			pass[3].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, pass, /*numparams*/4));
	}

	CTerrorPlayer* pPlayer = (CTerrorPlayer*)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid target player: %d!", params[1]);
	}

	CBaseEntity* pAirMovementCause = UTIL_GetCBaseEntity(params[2], false);
	if (pAirMovementCause == NULL) {
		return pContext->ThrowNativeError("Invalid attacker player: %d!", params[2]);
	}

	cell_t* vecImpulse;
	pContext->LocalToPhysAddr(params[3], &vecImpulse);

	Vector vSourceVector;
	Vector* pSourceVector = NULL;

	if (vecImpulse != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vSourceVector[0] = sp_ctof(vecImpulse[0]);
		vSourceVector[1] = sp_ctof(vecImpulse[1]);
		vSourceVector[2] = sp_ctof(vecImpulse[2]);

		pSourceVector = &vSourceVector;
	}

	unsigned char vstk[sizeof(CTerrorPlayer *) + sizeof(const Vector *) + sizeof(int) + sizeof(CBaseCombatCharacter *) + sizeof(float)];
	unsigned char* vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;
	vptr += sizeof(CTerrorPlayer *);

	*(const Vector **)vptr = pSourceVector;
	vptr += sizeof(const Vector *);

	*(int *)vptr = params[4];
	vptr += sizeof(int);

	*(CBaseCombatCharacter **)vptr = (CBaseCombatCharacter *)pAirMovementCause;
	vptr += sizeof(CBaseCombatCharacter *);

	*(float *)vptr = sp_ctof(params[5]);

	L4D_DEBUG_LOG("Going to execute CTerrorPlayer::Fling");
	pWrapper->Execute(vstk, NULL);
	L4D_DEBUG_LOG("Invoked CTerrorPlayer::Fling");

	return 1;
}

// native float L4D_GetPlayerSpawnTime(int player);
cell_t L4D_GetPlayerSpawnTime(IPluginContext *pContext, const cell_t *params)
{
	static int iOffset;
	if (!iOffset && (!g_pGameConf->GetOffset("SpawnTimer", &iOffset) || !iOffset)) {
		return pContext->ThrowNativeError("Could not read 'SpawnTimer' offset from GameConf");
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid player to retrieve spawn timer: %d!", params[1]);
	}

	L4D_DEBUG_LOG("Reading player %d spawn timer", params[1]);

	return sp_ftoc(*(float *)((unsigned char *)pPlayer + iOffset) - gpGlobals->curtime);
}

// native void L4D_SetHumanSpec(int bot, int client);
cell_t L4D_SetHumanSpec(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//SurvivorBot::SetHumanSpectator(CTerrorPlayer *)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("SurvivorBot::SetHumanSpectator", \
			PassInfo passInfo; \
			passInfo.flags = PASSFLAG_BYVAL; \
			passInfo.size = sizeof(CTerrorPlayer *); \
			passInfo.type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/&passInfo, /*numparams*/1));

		L4D_DEBUG_LOG("Built call wrapper SurvivorBot::SetHumanSpectator");
	}

	CBaseEntity *pBot = UTIL_GetCBaseEntity(params[1], true);
	if (pBot == NULL) {
		return pContext->ThrowNativeError("Invalid bot player: %d!", params[1]);
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[2], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid target player: %d!", params[2]);
	}

	unsigned char vstk[sizeof(CBaseEntity *) + sizeof(CBaseEntity *)];
	unsigned char *vptr = vstk;

	*(CBaseEntity **)vptr = pBot;
	vptr += sizeof(CBaseEntity *);

	*(CTerrorPlayer **)vptr = pPlayer;

	L4D_DEBUG_LOG("Going to execute SurvivorBot::SetHumanSpectator");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked SurvivorBot::SetHumanSpectator");

	return 1;
}

// native int L4D_GetLastKnownArea(int client);
cell_t L4D_GetLastKnownArea(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorPlayer::GetLastKnownArea(void)
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(CNavArea *);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_OFFSET("CTerrorPlayer::GetLastKnownArea", \
			pWrapper = g_pBinTools->CreateVCall(offset, 0, 0, /*retInfo*/&retInfo, /*paramInfo*/NULL, /*numparams*/0));

		L4D_DEBUG_LOG("Built vcall wrapper CTerrorPlayer::GetLastKnownArea");
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid target player: %d!", params[1]);
	}

	unsigned char vstk[sizeof(CTerrorPlayer *)];
	unsigned char *vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;

	CNavArea *pRet = NULL;

	L4D_DEBUG_LOG("Going to execute CTerrorPlayer::GetLastKnownArea");
	pWrapper->Execute(vstk, /*retbuffer*/&pRet);
	L4D_DEBUG_LOG("Invoked CTerrorPlayer::GetLastKnownArea");

	return reinterpret_cast<cell_t>(pRet);
}

 // native void L4D_TakeOverBot(int client);
cell_t L4D_TakeOverBot(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorPlayer::TakeOverBot(bool)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CTerrorPlayer::TakeOverBot", \
			PassInfo passInfo; \
			passInfo.flags = PASSFLAG_BYVAL; \
			passInfo.size = sizeof(bool); \
			passInfo.type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/&passInfo, /*numparams*/1));

		L4D_DEBUG_LOG("Built call wrapper CTerrorPlayer::TakeOverBot");
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid target player: %d!", params[1]);
	}

	unsigned char vstk[sizeof(CTerrorPlayer *) + sizeof(bool)];
	unsigned char *vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;
	vptr += sizeof(CTerrorPlayer *);

	*(bool *)vptr = true;

	L4D_DEBUG_LOG("Going to execute CTerrorPlayer::TakeOverBot");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CTerrorPlayer::TakeOverBot");

	return 1;
}

 //native void L4D_ReviveSurvivor(int client);
cell_t L4D_ReviveSurvivor(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorPlayer::OnRevived(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CTerrorPlayer::OnRevived", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/NULL, /*numparams*/0));

		L4D_DEBUG_LOG("Built call wrapper CTerrorPlayer::OnRevived");
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid target player: %d!", params[1]);
	}

	unsigned char vstk[sizeof(CTerrorPlayer *)];
	unsigned char *vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;

	L4D_DEBUG_LOG("Going to execute CTerrorPlayer::OnRevived");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CTerrorPlayer::OnRevived");

	return 1;
}

 // native L4D_StaggerPlayer(target, source_ent, Float:source_vector[3])
cell_t L4D_StaggerPlayer(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CBasePlayer::OnStaggered(CBaseEntity *, Vector *);
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CTerrorPlayer::OnStaggered", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(CBaseEntity *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(Vector *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, pass, /*numparams*/2));
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (pPlayer == NULL) {
		return pContext->ThrowNativeError("Invalid stagger target player: %d!", params[1]);
	}

	CBaseEntity *pSource = UTIL_GetCBaseEntity(params[2], false);
	if (pSource == NULL) {
		return pContext->ThrowNativeError("Invalid stagger source entity: %d!", params[2]);
	}

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[3], &source_vector);

	Vector vSourceVector;
	Vector *pSourceVector = NULL;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vSourceVector[0] = sp_ctof(source_vector[0]);
		vSourceVector[1] = sp_ctof(source_vector[1]);
		vSourceVector[2] = sp_ctof(source_vector[2]);

		pSourceVector = &vSourceVector;
	}

	unsigned char vstk[sizeof(CTerrorPlayer *) + sizeof(CBaseEntity *) + sizeof(Vector *)];
	unsigned char *vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;
	vptr += sizeof(CTerrorPlayer *);

	*(CBaseEntity **)vptr = pSource;
	vptr += sizeof(CBaseEntity *);

	*(Vector **)vptr = pSourceVector;

	L4D_DEBUG_LOG("Going to execute CTerrorPlayer::OnStaggered");
	pWrapper->Execute(vstk, NULL);
	L4D_DEBUG_LOG("Invoked CTerrorPlayer::OnStaggered");

	return 1;
}

// native void L4D_State_Transition(int client, int state)
cell_t L4D_State_Transition(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// void CCSPlayer::State_Transition( CSPlayerState newState )
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CCSPlayer::State_Transition", \
			PassInfo passInfo; \
			passInfo.flags = PASSFLAG_BYVAL; \
			passInfo.size = sizeof(int); \
			passInfo.type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/&passInfo, /*numparams*/1));

		L4D_DEBUG_LOG("Built vcall wrapper CCSPlayer::State_Transition");
	}

	CTerrorPlayer *pPlayer = (CTerrorPlayer *)UTIL_GetCBaseEntity(params[1], true);
	if (!pPlayer) {
		return pContext->ThrowNativeError("Invalid client index or not a player: %d!", params[1]);
	}

	TerrorPlayerState iTerrorState = static_cast<TerrorPlayerState>(params[2]);
	if (iTerrorState < STATE_ACTIVE || iTerrorState > STATE_INTRO_CAMERA) {
		return pContext->ThrowNativeError("Invalid parameter passed: %d! Values less than %d and greater than %d cannot be used!", \
			iTerrorState, STATE_ACTIVE, STATE_INTRO_CAMERA);
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CTerrorPlayer *) + sizeof(int)];
	unsigned char *vptr = vstk;

	*(CTerrorPlayer **)vptr = pPlayer;
	vptr += sizeof(CTerrorPlayer *);

	*(int *)vptr = iTerrorState;

	L4D_DEBUG_LOG("Going to execute CCSPlayer::State_Transition");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CCSPlayer::State_Transition");

	return 1;
}

sp_nativeinfo_t g_L4DoPlayerNatives[] =
{
	{"L4D2_CTerrorPlayer_Fling",		L4D2_CTerrorPlayer_Fling},
	{"L4D_GetPlayerSpawnTime",			L4D_GetPlayerSpawnTime},
	{"L4D_SetHumanSpec",				L4D_SetHumanSpec},
	{"L4D_GetLastKnownArea",			L4D_GetLastKnownArea},
	{"L4D_TakeOverBot",					L4D_TakeOverBot},
	{"L4D_ReviveSurvivor",				L4D_ReviveSurvivor},
	{"L4D_StaggerPlayer",				L4D_StaggerPlayer},
	{"L4D_State_Transition",			L4D_State_Transition},
	{NULL,								NULL}
};
