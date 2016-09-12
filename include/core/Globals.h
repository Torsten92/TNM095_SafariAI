#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __unix__
	#include <stdio.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <random>
#elif defined(_WIN32) || defined(WIN32)
	#include <windows.h>
	#include <stdio.h>
	#include <string>
	#include <tchar.h>
#endif

using namespace std;

//Exit game when quit is true
extern bool quit;

//Stores the current runtime, as well as delta time between each frame in seconds.
//Initialized by main and then handled by update function.
extern float current_time;
extern float dt;