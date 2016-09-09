#include "Texture.h"


Texture::Texture( SDL_Renderer* _renderer )
{
	//Initialize
	texture = NULL;
	renderer = _renderer;
	
	width = 0;
	height = 0;
}

Texture::Texture( SDL_Renderer* _renderer, std::string path )
{
	//Initialize
	texture = NULL;
	renderer = _renderer;
	width = 0;
	height = 0;
	loadFromFile( path );
}

bool Texture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	if(texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		
		if( texture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return texture != NULL;
}

void Texture::render( int x, int y, float scaleX, float scaleY )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, 
			static_cast<int>( ceil(width * scaleX) ), 
			static_cast<int>( ceil(height * scaleY) ) 
	};
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_RenderCopyEx( renderer, texture, NULL, &renderQuad, 0, NULL, flipType );
}

void Texture::render( int x, int y, SDL_Rect* clip, float scaleX, float scaleY )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, 
			static_cast<int>( ceil(clip->w * scaleX) ), 
			static_cast<int>( ceil(clip->h * scaleY) ) 
	};
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, 0, NULL, flipType );
}

void Texture::set_screen_dim(int w, int h)
{
	screen_width = w;
	screen_height = h;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}