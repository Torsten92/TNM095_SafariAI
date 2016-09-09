#pragma once

#include "../core/Globals.h"
#include "../core/Texture.h"

class Object
{
public:
	virtual ~Object() = default;
	Object( Texture* _tex, string _name, float x_pos = 0, float y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );
	
	void render(float scaleX = 1.0, float scaleY = 1.0);
	
	float get_x();
	float get_y();
	float get_x_camera();
	float get_y_camera();

	int get_colrad();
	string get_name();
	
	void set_x(float val);
	void set_y(float val);
	void set_x_camera(float val);
	void set_y_camera(float val);

	void set_screen_dim(int w, int h);
	
	void change_clip(SDL_Rect c);
	void change_texture(Texture* _tex);

	bool operator<(const Object& other) { return ( depth != other.depth ? depth > other.depth : 
											(y_pos != other.y_pos ? y_pos > other.y_pos : x_pos > other.x_pos) ); }

private:
	const string name;
	Texture* texture;
	
	//Screen positions in world coordinates and camera coordinates
	float x_pos, y_pos;
	float x_pos_camera, y_pos_camera;

	int depth;

	int collision_radius;
	
	SDL_Rect clip;
};