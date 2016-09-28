#ifndef WORLDSTATE_
#define WORLDSTATE_


#include "../core/Globals.h"
#include "../core/Utilities.h"
#include "../world/Animal.h"

#include <vector>

using namespace utilities;

// Holds the public information of an animal, i.e. what other animals see when they look at it.

struct PublicAnimal
{
	//Constructor. Each public animal must be initialized.
	PublicAnimal(int t, float s, int a, float cs, vec2 p)
		: type{t}, size{s}, action{a}, current_speed{cs}, pos{p}
	{}

	int type;
	float size;
	int action;
	float current_speed;
	vec2 pos;
};


// Holds information of an animals current state i.e. what it can see and how it feels (public information 
// of other nearby animals and its own relevant information).

struct WorldState
{
	//Constructor. Each world-state must be initialized.
	WorldState(vector<Object*> na)
		: nearby_animals{na}
	{}

	vector<Object*> nearby_animals;
};

#endif