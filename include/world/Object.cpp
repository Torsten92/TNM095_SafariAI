#include "Object.h"

Object::Object(Texture* _tex, int _type, float x_pos, float y_pos, int _depth, SDL_Rect _clip)
	: type{_type}, texture{_tex}, x_pos{x_pos}, y_pos{y_pos}, x_pos_camera{x_pos}, y_pos_camera{y_pos}, depth{_depth}
{
	RandomGenerator rg;
	id = RandomGenerator::unique();
	if( _clip.w == 0 && _clip.h == 0 )
	{
		_clip.w = texture->get_width();
		_clip.h = texture->get_height();
	}
	clip = _clip;
	
	//Currently uses simple radius based on texture size
	collision_radius = 1.5 * texture->get_width();
}

void Object::render(float scaleX, float scaleY)
{
	texture->render((x_pos_camera-texture->get_width()/2) * scaleX, (y_pos_camera-texture->get_height()/2) * scaleY, &clip, scaleX, scaleY);
}

float Object::get_x()
{
	return x_pos;
}

float Object::get_y()
{
	return y_pos;
}

float Object::get_x_camera()
{
	return x_pos_camera;
}

float Object::get_y_camera()
{
	return y_pos_camera;
}

int Object::get_type()
{
	return type;
}

unsigned long int Object::get_id()
{
	return id;
}

int Object::get_colrad()
{
	return collision_radius;
}

void Object::set_x(float val)
{
	x_pos = val;
}

void Object::set_y(float val)
{
	y_pos = val;
}

void Object::set_x_camera(float val)
{
	x_pos_camera = val;
}

void Object::set_y_camera(float val)
{
	y_pos_camera = val;
}

void Object::set_screen_dim(int w, int h)
{
	texture->set_screen_dim(w, h);
}

void Object::change_texture(Texture* _tex)
{
	texture = _tex;
}

void Object::change_clip(SDL_Rect c)
{
	clip = c;
}