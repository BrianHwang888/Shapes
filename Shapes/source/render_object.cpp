
#include"../header/render_object.h"

/*------ Definition of has_color constuctor ------*/
//reserve array of size determined by vertices
color_attribute::color_attribute(glm::vec4 color, int vertices) {
	this->color = color;
	color_buffer = new glm::vec4[vertices];

	for (int i = 0; i < vertices; i++) {
		color_buffer[i] = color;
	}
}

/*----- Definition of has_nnormal constructor -----*/
//reserve array of size determined by vertices
normal_attribute::normal_attribute(int vertices) {
	normal_buffer = new glm::vec3[vertices];
}

/*----- Definition of render_object default constructor -----*/
//sets all pointer attributes to NULL
//sets all int attributes to 0
//set position, rotation, translation components to 0
//set scale vector components to 1 
//sets all matrix attributes to identity matrix
//VAO and VBO is set to NULL
render_object::render_object() {
	total_vertices = 0;
	program = NULL;
	VAO = NULL;
	VBO = NULL;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	position_buffer = NULL;
	color = NULL;
	normal = NULL;
	model = glm::mat4(1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

};

/*----- Definition of constructor of render_object -----*/
//sets values corrisponding passed into parameters
//rotation and translation vector is set to 0 for all components
//scale vector is set to 1 for all components
//VAO, VBO, position_buffer, and program is set to NULL
render_object::render_object(int vertices, color_attribute* color, normal_attribute* normal, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) {
	position = pos;
	total_vertices = vertices;
	this->color = color;
	this->normal = normal;
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	VAO = NULL;
	VBO = NULL;
	position_buffer = NULL;
	program = NULL;
}

/*----- Definition of get_position_buffer -----*/
glm::vec3* render_object::get_position_buffer() {
	return position_buffer;
};
/*----- Definition of get_position_buffer -----*/
glm::vec3 render_object::get_position() { return position; }

/*----- Definition of get_color -----*/
glm::vec4 render_object::get_color() { 
	if (color->color_buffer == NULL)
		return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	return color->color_buffer[0]; 
}

/*----- Definition of get_model_mat -----*/
glm::mat4 render_object::get_model_mat() {
	return model;
}

/*----- Definition of get_translation -----*/
glm::vec3 render_object::get_translation() {
	return translation;
}

/*----- Definition of get_scale -----*/
glm::vec3 render_object::get_scale() {
	return scale;
}

/*----- Definition of get_rotation -----*/
glm::vec3 render_object::get_rotation() {
	return rotation;
}

/*----- Definition of set_model_mat -----*/
void render_object::set_model_mat(glm::mat4 model_mat) {
	model = model_mat;
}

/*----- Definition of set_shader_program -----*/
void render_object::set_shader_program(shader_program* program) {
	this->program = program;
};

/*----- Definition of gen_vertices_buffer -----*/
//Default generation of vertex data; creates tightly packed array holding vertex data
//General order of data is: position, color, normal
void render_object::gen_vertices_buffer() {
	unsigned long long vertex_data_size;
	GLint vertex_position, vertex_normal, vertex_color;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	vertex_position = glGetAttribLocation(program->ID, "vPosition"); //location of position data to be sent in shader (named: vPosition)
	vertex_normal = glGetAttribLocation(program->ID, "vNormal"); //location of normal data to be sent in shader (named: vNormal)
	vertex_color = glGetAttribLocation(program->ID, "vColor"); //location of color data to be sent in shader (named:vColor)

	vertex_data_size = sizeof(glm::vec3);

	//Generating data for objects with color or normals
	if (color != nullptr && normal != nullptr) {
		vertex_data_size += sizeof(glm::vec4) + sizeof(glm::vec3);
		glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total_vertices * sizeof(glm::vec3), position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), total_vertices * sizeof(glm::vec4), color->color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, total_vertices * (sizeof(glm::vec3) + sizeof(glm::vec4)), total_vertices * sizeof(glm::vec3), normal->normal_buffer);

		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribPointer(vertex_color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(total_vertices * sizeof(glm::vec3)));
		glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(total_vertices * (sizeof(glm::vec3) + sizeof(glm::vec4))));

		glEnableVertexAttribArray(vertex_position);
		glEnableVertexAttribArray(vertex_color);
		glEnableVertexAttribArray(vertex_normal);
		 
	}

	//Generating data for objects without normals
	else if (color != nullptr) {
		vertex_data_size += sizeof(glm::vec4);
		glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total_vertices * sizeof(glm::vec3), position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), total_vertices * sizeof(glm::vec4), color->color_buffer);

		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribPointer(vertex_color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(total_vertices * sizeof(glm::vec3)));

		glEnableVertexAttribArray(vertex_position);
		glEnableVertexAttribArray(vertex_color); 
		
	}

	//Generating data for objects without colors
	else if (normal != nullptr) {
		vertex_data_size += sizeof(glm::vec4);
		glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total_vertices * sizeof(glm::vec3), position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), total_vertices * sizeof(glm::vec3), normal->normal_buffer);
	

		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribPointer(vertex_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(total_vertices * sizeof(glm::vec3)));

		glEnableVertexAttribArray(vertex_position);
		glEnableVertexAttribArray(vertex_normal);
	}

	//Generating data for objects without colors or normals
	else {
		glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, position_buffer, GL_STATIC_DRAW);
		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(vertex_position);
	}
};

/*----- Definition for gen_normal_buffer -----*/
//Default generation of normal data; creates tightly packed normal data
void render_object::gen_normal_buffer() {
	glm::vec3 u, v, norm;

	//Generates normal data; cross product between i element and i + 1 and i + 2
	for (int i = 0; i < total_vertices; i += 3) {
		u = position_buffer[i + 2] - position_buffer[i];
		v = position_buffer[i + 1] - position_buffer[i];
		norm = glm::normalize(glm::cross(u, v));

		normal->normal_buffer[i] = norm;
		normal->normal_buffer[i + 1] = norm;
		normal->normal_buffer[i + 2] = norm;
	}
};

/*----- Definition for draw -----*/
//Default drawing of objects; draws using GL_TRIANGLES and all vertices
void render_object::draw() {
	glUseProgram(program->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, total_vertices);
};

/*----- Definition for get_shader_programID -----*/
GLuint render_object::get_shader_programID() { return program->ID; }

/*----- Definition for get_shader_program -----*/
shader_program render_object::get_shader_program() { return *program; }

/*----- Definition for default line constuctor -----*/
line::line() {
	length = 1.0f;
};

/*----- Definition for line constructor -----*/
line::line(float len, int orent, glm::vec4 color, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) : render_object(2, new color_attribute(color, 2), NULL, pos){
	glm::vec3 end_point;

	length = len;
	gen_position_buffer();
	
	end_point = position;

	if (!(orent ^ 0))
		end_point.x += length;

	else if (!(orent ^ 1))
		end_point.y += length;

	else if (!(orent ^ 2))
		end_point.z -= length;

	position_buffer[0] = position;
	position_buffer[1] = end_point;
};

/*----- Definition for gen_position_buffer (line)-----*/
void line::gen_position_buffer() {
	position_buffer = new glm::vec3[total_vertices];

};

/*----- Definition for gen_vertices_buffer (line)-----*/
void line::gen_vertices_buffer() {
	GLint vertex_position, vertex_color;
	unsigned int vertex_data_size = sizeof(glm::vec3) + sizeof(glm::vec4);
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	vertex_position = glGetAttribLocation(program->ID, "vPosition");
	vertex_color = glGetAttribLocation(program->ID, "vColor");

	glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_vertices * sizeof(glm::vec3), position_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), total_vertices * sizeof(glm::vec4), color->color_buffer);

	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribPointer(vertex_color, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(total_vertices * sizeof(glm::vec3)));

	glEnableVertexAttribArray(vertex_position);
	glEnableVertexAttribArray(vertex_color);
}

/*----- Definition for draw (line) -----*/
void line::draw() {
	glUseProgram(program->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE, 0, total_vertices);
}

/*----- Definition for default constructor of grid -----*/
grid::grid() {
	length = 0;
	depth = 0;
	height = 0;
};

/*----- Definition for constructor for grid -----*/
grid::grid(glm::vec3 position, glm::vec4* colors, float len, float dep, float hei){
	x_axis = new line(len, 0, colors[0]);
	y_axis = new line(hei, 1, colors[1]);
	z_axis = new line(dep, 2, colors[2]);

	length = len;
	depth = dep;
	height = hei;

	
};

/*----- Definition for gen_vertices_buffer (grid) -----*/
void grid::gen_vertices_buffer() {
	x_axis->gen_vertices_buffer();
	y_axis->gen_vertices_buffer();
	z_axis->gen_vertices_buffer();
}

/*----- Definition for set_shader_program (grid) -----*/
void grid::set_shader_program(shader_program* program) {
	x_axis->set_shader_program(program);
	y_axis->set_shader_program(program);
	z_axis->set_shader_program(program);
}

/*----- Definition for draw (grid) -----*/
void grid::draw() {
	shader_program axis_program;
	
	//draws gird components in following order: x_axix, y_axis, z_axis
	axis_program = x_axis->get_shader_program();
	axis_program.set_mat4("model", x_axis->get_model_mat());
	x_axis->draw();

	axis_program = y_axis->get_shader_program();
	axis_program.set_mat4("model", y_axis->get_model_mat());
	y_axis->draw();

	axis_program = z_axis->get_shader_program();
	axis_program.set_mat4("model", z_axis->get_model_mat());
	z_axis->draw();
}

/*----- Definitions for shape_2D -----*/
shape_2D::shape_2D() {
	number_sides = 0;
	total_internal_angles = 0;
	height = 0;
	base = 0;
	area = 0;
	perimeter = 0;
}
shape_2D::shape_2D(int vertices, color_attribute* color, normal_attribute* normal, float* measurements, glm::vec3 position) : render_object(vertices, color, normal, position) {
	height = measurements[0];
	base = measurements[1];
	number_sides = 0;
	total_internal_angles = 0;
	area = 0;
	perimeter = 0;

}

/*----- Definition for shape_3D ------*/
shape_3D::shape_3D() : render_object(){
	number_edges = 0;
	number_faces = 0;
	length = 0;
	depth = 0;
	height = 0;
	surface_area = 0;
	volume = 0;
}

shape_3D::shape_3D(int vertices, color_attribute* color, normal_attribute* normal, float* measurements, glm::vec3 position) :render_object(vertices, color, normal, position) {
	if (measurements != NULL) {
		length = measurements[0];
		height = measurements[1];
		depth = measurements[2];
	}
	else {
		length = 0;
		height = 0;
		depth = 0;
	}
	number_edges = 0;
	number_faces = 0;
	surface_area = 0;
	volume = 0;
}


/*----- Definition for default constructor for triangle -----*/
triangle::triangle() : shape_2D(){
	
};

/*----- Definition for constructor for triangle -----*/
triangle::triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements) : shape_2D(3, new color_attribute(color, 3), new normal_attribute(3), measurements, position) {
	gen_position_buffer();
	if (has_normals)
		gen_normal_buffer();
};

/*----- Definition for gen_position_buffer (triangle) -----*/
void triangle::gen_position_buffer() {
	glm::vec3 vertex;
	position_buffer = new glm::vec3[total_vertices];

	//creates a right triangle
	vertex.y = position.y - height / 2;
	vertex.x  = position.x - base / 2;
	vertex.z = position.z;
	position_buffer[0] = vertex;

	vertex.y += height;
	position_buffer[1] = vertex;


	vertex.y -= height;
	vertex.x += base;
	position_buffer[2] = vertex;
};

sphere::sphere() : shape_3D() {
	radius = 0.0f;
	diameter = 0.0f;
	number_segments = 0;
	number_slices = 0;
	segment_angle = 0.0f;
	slice_angle = 0.0f;
};

sphere::sphere(glm::vec3 position, glm::vec4 color, float radius, int num_seg, int num_slice) : shape_3D(0, NULL, NULL, NULL, position) {
	if(num_seg == 2)
		total_vertices = num_seg * num_slice * 3;
	else {
		total_vertices = 2 * 3 * num_slice;
		total_vertices += (num_seg - 2) * num_slice * 6;
	}
	this->position = position;
	this->color = new color_attribute(color, total_vertices);
	this->radius = radius;
	diameter = radius * 2;
	length = height = depth = diameter;

	number_segments = num_seg;
	number_slices = num_slice;
	segment_angle = 360.0f / number_segments;
	slice_angle = 180.0f / number_slices;
};

void sphere::gen_position_buffer() {};