#include "core/Globals.h"
#include "core/Renderer.h"
#include "core/Resources.h"
#include "core/Camera.h"
#include "core/ObjectHandler.h"

//Global variables (see Globals.h).
float current_time = 0;
float dt = 0;
bool quit = false;

/**
* Loads the image located at 'fileName' and copies it to the
* renderer 'renderer'
*/
int wmain(int argc, char** argv)
{
	Renderer renderer;
	Camera camera;

	if(!renderer.init())
	{
		cout << "Error: Renderer unable to initialize. Exiting..\n";
		return 1;
	}

	Resources resources(renderer.get_renderer());
	ObjectHandler object_handler(&resources, &camera);

	while( !quit )
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

	return 0;
}