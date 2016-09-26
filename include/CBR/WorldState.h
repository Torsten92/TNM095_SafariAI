#include "../core/Globals.h"

#include <vector>

enum actions { IDLE, FIND_FOOD, FIND_MATE, FLEE, ATTACK, FIGHT, MATE, EAT, DEAD };


// Holds the public information of an animal, i.e. what other animals see when they look at it.

struct PublicAnimal
{
	int type;
	float size;
	int action;
	float current_speed;
};


// Holds information of an animals current state i.e. what it can see and how it feels (public information 
// of other nearby animals and its own relevant information).

struct WorldState
{
	int type;
	float hunger;
	float size;
	vector<PublicAnimal> nearby_animals;
};