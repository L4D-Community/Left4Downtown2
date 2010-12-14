/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown 2 SourceMod Extension
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
 
 #if defined _l4d2weapons_included
  #endinput
#endif
#define _l4d2weapons_included

enum L4D2IntWeaponAttribute
{
	L4D2IWA_Damage,
	L4D2IWA_Bullets,
	L4D2IWA_ClipSize
};

enum L4D2FloatWeaponAttribute
{
	L4D2FWA_SpreadPerShot,
	L4D2FWA_MaxSpread,
	L4D2FWA_SpreadDecay,
	L4D2FWA_MinDuckingSpread,
	L4D2FWA_MinStandingSpread,
	L4D2FWA_MinInAirSpread,
	L4D2FWA_MaxMovementSpread,
	L4D2FWA_PenetrationNumLayers,
	L4D2FWA_PenetrationPower,
	L4D2FWA_PenetrationMaxDist,
	L4D2FWA_CharPenetrationMaxDist,
	L4D2FWA_Range,
	L4D2FWA_RangeModifier,
	L4D2FWA_CycleTime
};

/**
 * @brief Read an int-typed attribute for a given weapon from the WeaponInformationDatabase
 * @remarks Throws an error if the weapon is not found or the attribute is incorrect
 * 
 * @param weaponName 	Weapon to lookup attribute for
 * @param attr			Attribute to read from the weapon's info struct
 * @return 				The value read.
 */
native L4D2_GetIntWeaponAttribute(const String:weaponName[], L4D2IntWeaponAttribute:attr);

/**
 * @brief Read a float-typed attribute for a given weapon from the WeaponInformationDatabase
 * @remarks Throws an error if the weapon is not found or the attribute is incorrect
 * 
 * @param weaponName 	Weapon to lookup attribute for
 * @param attr			Attribute to read from the weapon's info struct
 * @return 				The value read.
 */
native Float:L4D2_GetFloatWeaponAttribute(const String:weaponName[], L4D2FloatWeaponAttribute:attr);

/**
 * @brief Set an int-typed attribute for a given weapon from the WeaponInformationDatabase to a given value
 * @remarks Throws an error if the weapon is not found or the attribute is incorrect
 * 
 * @param weaponName 	Weapon to lookup attribute for
 * @param attr			Attribute to alter in the weapon's info struct
 * @param value			Value to set the attribute to
 * @noreturn
 */
native L4D2_SetIntWeaponAttribute(const String:weaponName[], L4D2IntWeaponAttribute:attr, value);

/**
 * @brief Set a float-typed attribute for a given weapon from the WeaponInformationDatabase to a given value
 * @remarks Throws an error if the weapon is not found or the attribute is incorrect
 * 
 * @param weaponName 	Weapon to lookup attribute for
 * @param attr			Attribute to alter in the weapon's info struct
 * @param value			Value to set the attribute to
 * @noreturn
 */
native Float:L4D2_SetFloatWeaponAttribute(const String:weaponName[], L4D2FloatWeaponAttribute:attr, Float:value);
