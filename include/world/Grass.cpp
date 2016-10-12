#include "Grass.h"

Grass::Grass(Texture* _tex, int x_pos, int y_pos, int _depth, SDL_Rect _clip)
	: Object(_tex, GRASS, x_pos, y_pos, _depth, _clip)
{}

float Grass::get_food_value()
{
	return food_value;
}

float Grass::get_spawner_radius()
{
	return spawner_radius;
}

void Grass::eat_from(float amount)
{
	food_value -= amount * dt;
}

void Grass::scan_area()
{
	spawner_radius = 150.0;
}

void Grass::update()
{
	food_value = min(max(food_value + dt * 0.025, 0.0), 0.75);
}