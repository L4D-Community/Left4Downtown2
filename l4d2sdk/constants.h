/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2012 Michael "ProdigySim" Busby, 2021 A1mDev (A1m`)
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

#ifndef _INCLUDE_L4D2SDK_CONSTANTS_H_
#define _INCLUDE_L4D2SDK_CONSTANTS_H_

 // Provided by "BHaType":
 // For the "L4D_State_Transition" native.
 // X -> Y (means X state will become Y state on next frame or some seconds later)
enum TerrorPlayerState
{
	STATE_ACTIVE = 0,
	STATE_WELCOME,				// -> STATE_PICKING_TEAM
	STATE_PICKING_TEAM,
	STATE_PICKINGCLASS,			// -> STATE_ACTIVE
	STATE_DEATH_ANIM,			// -> STATE_DEATH_WAIT_FOR_KEY
	STATE_DEATH_WAIT_FOR_KEY,	// -> STATE_OBSERVER_MODE
	STATE_OBSERVER_MODE,
	STATE_WAITING_FOR_RESCUE,
	STATE_GHOST,
	STATE_INTRO_CAMERA
};

// ZombieClassType from here - https://github.com/shqke/smod_stocks/blob/master/stocks/left4dead2.inc
enum ZombieClassType
{
	Zombie_Common	= 0,
	Zombie_Smoker	= 1,
	Zombie_Boomer	= 2,
	Zombie_Hunter	= 3,
	Zombie_Spitter	= 4,
	Zombie_Jockey	= 5,
	Zombie_Charger	= 6,
	Zombie_Witch	= 7,
	Zombie_Tank 	= 8,
	Zombie_Survivor	= 9,
};

enum L4DTeam
{
	Team_None 		= 0,
	Team_Spectators	= 1,
	Team_Survivors	= 2,
	Team_Infected	= 3
};

#endif //_INCLUDE_L4D2SDK_CONSTANTS_H_

