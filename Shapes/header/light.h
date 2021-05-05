#ifndef __LIGHT_H__
#define __LIGHT_H__

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

class light {
public:
	glm::vec3 color;
	glm::vec3 position;

	float ambient_str;
	float specular_str;

	light();
	light(glm::vec3 position, glm::vec3 color, float ambient_val, float specular_val);

};


#endif // !__LIGHT_H__
