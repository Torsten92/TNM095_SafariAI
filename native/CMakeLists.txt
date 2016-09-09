cmake_minimum_required (VERSION 2.6)
project (SDL_test)

add_definitions (-std=c++11 -O2 -Wall)

include_directories(
	"../include" 
)

#Add project files here below main.cpp
add_executable(SDL_test 
	../include/main_native.cpp
	../include/core/Resources.cpp
	../include/core/Renderer.cpp
	../include/core/Camera.cpp
	../include/core/ObjectHandler.cpp
	../include/core/Texture.cpp
	../include/world/Object.cpp
	../include/world/Grass.cpp
	../include/world/Animal.cpp
	../include/world/Herbivore.cpp
	../include/world/Carnivore.cpp
)

target_link_libraries (SDL_test GL GLU SDL2 SDL2_image)