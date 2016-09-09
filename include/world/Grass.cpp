#include "Grass.h"

Grass::Grass(Texture* _tex, string _name, int x_pos, int y_pos, int _depth, SDL_Rect _clip)
	: Object(_tex, _name, x_pos, y_pos, _depth, _clip)
{}

int Grass::get_stage()
{
	return stage;
}

float Grass::get_age()
{
	return age;
}

void Grass::set_stage(int val)
{
	stage = val;
}


float Grass::get_spawner_radius()
{
	return spawner_radius;
}

void Grass::scan_area()
{
	spawner_radius = 150.0;
}

void Grass::update()
{
	age += dt;
}