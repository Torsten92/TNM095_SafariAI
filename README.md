# TNM095_SafariAI

This is a project developed for the course TNM095 at Linköping University.

It is a wildlife simulator in which animals try to survive in different environments. Some animals survive by preying on other animals, while others prefer eating the grass that grows on the ground.

The project is developed in C++, using SDL2 (https://www.libsdl.org/) for graphics and input.
Emscripten (http://kripken.github.io/emscripten-site/) was used as an alternative to compile the application into javascript, making it accessible from the web.
The animal images have been taken directly from http://pngimg.com/.


About:
Every animal act as an agent that is able to scan its surroundings for other animals and objects. This scan may decide the state of the animal. For example, an animal may run away if it sees another animal running towards it, since this most likely means that animal wants to eat it.