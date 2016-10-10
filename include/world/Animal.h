#ifndef ANIMAL_
#define ANIMAL_


#include "Object.h"
#include "Grass.h"
#include "../core/Utilities.h"
#include "../CBR/CBR.h"

#include <float.h> // required for FLT_MAX
#include <vector>
#include <functional>
#include <iomanip>

using namespace utilities;

class Animal : public Object
{
public:
	virtual ~Animal() = default;
	Animal( int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 },
			float _max_age = 1000.0, float _max_speed = 5.0, float stamina = 0.5, float _attack_power = 0.5, float _size = 1.0, float food_value = 0.5 );

	void render(float scaleX = 1.0, float scaleY = 1.0) override;

	float get_age();
	float get_food_value();
	float get_size();
	float get_speed();
	float get_hunger();
	int get_action();
	bool is_alive();
	Object* get_interacting_object();

	void set_interacting_object(Object* o);
	void set_dead_tex(Texture* tex);
	void set_selected_tex(Texture* tex);

	virtual void update() = 0;

	bool is_selected();
	void set_selected(bool val);

	//speed_dir is the current speed_vector used by the 
	//flocking algorithm (alignment).
	vec2 speed_dir = {0.0, 0.0};

protected:
	RandomGenerator generateRand;
	CBR* cbr;

	Texture* dead_tex = nullptr;
	Texture* selected_tex = nullptr;


	// Function scan_area returns information of objects in the vicinity of the animal. Decides which 
	// action to take. Uses ObjectHandler's method get_objects_in_radius to find the actual objects.
	vector<Object*> scan_area();
	function<vector<Object*>(float, float, float)> get_objects_in_radius;

	// Gets the current state. Executed every frame.
	function<void()> current_state;
	int current_action; //derived from current_state

	//states/actions
	function<void()> idle;
	function<void()> find_food;
	function<void()> find_mate;
	function<void()> flee;
	function<void()> attack;
	function<void()> fight; //shared state
	function<void()> mate; //shared state
	function<void()> eat; //special shared state (must be with another dead animal, or grass)
	function<void()> dead;

	// Constant variables initialized on creation.
	const float scan_radius = 500.0;
	const float max_age; //age at which the animal dies
	const float max_speed; //maximum running speed
	const float stamina; //determines how fast hunger_level decreases in special draining states
	const float attack_power; //higher means more likely to win a fight

	// Dynamic variables that is generally changed every frame.
	float age = 0.0; //in seconds
	float hunger_level = 0.5; //decreases with time. 0.0 = death. 1.0 = full.
	bool alive = true; //becomes false when animal dies
	float current_speed = 0.0; //current running speed
	float size; //the size of the animal
	float food_value; //amount of hunger animal restores when eaten
	float prefer_company; //higher means more likely to flock with others of its kind
	

	Object* interacting_object = nullptr;	//used by some states that require another object to interact with

	// Some animals like to stay in a group. flocking_dir is set by update_flocking_dir() and represents 
	// an adjusted movement direction towards the group.
	vec2 flocking_dir = {0.0, 0.0};
	void update_flocking_behaviour();

	// Used by the Case-based Reasoning machine to determine which action to take when in contact with another animal
	struct Species
	{
		unsigned int type;
		float danger_level;
		float hunt_level;
	};

	vector<Species> discovered_species;

	// State specific variables. Should probably be moved to separate class or something
	vec2 goal = { 0.0, 0.0 };
	void move(vec2 from, vec2 to, float speed_percent);

	// Display detailed information of the currently selected animal
	bool selected = false;

	//used for debugging purposes
	void print_info();
};

#endif