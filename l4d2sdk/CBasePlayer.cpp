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

#include "extension.h"
#include "CBasePlayer.h"

int CBasePlayer::sendprop_m_iTeamNum = 0;

bool CBasePlayer::OnLoad(char* error, size_t maxlength)
{
	sm_sendprop_info_t info;
	
	if (!gamehelpers->FindSendPropInfo("CBasePlayer", "m_iTeamNum", &info)) {
		snprintf(error, maxlength, "Unable to find SendProp \"СBasePlayer::m_iTeamNum\"");

		return false;
	}
	
	sendprop_m_iTeamNum = info.actual_offset;
	
	return true;
}

L4DTeam CBasePlayer::GetTeamNumber()
{
	L4DTeam m_iTeamNum = static_cast<L4DTeam>(*(int *)((unsigned char *)this + sendprop_m_iTeamNum));
	/*if (m_iTeamNum > Team_None && m_iTeamNum <= Team_Infected) {
		return m_iTeamNum;
	}
	
	return Team_None;*/
	return m_iTeamNum;
}
