#include "Renderer.h"

int Renderer::SCREEN_WIDTH = 800;
int Renderer::SCREEN_HEIGHT = 600;
SDL_Window* Renderer::window;
SDL_Renderer* Renderer::renderer;

Renderer::Renderer()
{}

bool Renderer::init()
{
	//Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
	//Create window
	window = SDL_CreateWindow( "Adventure Tales", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
						SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	
	if( window == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
	
	//Create renderer for window
	renderer =	SDL_CreateRenderer( window, -1,
						SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	if( renderer == NULL )
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor( renderer, 0x80, 0x80, 0x80, 0xFF );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return false;
	}

    return true;
}


Renderer::~Renderer()
{
	close();
}

void Renderer::close()
{
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Renderer* Renderer::get_renderer()
{
	return renderer;
}

int Renderer::get_width()
{
	return SCREEN_WIDTH;
}

int Renderer::get_height()
{
	return SCREEN_HEIGHT;
}

void Renderer::render_frame(vector<Object*>& background, vector<Object*>& v, float scale)
{
	SCREEN_WIDTH = SDL_GetWindowSurface(window)->w;
	SCREEN_HEIGHT = SDL_GetWindowSurface(window)->h;
	
	//Clear screen
	SDL_RenderClear( renderer );
	
	//Render textures to screen (first background, then objects)
	for(auto o : background)
		o->render(scale*SCREEN_WIDTH/800, scale*SCREEN_HEIGHT/600);
	for(auto o : v)
		o->render(scale*SCREEN_WIDTH/800, scale*SCREEN_HEIGHT/600);

	//Update screen
	SDL_RenderPresent(renderer);
}

void Renderer::clear_frame()
{
	SDL_RenderClear(renderer);
}

void Renderer::update_screen()
{
	SDL_RenderPresent(renderer);
}

void Renderer::render_object(Object* o)
{
	o->render();
}