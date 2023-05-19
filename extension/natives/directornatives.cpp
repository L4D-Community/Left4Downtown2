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
#include "vglobals.h"
#include "util.h"

// native bool L4D_IsFirstMapInScenario();
cell_t L4D_IsFirstMapInScenario(IPluginContext *pContext, const cell_t *params)
{
	return CDirectorWapper::IsFirstMapInScenario();
}

// native void L4D2_SendInRescueVehicle()
cell_t L4D2_SendInRescueVehicle(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CDirectorScriptedEventManager::SendInRescueVehicle(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirectorScriptedEventManager::SendInRescueVehicle", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, /*Pass*/NULL, /*numparams*/0));
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirectorScriptedEventManager* pScriptedeventmanager = (*g_ppDirector)->ScriptedEventManagerPtr;

	if (pScriptedeventmanager == NULL) {
		return pContext->ThrowNativeError("DirectorScriptedEventManager pointer is NULL");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirectorScriptedEventManager *)];
	unsigned char *vptr = vstk;

	*(CDirectorScriptedEventManager **)vptr = pScriptedeventmanager;

	L4D_DEBUG_LOG("Going to execute CDirectorScriptedEventManager::SendInRescueVehicle");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CDirectorScriptedEventManager::SendInRescueVehicle");

	return 1;
}

// native void L4D2_ChangeFinaleStage(int finaleType, const char[] arg);
cell_t L4D2_ChangeFinaleStage(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CDirectorScriptedEventManager::ChangeFinaleStage(CDirectorScriptedEventManager::FinaleStageType, char const *)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirectorScriptedEventManager::ChangeFinaleStage", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(int); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(char *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, /*Pass*/pass, /*numparams*/2));
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirectorScriptedEventManager *pScriptedeventmanager = (*g_ppDirector)->ScriptedEventManagerPtr;

	if (pScriptedeventmanager == NULL) {
		return pContext->ThrowNativeError("DirectorScriptedEventManager pointer is NULL");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirectorScriptedEventManager *) + sizeof(int) + sizeof(char *)];
	unsigned char *vptr = vstk;

	*(CDirectorScriptedEventManager **)vptr = pScriptedeventmanager;
	vptr += sizeof(CDirectorScriptedEventManager *);

	*(int *)vptr = params[1];
	vptr += sizeof(int *);

	char *arg = NULL;
	pContext->LocalToString(params[2], &arg);
	*(char **)vptr = arg;

	L4D_DEBUG_LOG("Going to execute CDirectorScriptedEventManager::ChangeFinaleStage");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CDirectorScriptedEventManager::ChangeFinaleStage");

	return 1;
}

// native float L4D_GetMobSpawnTimerRemaining();
cell_t L4D_GetMobSpawnTimerRemaining(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	L4D_DEBUG_LOG("Reading director mobspawntimer");

	return sp_ftoc(pDirector->MobSpawnTimer.GetRemainingTime());
}

// native float L4D_GetMobSpawnTimerDuration();
cell_t L4D_GetMobSpawnTimerDuration(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	L4D_DEBUG_LOG("Reading director mobspawntimer duration");

	return sp_ftoc(pDirector->MobSpawnTimer.GetCountdownDuration());
}

// native L4D_ResetMobTimer()
cell_t L4D_ResetMobTimer(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CDirector::ResetMobTimer(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirector::ResetMobTimer", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, /*Pass*/NULL, /*numparams*/0));
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirector *)];
	unsigned char *vptr = vstk;

	*(CDirector **)vptr = pDirector;

	L4D_DEBUG_LOG("Going to execute CDirector::ResetMobTimer");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CDirector::ResetMobTimer");

	return 1;
}

// native L4D_ScavengeBeginRoundSetupTime()
cell_t L4D_ScavengeBeginRoundSetupTime(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// Director::RestartScenario(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirectorScavengeMode_OnBeginRoundSetupTime", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, /*Pass*/NULL, /*numparams*/0));
	}

	/* Get the Director pointer */
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorScavengeMode *pDirectorScavengeMode = (*g_ppDirector)->ScavengeModePtr;
	if (pDirectorScavengeMode == NULL) {
		return pContext->ThrowNativeError("CDirectorScavengeMode not available--is this scavange mode?");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirectorScavengeMode *)];
	unsigned char *vptr = vstk;

	*(CDirectorScavengeMode **)vptr = pDirectorScavengeMode;

	L4D_DEBUG_LOG("Going to execute CDirectorScavengeMode_OnBeginRoundSetupTime");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CDirectorScavengeMode_OnBeginRoundSetupTime");

	return 1;
}

// native L4D_RestartScenarioFromVote(const String:map[])
cell_t L4D_RestartScenarioFromVote(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// Director::RestartScenario(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirector::RestartScenarioFromVote",
			PassInfo pass[1]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(char *); \
			pass[0].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, pass, /*numparams*/1));
	}

	/* Get the Director pointer */
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	L4D_DEBUG_LOG("Director pointer calculated to be 0x%x", pDirector);

	char *map = NULL;
	pContext->LocalToString(params[1], &map);

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirector *) + sizeof(char *)];
	unsigned char *vptr = vstk;

	*(CDirector **)vptr = pDirector;
	vptr += sizeof(CDirector *);

	*(char **)vptr = map;

	L4D_DEBUG_LOG("Going to execute CDirector::RestartScenarioFromVote");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked CDirector::RestartScenarioFromVote");

	return 1;
}

// native int L4D_GetHighestFlowSurvivor();
cell_t L4D_GetHighestFlowSurvivor(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CDirectorTacticalServices::GetHighestFlowSurvivor(TerrorNavArea::FlowType)const
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(CBaseEntity *);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CDirectorTacticalServices::GetHighestFlowSurvivor", \
			PassInfo passInfo; \
			passInfo.flags = PASSFLAG_BYVAL; \
			passInfo.size = sizeof(int); \
			passInfo.type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &retInfo, &passInfo, 1));

		L4D_DEBUG_LOG("Built call wrapper CDirectorTacticalServices::GetHighestFlowSurvivor");
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorTacticalServices *pDTacticalServices = (*g_ppDirector)->TacticalServicesPtr;

	if (pDTacticalServices == NULL) {
		return pContext->ThrowNativeError("CDirectorTacticalServices not available before map is loaded");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirectorTacticalServices *) + sizeof(int)];
	unsigned char *vptr = vstk;

	*(CDirectorTacticalServices **)vptr = pDTacticalServices;
	vptr += sizeof(CDirectorTacticalServices *);

	*(int *)vptr = 0;

	CBaseEntity *pPlayer = NULL;

	L4D_DEBUG_LOG("Going to execute CDirectorTacticalServices::GetHighestFlowSurvivor");
	pWrapper->Execute(vstk, &pPlayer);

	cell_t client = (pPlayer != NULL) ? IndexOfEdict(gameents->BaseEntityToEdict(pPlayer)) : 0;
	L4D_DEBUG_LOG("Invoked CDirectorTacticalServices::GetHighestFlowSurvivor, got back = client: %d, address: %08x", client, pPlayer);

	return client;
}

// native bool L4D2_AreTeamsFlipped()
cell_t L4D2_AreTeamsFlipped(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//CDirector::AreTeamsFlipped(void)
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(bool); //ret value in al on windows, eax on linux
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CDirector::AreTeamsFlipped", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &retInfo, /*paramInfo*/NULL, /*numparams*/0));
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(CDirector *)];
	unsigned char *vptr = vstk;

	*(CDirector **)vptr = pDirector;

	cell_t retbuffer = false;

	L4D_DEBUG_LOG("Going to execute CDirector::AreTeamsFlipped");
	pWrapper->Execute(vstk, &retbuffer);
	L4D_DEBUG_LOG("Invoked CDirector::AreTeamsFlipped");

	return retbuffer;
}

// native void L4D2_SwapTeams();
cell_t L4D2_SwapTeams(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// CDirector::SwapTeams(void)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("CDirector::SwapTeams", \
			pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, NULL, NULL, 0));
	}

	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	unsigned char vstk[sizeof(CDirector *)];
	unsigned char *vptr = vstk;

	*(CDirector **)vptr = pDirector;

	L4D_DEBUG_LOG("Going to execute CDirector::SwapTeams");
	pWrapper->Execute(vstk, NULL);
	L4D_DEBUG_LOG("Invoked CDirector::SwapTeams");

	return 1;
}

//native bool L4D_HasAnySurvivorLeftSafeArea();
cell_t L4D_HasAnySurvivorLeftSafeArea(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	return (pDirector->m_bHasSurvivorLeftSafeArea) ? true : false;
}

//native float L4D2_GetFurthestSurvivorFlow();
cell_t L4D2_GetFurthestSurvivorFlow(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	return sp_ftoc(pDirector->m_fFurthestSurvivorFlow);
}

// native int L4D2_GetTankCount();
cell_t L4D2_GetTankCount(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	return pDirector->m_iTankCount;
}

// native int L4D2_GetWitchCount();
cell_t L4D2_GetWitchCount(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *pDirector = *g_ppDirector;

	if (pDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	return pDirector->m_iWitchCount;
}

// native void L4D2_GetVersusCampaignScores(int scores[2]);
cell_t L4D2_GetVersusCampaignScores(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;

	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *scores;
	pContext->LocalToPhysAddr(params[1], &scores);

	scores[0] = pDirectorVersusMode->m_iCampaignScores[0];
	scores[1] = pDirectorVersusMode->m_iCampaignScores[1];

	return 1;
}

// native void L4D2_SetVersusCampaignScores(const int scores[2]);
cell_t L4D2_SetVersusCampaignScores(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;
	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *scores;
	pContext->LocalToPhysAddr(params[1], &scores);

	pDirectorVersusMode->m_iCampaignScores[0] = scores[0];
	pDirectorVersusMode->m_iCampaignScores[1] = scores[1];

	return 1;
}

// native void L4D2_GetVersusTankFlowPercent(float tankFlows[2])
cell_t L4D2_GetVersusTankFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;
	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);

	cell_flows[0] = sp_ftoc(pDirectorVersusMode->m_fTankSpawnFlowPercent[0]);
	cell_flows[1] = sp_ftoc(pDirectorVersusMode->m_fTankSpawnFlowPercent[1]);

	return 1;
}

// native void L4D2_SetVersusTankFlowPercent(const float tankFlows[2]);
cell_t L4D2_SetVersusTankFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;
	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);

	pDirectorVersusMode->m_fTankSpawnFlowPercent[0] = sp_ctof(cell_flows[0]);
	pDirectorVersusMode->m_fTankSpawnFlowPercent[1] = sp_ctof(cell_flows[1]);

	return 1;
}

// native void L4D2_GetVersusWitchFlowPercent(float witchFlows[2]);
cell_t L4D2_GetVersusWitchFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;
	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);

	cell_flows[0] = sp_ftoc(pDirectorVersusMode->m_fWitchSpawnFlowPercent[0]);
	cell_flows[1] = sp_ftoc(pDirectorVersusMode->m_fWitchSpawnFlowPercent[1]);

	return 1;
}

// native void L4D2_SetVersusWitchFlowPercent(const float witchFlows[2]);
cell_t L4D2_SetVersusWitchFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	if (*g_ppDirector == NULL) {
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}

	CDirectorVersusMode *pDirectorVersusMode = (*g_ppDirector)->VersusModePtr;
	if (pDirectorVersusMode == NULL) {
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}

	cell_t *cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);

	pDirectorVersusMode->m_fWitchSpawnFlowPercent[0] = sp_ctof(cell_flows[0]);
	pDirectorVersusMode->m_fWitchSpawnFlowPercent[1] = sp_ctof(cell_flows[1]);

	return 1;
}

sp_nativeinfo_t g_L4DoDirectorNatives[] =
{
	{"L4D_IsFirstMapInScenario",		L4D_IsFirstMapInScenario},
	{"L4D2_SendInRescueVehicle",		L4D2_SendInRescueVehicle},
	{"L4D2_ChangeFinaleStage",			L4D2_ChangeFinaleStage},
	{"L4D_GetMobSpawnTimerRemaining",	L4D_GetMobSpawnTimerRemaining},
	{"L4D_GetMobSpawnTimerDuration",	L4D_GetMobSpawnTimerDuration},
	{"L4D_ResetMobTimer",				L4D_ResetMobTimer},
	{"L4D_ScavengeBeginRoundSetupTime",	L4D_ScavengeBeginRoundSetupTime},
	{"L4D_RestartScenarioFromVote",		L4D_RestartScenarioFromVote},
	{"L4D_GetHighestFlowSurvivor",		L4D_GetHighestFlowSurvivor},
	{"L4D2_AreTeamsFlipped",			L4D2_AreTeamsFlipped},
	{"L4D2_SwapTeams",					L4D2_SwapTeams},
	{"L4D_HasAnySurvivorLeftSafeArea",	L4D_HasAnySurvivorLeftSafeArea},
	{"L4D2_GetFurthestSurvivorFlow",	L4D2_GetFurthestSurvivorFlow},
	{"L4D2_GetTankCount",				L4D2_GetTankCount},
	{"L4D2_GetWitchCount",				L4D2_GetWitchCount},
	{"L4D2_GetVersusCampaignScores",	L4D2_GetVersusCampaignScores},
	{"L4D2_SetVersusCampaignScores",	L4D2_SetVersusCampaignScores},
	{"L4D2_GetVersusTankFlowPercent",	L4D2_GetVersusTankFlowPercent},
	{"L4D2_SetVersusTankFlowPercent",	L4D2_SetVersusTankFlowPercent},
	{"L4D2_GetVersusWitchFlowPercent",	L4D2_GetVersusWitchFlowPercent},
	{"L4D2_SetVersusWitchFlowPercent",	L4D2_SetVersusWitchFlowPercent},
	{NULL,								NULL}
};
