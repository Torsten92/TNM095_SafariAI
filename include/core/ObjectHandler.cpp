#include "ObjectHandler.h"

ObjectHandler::ObjectHandler(Resources* _res, Camera* _cam)
{
	init_members(_res, _cam);
}

void ObjectHandler::init_members(Resources* _res, Camera* _cam)
{
	resources = _res;
	camera = _cam;
	set_map(1);
}

// Change the currently loaded map to the map specified by the argument val.
void ObjectHandler::set_map(int val)
{
	object_list.clear();
	if(val == 1)
	{
		//create background
		for(int i = 0; i <= 20; i++)
			for(int j = 0; j <= 20; j++)
				background.push_back( new Object(resources->get_texture("../assets/ground.png"), "background", -2400+i*240, -1800+j*180, -1) );
		
		srand(time(NULL));
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				object_list.push_back( new Grass(resources->get_texture("../assets/grass_low.png"), "grass", -150+generateRand(800), 0+generateRand(500)) );

		object_list.push_back( new Herbivore(resources->get_texture("../assets/deer.png"), "deer", bind(&ObjectHandler::get_objects_in_radius, this, _1, _2, _3 ), 50, 50) );
		object_list.push_back( new Herbivore(resources->get_texture("../assets/deer.png"), "deer", bind(&ObjectHandler::get_objects_in_radius, this, _1, _2, _3 ), 150, 50) );
		object_list.push_back( new Herbivore(resources->get_texture("../assets/deer.png"), "deer", bind(&ObjectHandler::get_objects_in_radius, this, _1, _2, _3 ), 100, 0) );
		object_list.push_back( new Object(resources->get_texture("../assets/bear.png"), "bear object", 250, 250) );
		object_list.push_back( new Object(resources->get_texture("../assets/wolf.png"), "wolf object", 350, 150) );
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
	copy_if(object_list.begin(), object_list.end(), back_inserter(nearby_objects),
		[&](Object* const & o)
		{
			return sqrt( pow(pos_x - o->get_x(), 2) + pow(pos_y - o->get_y(), 2) ) < radius;
		});
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
	//handle per-frame computation for specific objects
	for(auto& o : get_objects())
	{
		//Handle animals
		if( Animal* a = dynamic_cast<Animal*>(o) )
		{
			a->update(); //Different update functions for herbivores and carnivores

			if(!a->is_alive())
				a->set_dead_tex(resources->get_texture("../assets/dead.png"));

			if(a->get_food_value() <= 0.0)
			{
				object_list.erase(find(object_list.begin(), object_list.end(), a));
				continue;
			}
		}	
		
		//Handle grass
		if( Grass* g = dynamic_cast<Grass*>(o) )
		{
			g->update();
			float food_val = g->get_food_value();
			if( 0.00 < food_val && food_val <= 0.05 )
				g->change_texture(resources->get_texture("../assets/grass_low.png"));
			if( 0.05 < food_val && food_val <= 0.10 )
				g->change_texture(resources->get_texture("../assets/grass_mid.png"));
			if( 0.10 < food_val )
				g->change_texture(resources->get_texture("../assets/grass_high.png"));

			if(0.10 < food_val) 
			{
				if(update_info || g->get_spawner_radius() == 0)
				{
					g->scan_area();
				}

				//chance for each grass to spawn new grass
				if(generateRand(5000) == 0)
				{
					float r = g->get_spawner_radius();
					object_list.push_back( new Grass(resources->get_texture("../assets/grass_low.png"), "grass", 
						g->get_x() + r*cos(generateRand(2*314) / 100.0), g->get_y() + r*sin(generateRand(2*314) / 100.0)) );
				}
			}
		}
	}
}