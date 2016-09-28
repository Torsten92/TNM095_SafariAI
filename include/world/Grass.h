#pragma once

#include "Object.h"

class Grass : public Object
{
public:
	Grass( Texture* _tex, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );

	float get_food_value();
	float get_age();
	float get_spawner_radius();

	void eat(float amount);
	
	void scan_area();

	void update();
private:
	float food_value = 0.05;
	const float scan_radius = 10.0; //the radius each grass checks for other grass
	float spawner_radius = 0; //the radius in which grass can spawn new grass. Decided by amount of neighboring grass.
};