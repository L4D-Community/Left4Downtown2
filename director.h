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


// Win32: 1444 bytes
// Lin: 1464 bytes

typedef struct _CDirector {
	char unknown[256];
	int tankCount; // 0x100
	char unknown2[204];
	void *MobSpawnTimer; // 0x1d0, CountdownTimer pointer. See hl2sdk/game/shared/util_shared.h
	float MobSpawnTimer_m_duration; // 0x1d4 Countdowntimer duration
	float MobSpawnTimer_m_timestamp; // 0x1d8 Countdowntimer timestamp (end time)
	char unknown3[232];
	float MobSpawnSize; // 0x2c4
	char unknown4[712];	
#ifdef PLATFORM_LINUX
	char unknown_linonly[20];
#endif
} CDirector;

#endif //_INCLUDE_DIRECTOR_H_

