#pragma once

#include "Object.h"

#include <vector>

class Animal : public Object
{
public:
	virtual ~Animal() = default;
	Animal( Texture* _tex, string _name, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 },
			float _max_age = 1000.0, float _max_speed = 2.0, float stamina = 0.5, float _attack_power = 0.5 );

	float get_age();

	void set_stage(int val);

	virtual void update() = 0;
protected:
	float age = 0.0; //in seconds
	float hunger_level = 0.5; //decreases with time. 0.0 = death. 1.0 = full.
	bool alive = true; //becomes false when animal dies

	float size; //the size of the animal
	float food_value; //amount of hunger animal restores when eaten
	float prefer_company; //higher means more likely to flock with others of its kind
	
	//this function decides which action to take
	void scan_area();

	//states/actions
	void find_food();
	void find_mate();
	void flee();
	void attack();
	void fight(); //shared state
	void mate(); //shared state
	void eat(); //special shared state (must be with another dead animal, or grass)
	void dead();

	const float scan_radius = 50.0;
	const float max_age; //age at which the animal dies
	const float max_speed; //maximum running speed
	const float stamina; //determines how fast hunger_level decreases in special draining states
	const float attack_power; //higher means more likely to win a fight

	//Used by the Case-based Reasoning machine to determine which action to take when in contact with another animal
	struct Species
	{
		unsigned int type;
		float danger_level;
		float hunt_level;
	};

	vector<Species> discovered_species;
};