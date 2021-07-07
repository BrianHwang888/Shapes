/*--------------- BASE CLASS FOR ALL OBJECTS TO BE RENDERED ---------------*/
//Contains all abstract classes

#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader.h"

/*-----Header for color attribute-----*/
class color_delegate {
public:
	glm::vec4* color_buffer;
};
class has_color : public color_delegate {
protected:
	glm::vec4 color;

public:
	has_color(glm::vec4 color, int vertices);


	glm::vec4 get_color();
};

class non_color : public color_delegate {
};

/*-----Header for normal attribute-----*/
class normal_delegate {
public:
	glm::vec3* normal_buffer;
};
class has_normal : public normal_delegate {
public:
	has_normal(int vertices);
};
class non_normal : public normal_delegate {
};

/*-----Header for abstract class render_object-----*/
class render_object {
public:
	glm::mat4 model;

	glm::vec3* get_position_buffer();
	void set_shader_program(shader_program* program);

	virtual void gen_vertices_buffer();
	virtual void gen_normal_buffer();
	virtual void draw();

	glm::vec3 get_position();
	
protected:
	int total_vertices;
	glm::vec3 position;
	shader_program* program;
	GLuint VBO;
	GLuint VAO;
	glm::vec3* position_buffer;
	color_delegate* color;
	normal_delegate* normal; 

	render_object();
	render_object(int vertices, color_delegate* color, normal_delegate* normal, glm::vec3 pos);
	virtual void gen_position_buffer() = 0;
};

/*----- Header for class line -----*/
class line : public render_object {
protected:
	float length;

public:
	line();
	line(float len, int orent, glm::vec4 color, glm::vec3 pos);
	void gen_vertices_buffer() override;
	void draw() override;

private:
	void gen_position_buffer() override;
};

/*----- Header for class grid -----*/
class grid {
	public:
		grid();
		grid(glm::vec3 position, glm::vec4* colors, float len, float dep, float hei);
		void gen_vertices_buffer();
		void set_shader_program(shader_program* program);
		void draw();

	private:
		line* x_axis = NULL;
		line* y_axis = NULL;
		line* z_axis = NULL;

		float length;
		float depth;
		float height;

};

/*----- Header for abstract class shape -----*/
class shapes : public render_object {
protected:
	float height;
	float base;

	shapes();
	shapes(int vertices, color_delegate* color, normal_delegate* normal, float* measurements, glm::vec3 position);

private:
	void gen_position_buffer() override;
};

/*----- Header for triangle class -----*/
class triangle :public shapes {
public:
	triangle();
	triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements); //creates a right triangle by default

private:
	void gen_position_buffer() override;
};
#endif