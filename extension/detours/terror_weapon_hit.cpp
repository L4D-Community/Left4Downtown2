/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2019 Accelerator; 2021 A1m`, Accelerator;
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
	'int isDeadstop = *(int *)((unsigned char *)hEntity + 16024);'. New offset after update 15992

	Apparently some incorrect object was received here, this is very strange. =/
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
	  >   e7b8ac07  8b 93 98 3e 00 00  mov edx, [ebx+0x3e98] //old offset 16024
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

/*
	Another crash occurred on a different server:

	Thread 0 (crashed):
	   0: left4downtown.ext.2.l4d2.so!CTerrorPlayer::GetMemberAttemptingToPounce() + 0x9
		  eip: 0xe7b411e9  esp: 0xff9de5cc  ebp: 0x0000032e  ebx: 0x00000001
		  esi: 0xff9de71c  edi: 0xff9de824  eax: 0x00003e78  ecx: 0x00000280
		  edx: 0x10a337a0  efl: 0x00210202

		  e7b411d3  90                 nop
		  e7b411d4  8d b6 00 00 00 00  lea esi, [esi+0x0]
		  e7b411da  8d bf 00 00 00 00  lea edi, [edi+0x0]
		  e7b411e0  a1 f0 25 b7 e7     mov eax, [0xe7b725f0] //eax new offset 0x00003e78 == 15992
		  e7b411e5  8b 54 24 04        mov edx, [esp+0x4]
	  >   e7b411e9  8b 0c 02           mov ecx, [edx+eax]
		  e7b411ec  85 c9              test ecx, ecx
		  e7b411ee  0f 95 c0           setnz al
		  e7b411f1  c3                 ret
		  e7b411f2  66 90              nop
		  e7b411f4  66 90              nop

		  ff9de5cc  63 08 b4 e7                                       c...

		  Found via instruction pointer in context


	   1: left4downtown.ext.2.l4d2.so!Detours::TerrorWeaponHit::OnTerrorWeaponHit(CGameTrace&, Vector const&, bool) + 0xd3
		  eip: 0xe7b40863  esp: 0xff9de5d0  ebp: 0x0000032e  ebx: 0x00000001
		  esi: 0xff9de71c  edi: 0xff9de824

		  ff9de5d0  a0 37 a3 10 f0 e9 bc 0d  f8 e5 9d ff 9c e6 9d ff  .7..............
		  ff9de5e0  00 00 00 00 9c e6 9d ff  80 02 00 00 03 00 00 00  ................
		  ff9de5f0  ac e6 9d ff 2b 2d da 3e  91 8e 57 3f f5 5c a9 be  ....+-.>..W?.\..
		  ff9de600  a0 37 a3 10 f0 e9 bc 0d  00 00 00 00 a0 37 a3 10  .7...........7..
		  ff9de610  f0 e9 bc 0d 1c e7 9d ff  88 e7 9d ff 22 f8 5c ed  ............".\.

		  Found via call frame info

		Stack Trace:
		0	left4downtown.ext.2.l4d2.so!CTerrorPlayer::GetMemberAttemptingToPounce() + 0x9
		1	left4downtown.ext.2.l4d2.so!Detours::TerrorWeaponHit::OnTerrorWeaponHit(CGameTrace&, Vector const&, bool) + 0xd3
		2	server_srv.so!CTerrorWeapon::TestSwingCollision(Vector const&) + 0x522
		3	server_srv.so!CTerrorWeapon::DoSwing() + 0x315
		4	server_srv.so!CTerrorWeapon::ItemPostFrame() + 0x4ab
		5	server_srv.so!CTerrorGun::ItemPostFrame() + 0x38
		6	server_srv.so!CBasePlayer::ItemPostFrame() + 0x46f
		7	server_srv.so!CTerrorPlayer::ItemPostFrame() + 0xbe
		8	server_srv.so!CBasePlayer::PostThink() + 0xc12
		9	server_srv.so!CCSPlayer::PostThink() + 0xbb
*/

namespace Detours
{
	void* TerrorWeaponHit::OnTerrorWeaponHit(trace_t &trace/* a1 */, const Vector &swingVector/* a2 */, bool firstTime/* a3 */)
	{
		//bool firstTime - It seems this bool means that the first ray of the deadstop has hit, because the deadstop has a lot of rays
		//CBaseEntity *trace.m_pEnt - offset: trace + 76
		/* there's another check being performed here to see if the current gamemode allows bashing... we don't need it */
		if (firstTime && trace.m_pEnt)
		{
			CBaseEntity *pOwner = ((CBaseCombatWeapon *)this)->GetOwnerEntity(); //there is already a check if the player is

			if (pOwner != NULL)
			{
				cell_t result = Pl_Continue;

				cell_t ctSwingVector[3] = {sp_ftoc(swingVector[0]), sp_ftoc(swingVector[1]), sp_ftoc(swingVector[2])};

				int iClientIndex = IndexOfEdict(pOwner->edict());
				int iEntityIndex = IndexOfEdict(trace.m_pEnt->edict());
				int iWeaponIndex = IndexOfEdict((reinterpret_cast<CBaseEntity *>(this))->edict());

				/*
					deadstop check: see if it's going to be versus_shove_hunter_fov_pouncing(true) or versus_shove_hunter_fov(false)
					often returns 0 when it shouldn't  - either this shit is unreliable, or the game is buggy as fuck
					probably both

					We need to make sure here that we go to that memory area, does this really have a property 'm_isAttemptingToPounce'?
					Only players or all CBaseEntity objects have this property(including world)?
					But we only need players, we will use this check IsPlayer()!
					It seems that some objects return a true, which means we get garbage in some cases =(
				*/

				bool IsDeadstop = (trace.m_pEnt->IsPlayer() && ((CTerrorPlayer *)trace.m_pEnt)->IsAttemptingToPounce());

				g_pFwdOnTerrorWeaponHit->PushCell(iClientIndex); // who shoved
				g_pFwdOnTerrorWeaponHit->PushCell(iEntityIndex); // who got shoved
				g_pFwdOnTerrorWeaponHit->PushCell(iWeaponIndex); // weapon that's been held while shoving
				g_pFwdOnTerrorWeaponHit->PushArray(ctSwingVector, 3); // shove angles
				g_pFwdOnTerrorWeaponHit->PushCell(IsDeadstop); // reliable for high pounces only

				g_pFwdOnTerrorWeaponHit->Execute(&result);

				if (result == Pl_Handled) {
					return NULL;
				}
			}
		}

		return (this->*(GetTrampoline()))(trace, swingVector, firstTime);
	}
};
