#include "Herbivore.h"

Herbivore::Herbivore(int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _size, float _food_value, float _prefer_company, float _max_age, float _max_speed, float _stamina, float _attack_power)
	: Animal(_type, _tex, _selected_tex, scan_func, x_pos, y_pos, _depth, _clip, _max_age, _max_speed, _stamina, _attack_power)
{
	size = _size;
	food_value = _food_value;
	prefer_company = _prefer_company;
	
	//starting state
	current_state = find_food;

	cbr->init(DEER);
}

void Herbivore::update()
{
	age += dt;
	hunger_level -= dt * 0.005;
	//cout << get_type() << " hunger = " << hunger_level << " alive = " << alive << " food value = " << food_value << " current action = " << current_action << endl;
	if(hunger_level <= 0.0 || age > max_age)
	{
		alive = false;
		current_state = dead;
	}

	//override timer if we are hungry. This is a pretty hardcoded approach that may need refactoring later.
	if( flocking_timer > 1.0 || (hunger_level < 0.8 && dist(get_x(), get_y(), goal.x, goal.y) < 1.0) ) {
		flocking_timer = 0.0;
		update_flocking_behaviour();
	}

	//retrieve new state from case-base (the "fight" and "dead" state can not be entered willingly, they are 
	//derived from the "attack" state)
	if(alive) {
		current_action = cbr->retrieve_herbivore(WorldState(scan_area()));

		switch(current_action) {
			case IDLE:
				current_state = idle;
				break;
			case FIND_FOOD:
				current_state = find_food;
				break;
			case FIND_MATE:
				current_state = find_mate;
				break;
			case FLEE:
				current_state = flee;
				break;
			case ATTACK:
				current_state = attack;
				break;	
			case EAT:
				current_state = eat;
				break;
			case MATE:
				current_state = mate;
				break;		
		}	
	}
	else {
		current_state = dead;
	}

	//update animal according o its current state
	current_state();

	if(selected && print_info_timer > 2.0) {
		print_info();
		print_info_timer = 0.0;
	}

	flocking_timer+= dt;
	print_info_timer+= dt;
}