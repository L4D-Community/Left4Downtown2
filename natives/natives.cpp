/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor;
 * 2017-2021 Accelerator, 2021-2022 A1m`;
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
#include "vglobals.h"
#include "util.h"
#include "l4d2sdk/constants.h"
#include "natives.h"

// native void L4D_GetEntityWorldSpaceCenter(int entity, float vecPos[3])
cell_t L4D_GetEntityWorldSpaceCenter(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// const Vector &CBaseEntity::WorldSpaceCenter( ) const 
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYREF;
		retInfo.size = sizeof(Vector *);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_OFFSET("CBaseEntity::WorldSpaceCenter", \
			pWrapper = g_pBinTools->CreateVCall(offset, 0, 0, /*retInfo*/&retInfo, /*paramInfo*/NULL, /*numparams*/0));

		L4D_DEBUG_LOG("Built vcall wrapper CBaseEntity::WorldSpaceCenter");
	}

	CBaseEntity *pEntity = UTIL_GetCBaseEntity(params[1], false);
	if (pEntity == NULL) {
		return pContext->ThrowNativeError("Invalid entity index passed: %d!", params[1]);
	}

	unsigned char vstk[sizeof(CBaseEntity *)];
	unsigned char *vptr = vstk;

	*(CBaseEntity **)vptr = pEntity;

	Vector *vecCenter;

	L4D_DEBUG_LOG("Going to execute CBaseEntity::WorldSpaceCenter");
	pWrapper->Execute(vstk, &vecCenter);
	L4D_DEBUG_LOG("Invoked CBaseEntity::WorldSpaceCenter");

	cell_t *pSourceVector;
	pContext->LocalToPhysAddr(params[2], &pSourceVector);

	if (pSourceVector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		pSourceVector[0] = sp_ftoc(vecCenter->x);
		pSourceVector[1] = sp_ftoc(vecCenter->y);
		pSourceVector[2] = sp_ftoc(vecCenter->z);
	}

	return 1;
}

// native int L4D2_GetCurrentWeaponId(int weapon)
cell_t L4D2_GetCurrentWeaponId(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// int CWeaponCSBase::GetWeaponID(void)
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(int);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_OFFSET("CWeaponCSBase::GetWeaponID", \
			pWrapper = g_pBinTools->CreateVCall(offset, 0, 0, /*retInfo*/&retInfo, /*paramInfo*/NULL, /*numparams*/0));

		L4D_DEBUG_LOG("Built vcall wrapper CWeaponCSBase::GetWeaponID");
	}

	CBaseEntity *pWeapon = gamehelpers->ReferenceToEntity(params[1]);

	if (!pWeapon) {
		return pContext->ThrowNativeError("Invalid entity index %d for weapon", params[1]);
	}

	IServerUnknown *pUnk = (IServerUnknown *)pWeapon;
	IServerNetworkable *pNet = pUnk->GetNetworkable();
	SendTable *pTable = pNet->GetServerClass()->m_pTable;

	if (UTIL_ContainsDataTable(pTable, "DT_WeaponSpawn")) {
		return ((CWeaponSpawn *)pWeapon)->GetWeaponID();
	} 

	if (UTIL_ContainsDataTable(pTable, "DT_WeaponCSBase")) {
		unsigned char vstk[sizeof(CBaseEntity *)];
		unsigned char *vptr = vstk;

		*(CBaseEntity **)vptr = pWeapon;

		cell_t iRet = 0;

		L4D_DEBUG_LOG("Going to execute CWeaponCSBase::GetWeaponID");
		pWrapper->Execute(vstk, /*retbuffer*/&iRet);
		L4D_DEBUG_LOG("Invoked CWeaponCSBase::GetWeaponID");

		return iRet;
	}

	return pContext->ThrowNativeError("Entity index %d is not a weapon", params[1]);
}

// native int L4D_GetServerClassId(int entity);
cell_t L4D_GetServerClassId(IPluginContext *pContext, const cell_t *params)
{
	int iClassId = UTIL_GetServerClassId(params[1]);
	if (iClassId == -1) {
		return pContext->ThrowNativeError("Invalid entity passed: %d!", params[1]);
	}

	return iClassId;
}

// native Address L4D_GetPointer(PointerType ptr_type);
cell_t L4D_GetPointer(IPluginContext *pContext, const cell_t *params)
{
	int iPtrType = params[1];

	switch (iPtrType) {
		case POINTER_DIRECTOR:
			return NativeValidateAddress(*g_ppDirector, "CDirector", pContext);
		case POINTER_SERVER:
			return NativeValidateAddress(g_pServer, "CBaseServer", pContext);
		case POINTER_GAMERULES:
			return NativeValidateAddress(g_pSDKTools->GetGameRules(), "CTerrorGameRules", pContext);
		case POINTER_NAVMESH:
			return NativeValidateAddress(g_pNavMesh, "TerrorNavMesh", pContext);
		case POINTER_ZOMBIEMANAGER:
			return NativeValidateAddress(g_pZombieManager, "ZombieManager", pContext);
		case POINTER_WEAPONINFO:
			return NativeValidateAddress(g_pWeaponInfoDatabase, "WeaponInfoDatabase", pContext);
		case POINTER_MELEEINFO:
			return NativeValidateAddress(g_pMeleeWeaponInfoStore, "MeleeWeaponInfoStore", pContext);
		case POINTER_EVENTMANAGER:
			return NativeValidateAddress((*g_ppDirector)->ScriptedEventManagerPtr, "CDirectorScriptedEventManager", pContext);
		case POINTER_SCAVENGEMODE:
			return NativeValidateAddress((*g_ppDirector)->ScavengeModePtr, "CDirectorScavengeMode", pContext);
		case POINTER_VERSUSMODE:
			return NativeValidateAddress((*g_ppDirector)->VersusModePtr, "CDirectorVersusMode", pContext);
	}

	return pContext->ThrowNativeError("Return pointer type not specified or invalid: %d", iPtrType);
}

//GetNearestNavArea( origin, maxDist, checkLOS, checkGround ) - given a position in the world, return the nearest nav area that is closest to or below that height.
// native int L4D_GetNearestNavArea(const float vecPos[3], float maxDist = 10000.0, bool checkLOS = false, bool checkGround = true);
cell_t L4D_GetNearestNavArea(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//CNavArea* GetNearestNavArea( const Vector &pos, bool anyZ = false, float maxDist = 10000.0f, bool checkLOS = false, bool checkGround = true ) const;
	//CNavArea* CNavMesh::GetNearestNavArea(CNavMesh *this, const Vector *, bool, float, bool, bool, bool)
	if (!pWrapper) {
		PassInfo retInfo;
		retInfo.flags = PASSFLAG_BYVAL;
		retInfo.size = sizeof(CNavArea *);
		retInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("CNavMesh::GetNearestNavArea", \
			PassInfo pass[6]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(Vector *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(bool); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(float); \
			pass[2].type = PassType_Basic; \
			pass[3].flags = PASSFLAG_BYVAL; \
			pass[3].size = sizeof(bool); \
			pass[3].type = PassType_Basic; \
			pass[4].flags = PASSFLAG_BYVAL; \
			pass[4].size = sizeof(bool); \
			pass[4].type = PassType_Basic; \
			pass[5].flags = PASSFLAG_BYVAL; \
			pass[5].size = sizeof(bool); \
			pass[5].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/&retInfo, /*paramInfo*/pass, /*numparams*/6));

		L4D_DEBUG_LOG("Built call wrapper CNavMesh::GetNearestNavArea");
	}

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[1], &source_vector);

	Vector vectorPos;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vectorPos[0] = sp_ctof(source_vector[0]);
		vectorPos[1] = sp_ctof(source_vector[1]);
		vectorPos[2] = sp_ctof(source_vector[2]);
	}

	unsigned char vstk[sizeof(CNavMesh *) + sizeof(Vector *) + sizeof(bool) + sizeof(float) + sizeof(bool) + sizeof(bool) + sizeof(bool)];
	unsigned char *vptr = vstk;

	//CNavMesh *this
	*(CNavMesh **)vptr = g_pNavMesh;
	vptr += sizeof(CNavMesh *);

	// Arg 1: Vector pos
	// The position to look from
	*(Vector **)vptr = &vectorPos;
	vptr += sizeof(Vector *);

	// Arg 2: boolean anyZ = false
	// This argument is ignored and has no effect
	*(bool *)vptr = false;
	vptr += sizeof(bool);

	// Arg 3: number maxDist = 10000.0f
	// This is the maximum distance from the given position that the function will look for a CNavArea
	*(float *)vptr = sp_ctof(params[2]);
	vptr += sizeof(float);

	// Arg 4: boolean checkLOS = false
	// If this is set to true then the function will internally do a util.
	// TraceLine from the starting position to each potential CNavArea with a MASK_NPCSOLID_BRUSHONLY. 
	// If the trace fails then the CNavArea is ignored.
	*(bool *)vptr = (params[3]) ? true : false;
	vptr += sizeof(bool);

	// Arg 5: boolean checkGround = true
	// If checkGround is true then this function will internally call navmesh.
	// GetNavArea to check if there is a CNavArea directly below the position, and return it if so, before checking anywhere else.
	*(bool *)vptr = (params[4]) ? true : false;
	vptr += sizeof(bool);
	
	// Arg 5: bolean unknown = false
	// Unknown parameter
	*(bool *)vptr = false;

	CNavArea *pRet = NULL;

	L4D_DEBUG_LOG("Going to execute CNavMesh::GetNearestNavArea");
	pWrapper->Execute(vstk, &pRet);
	L4D_DEBUG_LOG("Invoked CNavMesh::GetNearestNavArea, got back = %x", pRet);

	return reinterpret_cast<cell_t>(pRet);
}

// native L4D_LobbyUnreserve()
cell_t L4D_LobbyUnreserve(IPluginContext *pContext, const cell_t *params)
{
	const int64_t cookieUnreserved = 0;

	if (g_pServer == NULL) {
		return pContext->ThrowNativeError("CBaseServer not available");
	}

	static ICallWrapper *pWrapper = NULL;

	/* CBaseServer::SetReservationCookie(
			uint64_t reservationCookie,
			const char *formatString,
			va_list ap)

		__cdecl on windows, __thiscall on linux
	*/
	if (!pWrapper) {
#ifdef PLATFORM_WINDOWS
		REGISTER_NATIVE_ADDR("CBaseServer::SetReservationCookie", \
			PassInfo pass[4]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(IServer *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(uint64_t); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(char const *); \
			pass[2].type = PassType_Basic; \
			pass[3].flags = PASSFLAG_BYVAL; \
			pass[3].size = sizeof(void *); \
			pass[3].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_Cdecl, /*retInfo*/NULL, /*paramInfo*/pass, /*numparams*/4));
#else
		REGISTER_NATIVE_ADDR("CBaseServer::SetReservationCookie", \
			PassInfo pass[3]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(uint64_t); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(char const *); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(void *); \
			pass[2].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*retInfo*/NULL, /*paramInfo*/pass, /*numparams*/3));
#endif
	}

	/* Build the vcall argument stack */
	unsigned char vstk[sizeof(IServer *) + sizeof(uint64_t) + sizeof(char const *) + sizeof(void *)];
	unsigned char *vptr = vstk;

	*(void **)vptr = g_pServer;
	vptr += sizeof(IServer *);

	*(uint64_t *)vptr = cookieUnreserved;
	vptr += sizeof(uint64_t);

	*(const char **)vptr = "Manually unreserved by Left 4 Downtown Extension";
	vptr += sizeof(char *);

	*(void **)vptr = NULL; // don't bother using a va_list ap :)

	pWrapper->Execute(vstk, /*retbuffer*/NULL);

	L4D_DEBUG_LOG("Invoked CBaseServer::SetReservationCookie(0,fmt,...)");

	return 1;
}

//DEPRECATED ON L4D2 and L4D1 Linux
// native bool:L4D_LobbyIsReserved()
cell_t L4D_LobbyIsReserved(IPluginContext *pContext, const cell_t *params)
{
	g_pSM->LogError(myself, "L4D_LobbyIsReserved() has been called. It is deprecated in L4D2, consider updating the plugin using this native.");

	return 1;
}

sp_nativeinfo_t g_L4DoNatives[] =
{
	{"L4D_GetEntityWorldSpaceCenter",	L4D_GetEntityWorldSpaceCenter},
	{"L4D2_GetCurrentWeaponId",			L4D2_GetCurrentWeaponId},
	{"L4D_GetServerClassId",			L4D_GetServerClassId},
	{"L4D_GetPointer",					L4D_GetPointer},
	{"L4D_GetNearestNavArea",			L4D_GetNearestNavArea},
	{"L4D_LobbyUnreserve",				L4D_LobbyUnreserve},
	{"L4D_LobbyIsReserved",				L4D_LobbyIsReserved},
	{NULL,								NULL}
};
