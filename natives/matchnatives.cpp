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
#include "util.h"

// native bool L4D2_IsGenericCooperativeMode();
cell_t L4D2_IsGenericCooperativeMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsGenericCooperativeMode();
}

// native bool L4D_IsCoopMode();
cell_t L4D_IsCoopMode(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsCoopMode();
}

// native bool L4D2_IsRealismMode();
cell_t L4D2_IsRealismMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsRealismMode();
}

// native bool L4D_IsSurvivalMode();
cell_t L4D_IsSurvivalMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsSurvivalMode();
}

// native bool L4D2_IsScavengeMode();
cell_t L4D2_IsScavengeMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsScavengeMode();
}

// native bool L4D_IsVersusMode();
cell_t L4D_IsVersusMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsVersusMode();
}

// native bool L4D2_IsSingleChapterMode();
cell_t L4D2_IsSingleChapterMode(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::IsSingleChapterMode();
}

// native bool L4D2_HasConfigurableDifficultySetting();
cell_t L4D2_HasConfigurableDifficultySetting(IPluginContext* pContext, const cell_t* params)
{
	return CTerrorGameRules::HasConfigurableDifficultySetting();
}

// native bool L4D_HasPlayerControlledZombies();
cell_t L4D_HasPlayerControlledZombies(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::HasPlayerControlledZombies();
}

// native bool L4D_IsFirstMapInScenario();
cell_t L4D_IsFirstMapInScenario(IPluginContext *pContext, const cell_t *params)
{
	return CDirectorWapper::IsFirstMapInScenario();
}

// native bool L4D_IsMissionFinalMap();
cell_t L4D_IsMissionFinalMap(IPluginContext *pContext, const cell_t *params)
{
	return CTerrorGameRules::IsMissionFinalMap();
}

sp_nativeinfo_t g_L4DoMatchNatives[] =
{
	{"L4D2_IsGenericCooperativeMode",			L4D2_IsGenericCooperativeMode},
	{"L4D_IsCoopMode",							L4D_IsCoopMode},
	{"L4D2_IsRealismMode",						L4D2_IsRealismMode},
	{"L4D_IsSurvivalMode",						L4D_IsSurvivalMode},
	{"L4D2_IsScavengeMode",						L4D2_IsScavengeMode},
	{"L4D_IsVersusMode",						L4D_IsVersusMode},
	{"L4D2_IsSingleChapterMode",				L4D2_IsSingleChapterMode},
	{"L4D2_HasConfigurableDifficultySetting",	L4D2_HasConfigurableDifficultySetting},
	{"L4D_HasPlayerControlledZombies",			L4D_HasPlayerControlledZombies},
	{"L4D_IsFirstMapInScenario",				L4D_IsFirstMapInScenario},
	{"L4D_IsMissionFinalMap",					L4D_IsMissionFinalMap},
	{NULL,										NULL}
};
