/*----- SCENE DEFINITION -----*/
#include"../header/scene.h"

scene::scene(render_object* object, int num_obj, light* light_source, int num_lights) {
	objects = object;
	lights = light_source;
};

//Methods to be implemented, not sure if needed
void scene::gen_buffers() {}; //header for gen_buffer
void scene::draw_objects() {};//header for draw_object