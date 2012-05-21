#ifndef _INCLUDE_SOURCEMOD_DETOUR_SELECT_WEIGHTED_SEQUENCE_
#define _INCLUDE_SOURCEMOD_DETOUR_SELECT_WEIGHTED_SEQUENCE_

#include "detour_template.h"

class CTerrorPlayer;

namespace Detours {

class SelectWeightedSequence;

typedef int (SelectWeightedSequence::*SelectWeightedSequenceFunc)(int);

class SelectWeightedSequence : public DetourTemplate<SelectWeightedSequenceFunc, SelectWeightedSequence>
{
private: //note: implementation of DetourTemplate abstracts

	int OnSelectWeightedSequence(int);

	// get the signature name (i.e. "SelectWeightedSequence") from the game conf
	virtual const char *GetSignatureName()
	{
		return "SelectWeightedSequence";
	}

	//notify our patch system which function should be used as the detour
	virtual SelectWeightedSequenceFunc GetDetour()
	{
		return &SelectWeightedSequence::OnSelectWeightedSequence;
	}
};

};
#endif
