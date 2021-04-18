/*----- LIGHT CLASS DEFINITION -----*/
#include"../header/light.h"

light::light() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambient_str = 1.0f;
	specular_str = 0.5f;
};

light::light(glm::vec3 position, glm::vec4 color, float ambient_val, float specular_val) {
	this->position = position;
	this->color = color;
	ambient_str = ambient_val;
	specular_str = specular_val;
}