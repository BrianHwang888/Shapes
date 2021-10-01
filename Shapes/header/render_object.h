/*--------------- BASE CLASS FOR ALL OBJECTS TO BE RENDERED ---------------*/
//Contains all abstract classes

#ifndef __RENDER_OBJECT_H__
#define __RENDER_OBJECT_H__
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include"shader.h"

enum class dimensions {
	SHAPE_3D,
	SHAPE_2D
};

/*-----Header for color attribute-----*/
class color_attribute {
public:
	color_attribute(glm::vec4 color, int vertices);
	glm::vec4 color;
	//pointer to buffer hold color data of vertices; buffer initalized in has_color constructor
	glm::vec4* color_buffer; 
};

/*-----Header for normal attribute-----*/
class normal_attribute {
public:
	normal_attribute(int vertices);
	glm::vec3* normal_buffer; //pointer to buffer holding normal data of vertices; initialized in has_normal constructor
};

/*-----Header for abstract class render_object-----*/
class render_object {
public:
	glm::vec3* get_position_buffer(); //returns position buffer of object
	
	//assigns shader program to render_object
	void set_shader_program(shader_program* program);
	
	/*generates data for vertices buffer; data at least has posistion of vertices; 
	generates color, and normal if applicable to object*/
	virtual void gen_vertices_buffer(); 
	
	//generates normal data in normal buffer ONLY IF OBJECT HAS NORMAL DATA
	virtual void gen_normal_buffer(); 
	virtual void draw(); //draw function to draw object during rendering sequence

	/*----- Getter functions -----*/
	glm::vec3 get_position(); //returns position of object
	glm::vec4 get_color(); //returns color of object
	glm::mat4 get_model_mat(); //returns model matrix of object
	
	/*returns net translations of object AKA the object's 
	displacement from it's spawned position*/
	glm::vec3 get_translation(); 
	glm::vec3 get_scale(); //returns net scaling of object

	/*returns the angle of net rotations of object in degrees; 
	each vector component corrisponds to the angle of rotation for each respective axis; 
	x:roll, y:yaw, z:pitch*/
	glm::vec3 get_rotation(); 
	GLuint get_shader_programID(); //returns assigned ID of shader program for this object
	shader_program get_shader_program(); //returns assigned shader_program object for this object

	void set_model_mat(glm::mat4 model_mat); //sets model matrix of this object
	
	
protected:
	unsigned long long total_vertices; //total vertices of this object
	glm::vec3 position; //position of this object
	shader_program* program; //shader_program assigned to this object
	GLuint VBO; //vertex buffer object
	GLuint VAO; //vertex attribute object
	glm::vec3* position_buffer; //pointer to positional data of vertices
	color_attribute* color; //pointer to color_delegate; determines if object has_color or non_color
	normal_attribute* normal; //pointer to normal_delegate; determines if object has_normal or non_normal
	glm::mat4 model; //model matrix 
	glm::vec3 translation; //net translations of object; objects displacement from spawning location
	glm::vec3 scale; //net scaling of object
	//angle of net rotations of object in degrees; each vector component corrisponds to the angle of rotation for each respective axis; x:roll, y:yaw, z:pitch
	glm::vec3 rotation; 

	render_object(); //default constructor
	//render_object constructor; requires total vertices, color_delegate, normal_delegate, and spawing position
	render_object(int vertices, color_attribute* color, normal_attribute* normal, glm::vec3 pos); 

	virtual void gen_position_buffer() = 0; //generates positional data of vertices; pure virtual since all objects have unique sturcture
};

/*----- Header for class line -----*/
class line : public render_object {
protected:
	float length; //holds length of line

public:
	line(); //default constructor
	line(float len, int orent, glm::vec4 color, glm::vec3 pos); //line constructor; requires length, orentation, color, and spawing position of line
	void gen_vertices_buffer() override; //overriding generating vertex data
	void draw() override; //override draw operation

private:
	void gen_position_buffer() override; //override positional data generation for vertices
};

/*----- Header for class grid -----*/
class grid {
	public:
		grid(); //default constructor

		//grid constructor; requires spawing position, array of colors, length (x-axis), depth (z-axis), height (y-axis)
		grid(glm::vec3 position, glm::vec4* colors, float len, float dep, float hei); 
		void gen_vertices_buffer(); //generate vertex data
		void set_shader_program(shader_program* program); //set shader_program
		void draw(); //draw operation

	private:
		line* x_axis = NULL; //line object represent x-axis
		line* y_axis = NULL; //line object represent y-axis
		line* z_axis = NULL; //line object represent z-axis

		float length; //measurement of x-axis
		float depth; //measurement of y-axis
		float height; //measurement of z-axis

};

/*----- Header for abstract class shape -----*/
class shapes : public render_object {

protected:
	float height; //measurement of height
	float base; //measurement of lenght

	//default constructor
	shapes(); 

	//shapes constructor; requires number of vertices, color delegate, normal delegate, array of measurements [2], spawing position
	shapes(int vertices, color_attribute* color, normal_attribute* normal, float* measurements, glm::vec3 position); 

private:
	//override generating position data 
	void gen_position_buffer() override;
};

/*----- Header for abstract class shape_2D -----*/
class shape_2D : public render_object {
protected:
	int number_sides;
	float total_internal_angles;
	float base;
	float height;
	float area;
	float perimeter;
	 dimensions type = dimensions::SHAPE_2D;
	shape_2D(int vertices, color_attribute* color, normal_attribute* normal, float* measurements, glm::vec3 position);
	shape_2D();
};

/*----- Header for abstract class shape_3D -----*/
class shape_3D : public render_object {
protected:
	int number_edges;
	int number_faces;
	float length;
	float height;
	float depth;
	float surface_area;
	float volume;
	dimensions type = dimensions::SHAPE_3D;
	shape_3D(int vertices, color_attribute* color, normal_attribute* normal, float* measurements, glm::vec3 position);
	shape_3D();

};

/*----- Header for triangle class -----*/
class triangle : public shapes {
public:
	triangle(); //default constuctor

	/*creates a right triangle by default
	requires spawning position, a color, if/if not have normals, array of measurements [2]*/
	triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements); 

private:
	//overriding generating position data
	void gen_position_buffer() override;
};

class sphere : public shape_3D {
public:
	sphere();
	sphere(glm::vec3 position, glm::vec4 color, float radius, int num_seg, int num_slice);

private:
	float radius;
	float diameter;
	float segment_angle;
	float slice_angle;
	int number_segments;
	int number_slices;

	void gen_position_buffer() override;

};
#endif