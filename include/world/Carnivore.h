#pragma once

#include "Animal.h"

class Carnivore : public Animal
{
public:
	Carnivore( int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );

	void update() override;
private:
	float flocking_timer = 0.0;
	float print_info_timer = 0.0;

	void init(int _type);
};