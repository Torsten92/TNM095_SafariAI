#include "Resources.h"

Resources::Resources(SDL_Renderer* _renderer)
	: renderer{_renderer} {}

void Resources::set_renderer(SDL_Renderer* _renderer)
{
	renderer = _renderer;
}

Texture* Resources::get_texture(string path)
{
	//Create if not exist
	if ( texture_map.find(path) == texture_map.end() ) 
		texture_map[path] = new Texture(renderer, path);
	
	return texture_map[path];
}
