/*--------------- BASE CLASS FOR ALL OBJECTS TO BE RENDERED ---------------*/
//Contains all abstract classes

#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__
#include<typeinfo>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader.h"

/*-----Header for color attribute-----*/
class color_delegate {
};
class has_color : public color_delegate {
protected:
	glm::vec4 color;
	glm::vec4* color_buffer;

public:
	has_color(glm::vec4 color, int vertices);
	glm::vec4* get_color_buffer();
};
class non_color : public color_delegate {
};

/*-----Header for normal attribute-----*/
class normal_delegate {
};
class has_normal : public normal_delegate {
public:
	glm::vec3* normal_buffer;

	has_normal(int vertices);
	glm::vec3* get_normal_buffer();
};
class non_normal : public normal_delegate {
};

/*-----Header for abstract class render_object-----*/
class render_object {
public:
	glm::vec3* get_position_buffer();
	void set_shader_program(shader_program* program);

	virtual void gen_vertices_buffer();
	virtual void gen_normal_buffer();
	virtual void draw();

protected:
	int total_vertices;
	shader_program* program;
	GLuint VBO;
	GLuint VAO;
	glm::vec3* position_buffer;
	color_delegate* color;
	normal_delegate* normal;

	render_object(int vertices, color_delegate* color, normal_delegate* normal);
};

/*----- Header for abstract class shape -----*/
class shapes :public render_object {
protected:
	float height;
	float base;
	shapes(int vertices, color_delegate* color, normal_delegate* normal, float* measurements);
};

/*----- Header for triangle class -----*/
class triangle :public shapes {
public:
	triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements); //creates a right triangle by default

private:
	void gen_position_buffer();

protected:
	glm::vec3 position;
};
#endif