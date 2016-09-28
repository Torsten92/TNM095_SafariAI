#pragma once

#include "Globals.h"
#include "RandomGenerator.h"
#include "Resources.h"
#include "Camera.h"

#include "../world/Object.h"
#include "../world/Herbivore.h"
#include "../world/Carnivore.h"
#include "../world/Grass.h"

#include <algorithm>
#include <iterator>
#include <vector>
#include <functional>

using namespace std::placeholders;

class ObjectHandler
{
public:
	ObjectHandler() = default; //Should only be used for JavaScript ports. Requires data members to be initialized separately
	ObjectHandler(Resources* _res, Camera* _cam);
	
	void init_members(Resources* _res, Camera* _cam);

	void update();
	void set_map(int val);

	void generate_grass(float x, float y);

	vector<Object*> get_objects();
	vector<Object*> get_objects_in_radius(float pos_x, float pos_y, float radius);
	vector<Object*> get_background();
	
private:
	vector<Object*> object_list;
	vector<Object*> sorted_objects;
	vector<Object*> background;
	Resources* resources;
	Camera* camera;
	RandomGenerator generateRand;

	int update_counter = 0; //makes sure we don't need to update some information every frame
	
	vector<Object*>::iterator removeObject(vector<Object*>::iterator it);

	enum game_states
	{
		WORLD,
		MENU
	};

};