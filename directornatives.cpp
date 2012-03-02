/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown 2 SourceMod Extension
 * Copyright (C) 2010 Michael "ProdigySim" Busby.
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
 
#include "extension.h"
#include "vglobals.h"
#include "sm_trie_tpl.h"

KTrie<int> g_KT_CDirectorVars;
KTrie<int> g_KT_CDirectorVersusModeVars;
KTrie<int> g_KT_CDirectorScavengeModeVars;

typedef enum _DirectorType {
	// Base director class
	DT_CDirector,
	// Director class with versus-related vars
	DT_CDirectorVersusMode,
	// Scavenge Director
	DT_CDirectorScavengeMode,
} DirectorType;

void InitializeDirectorNatives()
{
#define QUICK_ADDVAR(clsname,name) g_KT_##clsname##Vars.insert(#name, offsetof(clsname,name))

	QUICK_ADDVAR(CDirector,m_fMobSpawnSize);
	QUICK_ADDVAR(CDirector,m_iTankCount);
	QUICK_ADDVAR(CDirector,m_fTankFlowDistance);
	QUICK_ADDVAR(CDirector,m_fMobSpawnSize);
	QUICK_ADDVAR(CDirector,m_bIsFirstRoundFinished);
	QUICK_ADDVAR(CDirector,m_bIsSecondRoundFinished);

	QUICK_ADDVAR(CDirectorVersusMode,m_bVersusRoundInProgress);
	QUICK_ADDVAR(CDirectorVersusMode,m_bFirstMap);
	QUICK_ADDVAR(CDirectorVersusMode,m_bTransitioning);
	QUICK_ADDVAR(CDirectorVersusMode,m_iCampaignScores);
	QUICK_ADDVAR(CDirectorVersusMode,m_fTankSpawnFlowPercent);
	QUICK_ADDVAR(CDirectorVersusMode,m_fWitchSpawnFlowPercent);
	QUICK_ADDVAR(CDirectorVersusMode,m_bTankThisRound);
	QUICK_ADDVAR(CDirectorVersusMode,m_bWitchThisRound);
	QUICK_ADDVAR(CDirectorVersusMode,m_fFinaleTravelCompletionValue);
	QUICK_ADDVAR(CDirectorVersusMode,m_iFinaleTriggerVar2);
	QUICK_ADDVAR(CDirectorVersusMode,m_bInFinaleMap);
	QUICK_ADDVAR(CDirectorVersusMode,m_iNumMarkersReached);
	QUICK_ADDVAR(CDirectorVersusMode,m_iMarkersCount);

	QUICK_ADDVAR(CDirectorScavengeMode,m_bScavengeRoundInProgress);
	QUICK_ADDVAR(CDirectorScavengeMode,m_bAreTeamsSwapped);
	QUICK_ADDVAR(CDirectorScavengeMode,m_bInOvertime);
	QUICK_ADDVAR(CDirectorScavengeMode,m_bInOvertime2);

#undef QUICK_ADDVAR
}

void * GetDirectorPointer(DirectorType director)
{
	if (g_pDirector == NULL)
	{
		return NULL;
	}
	if (*g_pDirector == NULL)
	{
		return NULL;
	}
	switch(director)
	{
		case DT_CDirector:
			return (void*)*g_pDirector;
		case DT_CDirectorVersusMode:
			return (void*)((*g_pDirector)->VersusModePtr);
		case DT_CDirectorScavengeMode:
			return (void*)((*g_pDirector)->ScavengeModePtr);
		default:
			return NULL;
	}
}

size_t GetDirectorSize(DirectorType director)
{
	switch(director)
	{
		case DT_CDirector:
			return sizeof(CDirector);
		case DT_CDirectorVersusMode:
			return sizeof(CDirectorVersusMode);
		case DT_CDirectorScavengeMode:
			return sizeof(CDirectorScavengeMode);
		default:
			return 0;
	}
}

cell_t L4D2_GetTankCount(IPluginContext *pContext, const cell_t *params)
{
	if (g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}

	CDirector *director = *g_pDirector;

	if (director == NULL)
	{
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}
	return director->m_iTankCount;
}

cell_t L4D2_GetVersusCampaignScores(IPluginContext *pContext, const cell_t *params)
{
	if (g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}
	if (*g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}
	CDirectorVersusMode * director=(*g_pDirector)->VersusModePtr;
	if(director == NULL)
	{
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}
	cell_t * scores;
	pContext->LocalToPhysAddr(params[1], &scores);
	scores[0]=director->m_iCampaignScores[0];
	scores[1]=director->m_iCampaignScores[1];
	return 0;
}

cell_t L4D2_SetVersusCampaignScores(IPluginContext *pContext, const cell_t *params)
{
	if (g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}
	if (*g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}
	CDirectorVersusMode * director=(*g_pDirector)->VersusModePtr;
	if(director == NULL)
	{
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}
	cell_t * scores;
	pContext->LocalToPhysAddr(params[1], &scores);
	director->m_iCampaignScores[0]=scores[0];
	director->m_iCampaignScores[1]=scores[1];
	return 0;
}

cell_t L4D2_GetVersusTankFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}
	if (*g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}
	CDirectorVersusMode * director=(*g_pDirector)->VersusModePtr;
	if(director == NULL)
	{
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}
	cell_t * cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);
	cell_flows[0]=sp_ftoc(director->m_fTankSpawnFlowPercent[0]);
	cell_flows[1]=sp_ftoc(director->m_fTankSpawnFlowPercent[1]);
	return 0;
}

cell_t L4D2_SetVersusTankFlowPercent(IPluginContext *pContext, const cell_t *params)
{
	if (g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director unsupported or not available; file a bug report");
	}
	if (*g_pDirector == NULL)
	{
		return pContext->ThrowNativeError("Director not available before map is loaded");
	}
	CDirectorVersusMode * director=(*g_pDirector)->VersusModePtr;
	if(director == NULL)
	{
		return pContext->ThrowNativeError("DirectorVersusMode not available--is this versus mode?");
	}
	cell_t * cell_flows;
	pContext->LocalToPhysAddr(params[1], &cell_flows);
	director->m_fTankSpawnFlowPercent[0]=sp_ctof(cell_flows[0]);
	director->m_fTankSpawnFlowPercent[1]=sp_ctof(cell_flows[1]);
	return 0;
}

cell_t L4D2_Director_GetDataInt(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	return *(int *)(director + offset);
}

cell_t L4D2_Director_GetDataFloat(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	return sp_ftoc(*(float *)(director + offset));
}

cell_t L4D2_Director_GetDataBool(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	return *(director + offset);
}

cell_t L4D2_Director_SetDataInt(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	
	*(int *)(director + offset) = params[3];
	return 0;
}

cell_t L4D2_Director_SetDataFloat(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	
	*(float *)(director + offset) = sp_ctof(params[3]);
	return 0;
}

cell_t L4D2_Director_SetDataBool(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char * director = (char*)GetDirectorPointer(dt);
	if(director == NULL)
	{
		return pContext->ThrowNativeError("Director not found or DirectorType currently unavailable");
	}
	int offset = params[2];
	if(offset >= (int)GetDirectorSize(dt) || offset < 0)
	{
		return pContext->ThrowNativeError("Invalid offset specified (outside of specified director) %d [0,%d)", offset, GetDirectorSize(dt));
	}
	
	*(char *)(director + offset) = params[3] & 0xff;
	return 0;
}

cell_t L4D2_Director_GetVariableOffset(IPluginContext *pContext, const cell_t *params)
{
	DirectorType dt = static_cast<DirectorType>(params[1]);
	char *varname = NULL;
	pContext->LocalToString(params[2], &varname);
	
	KTrie<int> * vTrie;
	switch(dt)
	{
		case DT_CDirector:
			vTrie = &g_KT_CDirectorVars;
			break;
		case DT_CDirectorVersusMode:
			vTrie = &g_KT_CDirectorVersusModeVars;
			break;
		case DT_CDirectorScavengeMode:
			vTrie = &g_KT_CDirectorScavengeModeVars;
			break;
		default:
			return pContext->ThrowNativeError("Invalid DirectorType specified");
	}
	int * pOffset = vTrie->retrieve(varname);
	return pOffset ? *pOffset : -1;
}

sp_nativeinfo_t  g_L4DoDirectorNatives[] = 
{
	{"L4D2_GetTankCount",				L4D2_GetTankCount},
	{"L4D2_GetVersusCampaignScores",	L4D2_GetVersusCampaignScores},
	{"L4D2_SetVersusCampaignScores",	L4D2_SetVersusCampaignScores},
	{"L4D2_GetVersusTankFlowPercent",	L4D2_GetVersusTankFlowPercent},
	{"L4D2_SetVersusTankFlowPercent",	L4D2_SetVersusTankFlowPercent},
	{"L4D2_Director_GetDataInt",		L4D2_Director_GetDataInt},
	{"L4D2_Director_GetDataFloat",		L4D2_Director_GetDataFloat},
	{"L4D2_Director_GetDataBool",		L4D2_Director_GetDataBool},
	{"L4D2_Director_SetDataInt",		L4D2_Director_SetDataInt},
	{"L4D2_Director_SetDataFloat",		L4D2_Director_SetDataFloat},
	{"L4D2_Director_SetDataBool",		L4D2_Director_SetDataBool},
	{"L4D2_Director_GetVariableOffset", L4D2_Director_GetVariableOffset},
	{NULL,										NULL}
};
