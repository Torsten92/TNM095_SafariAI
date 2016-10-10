#include "Camera.h"

Camera::Camera()
{
	x_pos = 400;
	y_pos = 300;
}

void Camera::update(const vector<Object*>& background, const vector<Object*>& v, float screen_w, float screen_h)
{
	handle_input(screen_w, screen_h);
	set_view(v);
	set_background_view(background);

	//deselect dead objects to prevent pointer errors
	if(selected_object != nullptr && !selected_object->is_alive()) {
		selected_object = nullptr;
	}
}

void Camera::set_view(const vector<Object*>& v)
{
	reset_bounds(); //reset camera bounds to recalculate values
	for(auto& o : v)
	{
		//Middle of screen at (400, 300)
		o->set_x_camera( o->get_x() - x_pos + 400/scale );
		o->set_y_camera( o->get_y() - y_pos + 300/scale );

		x_min = (o->get_x() - 200/scale < x_min) ? o->get_x() - 200/scale : x_min;
		x_max = (o->get_x() + 200/scale > x_max) ? o->get_x() + 200/scale : x_max;
		y_min = (o->get_y() - 150/scale < y_min) ? o->get_y() - 150/scale : y_min;
		y_max = (o->get_y() + 150/scale > y_max) ? o->get_y() + 150/scale : y_max;
	}
	//Move camera inside bounds again if it was at limit and bounds shrunk
	x_pos = (x_pos < x_min) ? x_min : x_pos;
	x_pos = (x_pos > x_max) ? x_max : x_pos;
	y_pos = (y_pos < y_min) ? y_min : y_pos;
	y_pos = (y_pos > y_max) ? y_max : y_pos;
}

//Like set_view(), but moves the tiles so that it looks like map is infinite. For example, if user moves
//camera left the rightmost tiles of the background will be moved to the left of the leftmost ones.
void Camera::set_background_view(const vector<Object*>& v)
{
	for(auto& o : v)
	{
		if(o->get_x() > x_pos + 2400)
			o->set_x(o->get_x() - 4800);
		else if(o->get_x() < x_pos - 2400)
			o->set_x(o->get_x() + 4800);
		if(o->get_y() > y_pos + 1800)
			o->set_y(o->get_y() - 3600);
		else if(o->get_y() < y_pos - 1800)
			o->set_y(o->get_y() + 3600);

		//Middle of screen at (400, 300)
		o->set_x_camera( o->get_x() - x_pos + 400/scale );
		o->set_y_camera( o->get_y() - y_pos + 300/scale );
	}
}

void Camera::reset_bounds()
{
	int temp_x = x_min, temp_y = y_min;
	x_min = x_max;
	x_max = temp_x;
	y_min = y_max;
	y_max = temp_y;
}

int Camera::get_x_pos()
{
	return x_pos;
}

int Camera::get_y_pos()
{
	return y_pos;
}

float Camera::get_scale()
{
	return scale;
}

void Camera::set_x(float x)
{
	x_pos = min(max(x, x_min), x_max);
}

void Camera::set_y(float y)
{
	y_pos = min(max(y, y_min), y_max);
}

void Camera::set_object_list(vector<Object*>* ol)
{
	object_list = ol;
}

void Camera::deselect_object()
{
	if(selected_object != nullptr) {
		selected_object->set_selected(false);
		selected_object = nullptr;
	}
}

void Camera::handle_input(float screen_w, float screen_h)
{
	const Uint8 *keyState = SDL_GetKeyboardState(NULL);

	if( keyState[SDL_SCANCODE_LEFT] )
	{
		set_x(x_pos - dt*300/scale);
		//x_pos -= dt*300/scale;
	}
	if( keyState[SDL_SCANCODE_RIGHT] )
	{
		set_x(x_pos + dt*300/scale);
		//x_pos += dt*300/scale;
	}
	if( keyState[SDL_SCANCODE_DOWN] )
	{
		set_y(y_pos + dt*300/scale);
		//y_pos += dt*300/scale;
	}
	if( keyState[SDL_SCANCODE_UP] )
	{
		set_y(y_pos - dt*300/scale);
		//y_pos -= dt*300/scale;
	}

	while( SDL_PollEvent( &e ) != 0 )
	{
		if( e.type == SDL_QUIT )
			quit = true;
	
		//If mouse event happened
		if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEWHEEL)
		{
			//Get mouse position
			int x, y;
			SDL_GetMouseState( &x, &y );

			//Set mouse over sprite
			switch( e.type )
			{
				case SDL_MOUSEMOTION:
				{
					if(mouse_down)
					{
						set_x(x_pos + (mouse_last_x - x)/scale);
						set_y(y_pos + (mouse_last_y - y)/scale);
						//x_pos += (mouse_last_x - x)/scale;
						//y_pos += (mouse_last_y - y)/scale;
					}
				break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					mouse_down = true;
					mouse_down_x = x;
					mouse_down_y = y;
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					mouse_down = false;
					cout << x << ", " << y << " : " << scale << endl;
					bool selected = false;
					for(auto& o : (*object_list)) {
						if(Animal* a = dynamic_cast<Animal*>(o)) {
							if(sqrt( pow(x/scale - o->get_x_camera() * screen_w / 800, 2) + pow(y/scale - o->get_y_camera() * screen_h / 600, 2) ) < 32) {
								deselect_object();
								selected_object = a;
								selected_object->set_selected(true);
								selected = true;
								break;
							}
						}
					}
					
					if(!selected && sqrt( pow(x - mouse_down_x, 2) + pow(y - mouse_down_y, 2) ) < 10) {
						deselect_object();
					}

					break;
				}

				case SDL_MOUSEWHEEL:
				{
					//Scale scene (positive or negative)
					if( ( scale < 2.0 && e.wheel.y > 0) || (scale > 0.2 && e.wheel.y < 0) )
						scale += e.wheel.y > 0 ? 0.1 : -0.1;
					scale = min(max(scale, 0.2f), 2.0f); //some web-browers use too big scroll step.
					break;
				}
				default: break;
			}
			mouse_last_x = x;
			mouse_last_y = y;
		}
	}
}