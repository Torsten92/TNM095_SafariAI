# TNM095_WildlifeSimulator

This is a project developed for the course TNM095 at Linköping University.

## About
It is a wildlife simulator in which animals try to survive in an environment. Some animals survive by preying on other animals, while others prefer eating the grass that grows on the ground.

The project is developed in C++, using SDL2 (https://www.libsdl.org/) for graphics and input.
Emscripten (http://kripken.github.io/emscripten-site/) was used as an alternative to compile the application into javascript, making it accessible from the web.
The animal images have been taken from http://pngimg.com/.

The method used for animal reasoning is Case Based Reasning (CBR) which means that every animal uses its personal case-base to decide how to react to situations. Each animal will also learn from its experiences by potentially adding an experience as a new case into its case-base.


## Animal reasoning:
Every animal act as an agent that is able to scan its surroundings for other animals and objects. This scan may decide the state of the animal. For example, an animal may run away if it sees another animal running towards it, since this most likely means that animal wants to eat it. 

Animals do not have any inherent knowledge of other animals more that what they can see. This means that a deer with no previous knowledge may see a cow as a bigger threat than a wolf, simply because a cow is bigger. They will learn to fear the wolf more by experience, though.