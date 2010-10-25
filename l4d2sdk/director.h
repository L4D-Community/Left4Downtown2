/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2010 Michael "ProdigySim" Busby
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

#ifndef _INCLUDE_DIRECTOR_H_
#define _INCLUDE_DIRECTOR_H_
#include "l4d2timers.h"

// 1128 bytes
typedef struct _CDirectorItemManager {
	char unknown[1128];
} CDirectorItemManager;

// 92 bytes
typedef struct _CDirectorMusicBanks {
	char unknown[92];
} CDirectorMusicBanks;

// 76 bytes
typedef struct _CDirectorSessionManager {
	char unknown[76];
} CDirectorSessionManager;

// 156 bytes
typedef struct _CDirectorScriptedEventManager {
	char unknown[156];
} CDirectorScriptedEventManager;

// 92 bytes
typedef struct _CDirectorVersusMode {
	char unknown[28];
	CountdownTimer VersusStartTimer; // 0x1c Timer controlling saferoom door open, allowing PZ spawning.
	char unknown2[40]; // 0x28
	CountdownTimer UpdateMarkersTimer; // 0x50 Timer controlling when to check if survivors have passed markers. Normally set to 1s duration
} CDirectorVersusMode;

// 160 bytes
typedef struct _CDirectorSurvivalMode {
	char unknown[160];
} CDirectorSurvivalMode;

// 104 bytes
typedef struct _CDirectorScavengeMode {
	char unknown[104];
} CDirectorScavengeMode;

// 8 bytes
typedef struct _CDirectorChallengeMode {
	char unknown[8];
} CDirectorChallengeMode;


// Win32: 1460 bytes
// Lin: 1480 bytes

typedef struct _CDirector {
	char unknown[256]; // 0x0
	int tankCount; // 0x100
	float tankFlowDistance; // 0x104
	char unknown2[204]; // 0x108
	CountdownTimer MobSpawnTimer; // 0x1d4
	char unknown3[20]; // 0x1e0
	IntervalTimer SmokerDeathTimer; // 0x1f4
	IntervalTimer BoomerDeathTimer;
	IntervalTimer HunterDeathTimer;
	IntervalTimer SpitterDeathTimer;
	IntervalTimer JockeyDeathTimer;
	IntervalTimer ChargerDeathTimer;
	char unknown4[16]; // 0x224
	CountdownTimer SmokerSpawnTimer; // 0x234
	CountdownTimer BoomerSpawnTimer;
	CountdownTimer HunterSpawnTimer;
	CountdownTimer SpitterSpawnTimer;
	CountdownTimer JockeySpawnTimer;
	CountdownTimer ChargerSpawnTimer;
	char unknow5[76]; // 0x27c
	float MobSpawnSize; // 0x2c8
	char unknown6[704];	// 0x2cc
#ifdef PLATFORM_LINUX
	char unknown_linonly[20]; // 0x58c
#endif
	char unknown7[8]; // win 0x58c lin 0x5a0
	CDirectorItemManager * ItemManagerPtr; // win 0x594 lin 0x5a8
	CDirectorMusicBanks * MusicBanksPtr; 
	CDirectorSessionManager * SessionManagerPtr; 
	CDirectorScriptedEventManager * ScriptedEventManagerPtr; 
	CDirectorVersusMode * VersusModePtr; 
	CDirectorSurvivalMode * SurvivalModePtr; 
	CDirectorScavengeMode * ScavengeModePtr; 
	char unknown8[8]; // win 0x5a8 lin 0x5bc
	CDirectorChallengeMode * ChallengeModePtr; // win 0x5b0 lin 0x5c4
} CDirector;

#endif //_INCLUDE_DIRECTOR_H_
