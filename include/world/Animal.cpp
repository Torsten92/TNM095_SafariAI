#include "Animal.h"

Animal::Animal(int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos, int y_pos, int _depth, SDL_Rect _clip, 
	float _max_age, float _max_speed, float stamina, float _attack_power, float _size, float _food_value)
	: Object(_tex, _type, x_pos, y_pos, _depth, _clip), selected_tex{_selected_tex},
	  max_age{_max_age}, max_speed{_max_speed}, stamina{stamina}, attack_power{_attack_power}, size{_size}, food_value{_food_value}
{
	cbr = new CBR(this);
	get_objects_in_radius = scan_func;

	//initialize the idle state
	idle = [&]() {
		current_action = IDLE; //ugly hack to make sure state and action correspond to each other

		state_timer = max(0.f , state_timer - dt);
		vec2 pos = { get_x(), get_y() };

		//only update goal if current movement is finished
		if( (goal.x == 0.0 && goal.y == 0.0) || dist(goal, pos) < 5.0 )
		{
			goal = pos - flocking_dir;
		}

		//if(state_timer == 0.0)
			move(pos, goal, 0.15);
	};

	//NOTE: Hardcoded for herbivores right now!
	//initialize the find_food state
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
			else if( dist(get_x(), get_y(), o->get_x(), o->get_y()) < 5.0 && get_id() != o->get_id() ) {
					pos.x += get_x() - o->get_x();
					pos.y += get_y() - o->get_y();
				}
		}

		if(goal.x == 0.0 && goal.y ==0.0) {
			goal = pos - flocking_dir;
		}

		move(pos, goal, 0.25);
	};

	eat = [&]() {
		current_action = EAT; //ugly hack to make sure state and action correspond to each other
		
		if(interacting_object != nullptr) { // interacting_object should be the grass currently eaten
			Grass* g = dynamic_cast<Grass*>(interacting_object);
			g->eat(0.1 * size); // large animals eat more, but does not increase hunger_level more.
			hunger_level = min(hunger_level + 0.1 * dt, 1.0);
		}
		else {
			current_state = find_food; // find next patch of grass
		}
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
	texture->render(true_x * scaleX, true_y * scaleY, &clip, scaleX, scaleY);
	
	if(dead_tex != nullptr)
		dead_tex->render(true_x * scaleX, true_y * scaleY, &clip, scaleX, scaleY);

	if(selected && selected_tex != nullptr)
		selected_tex->render(true_x * scaleX, true_y * scaleY, &clip, scaleX, scaleY);
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

float Animal::get_hunger()
{
	return hunger_level;
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

void Animal::set_dead_tex(Texture* tex)
{
	dead_tex = tex;
}

void Animal::set_selected_tex(Texture* tex)
{
	selected_tex = tex;
}

// used by flocking algorithm (dist check to prevent possible division by 0)
vec2 calc_avoidance(vec2 dist)
{
	const int MAX_VALUE = 200;
	return length(dist) > 1.0 ? MAX_VALUE * normalize(dist) / length(dist) : MAX_VALUE * normalize(dist);
}

void Animal::update_flocking_behaviour()
{
	//weights to be applied to each final value
	const float w_alignment = 0.02;
	const float w_cohesion = 0.001;
	const float w_avoidance = 0.5;

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

		if(selected) {
			cout << "\n\nFlocking: " << setprecision(5) << length(avg_pos) << ", ";
		}

		flocking_dir += w_alignment * speed_diff + 
						w_cohesion * (avg_pos - vec2(get_x(), get_y())) + 
						w_avoidance * avoidance_vec;
	}

	//add some randomness. If no other animal is in sight, this will make our animal wander aimlessly.
	float radius = 0.01 + generateRand(10) / 100.0;
	float angle = generateRand(2*314)/100;
	vec2 rand_dir = { radius*cos(angle), radius*sin(angle) };
	flocking_dir += rand_dir;

	if(selected) {
		cout << length(rand_dir) << "\n\n";
	}

}

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

bool Animal::is_selected()
{
	return selected;
}

void Animal::set_selected(bool val)
{
	selected = val;
}

void Animal::print_info()
{
	string animal_type;

	switch(get_type()) {
		case DEER:
			animal_type = "Deer";
			break;
		case WOLF:
			animal_type = "Wolf";
			break;
		case BEAR:
			animal_type = "Bear";
			break;
		default:
			animal_type = "Unknown";
			break;
	}

	cout << "Animal selected: " << animal_type << ". Id: " << get_id() <<
	"\n[" << fixed <<
	"\n  hunger:" << setprecision(2) << hunger_level << 
	"\n  age: " << setprecision(1) << age << 
	"\n  animals/grass in sight: " << get_objects_in_radius(get_x(), get_y(), scan_radius).size() - 1 << 
	"\n]\n";
}