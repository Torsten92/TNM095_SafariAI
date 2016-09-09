#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <random>

using namespace std;

//Exit game when quit is true
extern bool quit;

//Stores the current runtime, as well as delta time between each frame in seconds.
//Initialized by main and then handled by update function.
extern float current_time;
extern float dt;