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

#ifndef _INCLUDE_SOURCEMOD_DETOUR_CHANGE_FINALE_STAGE_H_
#define _INCLUDE_SOURCEMOD_DETOUR_CHANGE_FINALE_STAGE_H_

#include "detour_template.h"

namespace Detours
{
	class ChangeFinaleStage;

	typedef void (ChangeFinaleStage::*ChangeFinaleStageFunc)(int, const char*);

	class ChangeFinaleStage : public DetourTemplate<ChangeFinaleStageFunc, ChangeFinaleStage>
	{
	private: //note: implementation of DetourTemplate abstracts

		void OnChangeFinaleStage(int, const char*);

		// get the signature name (i.e. "ChangeFinaleStage") from the game conf
		virtual const char *GetSignatureName()
		{
			return "ChangeFinaleStage";
		}

		//notify our patch system which function should be used as the detour
		virtual ChangeFinaleStageFunc GetDetour()
		{
			return &ChangeFinaleStage::OnChangeFinaleStage;
		}
	};
};

#endif //_INCLUDE_SOURCEMOD_DETOUR_CHANGE_FINALE_STAGE_H_
