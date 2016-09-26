#include "../core/Globals.h"
#include "WorldState.h"

// Contains information for a single case
// The CBR handles cases based on this information.


struct Case
{
public:
	// Holds all relevant information about the animal. What it can see, and its personal information.
	WorldState current_state;

	//Which action to take for this specific case
	int resulting_action;
};

