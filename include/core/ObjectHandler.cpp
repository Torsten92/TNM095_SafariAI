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

//Inserts object o in object_list, sorted by depth
void ObjectHandler::insert_sorted(Object* o)
{
	auto it = object_list.begin();
	for(; it != object_list.end(); ++it)
		if((**it) < (*o)) break;
	object_list.insert ( it, o );
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
				insert_sorted( new Grass(resources->get_texture("../assets/grass_low.png"), "grass", -150+rand()%300, 0+rand()%300) );

		insert_sorted(new Object(resources->get_texture("../assets/deer.png"), "hej", 50, 50));
		insert_sorted(new Object(resources->get_texture("../assets/deer.png"), "hej", 150, 50) );
		insert_sorted(new Object(resources->get_texture("../assets/deer.png"), "hej", 100, 0) );
		insert_sorted(new Object(resources->get_texture("../assets/bear.png"), "hej", 250, 250) );
		insert_sorted(new Object(resources->get_texture("../assets/wolf.png"), "hej", 350, 150) );
	}
}

// Returns a vector containing the objects close enough to fit the screen.
// Dimensions are 1600x1200 around camera. All in all it is 4x the size of camera window
// For a true wild.life simulation this optimisation should probably be turned off to allow
// whole scene to work even when user is not looking
vector<Object*> ObjectHandler::get_objects()
{
	vector<Object*> nearby_objects;
	
	copy_if(object_list.begin(), object_list.end(), back_inserter(nearby_objects),
		[this](Object* const & o)
		{
			return true;
			//return camera->get_x_pos() > o->get_x() - 800 &&
			//	   camera->get_x_pos() < o->get_x() + 800 &&
			//	   camera->get_y_pos() > o->get_y() - 600 &&
			//	   camera->get_y_pos() < o->get_y() + 600;
		});
	
	return nearby_objects;
}

vector<Object*> ObjectHandler::get_background()
{
	return background;
}

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
		//o->set_x(o->get_x()+1);
		/*
		if( Herbivore* h = dynamic_cast<Herbivore*>(o) )
		{
			
		}
		if( Carnivore* c = dynamic_cast<Carnivore*>(o) )
		{
			
		}
		*/
		if( Grass* g = dynamic_cast<Grass*>(o) )
		{
			g->update();
			if(g->get_age() > 2.0 && g->get_stage() == 1)
			{
				g->set_stage(2);
				g->change_texture(resources->get_texture("../assets/grass_mid.png"));
			}
			if(g->get_age() > 4.0 && g->get_stage() == 2)
			{
				g->set_stage(3);
				g->change_texture(resources->get_texture("../assets/grass_high.png"));
			}

			if(g->get_stage() == 3) 
			{
				if(update_info || g->get_spawner_radius() == 0)
				{
					g->scan_area();
				}

				//chance for each grass to spawn new grass
				if(generateRand(1000) == 0)
				{
					float r = g->get_spawner_radius();
					insert_sorted( new Grass(resources->get_texture("../assets/grass_low.png"), "grass", 
						g->get_x() + r*cos(generateRand(2*314) / 100.0), g->get_y() + r*sin(generateRand(2*314) / 100.0)) );
				}
			}
		}
	}
}