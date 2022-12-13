#if defined __l4do_constants_included
	#endinput
#endif
#define __l4do_constants_included

// Provided by "BHaType":
// For the "L4D_State_Transition" native.
// X -> Y (means X state will become Y state on next frame or some seconds later)
enum
{
	STATE_ACTIVE = 0,
	STATE_WELCOME,				// -> STATE_PICKING_TEAM
	STATE_PICKING_TEAM,
	STATE_PICKINGCLASS,			// -> STATE_ACTIVE
	STATE_DEATH_ANIM,			// -> STATE_DEATH_WAIT_FOR_KEY
	STATE_DEATH_WAIT_FOR_KEY,	// -> STATE_OBSERVER_MODE
	STATE_OBSERVER_MODE,
	STATE_WAITING_FOR_RESCUE,
	STATE_GHOST,
	STATE_INTRO_CAMERA
};

enum
{
	KNOCKDOWN_HUNTER = 1,
	KNOCKDOWN_TANK = 2,
	KNOCKDOWN_CHARGER = 3
};
