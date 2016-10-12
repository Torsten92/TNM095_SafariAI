#include "Herbivore.h"

Herbivore::Herbivore(int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip )
	: Animal(_type, _tex, _selected_tex, scan_func, x_pos, y_pos, _depth, _clip)
{
	init(type);

	find_food = [&]() {
		current_action = FIND_FOOD; //ugly hack to make sure state and action correspond to each other
		vec2 pos = { get_x(), get_y() };
		goal = {0.0, 0.0};
		float shortest_dist = FLT_MAX;

		//loop through all nearby objects, searching for animals of the same kind.
		for(auto& o : get_objects_in_radius(get_x(), get_y(), scan_radius))
		{
			if(auto g = dynamic_cast<Grass*>(o)) {
				if(dist(get_x(), get_y(), g->get_x(), g->get_y()) < shortest_dist) {
					shortest_dist = dist(get_x(), get_y(), g->get_x(), g->get_y());
					goal.x = g->get_x();
					goal.y = g->get_y();
				}
			}
		}

		if(goal.x == 0.0 && goal.y ==0.0) {
			goal = pos + flocking_dir;
		}

		move(pos, goal, 0.25);
	};

	eat = [&]() {
		current_action = EAT; //ugly hack to make sure state and action correspond to each other
		
		if(interacting_object != nullptr) { // interacting_object should be the grass currently eaten
			if(Grass* g = dynamic_cast<Grass*>(interacting_object)) {
				g->eat_from(0.1 * size); // large animals eat more, but does not increase hunger_level more.
				hunger_level = min(hunger_level + 0.1 * dt, 1.0);
				grow();
			}
		}
		else {
			current_state = find_food; // find next patch of grass
		}
	};


	//starting state
	current_state = find_food;
}

void Herbivore::update()
{
	age += dt;
	hunger_level -= dt * 0.005;
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
		current_action = cbr->retrieve(scan_area());
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

	if(selected && print_info_timer > 0.1) {
		print_info();
		print_info_timer = 0.0;
	}

	flocking_timer+= dt;
	print_info_timer+= dt;
}

void Herbivore::init(int _type)
{
	cbr->init(_type);
	
	switch(type)
	{
		case DEER:
		{
			w_alignment =  generateRand.distribution(0.015);
			w_cohesion = generateRand.distribution(0.02); // 0.0005
			w_avoidance = generateRand.distribution(0.6);

			scan_radius = 500.0;
			max_age = 1000.0;
			max_speed = 4.0;
			attack_power = 0.5;
			size = 1.0;
			break;
		}
	}

	food_value = size / 2.0;
}