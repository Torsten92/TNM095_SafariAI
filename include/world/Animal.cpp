#include "Animal.h"

Animal::Animal(Texture* _tex, string _name, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _max_age, float _max_speed, float stamina, float _attack_power)
	: Object(_tex, _name, x_pos, y_pos, _depth, _clip), 
	  max_age{_max_age}, max_speed{_max_speed}, stamina{stamina}, attack_power{_attack_power}
{}

float Animal::get_age()
{
	return age;
}