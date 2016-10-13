#include "Carnivore.h"

Carnivore::Carnivore(int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip )
	: Animal(_type, _tex, _selected_tex, scan_func, x_pos, y_pos, _depth, _clip)
{
	init(type);
	goal = { get_x(), get_y() };
	//speed_dir = {0.0, -1.0};
	
	find_food = [&]() {
		current_action = FIND_FOOD; //ugly hack to make sure state and action correspond to each other

		vec2 pos = { get_x(), get_y() };
		float shortest_dist = FLT_MAX;
		
		//loop through all nearby objects, searching for animals of the same kind.
		for(auto& o : get_objects_in_radius(get_x(), get_y(), scan_radius))
		{
			if(auto a = dynamic_cast<Animal*>(o)) {
				if(!a->is_alive() && dist(get_x(), get_y(), a->get_x(), a->get_y()) < shortest_dist) {
					shortest_dist = dist(get_x(), get_y(), a->get_x(), a->get_y());
					goal.x = a->get_x();
					goal.y = a->get_y();
				}
			}
		}

		if(dist(goal, pos) < 10.0) {
			//cout << length(flocking_dir) << " : " << generateRand.distribution(0.0, M_PI/4) << endl;
			goal = pos + flocking_dir + 10.0 * rotate(speed_dir, generateRand.distribution(0.0, M_PI/8));
		}

		move(pos, goal, (type == WOLF ? 0.25 : 0.2));
	};

	eat = [&]() {
		current_action = EAT; //ugly hack to make sure state and action correspond to each other
		
		if(interacting_object != nullptr) { // interacting_object should be the animal currently being eaten
			if(Animal* a = dynamic_cast<Animal*>(interacting_object)) {
				a->eat_from(0.1 * size); // large animals eat more, but does not increase hunger_level more.
				hunger_level = min(hunger_level + 0.1 * dt, 1.0);
				grow();
			}
		}
		else {
			current_state = find_food; // find next prey
		}
	};


	//starting state
	current_state = find_food;
}

void Carnivore::update()
{
	age += dt;
	hunger_level -= dt/1000;

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
			case FIGHT:
				current_state = fight;
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

void Carnivore::init(int _type)
{
	cbr->init(_type);

	switch(type)
	{
		case WOLF:
		{
			w_alignment =  generateRand.distribution(0.015);
			w_cohesion = generateRand.distribution(0.0005);
			w_avoidance = generateRand.distribution(0.9);

			scan_radius = 600.0;
			max_age = 1000.0;
			max_speed = generateRand.distribution(7.0, 1.5);
			attack_power = 2.5;
			fear_factor = generateRand.distribution(0.6, 0.15);
			size = 0.9;
			break;
		}

		case BEAR:
		{
			w_alignment =  generateRand.distribution(0.012);
			w_cohesion = generateRand.distribution(0.0004);
			w_avoidance = generateRand.distribution(0.6);

			scan_radius = 500.0;
			max_age = 1000.0;
			max_speed = generateRand.distribution(5.0, 1.0);
			attack_power = 3.5;
			fear_factor = generateRand.distribution(0.4, 0.15);
			size = 1.4;
			break;
		}
	}

	food_value = size / 2.0;
}