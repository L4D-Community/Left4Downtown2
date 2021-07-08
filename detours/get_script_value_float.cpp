/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2009-2011 Downtown1, ProdigySim; 2012-2015 Visor; 2021 A1m`;
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

#include "get_script_value_float.h"
#include "extension.h"

namespace Detours
{
	float GetScriptValueFloat::OnGetScriptValueFloat(const char *key, float defaultValue)
	{
		//L4D_DEBUG_LOG("CDirector::GetScriptValue(%s,%d) has been called", key, defaultValue);

		cell_t result = Pl_Continue;

		float actualInvocationResult = (this->*(GetTrampoline()))(key, defaultValue);

		float overrideValue = actualInvocationResult;
		if (g_pFwdOnGetScriptValueFloat) {
			//L4D_DEBUG_LOG("L4D_OnGetScriptValueFloat() forward has been sent out");
			
			g_pFwdOnGetScriptValueFloat->PushString(key);
			g_pFwdOnGetScriptValueFloat->PushFloatByRef(&overrideValue);
			g_pFwdOnGetScriptValueFloat->Execute(&result);
			
			//float exec = g_pFwdOnGetScriptValueFloat->Execute(&result);
			//L4D_DEBUG_LOG("L4D_OnGetScriptValueFloat() forward result = %d (0 means no error)", exec);
		}

		if (result == Pl_Handled) {
			//L4D_DEBUG_LOG("CDirector::OnGetScriptValueFloat return value overriden with %d", overrideValue);
			
			return overrideValue;
		}

		return actualInvocationResult;
	}
};
