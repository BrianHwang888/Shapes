/*----- SCENE DEFINITION -----*/
#include"../header/scene.h"

scene::scene(render_object* object, int num_obj, light* light_source, int num_lights) {
	objects = object;
	lights = light_source;
};