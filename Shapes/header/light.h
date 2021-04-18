#ifndef __LIGHT_H__
#define __LIGHT_H__

#include"resource_manager.h"

class light {
protected:
	glm::vec4 color;
	glm::vec3 position;

	float ambient_str;
	float specular_str;

public:
	light();
	light(glm::vec3 position, glm::vec4 color, float ambient_val, float specular_val);
};


#endif // !__LIGHT_H__
