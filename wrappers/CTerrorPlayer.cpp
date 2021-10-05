/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2021 A1mDev (A1m`)
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
 
#include "CTerrorPlayer.h"

int CTerrorPlayer::sendprop_m_isAttemptingToPounce = 0;
int CTerrorPlayer::sendprop_m_zombieClass = 0;

bool CTerrorPlayer::OnLoad(char* error, size_t maxlength)
{
	sm_sendprop_info_t info;
	
	if (!gamehelpers->FindSendPropInfo("CTerrorPlayer", "m_isAttemptingToPounce", &info)) {
		snprintf(error, maxlength, "Unable to find SendProp \"CTerrorPlayer::m_isAttemptingToPounce\"");

		return false;
	}
	
	sendprop_m_isAttemptingToPounce = info.actual_offset;
	
	if (!gamehelpers->FindSendPropInfo("CTerrorPlayer", "m_zombieClass", &info)) {
		snprintf(error, maxlength, "Unable to find SendProp \"CTerrorPlayer::m_zombieClass\"");

		return false;
	}
	
	sendprop_m_zombieClass = info.actual_offset;

	return true;
}

bool CTerrorPlayer::IsAttemptingToPounce()
{
	int m_isAttemptingToPounce = *(int *)((unsigned char *)this + sendprop_m_isAttemptingToPounce);
	
	return (m_isAttemptingToPounce) ? true : false;
}

ZombieClassType CTerrorPlayer::GetZombieClass()
{
	ZombieClassType m_zombieClass = static_cast<ZombieClassType>(*(int *)((unsigned char *)this + sendprop_m_zombieClass));
	/*if (m_zombieClass > Zombie_Common && m_zombieClass < Zombie_Survivor) {
		return m_zombieClass;
	}
	
	return Zombie_Common;*/
	return m_zombieClass;
}
