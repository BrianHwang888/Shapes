#include"../header/render_object.h"

void non_color::gen_color_buffer(int vertices) {};
void has_color::gen_color_buffer(int vertices) {
	color_buffer = new glm::vec4[vertices];
};
void non_normal::gen_normal_buffer(int vertices) {};
void has_normal::gen_normal_buffer(int vertices) {
	normal_buffer = new glm::vec3[vertices];
};