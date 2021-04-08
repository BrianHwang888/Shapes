#ifndef __SHADER_H__
#define	__SHADER_H__

#include<stdio.h>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

class shader_program {
public:
	GLuint ID;
	
	shader_program shader_program(const char* vertex_path, const char* fragment_path);
	void use();
	void set_bool(const char* name, bool value);
	void set_uniform_int(const char* name, GLint& value);
	void set_uniform_float(const char* name, GLfloat& value);
	void set_uniform_double(const char* name, GLdouble &value);

	/*----- set uniform vectors -----*/
	void set_uniform_vec2(const char* name, const glm::vec2 &value);
	void set_uniform_vec3(const char* name, const glm::vec3 &value);
	void set_uniform_vec4(const char* name, const glm::vec4 &value);

	/*----- set uniform matrices ----*/
	void set_uniform_mat2(const char* name, const glm::mat2 &value);
	void set_uniform_mat3(const char* name, const glm::mat3 &value);
	void set_uniform_mat4(const char* name, const glm::mat4 &value);
};

#endif // ! __SHADER_H__

