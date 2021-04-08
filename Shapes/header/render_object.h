/*--------------- BASE CLASS FOR ALL OBJECTS TO BE RENDERED ---------------*/

#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include"resource_manager.h"

/*-----Header for color attribute-----*/
class color_delegate {
public:
	virtual void gen_color_buffer(int vertices) = 0;

};
class has_color : public color_delegate {
public:
	glm::vec4* color_buffer;
	void gen_color_buffer(int vertices) override;

};
class non_color : public color_delegate {
public:
	void gen_color_buffer(int vertices) override;
};

/*-----Header for normal attribute-----*/
class normal_delegate {
public:
	virtual void gen_normal_buffer(int vertices) = 0;
};
class has_normal : public normal_delegate {
public:
	glm::vec3* normal_buffer;
	void gen_normal_buffer(int vertices) override;
};
class non_normal : public normal_delegate {
public:
	void gen_normal_buffer(int vertices) override;
};

/*-----Header for render_object-----*/
class render_object {
public:
	glm::vec3* position_buffer;
	color_delegate* colors;
	normal_delegate* normals;
	render_object();
	render_object(color_delegate* color);
	render_object(normal_delegate* normal);
	render_object(color_delegate* color, normal_delegate* normal);
	virtual void gen_vertices_buffer() = 0;
	virtual void draw() = 0;
};




#endif