#include "Animal.h"

Animal::Animal(Texture* _tex, string _name, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _max_age, float _max_speed, float stamina, float _attack_power)
	: Object(_tex, _name, x_pos, y_pos, _depth, _clip), 
	  max_age{_max_age}, max_speed{_max_speed}, stamina{stamina}, attack_power{_attack_power}
{
	//initialize the idle state
	idle = [&]() {
		vec2 pos = { get_x(), get_y() };
		if( (goal.x == 0.0 && goal.y == 0.0) || dist(goal, pos) < 5.0 )
		{
			float radius = 50 + generateRand(50);
			float angle = generateRand(2*314)/100;
			goal.x = pos.x + radius*cos(angle);
			goal.y = pos.x + radius*sin(angle);
		}
		move(pos, goal);
	};
}

float Animal::get_age()
{
	return age;
}

void Animal::move(vec2 from, vec2 to)
{
	vec2 move = normalize(from.x - to.x, from.y - to.y );
	set_x( from.x - move.x );
	set_y( from.y - move.y );
}