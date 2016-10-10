#pragma once

#include "Globals.h"
#include "../world/Animal.h"

#include <vector>

class Camera
{
public:
	Camera();
	
	void update(const vector<Object*>& background, const vector<Object*>& v, float screen_w, float screen_h);

	void set_view(const vector<Object*>& v);
	void set_background_view(const vector<Object*>& v);

	int get_x_pos();
	int get_y_pos();
	float get_scale();

	void reset_bounds();

	//Called once for initialization
	void set_object_list(vector<Object*>* ol);

	void handle_input(float screen_w, float screen_h);
	
private:
	//camera class handles all keyboard and mouse input
	SDL_Event e;

	float x_pos, y_pos;
	float x_min, y_min, x_max, y_max; //we can not move camera outside of these bounds. Decided by animal positions.

	//current zoom level
	float scale = 1.0;

	bool mouse_down = false;
	int mouse_last_x = 0, mouse_last_y = 0;
	int mouse_down_x = 0, mouse_down_y = 0;

	void set_x(float x);
	void set_y(float y);


	//Camera class also handles selection of objects

	void deselect_object();

	//reference to the currently selected object
	Animal* selected_object;

	//contains all objects in the scene. Used for finding potentially selected objects.
	vector<Object*>* object_list;
};