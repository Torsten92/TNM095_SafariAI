cmake_minimum_required (VERSION 2.6)
project (WildlifeSimulator)

add_definitions (-std=c++11 -g -Wall)

#Add project files here below main.cpp
add_executable(WildlifeSimulator 
	../include/main_native.cpp
	../include/core/Resources.cpp
	../include/core/Renderer.cpp
	../include/core/Camera.cpp
	../include/core/ObjectHandler.cpp
	../include/core/Utilities.cpp
	../include/core/RandomGenerator.cpp
	../include/core/Texture.cpp
	../include/world/Object.cpp
	../include/world/Grass.cpp
	../include/world/Animal.cpp
	../include/world/Herbivore.cpp
	../include/world/Carnivore.cpp
	../include/CBR/CBR.cpp
)

target_link_libraries (WildlifeSimulator GL GLU SDL2 SDL2_image)