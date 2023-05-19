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
#include "wrappers.h"
#include "vglobals.h"
#include "util.h"

// native void L4D_ReplaceTank(int tank, int newtank);
cell_t L4D_ReplaceTank(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// ZombieManager::ReplaceTank(CTerrorPlayer *, CTerrorPlayer *)
	if (!pWrapper) {
		REGISTER_NATIVE_ADDR("ZombieManager::ReplaceTank", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(CBaseEntity *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(CBaseEntity *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, /*returnInfo*/NULL, pass, /*numparams*/2));
	}

	if (g_pZombieManager == NULL) {
		return pContext->ThrowNativeError("ZombieManager unsupported or not available; file a bug report");
	}

	CBaseEntity *pTank = UTIL_GetCBaseEntity(params[1], true);
	if (pTank == NULL) {
		return pContext->ThrowNativeError("Invalid tank client: %d!", params[1]);
	}

	CBaseEntity *pNewtank = UTIL_GetCBaseEntity(params[2], true);
	if (pNewtank == NULL) {
		return pContext->ThrowNativeError("Invalid new tank client: %d!", params[2]);
	}

	unsigned char vstk[sizeof(ZombieManager *) + sizeof(CBaseEntity *) + sizeof(CBaseEntity *)];
	unsigned char *vptr = vstk;

	*(ZombieManager **)vptr = g_pZombieManager;
	vptr += sizeof(ZombieManager *);

	*(CBaseEntity **)vptr = pTank;
	vptr += sizeof(CBaseEntity *);

	*(CBaseEntity **)vptr = pNewtank;

	L4D_DEBUG_LOG("Going to execute ZombieManager::ReplaceTank");
	pWrapper->Execute(vstk, /*retbuffer*/NULL);
	L4D_DEBUG_LOG("Invoked ZombieManager::ReplaceTank");

	return 1;
}

// native int L4D2_SpawnSpecial(int zombieClass, const float vecOrigin[3], const float vecAngles[3]);
cell_t L4D2_SpawnSpecial(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//ZombieManager::SpawnSpecial(ZombieClassType, Vector const&, QAngle const&)
	if (!pWrapper) {
		PassInfo passInfo;
		passInfo.flags = PASSFLAG_BYVAL;
		passInfo.size = sizeof(CBaseEntity *);
		passInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("ZombieManager::SpawnSpecial", \
			PassInfo pass[3]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(ZombieClassType); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(Vector *); \
			pass[1].type = PassType_Basic; \
			pass[2].flags = PASSFLAG_BYVAL; \
			pass[2].size = sizeof(QAngle *); \
			pass[2].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &passInfo, pass, 3));
	}

	if (g_pZombieManager == NULL) {
		return pContext->ThrowNativeError("ZombieManager unsupported or not available; file a bug report");
	}

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[2], &source_vector);

	cell_t *source_qangle;
	pContext->LocalToPhysAddr(params[3], &source_qangle);

	Vector vecOrigin;
	QAngle qAngle;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vecOrigin[0] = sp_ctof(source_vector[0]);
		vecOrigin[1] = sp_ctof(source_vector[1]);
		vecOrigin[2] = sp_ctof(source_vector[2]);
	}

	if (source_qangle != pContext->GetNullRef(SP_NULL_VECTOR)) {
		qAngle[0] = sp_ctof(source_qangle[0]);
		qAngle[1] = sp_ctof(source_qangle[1]);
		qAngle[2] = sp_ctof(source_qangle[2]);
	}

	unsigned char vstk[sizeof(ZombieManager *) + sizeof(ZombieClassType) + sizeof(Vector *) + sizeof(QAngle *)];
	unsigned char *vptr = vstk;

	*(ZombieManager **)vptr = g_pZombieManager;
	vptr += sizeof(ZombieManager *);

	*(cell_t *)vptr = params[1];
	vptr += sizeof(ZombieClassType);

	*(Vector **)vptr = &vecOrigin;
	vptr += sizeof(Vector *);

	*(QAngle **)vptr = &qAngle;

	CBaseEntity *pEntity = NULL;

	L4D_DEBUG_LOG("Going to execute ZombieManager::SpawnSpecial");
	pWrapper->Execute(vstk, &pEntity);
	L4D_DEBUG_LOG("Invoked ZombieManager::SpawnSpecial");

	return gamehelpers->EntityToBCompatRef(pEntity);
}

// native int L4D2_SpawnTank(const float vecOrigin[3], const float vecAngles[3]);
cell_t L4D2_SpawnTank(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	// ZombieManager::SpawnTank(Vector const&, QAngle const&)
	if (!pWrapper) {
		PassInfo passInfo;
		passInfo.flags = PASSFLAG_BYVAL;
		passInfo.size = sizeof(CBaseEntity *);
		passInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("ZombieManager::SpawnTank", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(Vector *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(QAngle *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &passInfo, pass, 2));
	}

	if (g_pZombieManager == NULL) {
		return pContext->ThrowNativeError("ZombieManager unsupported or not available; file a bug report");
	}
	unsigned char vstk[sizeof(ZombieManager *) + sizeof(Vector *) + sizeof(QAngle *)];
	unsigned char *vptr = vstk;

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[1], &source_vector);

	cell_t *source_qangle;
	pContext->LocalToPhysAddr(params[2], &source_qangle);

	Vector vecOrigin;
	QAngle qAngle;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vecOrigin[0] = sp_ctof(source_vector[0]);
		vecOrigin[1] = sp_ctof(source_vector[1]);
		vecOrigin[2] = sp_ctof(source_vector[2]);
	}

	if (source_qangle != pContext->GetNullRef(SP_NULL_VECTOR)) {
		qAngle[0] = sp_ctof(source_qangle[0]);
		qAngle[1] = sp_ctof(source_qangle[1]);
		qAngle[2] = sp_ctof(source_qangle[2]);
	}

	*(ZombieManager **)vptr = g_pZombieManager;
	vptr += sizeof(ZombieManager *);

	*(Vector **)vptr = &vecOrigin;
	vptr += sizeof(Vector *);

	*(QAngle **)vptr = &qAngle;

	CBaseEntity *pEntity = NULL;

	L4D_DEBUG_LOG("Going to execute ZombieManager::SpawnTank");
	pWrapper->Execute(vstk, &pEntity);
	L4D_DEBUG_LOG("Invoked ZombieManager::SpawnTank");

	return gamehelpers->EntityToBCompatRef(pEntity);
}

// native int L4D2_SpawnWitchBride(const float vecOrigin[3], const float vecAngles[3]);
cell_t L4D2_SpawnWitchBride(IPluginContext *pContext, const cell_t *params)
{
	static ICallWrapper *pWrapper = NULL;

	//ZombieManager::SpawnWitchBride(Vector const&, QAngle const&)
	if (!pWrapper) {
		PassInfo passInfo;
		passInfo.flags = PASSFLAG_BYVAL;
		passInfo.size = sizeof(CBaseEntity *);
		passInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("ZombieManager::SpawnWitchBride", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(Vector *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(QAngle *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &passInfo, pass, 2));
	}

	if (g_pZombieManager == NULL) {
		return pContext->ThrowNativeError("ZombieManager unsupported or not available; file a bug report");
	}

	unsigned char vstk[sizeof(ZombieManager *) + sizeof(Vector *) + sizeof(QAngle *)];
	unsigned char *vptr = vstk;

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[1], &source_vector);

	cell_t *source_qangle;
	pContext->LocalToPhysAddr(params[2], &source_qangle);

	Vector vecOrigin;
	QAngle qAngle;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vecOrigin[0] = sp_ctof(source_vector[0]);
		vecOrigin[1] = sp_ctof(source_vector[1]);
		vecOrigin[2] = sp_ctof(source_vector[2]);
	}

	if (source_qangle != pContext->GetNullRef(SP_NULL_VECTOR)) {
		qAngle[0] = sp_ctof(source_qangle[0]);
		qAngle[1] = sp_ctof(source_qangle[1]);
		qAngle[2] = sp_ctof(source_qangle[2]);
	}

	*(ZombieManager **)vptr = g_pZombieManager;
	vptr += sizeof(ZombieManager *);

	*(Vector **)vptr = &vecOrigin;
	vptr += sizeof(Vector *);

	*(QAngle **)vptr = &qAngle;

	CBaseEntity *pEntity = NULL;

	L4D_DEBUG_LOG("Going to execute ZombieManager::SpawnWitchBride");
	pWrapper->Execute(vstk, &pEntity);
	L4D_DEBUG_LOG("Invoked ZombieManager::SpawnWitchBride");

	return gamehelpers->EntityToBCompatRef(pEntity);
}

// native int L4D2_SpawnWitch(const float vecOrigin[3], const float vecAngles[3]);
cell_t L4D2_SpawnWitch(IPluginContext *pContext, const cell_t *params)
{
	if (g_pZombieManager == NULL) {
		return pContext->ThrowNativeError("ZombieManager unsupported or not available; file a bug report");
	}

	static ICallWrapper *pWrapper = NULL;

	// ZombieManager::SpawnWitch(Vector const&, QAngle const&)
	if (!pWrapper) {
		PassInfo passInfo;
		passInfo.flags = PASSFLAG_BYVAL;
		passInfo.size = sizeof(CBaseEntity *);
		passInfo.type = PassType_Basic;

		REGISTER_NATIVE_ADDR("ZombieManager::SpawnWitch", \
			PassInfo pass[2]; \
			pass[0].flags = PASSFLAG_BYVAL; \
			pass[0].size = sizeof(Vector *); \
			pass[0].type = PassType_Basic; \
			pass[1].flags = PASSFLAG_BYVAL; \
			pass[1].size = sizeof(QAngle *); \
			pass[1].type = PassType_Basic; \
				pWrapper = g_pBinTools->CreateCall(addr, CallConv_ThisCall, &passInfo, pass, 2));
	}

	unsigned char vstk[sizeof(ZombieManager *) + sizeof(Vector *) + sizeof(QAngle *)];
	unsigned char *vptr = vstk;

	cell_t *source_vector;
	pContext->LocalToPhysAddr(params[1], &source_vector);

	cell_t *source_qangle;
	pContext->LocalToPhysAddr(params[2], &source_qangle);

	Vector vecOrigin;
	QAngle qAngle;

	if (source_vector != pContext->GetNullRef(SP_NULL_VECTOR)) {
		vecOrigin[0] = sp_ctof(source_vector[0]);
		vecOrigin[1] = sp_ctof(source_vector[1]);
		vecOrigin[2] = sp_ctof(source_vector[2]);
	}

	if (source_qangle != pContext->GetNullRef(SP_NULL_VECTOR)) {
		qAngle[0] = sp_ctof(source_qangle[0]);
		qAngle[1] = sp_ctof(source_qangle[1]);
		qAngle[2] = sp_ctof(source_qangle[2]);
	}

	*(ZombieManager **)vptr = g_pZombieManager;
	vptr += sizeof(ZombieManager *);

	*(Vector **)vptr = &vecOrigin;
	vptr += sizeof(Vector *);

	*(QAngle **)vptr = &qAngle;

	CBaseEntity *pEntity;

	L4D_DEBUG_LOG("Going to execute ZombieManager::SpawnWitch");
	pWrapper->Execute(vstk, &pEntity);
	L4D_DEBUG_LOG("Invoked ZombieManager::SpawnWitch");

	return gamehelpers->EntityToBCompatRef(pEntity);
}

sp_nativeinfo_t g_L4DoZombieManagerNatives[] =
{
	{"L4D_ReplaceTank",					L4D_ReplaceTank},
	{"L4D2_SpawnSpecial",				L4D2_SpawnSpecial},
	{"L4D2_SpawnTank",					L4D2_SpawnTank},
	{"L4D2_SpawnWitchBride",			L4D2_SpawnWitchBride},
	{"L4D2_SpawnWitch",					L4D2_SpawnWitch},
	{NULL,								NULL}
};
