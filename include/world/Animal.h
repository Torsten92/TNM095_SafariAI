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
	Animal( int _type, Texture* _tex, Texture* _selected_tex, function<vector<Object*>(float, float, float)> scan_func, int x_pos = 0, int y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );

	void render(float scaleX = 1.0, float scaleY = 1.0) override;

	float get_age();
	float get_food_value();
	float get_size();
	float get_speed();
	float get_max_speed();
	float get_hunger();
	float get_fear_factor();
	int get_action();
	bool is_alive();
	Object* get_interacting_object();

	void set_interacting_object(Object* o);
	void set_dead_tex(Texture* tex);
	void set_selected_tex(Texture* tex);

	virtual void update() = 0;

	bool is_selected();
	void set_selected(bool val);

	//Called when another animal is eating this animal
	void eat_from(float amount);

	//Resolves a fight between this animal and its interacting object
	void resolve_fight();
	void take_damage(float val);
	float get_fight_value();

	void set_case_speed(float val);

	void resolve_collisions();

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

	// Constant variables initialized on creation. (most are actually not constant  because of 
	// current alternative implementation, of which I am too lazy to fix)
	float scan_radius = 500.0; //how far the animal can see. Standard value may change depending on animal type.
	float max_age; //age at which the animal dies
	float max_speed; //maximum running speed. ~3 is considered slow while ~8 is considered high.
	float stamina; //determines how fast hunger_level decreases in special draining states.
	float attack_power; //higher means more likely to win a fight. 1.0 is average, 0 means always lose. Carnivores often have around 2 or more.
	float fear_factor; //higher means more likely to flee from danger. 1.0 is average, 0 means always attack if possible! No real upper limit, but 2.0 is considered high

	// Dynamic variables that is generally changed every frame.
	float age = 0.0; //in seconds
	float hunger_level = 0.8; //decreases with time. 0.0 = death. 1.0 = full.
	bool alive = true; //becomes false when animal dies
	float current_speed = 0.0; //current running speed
	float size; //the size of the animal
	float food_value; //amount of hunger animal restores when eaten (currently always equal to size/2)


	Object* interacting_object = nullptr;	//used by some states that require another object to interact with

	void grow();

	// Some animals like to stay in a group. flocking_dir is set by update_flocking_dir() and represents 
	// an adjusted movement direction towards the group.
	vec2 flocking_dir = {0.0, 0.0};
	float w_alignment = 0.012;
	float w_cohesion = 0.0005;
	float w_avoidance = 0.8;
	void update_flocking_behaviour();

	// State specific variables. Should probably be moved to separate class or something
	vec2 goal = { 0.0, 0.0 };
	void move(vec2 from, vec2 to, float speed_percent);
	float case_speed = 0.5; // the speed that is returned by a potential case

	// Display detailed information of the currently selected animal
	bool selected = false;

	//used for debugging purposes
	void print_info();
};

#endif