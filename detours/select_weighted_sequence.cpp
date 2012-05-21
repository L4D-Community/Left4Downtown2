#include "select_weighted_sequence.h"
#include "extension.h"

#define ACT_TERROR_ATTACK_MOVING 790
#define ACT_HULK_ATTACK_LOW 763

namespace Detours
{
	int SelectWeightedSequence::OnSelectWeightedSequence(int Activity)
	{
		//L4D_DEBUG_LOG("CBaseAnimating::SelectWeightedSequence(%d) has been called", Activity);
		cell_t result = Pl_Continue;
		int actualSequence;
		actualSequence = (this->*(GetTrampoline()))(Activity);

		if (Activity != ACT_TERROR_ATTACK_MOVING)
			if (Activity != ACT_HULK_ATTACK_LOW)
				return actualSequence;

		int overrideSequence = actualSequence;
		if (g_pFwdOnSelectTankAttack)
		{
			edict_t *pEntity = gameents->BaseEntityToEdict(reinterpret_cast<CBaseEntity*>(this));
			int client = IndexOfEdict(pEntity);
			L4D_DEBUG_LOG("L4D2_OnSelectTankAttack(client %d, sequence %d) forward has been sent out", client, overrideSequence);
			g_pFwdOnSelectTankAttack->PushCell(client);
			g_pFwdOnSelectTankAttack->PushCellByRef(&overrideSequence);
			g_pFwdOnSelectTankAttack->Execute(&result);
		}

		if(result == Pl_Handled)
		{
			L4D_DEBUG_LOG("CBaseAnimating::SelectWeightedSequence() return value overriden with %d", overrideSequence);
			return overrideSequence;
		}
		else
		{
			return actualSequence;
		}
	}
};
