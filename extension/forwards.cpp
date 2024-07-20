/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2021 Accelerator, 2021-2023 A1m`;
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

IForward* g_pFwdOnSpawnSpecial = NULL;
IForward* g_pFwdOnSpawnSpecialPost = NULL;
IForward* g_pFwdOnSpawnSpecialPostHandled = NULL;

IForward* g_pFwdOnSpawnTank = NULL;
IForward* g_pFwdOnSpawnTankPost = NULL;
IForward* g_pFwdOnSpawnTankPostHandled = NULL;

IForward* g_pFwdOnSpawnWitch = NULL;
IForward* g_pFwdOnSpawnWitchPost = NULL;
IForward* g_pFwdOnSpawnWitchBride = NULL;
IForward* g_pFwdOnSpawnWitchBridePost = NULL;
IForward* g_pFwdOnClearTeamScores = NULL;
IForward* g_pFwdOnSetCampaignScores = NULL;

IForward* g_pFwdOnFirstSurvivorLeftSafeArea = NULL;
IForward* g_pFwdOnFirstSurvivorLeftSafeAreaPost = NULL;
IForward* g_pFwdOnFirstSurvivorLeftSafeAreaPostHandled = NULL;

IForward* g_pFwdOnGetScriptValueInt = NULL;
IForward* g_pFwdOnGetScriptValueFloat = NULL;
IForward* g_pFwdOnGetScriptValueString = NULL;

IForward* g_pFwdOnTryOfferingTankBot = NULL;
IForward* g_pFwdOnMobRushStart = NULL;
IForward* g_pFwdOnSpawnITMob = NULL;
IForward* g_pFwdOnSpawnMob = NULL;

IForward* g_pFwdOnEnterGhostState = NULL;
IForward* g_pFwdOnEnterGhostStatePre = NULL;
IForward* g_pFwdOnEnterGhostStatePostHandled = NULL;

IForward* g_pFwdOnShovedBySurvivor = NULL;
IForward* g_pFwdOnShovedBySurvivorPost = NULL;
IForward* g_pFwdOnShovedBySurvivorPostHandled = NULL;

IForward* g_pFwdOnGetCrouchTopSpeed = NULL;
IForward* g_pFwdOnGetRunTopSpeed = NULL;
IForward* g_pFwdOnGetWalkTopSpeed = NULL;
IForward* g_pFwdOnHasConfigurableDifficulty = NULL;
IForward* g_pFwdOnGetSurvivorSet = NULL;
IForward* g_pFwdOnFastGetSurvivorSet = NULL;
IForward* g_pFwdOnGetMissionVersusBossSpawning = NULL;
IForward* g_pFwdOnCThrowActivate = NULL;
IForward* g_pFwdOnStartMeleeSwing = NULL;
IForward* g_pFwdOnReplaceTank = NULL;
IForward* g_pFwdOnUseHealingItems = NULL;
IForward* g_pFwdOnFindScavengeItem = NULL;
IForward* g_pFwdOnSendInRescueVehicle = NULL;
IForward* g_pFwdOnChangeFinaleStage = NULL;
IForward* g_pFwdOnEndVersusModeRound = NULL;
IForward* g_pFwdOnEndVersusModeRound_Post = NULL;
IForward* g_pFwdOnSelectTankAttack = NULL;
IForward* g_pFwdOnRevived = NULL;
IForward* g_pFwdOnWaterMove = NULL;
IForward* g_pFwdOnPlayerStagger = NULL;
IForward* g_pFwdOnTerrorWeaponHit = NULL;
IForward* g_pFwdAddonsDisabler = NULL;
IForward* g_pFwdOnShovedByPounceLanding = NULL;
IForward* g_pFwdOnChooseVictim = NULL;
IForward* g_pFwdOnLedgeGrabbed = NULL;
IForward* g_pFwdInfernoSpread = NULL;

IForward* g_pFwdOnKnockedDown = NULL;
IForward* g_pFwdOnKnockedDownPost = NULL;
IForward* g_pFwdOnKnockedDownPostHandled = NULL;

IForward* g_pFwdOnPlayerHit = NULL;
IForward* g_pFwdOnPlayerHitPost = NULL;
IForward* g_pFwdOnPlayerHitPostHandled = NULL;

void Left4Downtown::CreateForwards()
{
	g_pFwdOnSpawnSpecial = forwards->CreateForward("L4D_OnSpawnSpecial", ET_Event, 3, /*types*/NULL, Param_CellByRef, Param_Array, Param_Array);
	g_pFwdOnSpawnSpecialPost = forwards->CreateForward("L4D_OnSpawnSpecial_Post", ET_Ignore, 4, /*types*/NULL, Param_Cell, Param_Cell, Param_Array, Param_Array);
	g_pFwdOnSpawnSpecialPostHandled = forwards->CreateForward("L4D_OnSpawnSpecial_PostHandled", ET_Ignore, 4, /*types*/NULL, Param_Cell, Param_Cell, Param_Array, Param_Array);

	g_pFwdOnSpawnTank = forwards->CreateForward("L4D_OnSpawnTank", ET_Event, 2, /*types*/NULL, Param_Array, Param_Array);
	g_pFwdOnSpawnTankPost = forwards->CreateForward("L4D_OnSpawnTank_Post", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Array, Param_Array);
	g_pFwdOnSpawnTankPostHandled = forwards->CreateForward("L4D_OnSpawnTank_PostHandled", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Array, Param_Array);

	g_pFwdOnSpawnWitch = forwards->CreateForward("L4D_OnSpawnWitch", ET_Event, 2, /*types*/NULL, Param_Array, Param_Array);
	g_pFwdOnSpawnWitchPost = forwards->CreateForward("L4D_OnSpawnWitch_Post", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Array, Param_Array);
	g_pFwdOnSpawnWitchBride = forwards->CreateForward("L4D2_OnSpawnWitchBride", ET_Event, 2, /*types*/NULL, Param_Array, Param_Array);
	g_pFwdOnSpawnWitchBridePost = forwards->CreateForward("L4D2_OnSpawnWitchBride_Post", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Array, Param_Array);
	g_pFwdOnClearTeamScores = forwards->CreateForward("L4D_OnClearTeamScores", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnSetCampaignScores = forwards->CreateForward("L4D_OnSetCampaignScores", ET_Event, 2, /*types*/NULL, Param_CellByRef, Param_CellByRef);
	
	g_pFwdOnFirstSurvivorLeftSafeArea = forwards->CreateForward("L4D_OnFirstSurvivorLeftSafeArea", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnFirstSurvivorLeftSafeAreaPost = forwards->CreateForward("L4D_OnFirstSurvivorLeftSafeArea_Post", ET_Ignore, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnFirstSurvivorLeftSafeAreaPostHandled = forwards->CreateForward("L4D_OnFirstSurvivorLeftSafeArea_PostHandled", ET_Ignore, 1, /*types*/NULL, Param_Cell);

	g_pFwdOnGetScriptValueInt = forwards->CreateForward("L4D_OnGetScriptValueInt", ET_Event, 2, /*types*/NULL, Param_String, Param_CellByRef);
	g_pFwdOnGetScriptValueFloat = forwards->CreateForward("L4D_OnGetScriptValueFloat", ET_Event, 2, /*types*/NULL, Param_String, Param_FloatByRef);
	g_pFwdOnGetScriptValueString = forwards->CreateForward("L4D_OnGetScriptValueString", ET_Event, 4, /*types*/NULL, Param_String, Param_String, Param_String, Param_CellByRef);
	
	g_pFwdOnTryOfferingTankBot = forwards->CreateForward("L4D_OnTryOfferingTankBot", ET_Event, 2, /*types*/NULL, Param_Cell, Param_CellByRef);
	g_pFwdOnMobRushStart = forwards->CreateForward("L4D_OnMobRushStart", ET_Event, 0, /*types*/NULL);
	g_pFwdOnSpawnITMob = forwards->CreateForward("L4D_OnSpawnITMob", ET_Event, 1, /*types*/NULL, Param_CellByRef);
	g_pFwdOnSpawnMob = forwards->CreateForward("L4D_OnSpawnMob", ET_Event, 1, /*types*/NULL, Param_CellByRef);
	
	g_pFwdOnEnterGhostStatePre = forwards->CreateForward("L4D_OnEnterGhostStatePre", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnEnterGhostState = forwards->CreateForward("L4D_OnEnterGhostState", ET_Ignore, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnEnterGhostStatePostHandled = forwards->CreateForward("L4D_OnEnterGhostState_PostHandled", ET_Ignore, 1, /*types*/NULL, Param_Cell);

	g_pFwdOnShovedBySurvivor = forwards->CreateForward("L4D_OnShovedBySurvivor", ET_Event, 3, /*types*/NULL, Param_Cell, Param_Cell, Param_Array);
	g_pFwdOnShovedBySurvivorPost = forwards->CreateForward("L4D_OnShovedBySurvivor_Post", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Cell, Param_Array);
	g_pFwdOnShovedBySurvivorPostHandled = forwards->CreateForward("L4D_OnShovedBySurvivor_PostHandled", ET_Ignore, 3, /*types*/NULL, Param_Cell, Param_Cell, Param_Array);

	g_pFwdOnGetCrouchTopSpeed = forwards->CreateForward("L4D_OnGetCrouchTopSpeed", ET_Event, 2, /*types*/NULL, Param_Cell, Param_FloatByRef);
	g_pFwdOnGetRunTopSpeed = forwards->CreateForward("L4D_OnGetRunTopSpeed", ET_Event, 2, /*types*/NULL, Param_Cell, Param_FloatByRef);
	g_pFwdOnGetWalkTopSpeed = forwards->CreateForward("L4D_OnGetWalkTopSpeed", ET_Event, 2, /*types*/NULL, Param_Cell, Param_FloatByRef);
	g_pFwdOnHasConfigurableDifficulty = forwards->CreateForward("L4D_OnHasConfigurableDifficulty", ET_Event, 1, /*types*/NULL, Param_CellByRef);
	g_pFwdOnGetSurvivorSet = forwards->CreateForward("L4D_OnGetSurvivorSet", ET_Event, 1, /*types*/NULL, Param_CellByRef);
	g_pFwdOnFastGetSurvivorSet = forwards->CreateForward("L4D_OnFastGetSurvivorSet", ET_Event, 1, /*types*/NULL, Param_CellByRef);
	g_pFwdOnGetMissionVersusBossSpawning = forwards->CreateForward("L4D_OnGetMissionVSBossSpawning", ET_Event, 4, /*types*/NULL, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef);
	g_pFwdOnCThrowActivate = forwards->CreateForward("L4D_OnCThrowActivate", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnStartMeleeSwing = forwards->CreateForward("L4D_OnStartMeleeSwing", ET_Event, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnReplaceTank = forwards->CreateForward("L4D_OnReplaceTank", ET_Event, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnUseHealingItems = forwards->CreateForward("L4D2_OnUseHealingItems", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnFindScavengeItem = forwards->CreateForward("L4D2_OnFindScavengeItem", ET_Event, 2, /*types*/NULL, Param_Cell, Param_CellByRef);
	g_pFwdOnSendInRescueVehicle = forwards->CreateForward("L4D2_OnSendInRescueVehicle", ET_Event, 0, /*types*/NULL);
	g_pFwdOnChangeFinaleStage = forwards->CreateForward("L4D2_OnChangeFinaleStage", ET_Event, 2, /*types*/NULL, Param_CellByRef, Param_String);
	g_pFwdOnEndVersusModeRound = forwards->CreateForward("L4D2_OnEndVersusModeRound", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnEndVersusModeRound_Post = forwards->CreateForward("L4D2_OnEndVersusModeRound_Post", ET_Ignore, 0, /*types*/NULL);
	g_pFwdOnSelectTankAttack = forwards->CreateForward("L4D2_OnSelectTankAttack", ET_Event, 2, /*types*/NULL, Param_Cell, Param_CellByRef);
	g_pFwdOnRevived = forwards->CreateForward("L4D2_OnRevived", ET_Ignore, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnWaterMove = forwards->CreateForward("L4D2_OnWaterMove", ET_Event, 1, /*types*/NULL, Param_Cell);
	g_pFwdOnPlayerStagger = forwards->CreateForward("L4D2_OnStagger", ET_Event, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnTerrorWeaponHit = forwards->CreateForward("L4D2_OnEntityShoved", ET_Event, 5, /*types*/NULL, Param_Cell, Param_Cell, Param_Cell, Param_Array, Param_Cell);
	g_pFwdAddonsDisabler = forwards->CreateForward("L4D2_OnClientDisableAddons", ET_Event, 1, /*types*/NULL, Param_String);
	g_pFwdOnShovedByPounceLanding = forwards->CreateForward("L4D2_OnPounceOrLeapStumble", ET_Event, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnChooseVictim = forwards->CreateForward("L4D2_OnChooseVictim", ET_Event, 2, /*types*/ NULL, Param_Cell, Param_CellByRef);
	g_pFwdOnLedgeGrabbed = forwards->CreateForward("L4D_OnLedgeGrabbed", ET_Event, 1, /*types*/ NULL, Param_Cell);
	g_pFwdInfernoSpread = forwards->CreateForward("L4D2_OnSpitSpread", ET_Event, 3, /*types*/NULL, Param_Cell, Param_Cell, Param_Array);
	
	g_pFwdOnKnockedDown = forwards->CreateForward("L4D_OnKnockedDown", ET_Event, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnKnockedDownPost = forwards->CreateForward("L4D_OnKnockedDown_Post", ET_Ignore, 2, /*types*/NULL, Param_Cell, Param_Cell);
	g_pFwdOnKnockedDownPostHandled = forwards->CreateForward("L4D_OnKnockedDown_PostHandled", ET_Ignore, 2, /*types*/NULL, Param_Cell, Param_Cell);

	g_pFwdOnPlayerHit = forwards->CreateForward("L4D_TankClaw_OnPlayerHit_Pre", ET_Event, 4, /*types*/NULL, Param_Cell, Param_Cell, Param_Cell, Param_Cell);
	g_pFwdOnPlayerHitPost = forwards->CreateForward("L4D_TankClaw_OnPlayerHit_Post", ET_Ignore, 4, /*types*/NULL, Param_Cell, Param_Cell, Param_Cell, Param_Cell);
	g_pFwdOnPlayerHitPostHandled = forwards->CreateForward("L4D_TankClaw_OnPlayerHit_PostHandled", ET_Ignore, 4, /*types*/NULL, Param_Cell, Param_Cell, Param_Cell, Param_Cell);
}

void Left4Downtown::DestroyForwards()
{
	forwards->ReleaseForward(g_pFwdOnSpawnSpecial);
	forwards->ReleaseForward(g_pFwdOnSpawnSpecialPost);
	forwards->ReleaseForward(g_pFwdOnSpawnSpecialPostHandled);

	forwards->ReleaseForward(g_pFwdOnSpawnTank);
	forwards->ReleaseForward(g_pFwdOnSpawnTankPost);
	forwards->ReleaseForward(g_pFwdOnSpawnTankPostHandled);

	forwards->ReleaseForward(g_pFwdOnSpawnWitch);
	forwards->ReleaseForward(g_pFwdOnSpawnWitchPost);
	forwards->ReleaseForward(g_pFwdOnSpawnWitchBride);
	forwards->ReleaseForward(g_pFwdOnSpawnWitchBridePost);
	forwards->ReleaseForward(g_pFwdOnClearTeamScores);
	forwards->ReleaseForward(g_pFwdOnSetCampaignScores);

	forwards->ReleaseForward(g_pFwdOnFirstSurvivorLeftSafeArea);
	forwards->ReleaseForward(g_pFwdOnFirstSurvivorLeftSafeAreaPost);
	forwards->ReleaseForward(g_pFwdOnFirstSurvivorLeftSafeAreaPostHandled);

	forwards->ReleaseForward(g_pFwdOnGetScriptValueInt);
	forwards->ReleaseForward(g_pFwdOnGetScriptValueFloat);
	forwards->ReleaseForward(g_pFwdOnGetScriptValueString);

	forwards->ReleaseForward(g_pFwdOnTryOfferingTankBot);
	forwards->ReleaseForward(g_pFwdOnMobRushStart);
	forwards->ReleaseForward(g_pFwdOnSpawnITMob);
	forwards->ReleaseForward(g_pFwdOnSpawnMob);

	forwards->ReleaseForward(g_pFwdOnEnterGhostStatePre);
	forwards->ReleaseForward(g_pFwdOnEnterGhostState);
	forwards->ReleaseForward(g_pFwdOnEnterGhostStatePostHandled);

	forwards->ReleaseForward(g_pFwdOnShovedBySurvivor);
	forwards->ReleaseForward(g_pFwdOnShovedBySurvivorPost);
	forwards->ReleaseForward(g_pFwdOnShovedBySurvivorPostHandled);

	forwards->ReleaseForward(g_pFwdOnGetCrouchTopSpeed);
	forwards->ReleaseForward(g_pFwdOnGetRunTopSpeed);
	forwards->ReleaseForward(g_pFwdOnGetWalkTopSpeed);
	forwards->ReleaseForward(g_pFwdOnHasConfigurableDifficulty);
	forwards->ReleaseForward(g_pFwdOnGetSurvivorSet);
	forwards->ReleaseForward(g_pFwdOnFastGetSurvivorSet);
	forwards->ReleaseForward(g_pFwdOnGetMissionVersusBossSpawning);
	forwards->ReleaseForward(g_pFwdOnCThrowActivate);
	forwards->ReleaseForward(g_pFwdOnStartMeleeSwing);
	forwards->ReleaseForward(g_pFwdOnReplaceTank);
	forwards->ReleaseForward(g_pFwdOnUseHealingItems);
	forwards->ReleaseForward(g_pFwdOnFindScavengeItem);
	forwards->ReleaseForward(g_pFwdOnSendInRescueVehicle);
	forwards->ReleaseForward(g_pFwdOnChangeFinaleStage);
	forwards->ReleaseForward(g_pFwdOnEndVersusModeRound);
	forwards->ReleaseForward(g_pFwdOnEndVersusModeRound_Post);
	forwards->ReleaseForward(g_pFwdOnSelectTankAttack);
	forwards->ReleaseForward(g_pFwdOnRevived);
	forwards->ReleaseForward(g_pFwdOnWaterMove);
	forwards->ReleaseForward(g_pFwdOnPlayerStagger);
	forwards->ReleaseForward(g_pFwdOnTerrorWeaponHit);
	forwards->ReleaseForward(g_pFwdAddonsDisabler);
	forwards->ReleaseForward(g_pFwdOnShovedByPounceLanding);
	forwards->ReleaseForward(g_pFwdOnChooseVictim);
	forwards->ReleaseForward(g_pFwdOnLedgeGrabbed);
	forwards->ReleaseForward(g_pFwdInfernoSpread);

	forwards->ReleaseForward(g_pFwdOnKnockedDown);
	forwards->ReleaseForward(g_pFwdOnKnockedDownPost);
	forwards->ReleaseForward(g_pFwdOnKnockedDownPostHandled);

	forwards->ReleaseForward(g_pFwdOnPlayerHit);
	forwards->ReleaseForward(g_pFwdOnPlayerHitPost);
	forwards->ReleaseForward(g_pFwdOnPlayerHitPostHandled);
}
