#ifndef WORLDSTATE_
#define WORLDSTATE_


#include "../core/Globals.h"
#include "../core/Utilities.h"
#include "../world/Animal.h"

#include <vector>

using namespace utilities;


// Holds information of an animals current state i.e. what it can see and how it feels (public information 
// of other nearby animals and its own relevant information).

struct WorldState
{
	//Constructor. Each world-state must be initialized.
	WorldState(vector<Object*> na) : nearby_animals{na} {}

	vector<Object*> nearby_animals;
};

#endif