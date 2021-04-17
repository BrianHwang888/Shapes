/*--------------- BASE CLASS FOR ALL OBJECTS TO BE RENDERED ---------------*/
//Contains all abstract classes

#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__
#include<typeinfo>
#include"resource_manager.h"

/*-----Header for color attribute-----*/
class color_delegate {
protected:
	virtual void gen_color_buffer(int vertices) = 0;

};
class has_color : public color_delegate {
protected:
	glm::vec4 color;
	glm::vec4* color_buffer;

public:
	has_color(glm::vec4 color, int vertices);
	void gen_color_buffer(int vertices) override;
};
class non_color : public color_delegate {
public:
	void gen_color_buffer(int vertices) override;
};

/*-----Header for normal attribute-----*/
class normal_delegate {
protected:
	virtual void gen_normal_buffer(int vertices, glm::vec3* position_buffer) = 0;
};
class has_normal : public normal_delegate {
protected:
	glm::vec3* normal_buffer;

public:
	has_normal(int vertices);
	void gen_normal_buffer(int vertices, glm::vec3* position_buffer) override;
};
class non_normal : public normal_delegate {
public:
	void gen_normal_buffer(int vertices, glm::vec3* position_buffer) override;
};

/*-----Header for abstract class render_object-----*/
class render_object {
protected:
	int total_vertices;
	GLuint VBO;
	GLuint VAO;
	glm::vec3* position_buffer;
	color_delegate* color;
	normal_delegate* normal;

	render_object(int vertices, color_delegate* color, normal_delegate* normal);
	virtual void gen_vertices_buffer() = 0;
	
	virtual void draw() = 0;
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