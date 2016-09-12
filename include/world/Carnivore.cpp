#include "Carnivore.h"

Carnivore::Carnivore(Texture* _tex, string _name, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _size, float _food_value, float _prefer_company, float _max_age, float _max_speed, float _stamina, float _attack_power)
	: Animal(_tex, _name, scan_func, x_pos, y_pos, _depth, _clip, _max_age, _max_speed, _stamina, _attack_power)
{
	size = _size;
	food_value = _food_value;
	prefer_company = _prefer_company; 
}

void Carnivore::update()
{
	age += dt;
	hunger_level -= dt/1000;
}