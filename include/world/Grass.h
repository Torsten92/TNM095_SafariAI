#pragma once

#include "Object.h"

class Grass : public Object
{
public:
	Grass( Texture* _tex, string _name, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );

	int get_stage();
	float get_age();
	float get_spawner_radius();
	int get_neighbors();
	
	void scan_area();

	void set_stage(int val);
	
	void update();
private:
	int stage = 1; //1 = low. 2 = mid. 3 = high.
	float age = 0; //in seconds. Aging stages: 0-10 = low. 10-20 = mid. 20+ = high.
	const float scan_radius = 10.0; //the radius each grass checks for other grass
	float spawner_radius = 0; //the radius in which grass can spawn new grass. Decided by amount of neighboring grass.
};