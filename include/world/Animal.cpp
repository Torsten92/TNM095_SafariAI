#include "Animal.h"

Animal::Animal(int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip )
	: Object(_tex, _type, x_pos, y_pos, _depth, _clip), selected_tex{_selected_tex}
{
	cbr = new CBR(this);
	get_objects_in_radius = scan_func;

	//initialize the idle state
	idle = [&]() {
		current_action = IDLE; //ugly hack to make sure state and action correspond to each other

		vec2 pos = { get_x(), get_y() };

		//only update goal if current movement is finished
		if( (goal.x == 0.0 && goal.y == 0.0) || dist(goal, pos) < 5.0 ) {
			goal = pos + flocking_dir;
		}

		move(pos, goal, 0.15);
	};

	attack = [&]() {
		current_action = ATTACK; //ugly hack to make sure state and action correspond to each other

		vec2 pos = { get_x(), get_y() };

		if(interacting_object != nullptr) { // interacting_object should be the animal we are attacking
			goal.x = interacting_object->get_x();
			goal.y = interacting_object->get_y();

			move(pos, goal, 0.75);
		}
		else {
			current_state = idle; // force state change
		}
	};

	flee = [&]() {
		current_action = FLEE; //ugly hack to make sure state and action correspond to each other

		vec2 pos = { get_x(), get_y() };

		if(interacting_object != nullptr) { // interacting_object should be the animal we are attacking
			vec2 danger { interacting_object->get_x(), interacting_object->get_y() };
			goal = pos + normalize(pos - danger);

			move(pos, goal, 0.6);
		}
		else {
			current_state = idle; // force state change
		}
	};

	fight = [&]() {
		current_action = FIGHT;

		if(interacting_object != nullptr) {
			if(auto a = dynamic_cast<Animal*>(interacting_object)) {
				if(a->is_alive()) {
					resolve_fight();
				}
				else {
					interacting_object = nullptr;
				}
			}
		}
		else {
			current_state = idle; // force state change
		}
	};

	find_food = [&]() {
		//Overridden by subclasses
	};

	eat = [&]() {
		//Overridden by subclasses
	};

	//initialize the dead state
	dead = [&]() {
		current_action = DEAD; //ugly hack to make sure state and action correspond to each other

		food_value -= dt*0.05; // decaying factor
	};
}

void Animal::render(float scaleX, float scaleY)
{
	float true_x = get_x_camera()-texture->get_width() / 2;
	float true_y = get_y_camera()-texture->get_height() / 2;
	texture->render(true_x * scaleX, true_y * scaleY, &clip, scaleX * size, scaleY * size);
	
	if(dead_tex != nullptr)
		dead_tex->render(true_x * scaleX, true_y * scaleY, &clip, scaleX * size, scaleY * size);

	if(selected && selected_tex != nullptr)
		selected_tex->render(true_x * scaleX, true_y * scaleY, &clip, scaleX * size, scaleY * size);
}

vector<Object*> Animal::scan_area()
{
	return get_objects_in_radius(get_x(), get_y(), scan_radius);
}

float Animal::get_age()
{
	return age;
}

float Animal::get_food_value()
{
	return food_value;
}

float Animal::get_size()
{
	return size;
}

float Animal::get_speed()
{
	return current_speed;
}

float Animal::get_max_speed()
{
	return max_speed;
}

float Animal::get_hunger()
{
	return hunger_level;
}

float Animal::get_fight_value()
{
	return attack_power * size;
}


int Animal::get_action()
{
	return current_action;
}

bool Animal::is_alive()
{
	return alive;
}

Object* Animal::get_interacting_object()
{
	return interacting_object;
}

void Animal::set_interacting_object(Object* o)
{
	interacting_object = o;
}

bool Animal::is_selected()
{
	return selected;
}

void Animal::set_selected(bool val)
{
	selected = val;
}

void Animal::set_dead_tex(Texture* tex)
{
	dead_tex = tex;
}

void Animal::set_selected_tex(Texture* tex)
{
	selected_tex = tex;
}

void Animal::eat_from(float amount)
{
	food_value -= amount * dt;
}

void Animal::resolve_fight()
{
	if(interacting_object != nullptr) {
		//attacker will always have an advantage
		auto other = dynamic_cast<Animal*>(interacting_object);
		take_damage( (other->get_fight_value() / get_fight_value() + 0.2) * dt );
		other->take_damage( (get_fight_value() / other->get_fight_value() + 0.4) * dt );
	}

}

void Animal::take_damage(float val)
{
	hunger_level -= val;
}

void Animal::resolve_collisions()
{
	//loop through all nearby objects, searching for animals of the same kind.
	for(auto& o : get_objects_in_radius(get_x(), get_y(), scan_radius))
	{
		if( !dynamic_cast<Grass*>(o) && dist(get_x(), get_y(), o->get_x(), o->get_y()) < 5.0 && get_id() != o->get_id() ) {
			set_x( get_x() + (get_x() - o->get_x()) );
			set_y( get_y() + (get_y() - o->get_y()) );
		}
	}
}

void Animal::grow()
{
	size += (1 / pow(size, 1.5)) * 0.04 * dt;
	food_value = size / 2.0;
}

// used by flocking algorithm (dist length check to prevent possible division by 0)
vec2 calc_avoidance(vec2 dist)
{
	const int MAX_VALUE = 1500;
	return length(dist) > 1.0 ? MAX_VALUE * normalize(dist) / length(dist) : MAX_VALUE * normalize(dist);
}

void Animal::update_flocking_behaviour()
{
	//weights to be applied to each final value
	//const float w_alignment = 0.012;
	//const float w_cohesion = 0.0005;
	//const float w_avoidance = 0.8;

	flocking_dir = {0.0, 0.0};

	int count = 0;
	vec2 speed_diff = {0.0, 0.0};
	vec2 avg_pos = {0.0, 0.0};
	vec2 avoidance_vec = {0.0, 0.0};

	//loop though all nearby objects, searching for animals of the same type
	for(auto& o : get_objects_in_radius(get_x(), get_y(), scan_radius))
	{
		if(get_type() == o->get_type() && get_id() != o->get_id()) {
			if( Animal* a = dynamic_cast<Animal*>(o) ) { //should always be true
				if(a->is_alive()) {
					count++;
					speed_diff += a->speed_dir - speed_dir;
					avg_pos += vec2(a->get_x(), a->get_y());
					avoidance_vec += calc_avoidance(vec2(get_x() - a->get_x(), get_y() - a->get_y()));
				}
			}
		}
	}

	if(count > 0)
	{
		speed_diff = speed_diff / count;
		avg_pos = avg_pos / count;
		avoidance_vec = avoidance_vec / count;

		flocking_dir += w_alignment * speed_diff + 
						w_cohesion * (avg_pos - vec2(get_x(), get_y())) + 
						w_avoidance * avoidance_vec;
	}

	//add some randomness. If no other animal is in sight, this will make our animal wander aimlessly.
	float radius = 0.1 + generateRand(10) / 10.0;
	float angle = generateRand(2*314)/100;
	vec2 rand_dir = { radius*cos(angle), radius*sin(angle) };
	flocking_dir += rand_dir;
}

void Animal::move(vec2 from, vec2 to, float speed_percent)
{
	speed_percent = min(max(speed_percent, 0.0f), 1.0f); //limit values
	current_speed = speed_percent * max_speed;
	//vec2 move = speed_percent * max_speed * (normalize(from - to));
	vec2 move = speed_percent * max_speed * normalize(to - from);
	hunger_level -= pow(speed_percent, 2) * dt * 0.1; //moving closer to max_speed drains hunger quicker. (only )

	speed_dir = move;
	set_x( get_x() + speed_dir.x);
	set_y( get_y() + speed_dir.y);
	resolve_collisions();
}
/*
void Animal::move(vec2 from, vec2 to, float speed_percent)
{
	speed_percent = min(max(speed_percent, 0.0f), 1.0f); //limit values
	current_speed = speed_percent * max_speed;
	//vec2 move = speed_percent * max_speed * (normalize(from - to));
	vec2 move = speed_percent * max_speed * normalize(from - to);
	hunger_level -= pow(speed_percent, 2) * dt * 0.1; //moving closer to max_speed drains hunger quicker. (only )

	speed_dir = from - move;
	set_x( speed_dir.x);
	set_y( speed_dir.y);
}
*/
void Animal::print_info()
{
	string animal_type = convert_type(type);
	string action_taken = convert_action(current_action);
	string interacting_object_str = "";

	if(interacting_object != nullptr) {
		interacting_object_str = convert_type(interacting_object->get_type()) + 
								 "(" + to_string(interacting_object->get_id()) + ")";
	}
	else
		interacting_object_str = "-";

	int animals_in_sight = -1; // -1 since we don't count ourselves
	int grass_in_sight = 0;
	for (auto& o : get_objects_in_radius(get_x(), get_y(), scan_radius)) {
		if(dynamic_cast<Animal*>(o)) animals_in_sight++;
		if(dynamic_cast<Grass*>(o)) grass_in_sight++; 
	} 

	cout << "Animal selected: " << animal_type << " (id: " << get_id() << ")" <<
	"\n[" << fixed <<
	"\n  current action: " << action_taken <<
	"\n  " << (alive ? "hunger:" : "foodvalue left: ") << setprecision(2) << (alive ? hunger_level : food_value) <<
	"\n  age: " << setprecision(1) << age <<
	"\n  size: " << setprecision(2) << size <<
	"\n  current speed: " << setprecision(2) << current_speed << " (" << setprecision(0) << (current_speed / max_speed * 100) << "%)" <<
	"\n  animals/grass in sight: " << animals_in_sight << "/" << grass_in_sight <<
	"\n  position: (" << get_x() << ", " << get_y() << ")" <<
	"\n  interacting with: " << interacting_object_str <<
	"\n  flocking weights: " << setprecision(4) << w_cohesion << ", " << w_alignment << ", " << w_avoidance << " (cohesion, alignment, avoidance)" <<
	"\n]\n";
}