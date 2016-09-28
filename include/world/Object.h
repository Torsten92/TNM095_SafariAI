#ifndef OBJECT_
#define OBJECT_


#include "../core/Globals.h"
#include "../core/RandomGenerator.h"
#include "../core/Texture.h"

class Object
{
public:
	virtual ~Object() = default;
	Object( Texture* _tex, int _type, float x_pos = 0, float y_pos = 0, int _depth = 0, SDL_Rect _clip = { 0, 0, 0, 0 } );
	
	virtual void render(float scaleX = 1.0, float scaleY = 1.0); //may be overridden
	
	float get_x();
	float get_y();
	float get_x_camera();
	float get_y_camera();

	int get_colrad();
	int get_type();
	unsigned long int get_id();
	
	void set_x(float val);
	void set_y(float val);
	void set_x_camera(float val);
	void set_y_camera(float val);

	void set_screen_dim(int w, int h);
	
	void change_clip(SDL_Rect c);
	void change_texture(Texture* _tex);

	bool operator<(const Object& other) { return ( depth != other.depth ? depth < other.depth : 
											(y_pos != other.y_pos ? y_pos < other.y_pos : x_pos < other.x_pos) ); }
	bool operator==(const Object& other) { return id == other.id; }
protected:
	unsigned long int id; // a unique id for this specific object
	const int type;
	Texture* texture;
	
	//Screen positions in world coordinates and camera coordinates
	float x_pos, y_pos;
	float x_pos_camera, y_pos_camera;

	int depth;

	int collision_radius;
	
	SDL_Rect clip;
};

#endif