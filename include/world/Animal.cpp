#include "Animal.h"

Animal::Animal(Texture* _tex, string _name, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _max_age, float _max_speed, float stamina, float _attack_power, float _size, float _food_value)
	: Object(_tex, _name, x_pos, y_pos, _depth, _clip), 
	  max_age{_max_age}, max_speed{_max_speed}, stamina{stamina}, attack_power{_attack_power}, size{_size}, food_value{_food_value}
{
	scan_area = scan_func;

	//initialize the idle state
	idle = [&]() {

		//note that scan_area also return itself as a nearby object
		vector<Object*> nearby_objects = scan_area(get_x(), get_y(), scan_radius);

		idling = max(0.f , idling - dt);
		vec2 pos = { get_x(), get_y() };
		if( (goal.x == 0.0 && goal.y == 0.0) || dist(goal, pos) < 5.0 )
		{
			//one in four that animal just stands still
			if(idling == 0.f && generateRand(3) == 0)
				idling = 1.0 + generateRand(20) / 10; // 1.0-3.0 seconds
			else
			{
				float radius = 50 + generateRand(50);
				float angle = generateRand(2*314)/100;
				goal.x = pos.x + radius*cos(angle);
				goal.y = pos.y + radius*sin(angle);
			}
		}
		if(idling == 0.0)
			move(pos, goal, 0.15);
	};

	//initialize the find_food state
	find_food = [&]() {

		//note that scan_area also return itself as a nearby object
		vector<Object*> nearby_objects = scan_area(get_x(), get_y(), scan_radius);

		vec2 pos = { get_x(), get_y() };

		//wander around searching for food
		if( (goal.x == 0.0 && goal.y == 0.0) || dist(goal, pos) < 5.0 )
		{
			float radius = 50 + generateRand(50);
			float angle = generateRand(2*314)/100;
			goal.x = pos.x + radius*cos(angle);
			goal.y = pos.y + radius*sin(angle);
		}
		move(pos, goal, 0.25);
	};

	//initialize the dead state
	dead = [&]() {
		food_value -= dt*0.05; // decaying factor
	};
}

void Animal::render(float scaleX, float scaleY)
{
	float true_x = x_pos_camera-texture->get_width()/2;
	float true_y = y_pos_camera-texture->get_height()/2;
	texture->render(true_x * scaleX, true_y * scaleY, &clip, scaleX, scaleY);
	
	if(dead_tex != nullptr)
		dead_tex->render(true_x * scaleX, true_y * scaleY, &clip, scaleX, scaleY);
}

float Animal::get_age()
{
	return age;
}

float Animal::get_food_value()
{
	return food_value;
}

bool Animal::is_alive()
{
	return alive;
}

void Animal::set_dead_tex(Texture* tex)
{
	dead_tex = tex;
}

void Animal::move(vec2 from, vec2 to, float speed_percent)
{
	speed_percent = min(max(speed_percent, 0.0f), 1.0f); //limit values
	vec2 move = speed_percent * max_speed * normalize(from.x - to.x, from.y - to.y );
	hunger_level -= pow(speed_percent, 2) * dt * 0.1; //moving closer to max_speed drains hunger quicker. (only )
	set_x( from.x - move.x );
	set_y( from.y - move.y );
}