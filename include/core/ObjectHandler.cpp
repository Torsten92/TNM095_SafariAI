#include "ObjectHandler.h"

ObjectHandler::ObjectHandler(Resources* _res, Camera* _cam)
{
	init_members(_res, _cam);
}

void ObjectHandler::init_members(Resources* _res, Camera* _cam)
{
	resources = _res;
	camera = _cam;
	camera->set_object_list(&object_list);
	set_map(1);
}

// Change the currently loaded map to the map specified by the argument val.
void ObjectHandler::set_map(int val)
{
	object_list.clear();
	if(val == 1)
	{
		//create background (20x20 is enough to cover screen fully zoomed out)
		for(int i = 0; i <= 20; i++)
			for(int j = 0; j <= 20; j++) {
				background.push_back( new Object(
						resources->get_texture("../assets/ground.png"),
						-1,
						-2400+i*240,
						-1800+j*180,
						-1)
				);
			}

		for(int i = 0; i < 40; i++)
			for(int j = 0; j < 40; j++) {
				object_list.push_back( new Grass(
						resources->get_texture("../assets/grass_low.png"), 
						-5000+generateRand(10000), 
						-5000+generateRand(10000)) 
				);
			}
		for(int i = 0; i < 100; i++) {
				object_list.push_back( new Herbivore(
						DEER,
						resources->get_texture("../assets/deer.png"),
						resources->get_texture("../assets/selection_circle.png"),
						bind(&ObjectHandler::get_objects_in_radius, this, _1, _2, _3 ),
						-500+generateRand(1500),
						-500+generateRand(1000) )
				);
			}
	}
}

//Inserts object o in object_list, sorted by depth
struct PointerCompare {
	bool operator()(Object* o1, Object* o2) {
		return (*o1) < (*o2);
	}
};

//Returns all objects in the scene, sorted by depth.
//We need to re-sort every frame to render correct object depth. Possibly a better solution 
//exist, but this works for now.
vector<Object*> ObjectHandler::get_objects()
{
	vector<Object*> nearby_objects;

	copy(object_list.begin(), object_list.end(), back_inserter(nearby_objects));
	sort(nearby_objects.begin(), nearby_objects.end(), PointerCompare());
	
	return nearby_objects;
}

//Returns all objects within a radius of a position.
//Used by animals when scanning for other objects in their vicinity.
vector<Object*> ObjectHandler::get_objects_in_radius(float pos_x, float pos_y, float radius)
{
	vector<Object*> nearby_objects;
	
	for(auto& o : object_list) {
		if(sqrt( pow(pos_x - o->get_x(), 2) + pow(pos_y - o->get_y(), 2) ) < radius) {
			nearby_objects.push_back(o);
		}
	}
	
	return nearby_objects;
}

vector<Object*> ObjectHandler::get_background()
{
	return background;
}

//Updates all objects in the scene. (position, state etc.)
//May also create new objects if certain conditions are met
void ObjectHandler::update()
{
	bool update_info = false;
	if(update_counter >= 30) 
	{
		update_info = true;
		update_counter = 0;
	}

	//if new grass is spawned, its position is first stored here
	vector<vec2> new_grass;

	//handle per-frame computation for specific objects
	for( auto it = object_list.begin(); it != object_list.end(); )
	{
		//cout << "looping " << (*it)->get_id() << endl;
		bool erased = false; //if an object becomes deleted, this is set to true
		Object* o = *it;

		//handle animals
		if( Animal* a = dynamic_cast<Animal*>(o) )
		{
			a->update(); //Different update functions for herbivores and carnivores
			if(!a->is_alive())
				a->set_dead_tex(resources->get_texture("../assets/dead.png"));

			if(a->get_food_value() <= 0.0) {
				it = removeObject(it);
				it = object_list.begin();
				erased = true;
			}

/*
			// awful collision handling that just separates animals based on random values
			for( auto o2 : object_list ) {
				if( dynamic_cast<Animal*>(o2) && dist(o->get_x(), o->get_y(), o2->get_x(), o2->get_y()) < 0.5 ) {
					cout << "animal nearby: " << o2->get_type() << endl;
					float angle = generateRand(generateRand(2*314)/100);
					//o->set_x(o->get_x() + 0.5 * cos(angle));
					//o->set_y(o->get_y() + 0.5 * sin(angle));
				}
			}
*/
		}
		
		//handle grass
		else if( Grass* g = dynamic_cast<Grass*>(o) )
		{
			g->update();
			float food_val = g->get_food_value();
			if( food_val <= 0.0 ) {
				it = removeObject(it);
				it = object_list.begin();
				erased = true;
			}
			
			if( 0.00 < food_val && food_val <= 0.25 )
				g->change_texture(resources->get_texture("../assets/grass_low.png"));
			if( 0.25 < food_val && food_val <= 0.50 )
				g->change_texture(resources->get_texture("../assets/grass_mid.png"));
			if( 0.50 < food_val )
				g->change_texture(resources->get_texture("../assets/grass_high.png"));

			if(0.10 < food_val) {
				if(update_info || g->get_spawner_radius() == 0) {
					g->scan_area();
				}

				//chance for each grass to spawn new grass
				if(generateRand(5000) == 0) {
					float r = g->get_spawner_radius();
					new_grass.push_back(vec2(g->get_x() + r*cos(generateRand(2*314) / 100.0), 
											 g->get_y() + r*sin(generateRand(2*314) / 100.0)));
				}
			}
		}

		//iterate forward
		if(!erased) 
			++it;
	}

	//Creation of new objects should be handled last
	if(new_grass.size() && object_list.size() < MAX_OBJECTS) {
		for(auto g : new_grass)
		object_list.push_back( new Grass(resources->get_texture("../assets/grass_low.png"), g.x, g.y) );
	}
}

vector<Object*>::iterator ObjectHandler::removeObject(vector<Object*>::iterator it)
{
	for(auto& o : object_list) {
		if(auto a = dynamic_cast<Animal*>(o)) {
			if(a->get_interacting_object() && *(a->get_interacting_object()) == (**it)) {
				a->set_interacting_object(nullptr);
			}
		}
	}

	//cout << "deleting object: " << (*it)->get_id() << endl;
	delete *it;
	it = object_list.erase(it);

	return it;
}