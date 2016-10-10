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

	//retrives a case based on animals current visible worldstate. Herbivores and Carnivores reason differently.
	int retrieve_herbivore(WorldState s);
	int retrieve_carnivore(WorldState s);

	//after the retrieval step, a number of cases is passed to the reasoner.
	int reason_herbivore(vector<Case> cases);
	int reason_carnivore(vector<Case> cases);

	//instead of making new cases all the time, we may adapt the ones we have to fit new criteria
	void adapt_case(Case c);

	//Initializes the casebase based on which animal type it is
	void init(int type);

private:
	Object* me_obj;
	vector<Case> cases;

	void add_case(Case c);
};

#endif