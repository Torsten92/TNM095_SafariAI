#ifndef CBR_
#define CBR_

class CBR;

#include "../core/Globals.h"
#include "WorldState.h"

#include <vector>

// Contains information for a single case
// The CBR handles cases based on this information.
// For some cases certain parameters may be unnessecary. In this case, the parameter is set 
// to -1 since it should alway take positive values otherwise. (ex: when hungry, we don't care about our size)

struct Case
{
public:
	//Constructor. Each new case must be default initialized.
	Case(WorldState ws, int action)
		: current_state{ws}, resulting_action{action}
	{}

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
	CBR(Object* _me);
	int retrieve(WorldState s);

	//Initializes the casebase based on which animal type it is
	void init(int type);

private:
	Object* me_obj;
	vector<Case> cases;

	void add_case(Case c);
};

#endif