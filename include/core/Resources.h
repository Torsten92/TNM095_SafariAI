#pragma once

#include "Texture.h"

#include <map>

class Resources
{
public:
	Resources() = default; //Should only be used for JavaScript ports. Requires data members to be initialized separately
	Resources(SDL_Renderer* _renderer);
	
	void set_renderer(SDL_Renderer* _renderer);

	Texture* get_texture(string path);
	
private:
	SDL_Renderer* renderer;
	map<string, Texture*> texture_map;
};