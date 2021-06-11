/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor
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

#include "terror_weapon_hit.h"
#include "extension.h"

/*
	It is foolish to directly access memory without checks. 
	Even the game makes checks when executing this function and its clone.
	There was already one crash quite rare, when accessing memory, on this line of code: 
	'int isDeadstop = *(int *)((unsigned char *)hEntity + 16024);'.
	
	Apparently some incorrect object was received here, this is very strange. =/
	And maybe this function is related to crash: `CLagCompensationManager::StartLagCompensation with NULL CUserCmd!!!` ????
	It will be better and safer anyway!
	
	SIGSEGV /SEGV_MAPERR accessing 0x13e50058

	Thread 0 (crashed):
	   0: left4downtown.ext.2.l4d2.so!Detours::TerrorWeaponHit::OnTerrorWeaponHit(CGameTrace*, void*, bool) + 0x47
		  eip: 0xe7b8ac07  esp: 0xffa040f0  ebp: 0xffa0437c  ebx: 0x13e4c1c0
		  esi: 0x1381aea0  edi: 0xffa041bc  eax: 0xee0b2af0  ecx: 0x13e4c1c0
		  edx: 0xffa0437c  efl: 0x00210206  

		  e7b8abf1  0f 84 89 01 00 00  jz 0xe7b8ad80
		  e7b8abf7  8b 5f 4c           mov ebx, [edi+0x4c]
		  e7b8abfa  85 db              test ebx, ebx
		  e7b8abfc  0f 84 de 00 00 00  jz 0xe7b8ace0
		  e7b8ac02  a1 40 d6 bb e7     mov eax, [0xe7bbd640]
	  >   e7b8ac07  8b 93 98 3e 00 00  mov edx, [ebx+0x3e98]
		  e7b8ac0d  83 ec 08           sub esp, 0x8
		  e7b8ac10  8b 08              mov ecx, [eax]
		  e7b8ac12  89 54 24 1c        mov [esp+0x1c], edx
		  e7b8ac16  56                 push esi
		  e7b8ac17  50                 push eax

		  ffa040f0  00 00 40 40 00 00 40 40  00 00 40 40 01 00 00 00  ..@@..@@..@@....
		  ffa04100  00 00 00 00 00 01 a0 ff  75 65 de f7 89 2e dd f7  ........ue......
		  ffa04110  74 42 a0 ff 38 e1 34 f7  00 00 00 00 00 83 d4 18  tB..8.4.........
		  ffa04120  c0 c1 e4 13 a0 ae 81 13  00 00 00 00 c0 c1 e4 13  ................
		  ffa04130  bc 41 a0 ff a0 ae 81 13  e8 42 a0 ff 22 da 65 ed  .A.......B..".e.

		  Found via instruction pointer in context
		  
		Stack Trace:
		0	left4downtown.ext.2.l4d2.so!Detours::TerrorWeaponHit::OnTerrorWeaponHit(CGameTrace*, void*, bool) + 0x47
		1	server_srv.so!CTerrorWeapon::TestSwingCollision(Vector const&) + 0x582
		2	server_srv.so!CTerrorWeapon::DoSwing() + 0x365
		3	server_srv.so!CTerrorWeapon::ItemPostFrame() + 0x45b
		4	server_srv.so!CTerrorGun::ItemPostFrame() + 0x38
		5	server_srv.so!CBasePlayer::ItemPostFrame() + 0x47f
		6	server_srv.so!CTerrorPlayer::ItemPostFrame() + 0xba
		7	server_srv.so!CBasePlayer::PostThink() + 0xe3b
		8	server_srv.so!CCSPlayer::PostThink() + 0xc5
		9	server_srv.so!CTerrorPlayer::PostThink() + 0x140
*/
		
namespace Detours
{
	void *TerrorWeaponHit::OnTerrorWeaponHit(trace_t &trace/* a1 */, const Vector &swingVector/* a2 */, bool firstTime/* a3 */)
	{
		L4D_DEBUG_LOG("CTerrorWeapon::OnHit() has been called");
		
		//bool firstTime - It seems this bool means that the first ray of the deadstop has hit, because the deadstop has a lot of rays
		//CBaseEntity* trace.m_pEnt - offset: trace + 76
		/* there's another check being performed here to see if the current gamemode allows bashing... we don't need it */
		if (firstTime && trace.m_pEnt) { 
			CBaseEntity *pOwner = ((CBaseCombatWeapon *)this)->GetOwnerEntity();
			
			if (pOwner != NULL && g_pFwdOnTerrorWeaponHit) {	
				cell_t ctSwingVector[3] = {sp_ftoc(swingVector[0]), sp_ftoc(swingVector[1]), sp_ftoc(swingVector[2])};
				
				int iClientIndex = IndexOfEdict(gameents->BaseEntityToEdict(pOwner));
				int iEntityIndex = IndexOfEdict(gameents->BaseEntityToEdict(trace.m_pEnt));
				int iWeaponIndex = IndexOfEdict(gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity *>(this)));
				
				/*  
					deadstop check: see if it's going to be versus_shove_hunter_fov_pouncing(true) or versus_shove_hunter_fov(false)
					often returns 0 when it shouldn't  - either this shit is unreliable, or the game is buggy as fuck
					probably both
				*/	
				bool IsDeadstop = ((CTerrorPlayer *)trace.m_pEnt)->GetMemberAttemptingToPounce();
				
				g_pFwdOnTerrorWeaponHit->PushCell(iClientIndex); // who shoved
				g_pFwdOnTerrorWeaponHit->PushCell(iEntityIndex); // who got shoved
				g_pFwdOnTerrorWeaponHit->PushCell(iWeaponIndex); // weapon that's been held while shoving
				g_pFwdOnTerrorWeaponHit->PushArray(ctSwingVector, 3); // shove angles
				g_pFwdOnTerrorWeaponHit->PushCell(IsDeadstop); // reliable for high pounces only
				
				L4D_DEBUG_LOG("L4D2_OnEntityShoved(client: %d, entity: %d, weapon: %d, vector: %f %f %f, bIsHighPounce: %d)", 
								iClientIndex, iEntityIndex, iWeaponIndex, swingVector[0], swingVector[1], swingVector[2], IsDeadstop);
								
				cell_t result = Pl_Continue;
				g_pFwdOnTerrorWeaponHit->Execute(&result);
				
				if (result == Pl_Handled) {
					L4D_DEBUG_LOG("CTerrorWeapon::OnHit() will be skipped");
					return NULL;
				}
			}
		}
		
		return (this->*(GetTrampoline()))(trace, swingVector, firstTime);
	}
};
