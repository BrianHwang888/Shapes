#ifndef __SCENE_H__
#define __SCENE_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include"render_object.h"
#include"light.h"

class scene {
private:
	render_object* objects;
	light* lights;

protected:

public:
	scene(render_object* object, int num_obj, light* light_source, int num_lights);
	void gen_buffers();
	void draw_objects();

};

#endif // !__SCENE_H__
