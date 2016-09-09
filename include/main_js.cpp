#include <emscripten.h>

#include "core/Globals.h"
#include "core/Renderer.h"
#include "core/Resources.h"
#include "core/Camera.h"
#include "core/ObjectHandler.h"


//Global variables (see Globals.h).
float current_time = 0;
float dt = 0;
bool quit = false;

Renderer renderer;
Camera camera;
Resources resources;
ObjectHandler object_handler;

void main_loop()
{
	//update timestamp
	dt = static_cast<float>(SDL_GetTicks()) / 1000 - current_time;
	current_time = static_cast<float>(SDL_GetTicks()) / 1000;

	vector<Object*> background = object_handler.get_background();
	vector<Object*> objects = object_handler.get_objects();
	camera.update(background, objects, renderer.get_width(), renderer.get_height());
	renderer.render_frame(background, objects, camera.get_scale());
	object_handler.update();
}

int main()
{
	if(!renderer.init())
	{
		cout << "Error: Renderer unable to initialize. Exiting..\n";
		return 1;
	}

	resources.set_renderer(renderer.get_renderer());
	object_handler.init_members(&resources, &camera);

	emscripten_set_main_loop(main_loop, 0, 1);

	return 0;
}