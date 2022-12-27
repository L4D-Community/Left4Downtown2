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
#include "l4d2sdk/constants.h"

// native L4D_NotifyNetworkStateChanged()
cell_t L4D_NotifyNetworkStateChanged(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CGameRulesProxy::NotifyNetworkStateChanged(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CGameRulesProxy::NotifyNetworkStateChanged", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_Cdecl, /*returnInfo*/NULL, /*Pass*/NULL, /*numparams*/0));
	}

	L4D_DEBUG_LOG("Going to execute CGameRulesProxy::NotifyNetworkStateChanged");
	pWrapper->Execute(NULL, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CGameRulesProxy::NotifyNetworkStateChanged");

	return 1;
}

// native int L4D_GetVersusMaxCompletionScore();
cell_t L4D_GetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params)
{
	CTerrorGameRules *pGameRules = (CTerrorGameRules *)g_pSDKTools->GetGameRules();

	if (pGameRules == NULL) {
		return pContext->ThrowNativeError("GameRules not available before map is loaded");
	}

	int iOffset = -1;
	if (!g_pGameConf->GetOffset("VersusMaxCompletionScore", &iOffset) || !iOffset) {
		return pContext->ThrowNativeError("Could not read 'VersusMaxCompletionScore' offset from GameConf");
	}

	L4D_DEBUG_LOG("Reading Versus MaxDistance");

	return *(unsigned int *)((unsigned char *)pGameRules + iOffset);
}

// native void L4D_SetVersusMaxCompletionScore(int score);
cell_t L4D_SetVersusMaxCompletionScore(IPluginContext *pContext, const cell_t *params)
{
	CTerrorGameRules *pGameRules = (CTerrorGameRules *)g_pSDKTools->GetGameRules();

	if (pGameRules == NULL) {
		return pContext->ThrowNativeError("GameRules not available before map is loaded");
	}

	int iOffset = -1;
	if (!g_pGameConf->GetOffset("VersusMaxCompletionScore", &iOffset) || !iOffset) {
		return pContext->ThrowNativeError("Could not read 'VersusMaxCompletionScore' offset from GameConf");
	}

	L4D_DEBUG_LOG("Reading Versus MaxDistance");

	unsigned int *pVersusMaxCompletionScore = (unsigned int *)((unsigned char *)pGameRules + iOffset);

	*pVersusMaxCompletionScore = params[1];

	return 1;
}

// native L4D_GetTeamScore(logical_team, campaign_score=false)
cell_t L4D_GetTeamScore(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorGameRules::GetTeamScore(int, bool)
	if (!pWrapper) {
		PassInfo ret;
		ret.flags = PASSFLAG_BYVAL;
		ret.size = sizeof(int);
		ret.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CTerrorGameRules::GetTeamScore",
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(int); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(bool); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &ret, pass, /*numparams*/2));
	}

	//sanity check that the team index is valid
	if (params[1] != VersusTeam_A && params[1] != VersusTeam_B) {
		return pContext->ThrowNativeError("Logical team %d is invalid", params[1]);
	}

	//campaign_score is a boolean so should be 0 (use round score) or 1 only
	if (params[2] != ScoreTypeRound && params[2] != ScoreTypeCampaign) {
		return pContext->ThrowNativeError("campaign_score %d is invalid, needs to be 0 or 1", params[2]);
	}

	CTerrorGameRules *pGameRules = (CTerrorGameRules *)g_pSDKTools->GetGameRules();

	if (pGameRules == NULL) {
		return pContext->ThrowNativeError("GameRules not available before map is loaded");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CTerrorGameRules *) + sizeof(int) + sizeof(bool)];
	unsigned char *vptr = vstk;

	*(CTerrorGameRules **)vptr = pGameRules;
	vptr += sizeof(CTerrorGameRules *);

	*(int *)vptr = params[1];
	vptr += sizeof(int);

	*(bool *)vptr = params[2] ? true : false;

	cell_t retbuffer;

	L4D_DEBUG_LOG("Going to execute CTerrorGameRules::GetTeamScore");
	pWrapper->Execute(vstk, &retbuffer);
	L4D_DEBUG_LOG("Invoked CTerrorGameRules::GetTeamScore");

	return retbuffer;
}

// native int L4D2_GetVersusCompletionPlayer(int client)
cell_t L4D2_GetVersusCompletionPlayer(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CTerrorGameRules::GetVersusCompletion(CTerrorPlayer *pPlayer);
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(int);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CTerrorGameRules::GetVersusCompletion",
			PassInfo passInfo; \
			passInfo.flags = PASSFLAG_BYVAL; \
			passInfo.size = sizeof(CBaseEntity *); \
			passInfo.type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &retInfo, &passInfo, /*numparams*/1));
	}

	CTerrorGameRules *pGameRules = (CTerrorGameRules *)g_pSDKTools->GetGameRules();

	if (pGameRules == NULL) {
		return pContext->ThrowNativeError("GameRules not available before map is loaded");
	}

	int iTarget = params[1];
	CBaseEntity *pTarget = UTIL_GetCBaseEntity(iTarget, true);

	if (pTarget == NULL) {
		return pContext->ThrowNativeError("Invalid player %d", iTarget);
	}

	unsigned char vstk[sizeof(CTerrorGameRules *) + sizeof(CBaseEntity *)];
	unsigned char *vptr = vstk;

	*(CTerrorGameRules **)vptr = pGameRules;
	vptr += sizeof(CTerrorGameRules *);

	*(CBaseEntity **)vptr = pTarget;

	cell_t retbuffer = 0;

	L4D_DEBUG_LOG("Going to execute CTerrorGameRules::GetVersusCompletion");
	pWrapper->Execute(vstk, &retbuffer);
	L4D_DEBUG_LOG("Invoked CTerrorGameRules::GetVersusCompletion");

	return retbuffer;
}

// native bool L4D2_IsGenericCooperativeMode();
cell_t L4D2_IsGenericCooperativeMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsGenericCooperativeMode();
}

// native bool L4D_IsCoopMode();
cell_t L4D_IsCoopMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsCoopMode();
}

// native bool L4D2_IsRealismMode();
cell_t L4D2_IsRealismMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsRealismMode();
}

// native bool L4D_IsSurvivalMode();
cell_t L4D_IsSurvivalMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsSurvivalMode();
}

// native bool L4D2_IsScavengeMode();
cell_t L4D2_IsScavengeMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsScavengeMode();
}

// native bool L4D_IsVersusMode();
cell_t L4D_IsVersusMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsVersusMode();
}

// native bool L4D2_IsSingleChapterMode();
cell_t L4D2_IsSingleChapterMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsSingleChapterMode();
}

// native bool L4D2_HasConfigurableDifficultySetting();
cell_t L4D2_HasConfigurableDifficultySetting(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::HasConfigurableDifficultySetting();
}

// native bool L4D_HasPlayerControlledZombies();
cell_t L4D_HasPlayerControlledZombies(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::HasPlayerControlledZombies();
}

// native bool L4D_IsMissionFinalMap();
cell_t L4D_IsMissionFinalMap(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsMissionFinalMap();
}

sp_nativeinfo_t g_L4DoGameRulesNatives[] =
{
	{"L4D_NotifyNetworkStateChanged",			L4D_NotifyNetworkStateChanged},
	{"L4D_GetVersusMaxCompletionScore",			L4D_GetVersusMaxCompletionScore},
	{"L4D_SetVersusMaxCompletionScore",			L4D_SetVersusMaxCompletionScore},
	{"L4D_GetTeamScore",						L4D_GetTeamScore},
	{"L4D2_GetVersusCompletionPlayer",			L4D2_GetVersusCompletionPlayer},
	{"L4D2_IsGenericCooperativeMode",			L4D2_IsGenericCooperativeMode},
	{"L4D_IsCoopMode",							L4D_IsCoopMode},
	{"L4D2_IsRealismMode",						L4D2_IsRealismMode},
	{"L4D_IsSurvivalMode",						L4D_IsSurvivalMode},
	{"L4D2_IsScavengeMode",						L4D2_IsScavengeMode},
	{"L4D_IsVersusMode",						L4D_IsVersusMode},
	{"L4D2_IsSingleChapterMode",				L4D2_IsSingleChapterMode},
	{"L4D2_HasConfigurableDifficultySetting",	L4D2_HasConfigurableDifficultySetting},
	{"L4D_HasPlayerControlledZombies",			L4D_HasPlayerControlledZombies},
	{"L4D_IsMissionFinalMap",					L4D_IsMissionFinalMap},
	{NULL,										NULL}
};
