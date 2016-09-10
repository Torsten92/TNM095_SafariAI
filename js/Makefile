include_dir_1= ../include
CFLAGS=-std=c++11 -O2 -Wall -I $(include_dir_1)
EMSCRIPTENFLAGS=-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file ../assets -o index.html
SOURCES=../include/main_js.cpp ../include/core/Renderer.cpp ../include/core/Resources.cpp ../include/core/Camera.cpp ../include/core/ObjectHandler.cpp ../include/core/Texture.cpp ../include/world/Object.cpp ../include/world/Grass.cpp ../include/world/Animal.cpp ../include/world/Herbivore.cpp ../include/world/Carnivore.cpp ../include/core/Utilities.cpp

all: js

js:
	em++ $(CFLAGS) $(SOURCES) $(EMSCRIPTENFLAGS)

clean:
	
