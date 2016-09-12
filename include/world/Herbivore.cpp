#include "Herbivore.h"

Herbivore::Herbivore(Texture* _tex, string _name, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _size, float _food_value, float _prefer_company, float _max_age, float _max_speed, float _stamina, float _attack_power)
	: Animal(_tex, _name, x_pos, y_pos, _depth, _clip, _max_age, _max_speed, _stamina, _attack_power)
{
	size = _size;
	food_value = _food_value;
	prefer_company = _prefer_company;
	current_state = idle;
}

void Herbivore::update()
{
	age += dt;
	hunger_level -= dt * 0.001;
	cout << get_name() << " hunger = " << hunger_level << " alive = " << alive << " food value = " << food_value << endl;
	if(hunger_level <= 0.0 || age > max_age)
	{
		alive = false;
		current_state = dead;
	}

	current_state();

}