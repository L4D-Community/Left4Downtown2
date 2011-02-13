/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown 2 SourceMod Extension
 * Copyright (C) 2010 Michael "ProdigySim" Busby.
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
#include "vglobals.h"

enum L4D2BoolMeleeWeaponAttributes
{
	L4D2BMWA_Decapitates
};
enum L4D2IntMeleeWeaponAttributes
{
	L4D2IMWA_DamageFlags,
	L4D2IMWA_RumbleEffect
};

enum L4D2FloatMeleeWeaponAttributes
{
	L4D2FMWA_Damage,
	L4D2FMWA_RefireDelay,
	L4D2FMWA_WeaponIdleTime
};

bool * BoolIdToAttr(CMeleeWeaponInfo *pInfo, int id)
{
	if(pInfo == NULL) return NULL;
	
	switch(id)
	{
		case L4D2BMWA_Decapitates:
			return &pInfo->m_bDecapitates;
		default:
			return NULL;
	}
}

int * IntIdToAttr(CMeleeWeaponInfo *pInfo, int id)
{
	if(pInfo == NULL) return NULL;
	
	switch(id)
	{
		case L4D2IMWA_DamageFlags:
			return &pInfo->m_iDamageFlags;
		case L4D2IMWA_RumbleEffect:
			return &pInfo->m_iRumbleEffect;
		default:
			return NULL;
	}
}
float * FloatIdToAttr(CMeleeWeaponInfo *pInfo, int id)
{
	if(pInfo == NULL) return NULL;
	
	switch(id)
	{
		case L4D2FMWA_Damage:
			return &pInfo->m_fDamage;
		case L4D2FMWA_RefireDelay:
			return &pInfo->m_fRefireDelay;
		case L4D2FMWA_WeaponIdleTime:
			return &pInfo->m_fWeaponIdleTime;
		default:
			return NULL;
	}
}

/*************************************
	Melee Weapon Natives 
	***********************************/

// native L4D2_GetMeleeWeaponIndex(const String:weaponName[]);	
cell_t L4D2_GetMeleeWeaponIndex(IPluginContext *pContext, const cell_t *params)
{
	if (g_pMeleeWeaponInfoStore == NULL)
	{
		return pContext->ThrowNativeError("MeleeWeaponInfoStore unavailable or unsupported. File a bug report.");
	}
	//char * weapon = NULL;
	//pContext->LocalToString(params[1], &weapon);
	CMeleeWeaponInfo * pInfo = NULL;
	FOR_EACH_VEC( (g_pMeleeWeaponInfoStore->weaponNames), i )
	{
		L4D_DEBUG_LOG("Weapon %d: %s", i, g_pMeleeWeaponInfoStore->weaponNames[i]);
		pInfo = g_pMeleeWeaponInfoStore->weaponInfo[i];
		L4D_DEBUG_LOG("%s (%d) Refire Delay: %f", pInfo->src, i, pInfo->m_fRefireDelay);
	}
	return 0;
}
/*
// native L4D2_GetIntMeleeAttribute(const String:weaponName[], L4D2IntMeleeWeaponAttribute:attr);
cell_t L4D2_GetIntMeleeAttribute(IPluginContext *pContext, const cell_t *params)
{
	if (g_pMeleeWeaponInfoStore == NULL)
	{
		return pContext->ThrowNativeError("MeleeWeaponInfoStore unavailable or unsupported. File a bug report.");
	}
	char * weapon = NULL;
	pContext->LocalToString(params[1], &weapon);
	int iIndex = g_pMeleeWeaponInfo->Find(weapon);

	if (!g_pMeleeWeaponInfo->IsValidIndex(iIndex))
	{
		return pContext->ThrowNativeError("Invalid weapon name or weapon unavailable");
	}
	CTerrorWeaponInfo *pInfo = g_pMeleeWeaponInfo->Element(iIndex);
	
	int * attr = IntIdToAttr(pInfo, params[2]);
	if(attr == NULL)
	{
		return pContext->ThrowNativeError("Invalid attribute id");
	}
	return *attr;
	return 0;
}

//native Float:L4D2_GetFloatMeleeAttribute(const String:weaponName[], L4D2FloatMeleeWeaponAttributes:attr);
cell_t L4D2_GetFloatMeleeAttribute(IPluginContext *pContext, const cell_t *params)
{

	if (g_pMeleeWeaponInfo == NULL)
	{
		return pContext->ThrowNativeError("MeleeWeaponInfo unavailable or unsupported. File a bug report.");
	}
	char * weapon = NULL;
	pContext->LocalToString(params[1], &weapon);
	int iIndex = g_pMeleeWeaponInfo->Find(weapon);

	if (!g_pMeleeWeaponInfo->IsValidIndex(iIndex))
	{
		return pContext->ThrowNativeError("Invalid weapon name or weapon unavailable");
	}
	CTerrorWeaponInfo *pInfo = g_pMeleeWeaponInfo->Element(iIndex);
	
	float * attr = FloatIdToAttr(pInfo, params[2]);
	if(attr == NULL)
	{
		return pContext->ThrowNativeError("Invalid attribute id");
	}
	return sp_ftoc(*attr);
	return 0;
}

// native L4D2_SetIntMeleeAttribute(const String:weaponName[], L4D2IntMeleeWeaponAttributes:attr, value);
cell_t L4D2_SetIntMeleeAttribute(IPluginContext *pContext, const cell_t *params)
{
	if (g_pMeleeWeaponInfo == NULL)
	{
		return pContext->ThrowNativeError("MeleeWeaponInfo unavailable or unsupported. File a bug report.");
	}
	char * weapon = NULL;
	pContext->LocalToString(params[1], &weapon);
	int iIndex = g_pMeleeWeaponInfo->Find(weapon);

	if (!g_pMeleeWeaponInfo->IsValidIndex(iIndex))
	{
	    return pContext->ThrowNativeError("Invalid weapon name or weapon unavailable");
	}
	CTerrorWeaponInfo *pInfo = g_pMeleeWeaponInfo->Element(iIndex);
	
	int * attr = IntIdToAttr(pInfo, params[2]);
	if(attr == NULL)
	{
		return pContext->ThrowNativeError("Invalid attribute id");
	}
	*attr=params[3];
	return 0;
}

// native Float:L4D2_SetFloatMeleeAttribute(const String:weaponName[], L4D2FloatMeleeWeaponAttributes:attr, Float:value);
cell_t L4D2_SetFloatMeleeAttribute(IPluginContext *pContext, const cell_t *params)
{
	if (g_pMeleeWeaponInfo == NULL)
	{
		return pContext->ThrowNativeError("MeleeWeaponInfo unavailable or unsupported. File a bug report.");
	}
	char * weapon = NULL;
	pContext->LocalToString(params[1], &weapon);
	int iIndex = g_pMeleeWeaponInfo->Find(weapon);

	if (!g_pMeleeWeaponInfo->IsValidIndex(iIndex))
	{
	    return pContext->ThrowNativeError("Invalid weapon name or weapon unavailable");
	}
	CTerrorWeaponInfo *pInfo = g_pMeleeWeaponInfo->Element(iIndex);
	
	float * attr = FloatIdToAttr(pInfo, params[2]);
	if(attr == NULL)
	{
		return pContext->ThrowNativeError("Invalid attribute id");
	}
	*attr=sp_ctof(params[3]);
	
	return 0; 
}*/

sp_nativeinfo_t g_L4DoMeleeWeaponNatives[] = 
{
	{"L4D2_GetMeleeWeaponIndex", L4D2_GetMeleeWeaponIndex},
/*	{"L4D2_GetIntMeleeAttribute",	L4D2_GetIntMeleeAttribute},
	{"L4D2_GetFloatMeleeAttribute",	L4D2_GetFloatMeleeAttribute},
	{"L4D2_SetIntMeleeAttribute",	L4D2_SetIntMeleeAttribute},
	{"L4D2_SetFloatMeleeAttribute",	L4D2_SetFloatMeleeAttribute}, */
	{NULL,										NULL}
};
