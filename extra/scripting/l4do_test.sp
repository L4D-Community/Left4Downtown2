#include <sourcemod>
#include <sdktools>

//set to 1 to require left4downtown
//set to 0 to just work without it (eg check gameconf)
#define USE_NATIVES 1

#if USE_NATIVES
#include "left4downtown.inc"
#endif

#define TEST_DEBUG 1
#define TEST_DEBUG_LOG 1

#define TEAM_SURVIVOR 2

#define GAMECONFIG_FILE "left4downtown.l4d2"
#define ENTITY_NAME_LENGTH 64

// TerrorNavArea
// Bitflags for TerrorNavArea.SpawnAttributes
enum
{
	TERROR_NAV_EMPTY = 2,
	TERROR_NAV_STOP = 4,
	TERROR_NAV_FINALE = 0x40,
	TERROR_NAV_BATTLEFIELD = 0x100,
	TERROR_NAV_PLAYER_START = 0x80,
	TERROR_NAV_IGNORE_VISIBILITY = 0x200,
	TERROR_NAV_NOT_CLEARABLE = 0x400,
	TERROR_NAV_CHECKPOINT = 0x800,
	TERROR_NAV_OBSCURED = 0x1000,
	TERROR_NAV_NO_MOBS = 0x2000,
	TERROR_NAV_THREAT = 0x4000,
	TERROR_NAV_NOTHREAT = 0x80000,
	TERROR_NAV_LYINGDOWN = 0x100000,
	TERROR_NAV_RESCUE_CLOSET = 0x10000,
	TERROR_NAV_RESCUE_VEHICLE = 0x8000
};

int
	g_iOffs_SpawnAttributes = -1;

Handle
	gConf = null;

ConVar
	cvarBlockTanks = null,
	cvarBlockWitches = null,
	cvarBlockSpecials = null,
	cvarSetCampaignScores = null,
	cvarFirstSurvivorLeftSafeArea = null,
	cvarProhibitBosses = null,
	cvarBlockRocks = null,
	cvarForceSpecials = null;

public Plugin myinfo =
{
	name = "L4D2 Downtown Extension Test Plugin",
	author = "L4D2 Downtown Devs",
	description = "Ensures functions/offsets are valid and provides commands to test-call most natives directly",
	version = "0.8.2",
	url = "https://github.com/L4D-Community/Left4Downtown2"
};

public void OnPluginStart()
{
	gConf = LoadGameConfigFile(GAMECONFIG_FILE);
	if (gConf == null) {
		DebugPrint("Could not load gamedata/%s.txt", GAMECONFIG_FILE);
	}

	g_iOffs_SpawnAttributes = GameConfGetOffset(gConf, "TerrorNavArea::m_spawnAttributes");
	if (g_iOffs_SpawnAttributes == -1) {
		DebugPrint("Could not find offset for 'TerrorNavArea::m_spawnAttributes'");
	}

	SearchForOffset("TheDirector"); //fails on Linux
	SearchForOffset("TheZombieManager"); //fails on Linux
	SearchForOffset("HasConfigurableDifficultySetting"); //fails on Linux
	SearchForOffset("WeaponInfoDatabase"); //fails on Linux
	SearchForOffset("ValveRejectServerFullFirst");

	SearchForFunction("CTerrorGameRules::GetTeamScore");
	SearchForFunction("CTerrorGameRules::SetCampaignScores");
	SearchForFunction("CTerrorGameRules::ClearTeamScores");
	SearchForFunction("CBaseServer::SetReservationCookie");
	SearchForFunction("CTerrorPlayer::TakeOverBot");
	SearchForFunction("SurvivorBot::SetHumanSpectator");

	SearchForFunction("CDirectorScavengeMode_OnBeginRoundSetupTime");
	SearchForFunction("CTerrorGameRules_ResetRoundNumber");
	SearchForFunction("CTerrorGameRules_SetRoundEndTime");
	SearchForFunction("CDirector::AreWanderersAllowed");
	SearchForFunction("CDirectorMusicBanks::OnRoundStart");

	SearchForFunction("TheDirector"); //fails on Windows
	SearchForFunction("CDirector::RestartScenarioFromVote");

	SearchForFunction("ZombieManager::SpawnTank");
	SearchForFunction("ZombieManager::SpawnWitch");
	SearchForFunction("ZombieManager::SpawnSpecial");
	SearchForFunction("ZombieManager::SpawnWitchBride");
	SearchForFunction("CDirector::OnFirstSurvivorLeftSafeArea");
	SearchForFunction("CDirector::GetScriptValueInt");
	SearchForFunction("CDirector::GetScriptValueFloat");
	SearchForFunction("CDirector::GetScriptValueFloat");
	SearchForFunction("CDirector::IsFinaleEscapeInProgress");
	SearchForFunction("CTerrorPlayer::CanBecomeGhost");

	SearchForFunction("CTerrorPlayer::OnEnterGhostState");
	SearchForFunction("CDirector_IsFinale");

	SearchForFunction("CDirector::TryOfferingTankBot");
	SearchForFunction("CDirector::OnMobRushStart");
	SearchForFunction("ZombieManager::SpawnITMob");
	SearchForFunction("ZombieManager::SpawnMob");

	SearchForFunction("CTerrorPlayer::OnStaggered");
	SearchForFunction("CTerrorPlayer::OnShovedBySurvivor");
	SearchForFunction("CTerrorPlayer::GetCrouchTopSpeed");
	SearchForFunction("CTerrorPlayer::GetRunTopSpeed");
	SearchForFunction("CTerrorPlayer::GetWalkTopSpeed");
	SearchForFunction("CTerrorGameRules::HasConfigurableDifficultySetting"); // fails on windows
	SearchForFunction("DifficultyChanged");
	SearchForFunction("CTerrorGameRules::GetSurvivorSet");
	SearchForFunction("CTerrorGameRules::FastGetSurvivorSet");
	SearchForFunction("CDirectorVersusMode::GetMissionVersusBossSpawning");
	SearchForFunction("CThrow::ActivateAbililty");
	SearchForFunction("CTerrorMeleeWeapon::StartMeleeSwing");
	SearchForFunction("ReadWeaponDataFromFileForSlot");
	SearchForFunction("CDirectorScriptedEventManager::SendInRescueVehicle");
	SearchForFunction("CDirectorScriptedEventManager::ChangeFinaleStage");
	SearchForFunction("CDirectorVersusMode::EndVersusModeRound");
	SearchForFunction("CBaseAnimating::SelectWeightedSequence");
	SearchForFunction("CTerrorPlayer_OnRevived");

	/*
	* These searches will fail when slots are patched
	*/
	SearchForFunction("ConnectClientLobbyCheck");
	SearchForFunction("HumanPlayerLimitReached");
	SearchForFunction("GetMaxHumanPlayers");

	SearchForFunction("GetMasterServerPlayerCounts");

	//////
	RegConsoleCmd("sm_brst", Command_BeginRoundSetupTime);
	RegConsoleCmd("sm_rrn", Command_ResetRoundNumber);
	RegConsoleCmd("sm_sret", Command_SetRoundEndTime);
	RegConsoleCmd("sm_sig", Command_FindSig);

	RegConsoleCmd("sm_ir", Command_IsReserved);
	RegConsoleCmd("sm_rsfv", Command_RestartScenarioFromVote);
	RegConsoleCmd("sm_ur", Command_Unreserve);
	RegConsoleCmd("sm_horde", Command_Horde);
	RegConsoleCmd("sm_spawntime", Command_SpawnTimer);
	RegConsoleCmd("sm_l4d2timers", Command_L4D2Timers);

	RegConsoleCmd("sm_spawntank", Command_SpawnTank);
	RegConsoleCmd("sm_spawnwitch", Command_SpawnWitch);
	RegConsoleCmd("sm_spawnwitchbride", Command_SpawnWitchBride);
	RegConsoleCmd("sm_spawnspecial", Command_SpawnSpecial);

	RegConsoleCmd("sm_readweaponattr", Command_ReadWeaponAttributes);
	RegConsoleCmd("sm_setiweaponattr", Command_SetIntWeaponAttr);
	RegConsoleCmd("sm_setfweaponattr", Command_SetFloatWeaponAttr);

	RegConsoleCmd("sm_readmeleeattr", Command_ReadMeleeAttributes);
	RegConsoleCmd("sm_setimeleeattr", Command_SetIntMeleeAttr);
	RegConsoleCmd("sm_setfmeleeattr", Command_SetFloatMeleeAttr);
	RegConsoleCmd("sm_setbmeleeattr", Command_SetBoolMeleeAttr);

	RegConsoleCmd("sm_scores", Command_GetScores);
	RegConsoleCmd("sm_tankcnt", Command_GetTankCount);
	RegConsoleCmd("sm_flows", Command_GetTankFlows);

	RegConsoleCmd("sm_sendrescue", Command_SendInRescueVehicle);
	RegConsoleCmd("sm_finalestage", Command_SetFinaleStage);

	RegConsoleCmd("sm_revive_me", Cmd_ReviveMe);
	RegConsoleCmd("sm_get_highes_flow", Cmd_GetHighestFlow);
	RegConsoleCmd("sm_are_teams_flipped", Cmd_AreTeamsFlipped);
	RegConsoleCmd("sm_get_versus_completion", Cmd_GetVersusCompletion);
	RegConsoleCmd("sm_takeover_test", Cmd_TakeOverTest);
	RegConsoleCmd("sm_swap_teams", Cmd_SwapTeams);
	RegConsoleCmd("sm_setclass", Cmd_SetClass);
	RegConsoleCmd("sm_test_navarea", Cmd_TestNavArea);
	RegConsoleCmd("sm_gamemode", Cmd_GetGameMode);
	RegConsoleCmd("sm_getserverclassid", Cmd_GetServerClassId);
	RegConsoleCmd("sm_getweaponid", Cmd_GetCurrentWeaponId);
	RegConsoleCmd("sm_state_transition", Cmd_SetStateTransition);
	RegConsoleCmd("sm_check_gamemode", Cmd_CheckGameMode);
	RegConsoleCmd("sm_check_mission", Cmd_CheckMission);
	RegConsoleCmd("sm_entity_center", Cmd_GetEntityCenter);
	RegConsoleCmd("sm_fling", Cmd_FlingPlayer);
	RegConsoleCmd("sm_test_unreserve", Cmd_TestUnreserve);
	RegConsoleCmd("sm_check_lobby", Cmd_CheckLobby);
	RegConsoleCmd("sm_set_lobby", Cmd_SetLobby);
	RegConsoleCmd("sm_getrealnumclients", Cmd_GetRealNumClients);

	cvarBlockRocks = CreateConVar("l4do_block_rocks", "0", "Disable CThrow::ActivateAbility", FCVAR_SPONLY|FCVAR_NOTIFY);
	cvarBlockTanks = CreateConVar("l4do_block_tanks", "0", "Disable ZombieManager::SpawnTank", FCVAR_SPONLY|FCVAR_NOTIFY);
	cvarBlockWitches = CreateConVar("l4do_block_witches", "0", "Disable ZombieManager::SpawnWitch", FCVAR_SPONLY|FCVAR_NOTIFY);
	cvarBlockSpecials = CreateConVar("l4do_block_specials", "0", "Disable ZombieManager::SpawnSpecial", FCVAR_SPONLY|FCVAR_NOTIFY);
	cvarSetCampaignScores = CreateConVar("l4do_set_campaign_scores", "0", "Override campaign score if non-0", FCVAR_SPONLY|FCVAR_NOTIFY);

	cvarFirstSurvivorLeftSafeArea = CreateConVar("l4do_versus_round_started", "0", "Block versus round from starting if non-0", FCVAR_SPONLY|FCVAR_NOTIFY);
	cvarProhibitBosses = CreateConVar("l4do_unprohibit_bosses", "0", "Override ProhibitBosses script key if non-0", FCVAR_SPONLY|FCVAR_NOTIFY);

	cvarForceSpecials = CreateConVar("l4do_force_specials", "0", "Override SpawnSpecial to spawn a certain type of special if non-zero", FCVAR_SPONLY|FCVAR_NOTIFY);
}

Action Cmd_GetRealNumClients(int iClient, int iArgs)
{
	ReplyToCommand(iClient, "[L4D_GetRealNumClients] %d", L4D_GetRealNumClients());
	
	return Plugin_Handled;
}

Action Cmd_CheckLobby(int iClient, int iArgs)
{
	ReplyToCommand(iClient, "[L4D_LobbyIsReserved] %s", (L4D_LobbyIsReserved()) ? "true" : "false");
	
	char sLobbyCookie[20];
	bool bGetCookie = L4D_GetLobbyReservation(sLobbyCookie, sizeof(sLobbyCookie));
	ReplyToCommand(iClient, "[L4D_GetLobbyReservation] Result: %s, cookie: %s", (bGetCookie) ? "true" : "false", sLobbyCookie);

	return Plugin_Handled;
}

Action Cmd_SetLobby(int iClient, int iArgs)
{
	if (!L4D_LobbyIsReserved()) {
		ReplyToCommand(iClient, "[L4D_LobbyIsReserved] false!");
		return Plugin_Handled;
	}

	char sLobbyCookie[20];
	bool bGetCookie = L4D_GetLobbyReservation(sLobbyCookie, sizeof(sLobbyCookie));
	ReplyToCommand(iClient, "[L4D_GetLobbyReservation] Result: %s, cookie: %s", (bGetCookie) ? "true" : "false", sLobbyCookie);
	L4D_LobbyUnreserve();
	
	DataPack hData = null;
	CreateDataTimer(3.0, Timer_SetLobbyDelay, hData, TIMER_FLAG_NO_MAPCHANGE);
	hData.WriteString(sLobbyCookie);

	return Plugin_Handled;
}

Action Timer_SetLobbyDelay(Handle hTimer, DataPack hData)
{
	hData.Reset();

	if (L4D_LobbyIsReserved()) {
		PrintToChatAll("[L4D_LobbyIsReserved] true!");
		return Plugin_Stop;
	}

	char sLobbyCookie[20];
	hData.ReadString(sLobbyCookie, sizeof(sLobbyCookie));
	bool bSetCookie = L4D_SetLobbyReservation(sLobbyCookie);

	PrintToChatAll("[L4D_SetLobbyReservation] Result: %s, cookie: %s", (bSetCookie) ? "true" : "false", sLobbyCookie);

	return Plugin_Stop;
}

Action Cmd_TestUnreserve(int iClient, int iArgs)
{
	L4D_LobbyUnreserve();
	ReplyToCommand(iClient, "[L4D_LobbyUnreserve] Remove lobby reservation");

	return Plugin_Handled;
}

public Action Command_BeginRoundSetupTime(int client, int args)
{
	L4D_ScavengeBeginRoundSetupTime()

	return Plugin_Handled;
}

public Action Command_ResetRoundNumber(int client, int args)
{
	L4D_ResetRoundNumber();

	return Plugin_Handled;
}

public Action Command_SetRoundEndTime(int client, int args)
{
	if (args < 1) {
		ReplyToCommand(client, "[SM] Error: Specify a round end time");
		return Plugin_Handled;
	}

	char functionName[256];
	GetCmdArg(1, functionName, sizeof(functionName));
	float time = StringToFloat(functionName);

	L4D_SetRoundEndTime(time);
	return Plugin_Handled;
}


public Action Command_FindSig(int client, int args)
{
	/*
	* DOES NOT ACTUALLY WORK :(
	*
	*/
	if (args < 1) {
		ReplyToCommand(client, "[SM] Error: Specify a signature");
		return Plugin_Handled;
	}

	char functionName[256];
	GetCmdArg(1, functionName, sizeof(functionName));
	int len = strlen(functionName);

	StartPrepSDKCall(SDKCall_Static);
	if (PrepSDKCall_SetSignature(SDKLibrary_Server, functionName, len)) {
		DebugPrint("Signature '%s' initialized.", functionName);
	} else {
		DebugPrint("Signature '%s' NOT FOUND.", functionName);
	}

	return Plugin_Handled;
}

public Action L4D_OnSpawnSpecial(int &zombieClass, const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnSpecial(zombieClass=%d, vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					zombieClass, vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);

	if (cvarBlockSpecials.BoolValue) {
		DebugPrint("Blocking special spawn...");
		return Plugin_Handled;
	} else if (cvarForceSpecials.IntValue > 0) {
		zombieClass = GetConVarInt(cvarForceSpecials);
		DebugPrint("Converting to type %d...", zombieClass);
		return Plugin_Changed;
	}

	return Plugin_Continue;
}

public void L4D_OnSpawnSpecial_Post(int client, int zombieClass, const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnSpecial_Post(client=%d, zombieClass=%d, vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					client, zombieClass, vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);
}

public Action L4D_OnSpawnTank(const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnTank(vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);

	if (cvarBlockTanks.BoolValue) {
		DebugPrint("Blocking tank spawn...");
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public void L4D_OnSpawnTank_Post(int client, const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnTank_Post(client=%d, vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					client, vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);
}

public Action L4D_OnSpawnWitch(const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnWitch(vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);

	if (cvarBlockWitches.BoolValue) {
		DebugPrint("Blocking witch spawn...");
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public void L4D_OnSpawnWitch_Post(int entity, const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D_OnSpawnWitch_Post(entity=%d, vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					entity, vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);
}

public Action L4D2_OnSpawnWitchBride(const float vecPos[3], const float vecAng[3])
{
	DebugPrint("OnSpawnWitchBride(vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);

	if (cvarBlockWitches.BoolValue) {
		DebugPrint("Blocking witch bride spawn...");
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public void L4D2_OnSpawnWitchBride_Post(int entity, const float vecPos[3], const float vecAng[3])
{
	DebugPrint("L4D2_OnSpawnWitchBride_Post(entity=%d, vecPos[%f,%f,%f], vecAng[%f,%f,%f])", \
					entity, vecPos[0], vecPos[1], vecPos[2], vecAng[0], vecAng[1], vecAng[2]);
}

public Action L4D_OnClearTeamScores(bool newCampaign)
{
	DebugPrint("OnClearTeamScores(newCampaign=%d)", newCampaign);

	return Plugin_Continue;
}

public Action L4D_OnSetCampaignScores(int &scoreA, int &scoreB)
{
	DebugPrint("SetCampaignScores(A=%d, B=%d", scoreA, scoreB);

	if (GetConVarInt(cvarSetCampaignScores)) {
		scoreA = GetConVarInt(cvarSetCampaignScores);
		DebugPrint("Overrode with SetCampaignScores(A=%d, B=%d", scoreA, scoreB);
	}

	return Plugin_Continue;
}

public Action L4D_OnFirstSurvivorLeftSafeArea(int client)
{
	DebugPrint("OnFirstSurvivorLeftSafeArea(client=%d)", client);

	if (GetConVarInt(cvarFirstSurvivorLeftSafeArea)) {
		DebugPrint("Blocking OnFirstSurvivorLeftSafeArea...");
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public Action L4D_OnGetScriptValueInt(const String:key[], &retVal)
{
	//DebugPrint("OnGetScriptValueInt(key=\"%s\",retVal=%d)", key, retVal);

	if (GetConVarInt(cvarProhibitBosses) && StrEqual(key, "ProhibitBosses")) {
		//DebugPrint("Overriding OnGetScriptValueInt(ProhibitBosses)...");
		retVal = 0; //no, do not prohibit bosses thank you very much
		return Plugin_Handled;
	}

	return Plugin_Continue;
}

public Action L4D_OnGetScriptValueFloat(const String:key[], &Float:retVal)
{
	//DebugPrint("OnGetScriptValueFloat(key=\"%s\",retVal=%f)", key, retVal);
	return Plugin_Continue;
}

public Action L4D_OnGetScriptValueString(const String:key[], const String:defaultVal[], String:retVal[128])
{
	DebugPrint("L4D_OnGetScriptValueString(key=\"%s\", retVal=\"%s\")", key, retVal);
	return Plugin_Continue;
}

public Action L4D_OnEnterGhostStatePre(int client)
{
	DebugPrint("L4D_OnEnterGhostStatePre(client=%N)", client);

	return Plugin_Continue;
}

public void L4D_OnEnterGhostState(int client)
{
	DebugPrint("L4D_OnEnterGhostStatePost(client=%N)", client);
}

public Action L4D_OnTryOfferingTankBot(tank_index, &bool:enterStasis)
{
	DebugPrint("L4D_OnTryOfferingTankBot() fired with tank %d and enterstasis %d", tank_index, enterStasis);
	return Plugin_Continue;
}

public Action L4D_OnMobRushStart()
{
	DebugPrint("L4D_OnMobRushStart() fired");
	return Plugin_Continue;
}

public Action L4D_OnSpawnITMob(&amount)
{
	DebugPrint("L4D_OnSpawnITMob(%d) fired", amount);
	return Plugin_Continue;
}

public Action L4D_OnSpawnMob(&amount)
{
	DebugPrint("L4D_OnSpawnMob(%d) fired", amount);
	return Plugin_Continue;
}

public Action L4D_OnShovedBySurvivor(client, victim, const Float:vector[3])
{
	DebugPrint("L4D_OnShovedBySurvivor() fired, victim %N", victim);

	if (client)
	{
		DebugPrint("Shoving client: %N", client);
	}
	// return Plugin_Handled to completely stop melee effects on SI
	return Plugin_Continue;
}

// caution, those 3 are super spammy
public Action L4D_OnGetCrouchTopSpeed(target, &Float:retVal)
{
	// DebugPrint("OnOnGetCrouchTopSpeed(target=%N, retVal=%f)", target, retVal);
	return Plugin_Continue;
}

public Action L4D_OnGetRunTopSpeed(target, &Float:retVal)
{
	// DebugPrint("OnOnGetRunTopSpeed(target=%N, retVal=%f)", target, retVal);
	return Plugin_Continue;
}

public Action L4D_OnGetWalkTopSpeed(target, &Float:retVal)
{
	// DebugPrint("OnOnGetWalkTopSpeed(target=%N, retVal=%f)", target, retVal);
	return Plugin_Continue;
}

public Action L4D_OnHasConfigurableDifficulty(&retVal)
{
	// 0 to disallow configuration, 1 to allow

	//DebugPrint("OnGetDifficulty(retVal=%i)", retVal);

	//retVal = 1;
	//return Plugin_Handled;

	return Plugin_Continue;
}

public Action L4D_OnGetSurvivorSet(&retVal)
{
	// Which set of survivors should be used. 1=L4D1, 2=L4D2
	// Unfortunately has side effects. On L4D2 maps, Bot Character Icons and Score Names stay L4D2. Also, Zombie Skins appear bugged

	//DebugPrint("OnGetSurvivorSet(retVal=%i)", retVal);
	//retVal = 1;
	//return Plugin_Handled;

	return Plugin_Continue;
}

public Action L4D_OnFastGetSurvivorSet(&retVal)
{
	// Which set of survivors should be used. 1=L4D1, 2=L4D2
	// Unfortunately has side effects. On L4D2 maps, Bot Character Icons and Score Names stay L4D2. Also, Zombie Skins appear bugged

	//DebugPrint("OnFastGetSurvivorSet(retVal=%i)", retVal);
	//retVal = 1;
	//return Plugin_Handled;

	return Plugin_Continue;
}

public Action L4D_OnGetMissionVSBossSpawning(&Float:spawn_pos_min, &Float:spawn_pos_max, &Float:tank_chance, &Float:witch_chance)
{
	DebugPrint("L4D_OnGetMissionVersusBossSpawning(%f, %f, %f, %f) fired", spawn_pos_min, spawn_pos_max, tank_chance, witch_chance);
#if TEST_DEBUG_LOG
	LogMessage("L4D_OnGetMissionVersusBossSpawning(%f, %f, %f, %f) fired", spawn_pos_min, spawn_pos_max, tank_chance, witch_chance);
#endif

	return Plugin_Continue;
}

public Action L4D_OnCThrowActivate()
{
	DebugPrint("L4D_OnCThrowActivate() fired");
	if (GetConVarBool(cvarBlockRocks))
	{
		DebugPrint("Blocking!")
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action L4D_OnStartMeleeSwing(client, bool:boolean)
{
	DebugPrint("L4D_OnStartMeleeSwing(client %i, boolean %i) fired", client, boolean);
	return Plugin_Continue;
}

static bool:passOnSendInRescueVehicle = true;

public Action L4D2_OnSendInRescueVehicle()
{
	DebugPrint("L4D2_OnSendInRescueVehicle() fired");

	if (passOnSendInRescueVehicle)
	{
		//passOnSendInRescueVehicle = false;
		return Plugin_Continue;
	}

	return Plugin_Handled;
}

static bool:passOnChangeFinaleStage = true;

public Action L4D2_OnChangeFinaleStage(int &finaleType, const char[] arg)
{
	DebugPrint("L4D2_OnChangeFinaleStage(%d, [%s]) fired", finaleType, arg);

	if (passOnChangeFinaleStage)
	{
		//passOnChangeFinaleStage = false;
		return Plugin_Continue;
	}

	return Plugin_Handled;
}

public Action L4D2_OnEndVersusModeRound(bool countSurvivors)
{
	DebugPrint("L4D2_OnEndVersusModeRound(%s) fired", countSurvivors ? "true" : "false");
	return Plugin_Continue;
}

public Action L4D2_OnSelectTankAttack(int client, int &sequence)
{
	DebugPrint("L4D2_OnSelectTankAttack(%d, %d) fired", client, sequence);
	return Plugin_Continue;
}

public void L4D2_OnRevived(int client)
{
	DebugPrint("L4D2_OnRevived(%d) fired", client);
}

public Action L4D_OnKnockedDown(int client, int reason)
{
	DebugPrint("L4D_OnKnockedDown(client=%d, reason=%d)", client, reason);

	return Plugin_Continue;
}

public void L4D_OnKnockedDown_Post(int client, int reason)
{
	DebugPrint("L4D_OnKnockedDown_Post(client=%d, reason=%d)", client, reason);
}

public Action L4D_TankClaw_OnPlayerHit_Pre(int tank, int claw, int player, bool incapacitated)
{
	char sTankName[MAX_NAME_LENGTH], sVictimName[MAX_NAME_LENGTH], sClawEntityName[ENTITY_NAME_LENGTH];
	GetValidEntityName(claw, sClawEntityName, sizeof(sClawEntityName));
	GetValidPlayerName(tank, sTankName, sizeof(sTankName));
	GetValidPlayerName(player, sVictimName, sizeof(sVictimName));

	DebugPrint("L4D_TankClaw_OnPlayerHit_Pre(tank = %d (%s), claw = %d (%s), player = %d (%s), incapacitated = %d)", \
					tank, sTankName, claw, sClawEntityName, player, sVictimName, incapacitated);

	return Plugin_Continue;
}

public void L4D_TankClaw_OnPlayerHit_Post(int tank, int claw, int player, bool incapacitated)
{
	char sTankName[MAX_NAME_LENGTH], sVictimName[MAX_NAME_LENGTH], sClawEntityName[ENTITY_NAME_LENGTH];
	GetValidEntityName(claw, sClawEntityName, sizeof(sClawEntityName));
	GetValidPlayerName(tank, sTankName, sizeof(sTankName));
	GetValidPlayerName(player, sVictimName, sizeof(sVictimName));

	DebugPrint("L4D_TankClaw_OnPlayerHit_Post(tank = %d (%s), claw = %d (%s), player = %d (%s), incapacitated = %d)", \
					tank, sTankName, claw, sClawEntityName, player, sVictimName, incapacitated);
}

/*public void OnMapStart()
{
	//CreateTimer(0.1, Timer_GetCampaignScores, _);
}*/

public Action Command_IsReserved(int client, int args)
{
#if USE_NATIVES
	//new bool:res = L4D_LobbyIsReserved();

	//DebugPrint("Lobby is %s reserved...", res ? "" : "NOT");
#endif

	return Plugin_Handled;
}

public Action Command_RestartScenarioFromVote(int client, int args)
{
#if USE_NATIVES
	char currentmap[128];
	GetCurrentMap(currentmap, sizeof(currentmap));

	DebugPrint("Restarting scenario from vote ...");
	L4D_RestartScenarioFromVote(currentmap);
#endif

	return Plugin_Handled;
}

public Action Command_Unreserve(int client, int args)
{
#if USE_NATIVES
	DebugPrint("Invoking L4D_LobbyUnreserve() ...");
	L4D_LobbyUnreserve();
#endif

	return Plugin_Handled;
}

public Action Command_Horde(int client, int args)
{
#if USE_NATIVES
	float hordetime = L4D2_CTimerGetRemainingTime(L4D2CT_MobSpawnTimer);
	float hordeduration = L4D2_CTimerGetCountdownDuration(L4D2CT_MobSpawnTimer);
	DebugPrint("Time remaining for next horde is: %f  Duration %f ", hordetime, hordeduration);
	ReplyToCommand(client, "Remaining: %f Duration: %f", hordetime, hordeduration);
#endif

	return Plugin_Handled;
}

public Action Command_SpawnTimer(int client, int args)
{
#if USE_NATIVES
	float SpawnTimer = L4D_GetPlayerSpawnTime(client);
	DebugPrint("Spawn Timer for player %N: %f", client, SpawnTimer);
	ReplyToCommand(client, "Remaining: %f", SpawnTimer);
#endif

	return Plugin_Handled;
}

PrintL4D2CTimerJunk(client, const String:name[], L4D2CountdownTimer:timer)
{
	//DebugPrint("%s - Remaining: %f Duration %f", name, L4D2CountdownTimerGetRemainingTime(timer), L4D2CountdownTimerGetCountdownDuration(timer));
	ReplyToCommand(client, "%s - Remaining: %f Duration %f", name, L4D2_CTimerGetRemainingTime(timer), L4D2_CTimerGetCountdownDuration(timer));
}

PrintL4D2ITimerJunk(client, const String:name[], L4D2IntervalTimer:timer)
{
	//DebugPrint("%s - Elapsed: %f", name, L4D2IntervalTimerGetElapsedTime(timer));
	ReplyToCommand(client, "%s - Elapsed: %f", name, L4D2_ITimerGetElapsedTime(timer));
}

public Action Command_L4D2Timers(int client, int args)
{
#if USE_NATIVES
	PrintL4D2CTimerJunk(client, "MobSpawnTimer", L4D2CT_MobSpawnTimer)
	PrintL4D2CTimerJunk(client, "SmokerSpawnTimer", L4D2CT_SmokerSpawnTimer);
	PrintL4D2CTimerJunk(client, "BoomerSpawnTimer", L4D2CT_BoomerSpawnTimer);
	PrintL4D2CTimerJunk(client, "HunterSpawnTimer", L4D2CT_HunterSpawnTimer);
	PrintL4D2CTimerJunk(client, "SpitterSpawnTimer", L4D2CT_SpitterSpawnTimer);
	PrintL4D2CTimerJunk(client, "JockeySpawnTimer", L4D2CT_JockeySpawnTimer);
	PrintL4D2CTimerJunk(client, "ChargerSpawnTimer", L4D2CT_ChargerSpawnTimer);
	PrintL4D2CTimerJunk(client, "VersusStartTimer", L4D2CT_VersusStartTimer);
	PrintL4D2CTimerJunk(client, "UpdateMarkersTimer", L4D2CT_UpdateMarkersTimer);
	PrintL4D2ITimerJunk(client, "SmokerDeathTimer", L4D2IT_SmokerDeathTimer);

	PrintL4D2ITimerJunk(client, "BoomerDeathTimer", L4D2IT_BoomerDeathTimer);
	PrintL4D2ITimerJunk(client, "HunterDeathTimer", L4D2IT_HunterDeathTimer);
	PrintL4D2ITimerJunk(client, "SpitterDeathTimer", L4D2IT_SpitterDeathTimer);
	PrintL4D2ITimerJunk(client, "JockeyDeathTimer", L4D2IT_JockeyDeathTimer);
	PrintL4D2ITimerJunk(client, "ChargerDeathTimer", L4D2IT_ChargerDeathTimer);
#endif
	return Plugin_Handled;
}

public Action Command_SpawnTank(int client, int args)
{
	float origin[3], angles[3];
	GetClientAbsOrigin(client, origin);
	GetClientAbsAngles(client, angles);

	origin[2] += 160;

	L4D2_SpawnTank(origin, angles);
	return Plugin_Handled;
}

public Action Command_SpawnWitch(int client, int args)
{
	float origin[3], angles[3];
	GetClientAbsOrigin(client, origin);
	GetClientAbsAngles(client, angles);

	origin[2] += 160;

	L4D2_SpawnWitch(origin, angles);
	return Plugin_Handled;
}

public Action Command_SpawnWitchBride(int client, int args)
{
	float origin[3], angles[3];
	GetClientAbsOrigin(client, origin);
	GetClientAbsAngles(client, angles);

	origin[2] += 160;

	L4D2_SpawnWitchBride(origin, angles);
	return Plugin_Handled;
}

public Action Command_SpawnSpecial(int client, int args)
{
	char sClass[5];
	float origin[3], angles[3];
	GetCmdArg(1, sClass, sizeof(sClass));
	new class = StringToInt(sClass);
	GetClientAbsOrigin(client, origin);
	GetClientAbsAngles(client, angles);

	origin[2] += 160;

	L4D2_SpawnSpecial(class, origin, angles);
	return Plugin_Handled;
}

PrintL4D2IntWeaponAttrib(client, const String:weapon[], const String:name[], L4D2IntWeaponAttributes:attr)
{
	//DebugPrint("%s = %f", name, L4D2_GetIntWeaponAttribute(weapon, attr));
	ReplyToCommand(client, "%s = %i", name, L4D2_GetIntWeaponAttribute(weapon, attr));
}

PrintL4D2FloatWeaponAttrib(client, const String:weapon[], const String:name[], L4D2FloatWeaponAttributes:attr)
{
	//DebugPrint("%s = %f", name, L4D2_GetFloatWeaponAttribute(weapon, attr));
	ReplyToCommand(client, "%s = %f", name, L4D2_GetFloatWeaponAttribute(weapon, attr));
}

public Action Command_ReadWeaponAttributes(int client, int args)
{
#if USE_NATIVES
	char sWeapon[80];
	GetCmdArg(1, sWeapon, sizeof(sWeapon));

	ReplyToCommand(client, "Attributes for %s:", sWeapon);
	PrintL4D2IntWeaponAttrib(client, sWeapon, "Damage", L4D2IWA_Damage);
	PrintL4D2IntWeaponAttrib(client, sWeapon, "Bullets", L4D2IWA_Bullets);
	PrintL4D2IntWeaponAttrib(client, sWeapon, "ClipSize", L4D2IWA_ClipSize);
	PrintL4D2IntWeaponAttrib(client, sWeapon, "Bucket", L4D2IWA_Bucket);
	PrintL4D2IntWeaponAttrib(client, sWeapon, "Tier", L4D2IWA_Tier);

	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MaxPlayerSpeed", L4D2FWA_MaxPlayerSpeed);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "SpreadPerShot", L4D2FWA_SpreadPerShot);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MaxSpread", L4D2FWA_MaxSpread);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "SpreadDecay", L4D2FWA_SpreadDecay);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MinDuckingSpread", L4D2FWA_MinDuckingSpread);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MinStandingSpread", L4D2FWA_MinStandingSpread);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MinInAirSpread", L4D2FWA_MinInAirSpread);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "MaxMovementSpread", L4D2FWA_MaxMovementSpread);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "PenetrationNumLayers", L4D2FWA_PenetrationNumLayers);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "PenetrationPower", L4D2FWA_PenetrationPower);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "PenetrationMaxDistance", L4D2FWA_PenetrationMaxDist);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "CharacterPenetrationMaxDistance", L4D2FWA_CharPenetrationMaxDist);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "Range", L4D2FWA_Range);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "RangeModifier", L4D2FWA_RangeModifier);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "CycleTime", L4D2FWA_CycleTime);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "VerticalPunch", L4D2FWA_VerticalPunch);
	PrintL4D2FloatWeaponAttrib(client, sWeapon, "HorizontalPunch", L4D2FWA_HorizontalPunch);
#endif

	return Plugin_Handled;
}

public Action Command_SetIntWeaponAttr(int client, int args)
{
#if USE_NATIVES
	decl String:weapon[80], String:argbuf[32];
	GetCmdArg(1, weapon, sizeof(weapon));
	GetCmdArg(2, argbuf, sizeof(argbuf));
	new L4D2IntWeaponAttributes:attr = L4D2IntWeaponAttributes:StringToInt(argbuf);
	GetCmdArg(3, argbuf, sizeof(argbuf));
	new value = StringToInt(argbuf);
	ReplyToCommand(client, "%s: Attribute %d was %d", weapon, attr, L4D2_GetIntWeaponAttribute(weapon, attr));
	L4D2_SetIntWeaponAttribute(weapon, attr, value)
	ReplyToCommand(client, "%s: Attribute %d is now %d", weapon, attr, L4D2_GetIntWeaponAttribute(weapon, attr));
#endif
	return Plugin_Handled;
}

public Action Command_SetFloatWeaponAttr(int client, int args)
{
#if USE_NATIVES
	decl String:weapon[80], String:argbuf[32];
	GetCmdArg(1, weapon, sizeof(weapon));
	GetCmdArg(2, argbuf, sizeof(argbuf));
	new L4D2FloatWeaponAttributes:attr = L4D2FloatWeaponAttributes:StringToInt(argbuf);
	GetCmdArg(3, argbuf, sizeof(argbuf));
	float value = StringToFloat(argbuf);
	ReplyToCommand(client, "%s: Attribute %d was %f", weapon, attr, L4D2_GetFloatWeaponAttribute(weapon, attr));
	L4D2_SetFloatWeaponAttribute(weapon, attr, value);
	ReplyToCommand(client, "%s: Attribute %d is now %f", weapon, attr, L4D2_GetFloatWeaponAttribute(weapon, attr));
#endif
	return Plugin_Handled;
}

void PrintL4D2IntMeleeAttrib(int iClient, int iId, const char[] sName, L4D2IntMeleeWeaponAttributes iAttr)
{
	ReplyToCommand(iClient, "%s = %i", sName, L4D2_GetIntMeleeAttribute(iId, iAttr));
}

void PrintL4D2FloatMeleeAttrib(int iClient, int iId, const char[] sName, L4D2FloatMeleeWeaponAttributes iAttr)
{
	ReplyToCommand(iClient, "%s = %f", sName, L4D2_GetFloatMeleeAttribute(iId, iAttr));
}

void PrintL4D2BoolMeleeAttrib(int iClient, int iId, const char[] sName, L4D2BoolMeleeWeaponAttributes iAttr)
{
	ReplyToCommand(iClient, "%s = %b", sName, L4D2_GetBoolMeleeAttribute(iId, iAttr));
}

public Action Command_ReadMeleeAttributes(int client, int args)
{
#if USE_NATIVES
	char sWeapon[80];
	GetCmdArg(1, sWeapon, sizeof(sWeapon));
	int iId = L4D2_GetMeleeWeaponIndex(sWeapon);
	if (iId == -1) {
		ReplyToCommand(client, "%s not found in melee database", sWeapon);
		return Plugin_Handled;
	}

	ReplyToCommand(client, "Attributes for %s:", sWeapon);

	PrintL4D2FloatMeleeAttrib(client, iId, "Damage", L4D2FMWA_Damage);
	PrintL4D2FloatMeleeAttrib(client, iId, "RefireDelay", L4D2FMWA_RefireDelay);
	PrintL4D2FloatMeleeAttrib(client, iId, "WeaponIdleTime", L4D2FMWA_WeaponIdleTime);
	PrintL4D2IntMeleeAttrib(client, iId, "DamageFlags", L4D2IMWA_DamageFlags);
	PrintL4D2IntMeleeAttrib(client, iId, "RumbleEffect", L4D2IMWA_RumbleEffect);
	PrintL4D2BoolMeleeAttrib(client, iId, "Decapitates", L4D2BMWA_Decapitates);
#endif

	return Plugin_Handled;
}

public Action Command_SetIntMeleeAttr(int client, int args)
{
#if USE_NATIVES
	char sWeapon[80], sArgBuf[32];
	GetCmdArg(1, sWeapon, sizeof(sWeapon));
	int iId = L4D2_GetMeleeWeaponIndex(sWeapon);
	if (iId == -1) {
		ReplyToCommand(client, "%s not found in melee database", sWeapon);
		return Plugin_Handled;
	}

	GetCmdArg(2, sArgBuf, sizeof(sArgBuf));
	L4D2IntMeleeWeaponAttributes iAttr = view_as<L4D2IntMeleeWeaponAttributes>(StringToInt(sArgBuf));
	GetCmdArg(3, sArgBuf, sizeof(sArgBuf));

	int iValue = StringToInt(sArgBuf);
	ReplyToCommand(client, "%s: Attribute %d was %d", sWeapon, iAttr, L4D2_GetIntMeleeAttribute(iId, iAttr));
	L4D2_SetIntMeleeAttribute(iId, iAttr, iValue)
	ReplyToCommand(client, "%s: Attribute %d is now %d", sWeapon, iAttr, L4D2_GetIntMeleeAttribute(iId, iAttr));
#endif

	return Plugin_Handled;
}

public Action Command_SetFloatMeleeAttr(int client, int args)
{
#if USE_NATIVES
	char sWeapon[80], sArgBuf[32];
	GetCmdArg(1, sWeapon, sizeof(sWeapon));
	int iId = L4D2_GetMeleeWeaponIndex(sWeapon);
	if (iId == -1) {
		ReplyToCommand(client, "%s not found in melee database", sWeapon);
		return Plugin_Handled;
	}

	GetCmdArg(2, sArgBuf, sizeof(sArgBuf));
	L4D2FloatMeleeWeaponAttributes iAttr = view_as<L4D2FloatMeleeWeaponAttributes>(StringToInt(sArgBuf));
	GetCmdArg(3, sArgBuf, sizeof(sArgBuf));

	float fValue = StringToFloat(sArgBuf);

	ReplyToCommand(client, "%s: Attribute %d was %f", sWeapon, iAttr, L4D2_GetFloatMeleeAttribute(iId, iAttr));
	L4D2_SetFloatMeleeAttribute(iId, iAttr, fValue);
	ReplyToCommand(client, "%s: Attribute %d is now %f", sWeapon, iAttr, L4D2_GetFloatMeleeAttribute(iId, iAttr));
#endif

	return Plugin_Handled;
}

public Action Command_SetBoolMeleeAttr(int client, int args)
{
#if USE_NATIVES
	char sWeapon[80], argbuf[32];
	GetCmdArg(1, sWeapon, sizeof(sWeapon));
	int iId = L4D2_GetMeleeWeaponIndex(sWeapon);
	if (iId == -1) {
		ReplyToCommand(client, "%s not found in melee database", sWeapon);
		return Plugin_Handled;
	}

	GetCmdArg(2, argbuf, sizeof(argbuf));
	L4D2BoolMeleeWeaponAttributes iAttr = view_as<L4D2BoolMeleeWeaponAttributes>(StringToInt(argbuf));
	GetCmdArg(3, argbuf, sizeof(argbuf));
	int iValue = StringToInt(argbuf);

	ReplyToCommand(client, "%s: Attribute %d was %d", sWeapon, iAttr, L4D2_GetBoolMeleeAttribute(iId, iAttr));
	L4D2_SetBoolMeleeAttribute(iId, iAttr, view_as<bool>(iValue));
	ReplyToCommand(client, "%s: Attribute %d is now %d", sWeapon, iAttr, L4D2_GetBoolMeleeAttribute(iId, iAttr));
#endif

	return Plugin_Handled;
}

public Action Command_GetScores(int client, int args)
{
#if USE_NATIVES
	int iScores[2];
	L4D2_GetVersusCampaignScores(iScores);
	ReplyToCommand(client, "Score 0: %d Score 1: %d", iScores[0], iScores[1]);
#endif

	return Plugin_Handled;
}

public Action Command_GetTankFlows(int client, int args)
{
#if USE_NATIVES
	float fFlows[2];
	L4D2_GetVersusTankFlowPercent(fFlows);
	ReplyToCommand(client, "Flow 0: %f Flow 1: %f", fFlows[0], fFlows[1]);
#endif

	return Plugin_Handled;
}

public Action Command_GetTankCount(int client, int args)
{
#if USE_NATIVES
	ReplyToCommand(client, "Tanks: %d", L4D2_GetTankCount());
#endif

	return Plugin_Handled;
}

public Action Command_SendInRescueVehicle(int client, int args)
{
#if USE_NATIVES
	ReplyToCommand(client, "Attempting to call CDirectorScriptedEventManager::SendInRescueVehicle(void)");
	passOnSendInRescueVehicle = true;
	L4D2_SendInRescueVehicle();
#endif

	return Plugin_Handled;
}

public Action Command_SetFinaleStage(int client, int args)
{
#if USE_NATIVES
	char sBuffer[64];
	GetCmdArg(1, sBuffer, sizeof(sBuffer));
	int iFinaleType = StringToInt(sBuffer);
	strcopy(sBuffer, sizeof(sBuffer), "");

	if (GetCmdArgs() > 1) {
		GetCmdArg(2, sBuffer, sizeof(sBuffer));
	}

	ReplyToCommand(client, "Attempting to call CDirectorScriptedEventManager::ChangeFinaleStage(%i, %s)", iFinaleType, sBuffer);
	passOnChangeFinaleStage = true;
	L4D2_ChangeFinaleStage(iFinaleType, sBuffer);
#endif

	return Plugin_Handled;
}

public Action Cmd_ReviveMe(int client, int args)
{
	if (client == 0 || GetClientTeam(client) != 2 || !IsPlayerAlive(client)) {
		PrintToChat(client, "This command is only available to living survivors!");
		return Plugin_Handled;
	}

	L4D_ReviveSurvivor(client);
	PrintToChat(client, "L4D_ReviveSurvivor(%N);", client);
	return Plugin_Handled;
}

public Action Cmd_GetHighestFlow(int client, int args)
{
	int iTarget = L4D_GetHighestFlowSurvivor();
	if (iTarget > 0 && iTarget <= MaxClients && IsClientInGame(iTarget)) {
		PrintToChat(client, "L4D_GetHighestFlowSurvivor(); %N ", iTarget);
	} else {
		PrintToChat(client, "L4D_GetHighestFlowSurvivor(); %d ", iTarget);
	}

	return Plugin_Handled;
}

public Action Cmd_AreTeamsFlipped(int client, int args)
{
	int iTarget = L4D2_AreTeamsFlipped();

	PrintToChat(client, "L4D2_AreTeamsFlipped(); %d ", iTarget);
	return Plugin_Handled;
}

public Action Cmd_GetVersusCompletion(int client, int args)
{
	if (client == 0 || GetClientTeam(client) < TEAM_SURVIVOR || !IsPlayerAlive(client)) {
		PrintToChat(client, "This command is only available to living survivors and infected!");
	}

	int iRet = L4D2_GetVersusCompletionPlayer(client);
	PrintToChat(client, "L4D2_GetVersusCompletionPlayer(); %d ", iRet);
	return Plugin_Handled;
}

public Action Cmd_TakeOverTest(int client, int args)
{
	if (client == 0) {
		PrintToChat(client, "This command is available to players who are in the game!");
		return Plugin_Handled;
	}

	int iBot = -1;

	ChangeClientTeam(client, 1);

	// Search for an empty bot
	for (int i = 1; i <= MaxClients; i++) {
		if (IsClientInGame(i) && IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVOR) {
			iBot = i;
			break;
		}
	}

	if (iBot < 1) {
		PrintToChat(client, "Could not find a free bot in the team of survivors!");
		return Plugin_Handled;
	}

	// force player to spec humans
	L4D_SetHumanSpec(iBot, client);

	// force player to take over bot
	L4D_TakeOverBot(client);

	PrintToChat(client, "L4D_TakeOverBot();");
	return Plugin_Handled;
}

public Action Cmd_SwapTeams(int client, int args)
{
	L4D2_SwapTeams();
	PrintToChat(client, "L4D2_SwapTeams();");

	return Plugin_Handled;
}

public Action Cmd_SetClass(int iClient, int iArgs)
{
	if (iClient == 0) {
		ReplyToCommand(iClient, "This command is not available for the server!");
		return Plugin_Handled;
	}

	if (GetClientTeam(iClient) != 3) {
		PrintToChat(iClient, "This command is only available to infected!");
		return Plugin_Handled;
	}

	if (iArgs != 1) {
		PrintToChat(iClient, "Enter class number from 1 to 6.");
		PrintToChat(iClient, "Example: sm_setclass 2.");
		return Plugin_Handled;
	}

	char sArgs[4];
	GetCmdArg(1, sArgs, sizeof(sArgs));
	int iZClass = StringToInt(sArgs);
	if (iZClass < 1 || iZClass > 6) {
		PrintToChat(iClient, "Enter class number from 1 to 6.");
		PrintToChat(iClient, "Example: sm_setclass 2.");
		return Plugin_Handled;
	}

	PrintToChat(iClient, "Start call L4D_SetClass(%d, %d)", iClient, iZClass);
	L4D_SetClass(iClient, iZClass);
	PrintToChat(iClient, "Called successfully L4D_SetClass(%d, %d)", iClient, iZClass);

	return Plugin_Handled;
}

public Action Cmd_GetGameMode(int iClient, int iArgs)
{
	PrintToChat(iClient, "L4D_IsCoopMode: %s", (L4D_IsCoopMode()) ? "true" : "false");
	return Plugin_Handled;
}

public Action Cmd_GetServerClassId(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");
		return Plugin_Handled
	}

	int iTarget = GetClientAimTarget(iClient, false);
	if (iTarget < 0 || !IsValidEdict(iTarget)) {
		PrintToChat(iClient, "You are looking at the invalid entity!");
		return Plugin_Handled
	}

	char sEntityName[ENTITY_NAME_LENGTH];
	GetEdictClassname(iTarget, sEntityName, sizeof(sEntityName));
	PrintToChat(iClient, "L4D_GetServerClassId: %d (%s)", L4D_GetServerClassId(iTarget), sEntityName);

	return Plugin_Handled;
}

public Action Cmd_GetCurrentWeaponId(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");
		return Plugin_Handled;
	}

	int iTarget = GetClientAimTarget(iClient, false);
	if (iTarget < 0 || !IsValidEdict(iTarget)) {
		PrintToChat(iClient, "You are looking at the invalid entity!");
		return Plugin_Handled;
	}

	char sEntityName[ENTITY_NAME_LENGTH];
	GetEdictClassname(iTarget, sEntityName, sizeof(sEntityName));

	if (strncmp("weapon_", sEntityName, 7) != 0) {
		PrintToChat(iClient, "You are looking at another object %s (%d), but not at the weapon!", sEntityName, iTarget);
		return Plugin_Handled;
	}

	PrintToChat(iClient, "L4D2_GetCurrentWeaponId: %d (%s)", L4D2_GetCurrentWeaponId(iTarget), sEntityName);

	return Plugin_Handled;
}

public Action Cmd_CheckGameMode(int iClient, int iArgs)
{
	ReplyToCommand(iClient, "IsGenericCoopMode: %d, IsCoopMode: %d", L4D2_IsGenericCooperativeMode(), L4D_IsCoopMode());
	ReplyToCommand(iClient, "IsRealismMode: %d, IsSurvivalMode: %d", L4D2_IsRealismMode(), L4D_IsSurvivalMode());
	ReplyToCommand(iClient, "IsScavengeMode: %d, IsVersusMode: %d", L4D2_IsScavengeMode(), L4D_IsVersusMode());
	ReplyToCommand(iClient, "HasPlayerControlledZombies: %d, HasConfigurableDifficultySetting: %d", L4D_HasPlayerControlledZombies(), L4D2_HasConfigurableDifficultySetting());
	ReplyToCommand(iClient, "IsSingleChapterMode: %d", L4D2_IsSingleChapterMode());

	return Plugin_Handled;
}

public Action Cmd_CheckMission(int iClient, int iArgs)
{
	ReplyToCommand(iClient, "IsFirstMapInScenario: %d, IsMissionFinalMap: %d", L4D_IsFirstMapInScenario(), L4D_IsMissionFinalMap());

	return Plugin_Handled;
}

public Action Cmd_GetEntityCenter(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");

		return Plugin_Handled;
	}

	int iEntityTarget = GetClientAimTarget(iClient, false);
	if (iEntityTarget < 0 || !IsValidEntity(iEntityTarget)) {
		PrintToChat(iClient, "You are looking at the invalid entity!");

		return Plugin_Handled;
	}

	char sEntityName[ENTITY_NAME_LENGTH];
	GetEntityClassname(iEntityTarget, sEntityName, sizeof(sEntityName));

	float fCenter[3];
	L4D_GetEntityWorldSpaceCenter(iEntityTarget, fCenter);

	PrintToChat(iClient, "Entity %s (%d). WorldSpaceCenter: %f %f %f ", sEntityName, iEntityTarget, fCenter[0], fCenter[1], fCenter[2]);

	return Plugin_Handled;
}

Action Cmd_FlingPlayer(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");

		return Plugin_Handled;
	}

	if (GetClientTeam(iClient) < 2 || !IsPlayerAlive(iClient)) {
		PrintToServer("This command is only available to live players!");

		return Plugin_Handled;
	}

	int iEntityTarget = GetClientAimTarget(iClient, true);
	if (iEntityTarget < 0 || !IsValidEntity(iEntityTarget)) {
		PrintToChat(iClient, "You are looking at the invalid player!");

		return Plugin_Handled;
	}

	float tpos[3], spos[3];
	float distance[3], ratio[3], addVel[3], tvec[3];

	GetClientAbsOrigin(iEntityTarget, tpos);
	GetClientAbsOrigin(iClient, spos);

	distance[0] = (spos[0] - tpos[0]);
	distance[1] = (spos[1] - tpos[1]);
	distance[2] = (spos[2] - tpos[2]);

	GetEntPropVector(iEntityTarget, Prop_Data, "m_vecVelocity", tvec);

	ratio[0] = distance[0] / SquareRoot(distance[1] * distance[1] + distance[0] * distance[0]); //Ratio x/hypo
	ratio[1] = distance[1] / SquareRoot(distance[1] * distance[1] + distance[0] * distance[0]); //Ratio y/hypo

	addVel[0] = (ratio[0] * -1) * 500.0;
	addVel[1] = (ratio[1] * -1) * 500.0;
	addVel[2] = 500.0;

	L4D2_CTerrorPlayer_Fling(iEntityTarget, iClient, addVel, 76, 3.0);

	PrintToChat(iClient, "[L4D2_CTerrorPlayer_Fling] Player: %N (%d). Attacker: %N (%d), addVel: %f %f %f ", \
								iEntityTarget, iEntityTarget, iClient, iClient, addVel[0], addVel[1], addVel[2]);

	return Plugin_Handled;
}

public Action Cmd_SetStateTransition(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");
		return Plugin_Handled
	}

	if (iArgs < 1) {
		PrintToChat(iClient, "You did not enter which transition state should be set!");
		return Plugin_Handled
	}

	char sArgs[16];
	GetCmdArg(1, sArgs, sizeof(sArgs));
	int iState = StringToInt(sArgs);

	if (iState < STATE_ACTIVE || iState > STATE_INTRO_CAMERA){
		PrintToChat(iClient, "Invalid parameter passed: %d! Values less than %d and greater than %d cannot be used!", \
						iState, STATE_ACTIVE, STATE_INTRO_CAMERA);
		return Plugin_Handled
	}

	L4D_State_Transition(iClient, iState);
	PrintToChat(iClient, "L4D_State_Transition: %d", iState);

	return Plugin_Handled;
}

public Action Cmd_TestNavArea(int iClient, int iArgs)
{
	if (iClient == 0) {
		PrintToServer("This command is not available for the server!");
		return Plugin_Handled;
	}

	if (g_iOffs_SpawnAttributes == -1) {
		PrintToChat(iClient, "Could not find offset for 'TerrorNavArea::m_spawnAttributes'");
		return Plugin_Handled;
	}

	float fVec[3];
	GetClientAbsOrigin(iClient, fVec);

	Address pAddr = view_as<Address>(L4D_GetNearestNavArea(fVec));

	if (pAddr != Address_Null) {
		int iAttri = TerrorNavArea_GetSpawnAttributes(pAddr);
		PrintToChatAll("[L4D_GetNearestNavArea] Client: %N, navarea address: %x, navarea attributes: %d, in saferoom: %s", \
							iClient, pAddr, iAttri, (iAttri & TERROR_NAV_CHECKPOINT) ? "true" : "false");
	} else {
		PrintToChatAll("[L4D_GetNearestNavArea] Client: %N, navarea address: %x (null)", iClient, pAddr);
	}

	pAddr = view_as<Address>(L4D_GetLastKnownArea(iClient));

	if (pAddr != Address_Null) {
		int iAttri = TerrorNavArea_GetSpawnAttributes(pAddr);
		PrintToChatAll("[L4D_GetLastKnownArea] Client: %N, navarea address: %x, navarea attributes: %d, in saferoom: %s", \
							iClient, pAddr, iAttri, (iAttri & TERROR_NAV_CHECKPOINT) ? "true" : "false");
	} else {
		PrintToChatAll("[L4D_GetLastKnownArea] Client: %N, navarea address: %x (null)", iClient, pAddr);
	}

	return Plugin_Handled;
}

int TerrorNavArea_GetSpawnAttributes(Address iNav)
{
	return LoadFromAddress(iNav + view_as<Address>(g_iOffs_SpawnAttributes), NumberType_Int32);
}

void SearchForFunction(const char[] sFunctionName)
{
	StartPrepSDKCall(SDKCall_Static);

	if (PrepSDKCall_SetFromConf(gConf, SDKConf_Signature, sFunctionName)) {
		DebugPrint("Function '%s' initialized.", sFunctionName);
	} else {
		DebugPrint("Function '%s' not found.", sFunctionName);
	}
}

void SearchForOffset(const char[] sOffsetName)
{
	int iOffset = GameConfGetOffset(gConf, sOffsetName);
	DebugPrint("Offset for '%s' is %d", sOffsetName, iOffset);
}

void DebugPrint(const char[] szFormat, any ...)
{
#if TEST_DEBUG || TEST_DEBUG_LOG
	char sBuffer[192];

	VFormat(sBuffer, sizeof(sBuffer), szFormat, 2);

	#if TEST_DEBUG
		PrintToChatAll("[TEST-L4DO] %s", sBuffer);
		PrintToConsole(0, "[TEST-L4DO] %s", sBuffer);
	#endif

	LogMessage("%s", sBuffer);
#else
	//suppress "szFormat" never used warning
	if (szFormat[0])
		return;
	else
		return;
#endif
}

stock void L4D_SetRoundEndTime(float fEndTime)
{
	static bool bInit = false;
	static Handle hFunc = null;

	if (!bInit) {
		Handle hConf = LoadGameConfigFile(GAMECONFIG_FILE);
		if (hConf == null)
		{
			LogError("Could not load gamedata/%s.txt", GAMECONFIG_FILE);
			DebugPrint("Could not load gamedata/%s.txt", GAMECONFIG_FILE);
		}

		StartPrepSDKCall(SDKCall_GameRules);
		if (!PrepSDKCall_SetFromConf(hConf, SDKConf_Signature, "CTerrorGameRules_SetRoundEndTime")) {
			ThrowError("Failed to read function from game configuration file");
		}

		PrepSDKCall_AddParameter(SDKType_Float, SDKPass_Plain);
		hFunc = EndPrepSDKCall();

		if (hFunc == null)
		{
			ThrowError("Failed to end prep sdk call");
		}

		bInit = true;
	}

	SDKCall(hFunc, fEndTime);
	DebugPrint("CTerrorGameRules::SetRoundTime(%f)", fEndTime);
}

stock void L4D_ResetRoundNumber()
{
	static bool bInit = false;
	static Handle hCall = null;

	if (!bInit) {
		Handle hConf = LoadGameConfigFile(GAMECONFIG_FILE);
		if (hConf == null) {
			LogError("Could not load gamedata/%s.txt", GAMECONFIG_FILE);
			DebugPrint("Could not load gamedata/%s.txt", GAMECONFIG_FILE);
		}

		StartPrepSDKCall(SDKCall_GameRules);
		if (!PrepSDKCall_SetFromConf(hConf, SDKConf_Signature, "CTerrorGameRules_ResetRoundNumber")) {
			ThrowError("Failed to read function from game configuration file");
		}

		hCall = EndPrepSDKCall();

		if (hCall == null) {
			ThrowError("Failed to end prep sdk call");
		}

		bInit = true;
	}

	SDKCall(hCall);

	DebugPrint("CTerrorGameRules::ResetRoundNumber()");
}

bool GetValidPlayerName(int iClient, char[] sPlayerName, const int iStrSize)
{
	if (iClient < 1 || iClient > MaxClients || !IsClientConnected(iClient)) {
		Format(sPlayerName, iStrSize, "Invalid player");
		return false;
	}

	GetClientName(iClient, sPlayerName, iStrSize);
	return true;
}

bool GetValidEntityName(int iEntity, char[] sEntityName, const int iStrSize)
{
	if (!IsValidEntity(iEntity)) {
		Format(sEntityName, iStrSize, "Invalid entity");
		return false;
	}

	if (!GetEntityClassname(iEntity, sEntityName, iStrSize)) {
		Format(sEntityName, iStrSize, "Invalid entity");
		return false;
	}

	return true;
}
