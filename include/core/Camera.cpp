#include "Camera.h"

Camera::Camera()
{
	x_pos = 400;
	y_pos = 300;
}

void Camera::update(const vector<Object*>& background, const vector<Object*>& v, float screen_w, float screen_h)
{
	handle_input();
	set_view(v, screen_w, screen_h);
	set_background_view(background, screen_w, screen_h);
}

void Camera::set_view(const vector<Object*>& v, float screen_w, float screen_h)
{
	reset_bounds(); //reset camera bounds to recalculate values
	for(auto& o : v)
	{
		//Middle of screen at (400, 300)
		o->set_x_camera( o->get_x() - x_pos + 400/scale );
		o->set_y_camera( o->get_y() - y_pos + 300/scale );

		x_min = (o->get_x() - 200/scale < x_min) ? o->get_x() - 200/scale : x_min;
		x_max = (o->get_x() - 200/scale > x_max) ? o->get_x() + 200/scale : x_max;
		y_min = (o->get_y() - 150/scale < y_min) ? o->get_y() - 150/scale : y_min;
		y_max = (o->get_y() + 150/scale > y_max) ? o->get_y() + 150/scale : y_max;
	}
	x_pos = (x_pos < x_min) ? x_min : x_pos;
	x_pos = (x_pos > x_max) ? x_max : x_pos;
	y_pos = (y_pos < y_min) ? y_min : y_pos;
	y_pos = (y_pos > y_max) ? y_max : y_pos;
}

//Like set_view(), but moves the tiles so that it looks like map is infinite. For example, if user moves
//camera left the rightmost tiles of the background will be moved to the left of the leftmost ones.
void Camera::set_background_view(const vector<Object*>& v, float screen_w, float screen_h)
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

void Camera::handle_input()
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

	SDL_Event e;
	//User requests quit
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
					if(mouse_down)
					{
						set_x(x_pos + (mouse_last_x - x)/scale);
						set_y(y_pos + (mouse_last_y - y)/scale);
						//x_pos += (mouse_last_x - x)/scale;
						//y_pos += (mouse_last_y - y)/scale;
					}
				break;

				case SDL_MOUSEBUTTONDOWN:
					mouse_down = true;
				break;

				case SDL_MOUSEBUTTONUP:
					mouse_down = false;
				break;

				case SDL_MOUSEWHEEL:
					//Scale scene (positive or negative)
					if( ( scale < 2.0 && e.wheel.y > 0) || (scale > 0.2 && e.wheel.y < 0) )
						scale += e.wheel.y > 0 ? 0.1 : -0.1;
					scale = min(max(scale, 0.2f), 2.0f); //some web-browers use too big scroll step.
				break;
			}
			mouse_last_x = x;
			mouse_last_y = y;
		}
	}
}