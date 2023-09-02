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

#include <string.h>

#include "extension.h"
#include "iplayerinfo.h"
#include "vglobals.h"
#include "util.h"
//#include "l4d2sdk/convar_public.h"

#include <amtl/os/am-shared-library.h>

#include "codepatch/patchmanager.h"
#include "codepatch/autopatch.h"

#include "detours/change_finale_stage.h"
#include "detours/choose_victim.h"
#include "detours/clear_team_scores.h"
#include "detours/cthrow_activate_ability.h"
#include "detours/end_versus_mode_round.h"
#include "detours/fast_get_survivor_set.h"
#include "detours/find_scavenge_item.h"
#include "detours/first_survivor_left_safe_area.h"
#include "detours/get_crouch_top_speed.h"
#include "detours/get_mission_info.h"
#include "detours/get_mission_versus_boss_spawning.h"
#include "detours/get_run_top_speed.h"
#include "detours/get_script_value_float.h"
#include "detours/get_script_value_int.h"
#include "detours/get_script_value_string.h"
#include "detours/get_survivor_set.h"
#include "detours/get_walk_top_speed.h"
#include "detours/has_configurable_difficulty.h"
#include "detours/inferno_spread.h"
#include "detours/mob_rush_start.h"
#include "detours/on_enter_ghost_state.h"
#include "detours/on_ledgegrabbed.h"
#include "detours/on_knocked_down.h"
#include "detours/on_player_hit.h"
#include "detours/on_revived.h"
#include "detours/on_stagger.h"
#include "detours/replace_tank.h"
#include "detours/select_weighted_sequence.h" //for SelectTankAttack
#include "detours/send_in_rescue_vehicle.h"
#include "detours/set_campaign_scores.h"
#include "detours/shoved_by_pounce_landing.h"
#include "detours/shoved_by_survivor.h"
#include "detours/spawn_it_mob.h"
#include "detours/spawn_mob.h"
#include "detours/spawn_special.h"
#include "detours/spawn_tank.h"
#include "detours/spawn_witch.h"
#include "detours/spawn_witchbride.h"
#include "detours/start_melee_swing.h"
#include "detours/terror_weapon_hit.h"
#include "detours/try_offering_tank_bot.h"
#include "detours/use_healing_items.h"
#include "detours/water_move.h"

#include "addons_disabler.h"

#define GAMECONFIG_FILE "left4downtown.l4d2"

#if !defined(_WIN32) && SOURCE_ENGINE == SE_LEFT4DEAD2
	#define MATCHMAKING_LIB_DS_SUFFIX "_ds_srv"
#else
	#define MATCHMAKING_LIB_DS_SUFFIX "_ds"
#endif

Left4Downtown g_Left4DowntownTools;		/**< Global singleton for extension's main interface */
IGameConfig *g_pGameConf = NULL;
IGameConfig *g_pGameConfSDKTools = NULL;
IBinTools *g_pBinTools = NULL;
CBaseServer *g_pServer = NULL; //ptr to CBaseServer
ISDKTools *g_pSDKTools = NULL;
IServerGameEnts *gameents = NULL;
CGlobalVars *gpGlobals;
ConVar *mp_gamemode = NULL;
IMatchFramework *g_pMatchFramework = NULL;
IMatchExtL4D *g_pMatchExtL4D = NULL;

bool g_bRoundEnd = false;

ICvar *icvar = NULL;
SMEXT_LINK(&g_Left4DowntownTools);

ConVar g_Version("left4downtown_version", SMEXT_CONF_VERSION, FCVAR_SPONLY|FCVAR_NOTIFY, "Left 4 Downtown Extension Version");
ConVar g_AddonsEclipse("l4d2_addons_eclipse", "-1", FCVAR_SPONLY|FCVAR_NOTIFY, "Addons Manager(-1: use addonconfig; 0/1: override addonconfig");
ConVar g_UnlockMelees("l4d2_unlock_melees", "0", FCVAR_SPONLY|FCVAR_NOTIFY, "1: Unlock all melees, 0: don't (set to 0 if you're experiencing crashes on modes other than versus)");

PatchManager g_PatchManager;

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */
bool Left4Downtown::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	//only load extension for l4d2
	if (strcmp(g_pSM->GetGameFolderName(), "left4dead2") != 0) {
		UTIL_Format(error, maxlength, "Cannot Load Left 4 Downtown Ext on mods other than L4D2");
		return false;
	}

	mp_gamemode = g_pCVar->FindVar("mp_gamemode");
	if (mp_gamemode == NULL) {
		UTIL_Format(error, maxlength, "Cvar \"mp_gamemode\" not found");
		return false;
	}

	if (!SetupFromMatchmakingLibrary(error, maxlength)) {
		return false;
	}

	//load sigscans and offsets, etc from our gamedata file
	char conf_error[255] = "";
	if (!gameconfs->LoadGameConfigFile(GAMECONFIG_FILE, &g_pGameConf, conf_error, sizeof(conf_error))) {
		if (conf_error[0]) {
			UTIL_Format(error, maxlength, "Could not read " GAMECONFIG_FILE ".txt: %s", conf_error);
		}
		return false;
	}

	//load sigscans and offsets from the sdktools gamedata file
	if (!gameconfs->LoadGameConfigFile("sdktools.games", &g_pGameConfSDKTools, conf_error, sizeof(conf_error))) {
		return false;
	}

	if (!L4D2_GetOffsets(error, maxlength)) {
		return false;
	}

	sharesys->AddDependency(myself, "bintools.ext", true, true);
	sharesys->RegisterLibrary(myself, "left4downtown2");
	
	sharesys->AddNatives(myself, g_L4DoNatives);
	sharesys->AddNatives(myself, g_L4DoTimerNatives);
	sharesys->AddNatives(myself, g_L4DoWeaponNatives);
	sharesys->AddNatives(myself, g_L4DoMeleeWeaponNatives);
	sharesys->AddNatives(myself, g_L4DoDirectorNatives);
	sharesys->AddNatives(myself, g_L4DoGameRulesNatives);
	sharesys->AddNatives(myself, g_L4DoPlayerNatives);
	sharesys->AddNatives(myself, g_L4DoZombieManagerNatives);
	sharesys->AddNatives(myself, g_L4DoEngineNatives);

	CreateForwards();

	plsys->AddPluginsListener(&g_Left4DowntownTools);
	playerhelpers->RegisterCommandTargetProcessor(&g_Left4DowntownTools);

	Detour::Init(g_pSM->GetScriptingEngine(), g_pGameConf);

	return true;
}

void Left4Downtown::SDK_OnAllLoaded()
{
	SM_GET_LATE_IFACE(BINTOOLS, g_pBinTools);
	SM_GET_LATE_IFACE(SDKTOOLS, g_pSDKTools);

	if (!g_pBinTools || !g_pSDKTools) {
		//L4D_DEBUG_LOG("Failed to load bintools or failed to load sdktools");
		g_pSM->LogError(myself, "Failed to load bintools or failed to load sdktools");
		return;
	}

	CBaseServer *pServer = (CBaseServer*)g_pSDKTools->GetIServer();

	//reading out server->GetName() we consistently seem to get (the same?)
	//garbage characters. this is possibly causing a crash on windows servers
	//when a player connects. so lets not read the server name :(
	//L4D_DEBUG_LOG("Server name is %s", server->GetName());
	if (pServer == NULL) {
		g_pSM->LogError(myself, "Couldn't find IServer instance!");
	}

	L4D_DEBUG_LOG("Address of IServer is %p", pServer);

	g_pServer = pServer;

	InitializeValveGlobals();

	g_AddonsEclipse.InstallChangeCallback(::OnAddonsEclipseChanged);
	AddonsDisabler::AddonsEclipse = g_AddonsEclipse.GetInt();

	/*
	detour the witch/spawn spawns
	*/
	//automatically will unregister/cleanup themselves when the ext is unloaded

	g_PatchManager.Register(new AutoPatch<Detours::SpawnSpecial>());
	g_PatchManager.Register(new AutoPatch<Detours::SpawnTank>());
	g_PatchManager.Register(new AutoPatch<Detours::SpawnWitch>());
	g_PatchManager.Register(new AutoPatch<Detours::SpawnWitchBride>());
	g_PatchManager.Register(new AutoPatch<Detours::ClearTeamScores>());
	g_PatchManager.Register(new AutoPatch<Detours::SetCampaignScores>());
	g_PatchManager.Register(new AutoPatch<Detours::FirstSurvivorLeftSafeArea>());
	g_PatchManager.Register(new AutoPatch<Detours::GetScriptValueInt>());
	g_PatchManager.Register(new AutoPatch<Detours::GetScriptValueFloat>());
	g_PatchManager.Register(new AutoPatch<Detours::GetScriptValueString>());
	g_PatchManager.Register(new AutoPatch<Detours::TryOfferingTankBot>());
	g_PatchManager.Register(new AutoPatch<Detours::MobRushStart>());
	g_PatchManager.Register(new AutoPatch<Detours::SpawnITMob>());
	g_PatchManager.Register(new AutoPatch<Detours::SpawnMob>());
	g_PatchManager.Register(new AutoPatch<Detours::OnEnterGhostStateDetour>());
	g_PatchManager.Register(new AutoPatch<Detours::ShovedBySurvivor>());
	g_PatchManager.Register(new AutoPatch<Detours::GetCrouchTopSpeed>());
	g_PatchManager.Register(new AutoPatch<Detours::GetRunTopSpeed>());
	g_PatchManager.Register(new AutoPatch<Detours::GetWalkTopSpeed>());
	g_PatchManager.Register(new AutoPatch<Detours::HasConfigurableDifficulty>());
	g_PatchManager.Register(new AutoPatch<Detours::GetSurvivorSet>());
	g_PatchManager.Register(new AutoPatch<Detours::FastGetSurvivorSet>());
	g_PatchManager.Register(new AutoPatch<Detours::GetMissionVersusBossSpawning>());
	g_PatchManager.Register(new AutoPatch<Detours::CThrowActivate>());
	g_PatchManager.Register(new AutoPatch<Detours::BossZombiePlayerBotChooseVictim>());
	g_PatchManager.Register(new AutoPatch<Detours::StartMeleeSwing>());
	g_PatchManager.Register(new AutoPatch<Detours::ReplaceTank>());
	g_PatchManager.Register(new AutoPatch<Detours::UseHealingItems>());
	g_PatchManager.Register(new AutoPatch<Detours::FindScavengeItem>());
	g_PatchManager.Register(new AutoPatch<Detours::SendInRescueVehicle>());
	g_PatchManager.Register(new AutoPatch<Detours::ChangeFinaleStage>());
	g_PatchManager.Register(new AutoPatch<Detours::EndVersusModeRound>());
	g_PatchManager.Register(new AutoPatch<Detours::SelectWeightedSequence>());//for SelectTankAttack
	g_PatchManager.Register(new AutoPatch<Detours::Revived>());
	g_PatchManager.Register(new AutoPatch<Detours::WaterMove>());
	g_PatchManager.Register(new AutoPatch<Detours::PlayerStagger>());
	g_PatchManager.Register(new AutoPatch<Detours::TerrorWeaponHit>());
	g_PatchManager.Register(new AutoPatch<Detours::CTerrorGameRules>());
	g_PatchManager.Register(new AutoPatch<Detours::ShovedByPounceLanding>());
	g_PatchManager.Register(new AutoPatch<Detours::CBaseServer>());
	g_PatchManager.Register(new AutoPatch<Detours::CLedgeGrabbed>());
	g_PatchManager.Register(new AutoPatch<Detours::InfernoSpread>());
	g_PatchManager.Register(new AutoPatch<Detours::CKnockedDown>());
	g_PatchManager.Register(new AutoPatch<Detours::CPlayerHit>());
}

void Left4Downtown::SDK_OnUnload()
{
	gameconfs->CloseGameConfigFile(g_pGameConf);
	gameconfs->CloseGameConfigFile(g_pGameConfSDKTools);

	plsys->RemovePluginsListener(&g_Left4DowntownTools);
	playerhelpers->UnregisterCommandTargetProcessor(&g_Left4DowntownTools);

	AddonsDisabler::Unpatch();

	g_PatchManager.UnregisterAll();

	DestroyForwards();
}

void Left4Downtown::OnPluginLoaded(IPlugin *plugin)
{
	g_PatchManager.PatchAll();
}

void Left4Downtown::OnPluginUnloaded(IPlugin *plugin)
{
	// TODO: maybe there is a better way? need to rewrite the code ... :)
	g_PatchManager.UnpatchAll();
	g_PatchManager.PatchAll();
}

class BaseAccessor : public IConCommandBaseAccessor
{
public:
	bool RegisterConCommandBase(ConCommandBase *pCommandBase)
	{
		/* Always call META_REGCVAR instead of going through the engine. */
		return META_REGCVAR(pCommandBase);
	}
} s_BaseAccessor;


bool Left4Downtown::SDK_OnMetamodLoad(SourceMM::ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);

	g_pCVar = icvar;
	ConVar_Register(0, &s_BaseAccessor);

	GET_V_IFACE_ANY(GetServerFactory, gameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	gpGlobals = ismm->GetCGlobals();

	return true;
}

// From extension - https://github.com/shqke/imatchext
bool Left4Downtown::SetupFromMatchmakingLibrary(char *error, int maxlength)
{
	char path[PLATFORM_MAX_PATH];
	smutils->BuildPath(Path_Game, path, sizeof(path), "bin/matchmaking%s." PLATFORM_LIB_EXT, engine->IsDedicatedServer() ? MATCHMAKING_LIB_DS_SUFFIX : "");

	char libErr[256];
	ke::RefPtr<ke::SharedLib> mms_lib = ke::SharedLib::Open(path, libErr, sizeof(libErr));
	if (!mms_lib) {
		V_snprintf(error, maxlength, "Unable to load matchmaking library \"%s\" (error: \"%s\")", path, libErr);
		return false;
	}

	CreateInterfaceFn matchmakingFactory = mms_lib->get<CreateInterfaceFn>(CREATEINTERFACE_PROCNAME);
	if (matchmakingFactory == NULL) {
		V_snprintf(error, maxlength, "Unable to retrieve matchmaking factory (file: \"%s\")", path);
		return false;
	}

	g_pMatchFramework = static_cast<IMatchFramework *>(matchmakingFactory(IMATCHFRAMEWORK_VERSION_STRING, NULL));
	if (g_pMatchFramework == NULL) {
		V_snprintf(error, maxlength, "Unable to find interface \"" IMATCHFRAMEWORK_VERSION_STRING "\" (path: \"%s\")", path);
		return false;
	}

	g_pMatchExtL4D = static_cast<IMatchExtL4D *>(matchmakingFactory(IMATCHEXT_L4D_INTERFACE, NULL));
	if (g_pMatchExtL4D == NULL) {
		V_snprintf(error, maxlength, "Unable to find interface \"" IMATCHEXT_L4D_INTERFACE "\" (path: \"%s\")", path);
		return false;
	}

	return true;
}

bool Left4Downtown::ProcessCommandTarget(cmd_target_info_t *info)
{
	int max_clients;
	IPlayerInfo *pInfo;
	unsigned int team_index = 0;
	IGamePlayer *pPlayer, *pAdmin;

	if ((info->flags & COMMAND_FILTER_NO_MULTI) == COMMAND_FILTER_NO_MULTI) {
		return false;
	}

	if (info->admin) {
		if ((pAdmin = playerhelpers->GetGamePlayer(info->admin)) == NULL) {
			return false;
		}

		if (!pAdmin->IsInGame()) {
			return false;
		}
	} else {
		pAdmin = NULL;
	}

	if (strcmp(info->pattern, "@survivors") == 0 ) {
		team_index = 2;
	} else if (strcmp(info->pattern, "@infected") == 0) {
		team_index = 3;
	} else {
		return false;
	}

	info->num_targets = 0;

	max_clients = playerhelpers->GetMaxClients();
	for (int i = 1; i <= max_clients && (cell_t)info->num_targets < info->max_targets; i++) {
		if ((pPlayer = playerhelpers->GetGamePlayer(i)) == NULL){
			continue;
		}

		if (!pPlayer->IsInGame()) {
			continue;
		}

		if ((pInfo = pPlayer->GetPlayerInfo()) == NULL) {
			continue;
		}

		if (pInfo->GetTeamIndex() != (int)team_index) {
			continue;
		}

		if (playerhelpers->FilterCommandTarget(pAdmin, pPlayer, info->flags) != COMMAND_TARGET_VALID) {
			continue;
		}

		info->targets[info->num_targets] = i;
		info->num_targets++;
	}

	if (info->num_targets == 0) {
		info->reason = COMMAND_TARGET_EMPTY_FILTER;
	} else {
		info->reason = COMMAND_TARGET_VALID;
	}

	info->target_name_style = COMMAND_TARGETNAME_RAW;
	if (team_index == 2) {
		UTIL_Format(info->target_name, info->target_name_maxlength, "Survivors");
	} else if (team_index == 3) {
		UTIL_Format(info->target_name, info->target_name_maxlength, "Infected");
	}

	return true;
}
