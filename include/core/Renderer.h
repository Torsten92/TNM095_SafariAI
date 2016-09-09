#pragma once

#include "Globals.h"
#include "../world/Object.h"

#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	int get_width();
	int get_height();
	
	static bool init();
	void close();
	
	SDL_Renderer* get_renderer();
	
	static void render_frame(vector<Object*>& background, vector<Object*>& v, float scale);

	void clear_frame();
	void update_screen();

	static void render_object(Object* o);

private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
};