#pragma once

#include "Globals.h"
#include "../world/Object.h"

#include <vector>

class Camera
{
public:
	Camera();
	
	void update(const vector<Object*>& background, const vector<Object*>& v, float screen_w, float screen_h);

	void set_view(const vector<Object*>& v, float screen_w, float screen_h);
	void set_background_view(const vector<Object*>& v, float screen_w, float screen_h);

	int get_x_pos();
	int get_y_pos();
	float get_scale();

	void reset_bounds();

	void handle_input();
	
private:
	float x_pos, y_pos;
	float x_min, y_min, x_max, y_max; //we can not move camera outside of these bounds. Decided by animal positions.

	//current zoom level
	float scale = 1.0;

	bool mouse_down = false;
	int mouse_last_x = 0, mouse_last_y = 0;

	void set_x(float x);
	void set_y(float y);
};