#ifndef CBR_
#define CBR_

class CBR;

#include "../core/Globals.h"
#include "../world/Animal.h"

#include <vector>

// Contains information for a single case
// The CBR handles cases based on this information.
// For some cases certain parameters may be unnessecary. In this case, the parameter is set 
// to -1 since it should alway take positive values otherwise. (ex: when hungry, we don't care about our size)

/*
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
*/

struct Case
{
	//resulting speed s not fixed at creation, but rather adapted to the situation
	Case(float m_s, float m_h, int o_t, float o_si, float o_sp, float d, int res_a, float i = 1.0, float res_s = 0.5);
	
	//we want to remember how hungry- and how big we were at the time of this case to compare
	float my_size;
	float my_hunger;

	int other_type;
	float other_size;
	float other_speed;
	float distance;
	float importance; //how highly ranked a case is

	int resulting_action;
	float resulting_speed;
	Object* resulting_interacting_object;
	float resulting_weight;
};


// The Case-Based Reasoner handles cases and is responsible for updating an animals state regarding 
// its reactions to its environment.

class CBR
{
public:
	CBR(Object* _me);

	//retrives a case based on animals current visible worldstate.
	int retrieve(vector<Object*> nearby_animals);

	//after the retrieval step, a number of cases is passed to the reasoner. Herbivores and Carnivores reason differently.
	int reason_herbivore(vector<Case> cases);
	int reason_carnivore(vector<Case> cases);

	//instead of making new cases all the time, we may adapt the ones we have to fit new criteria
	void adapt_case(Case* c);

	//Initializes the casebase based on which animal type it is
	void init(int type);

private:
	Object* me_obj;
	bool i_am_herbivore = false; // should be removed when code is more general

	vector<Case> cases;

	void add_case(Case c);
};

#endif