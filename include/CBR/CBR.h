#include "../core/Globals.h"
#include "WorldState.h"

#include <vector>

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


// The Case-Based Reasoner handles cases and is responsible for updating an animals state regarding 
// its reactions to its environment.

class CBR
{
public:
	CBR();

	int reason(WorldState s);

private:
	vector<Case> cases;

	void add_case(Case c);
};