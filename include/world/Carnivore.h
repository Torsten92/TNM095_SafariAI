#pragma once

#include "Animal.h"

class Carnivore : public Animal
{
public:
	Carnivore( int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 },
				float _size = 1.0, float _food_value = 1.0, float _prefer_company = 0.5, float _max_age = 1000.0, float _max_speed = 2.0, float stamina = 0.5, float _attack_power = 0.5 );

	void update() override;
private:

};