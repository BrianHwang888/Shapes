#include"../header/render_object.h"

has_color::has_color(glm::vec4 color, int vertices) {
	this->color = color;
	color_buffer = new glm::vec4[vertices];

	for (int i = 0; i < vertices; i++) {
		color_buffer[i] = color;
	}
}
glm::vec4 has_color::get_color() { return color; };

has_normal::has_normal(int vertices) {
	normal_buffer = new glm::vec3[vertices];
}

render_object::render_object() {
	total_vertices = 0;
	program = NULL;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	position_buffer = NULL;
	color = NULL;
	normal = NULL;
	model = glm::mat4(1.0f);
};
render_object::render_object(int vertices, color_delegate* color, normal_delegate* normal, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) {
	position = pos;
	total_vertices = vertices;
	this->color = color;
	this->normal = normal;
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}
glm::vec3* render_object::get_position_buffer() {
	return position_buffer;
};
glm::vec3 render_object::get_position() { return position; }
glm::vec4 render_object::get_color() { 
	if (color->color_buffer == NULL)
		return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	return color->color_buffer[0]; 
}
void render_object::set_shader_program(shader_program* program) {
	this->program = program;
};
void render_object::gen_vertices_buffer() {
	unsigned int vertex_data_size;
	GLint vertex_position, vertex_normal, vertex_color;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	vertex_position = glGetAttribLocation(program->ID, "vPosition");
	vertex_normal = glGetAttribLocation(program->ID, "vNormal");
	vertex_color = glGetAttribLocation(program->ID, "vColor");

	vertex_data_size = sizeof(glm::vec3);

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

	else {
		glBufferData(GL_ARRAY_BUFFER, total_vertices * vertex_data_size, position_buffer, GL_STATIC_DRAW);
		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(vertex_position);
	}
};
void render_object::gen_normal_buffer() {
	glm::vec3 u, v, norm;

	for (int i = 0; i < total_vertices; i += 3) {
		u = position_buffer[i + 2] - position_buffer[i];
		v = position_buffer[i + 1] - position_buffer[i];
		norm = glm::normalize(glm::cross(u, v));

		normal->normal_buffer[i] = norm;
		normal->normal_buffer[i + 1] = norm;
		normal->normal_buffer[i + 2] = norm;
	}
};
void render_object::draw() {
	glUseProgram(program->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, total_vertices);
};
GLuint render_object::get_shader_programID() { return program->ID; }
shader_program render_object::get_shader_program() { return *program; }
line::line() {
	length = 1.0f;
};
line::line(float len, int orent, glm::vec4 color, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) : render_object(2, new has_color(color, 2), new non_normal, pos){
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
void line::gen_position_buffer() {
	position_buffer = new glm::vec3[total_vertices];

};
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
void line::draw() {
	glUseProgram(program->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE, 0, total_vertices);
}

grid::grid() {
	length = 0;
	depth = 0;
	height = 0;
};
grid::grid(glm::vec3 position, glm::vec4* colors, float len, float dep, float hei){
	x_axis = new line(len, 0, colors[0]);
	y_axis = new line(hei, 1, colors[1]);
	z_axis = new line(dep, 2, colors[2]);

	length = len;
	depth = dep;
	height = hei;

	
};
void grid::gen_vertices_buffer() {
	x_axis->gen_vertices_buffer();
	y_axis->gen_vertices_buffer();
	z_axis->gen_vertices_buffer();
}
void grid::set_shader_program(shader_program* program) {
	x_axis->set_shader_program(program);
	y_axis->set_shader_program(program);
	z_axis->set_shader_program(program);
}
void grid::draw() {
	shader_program axis_program;
	
	axis_program = x_axis->get_shader_program();
	axis_program.set_mat4("model", x_axis->model);
	x_axis->draw();

	axis_program = y_axis->get_shader_program();
	axis_program.set_mat4("model", y_axis->model);
	y_axis->draw();

	axis_program = z_axis->get_shader_program();
	axis_program.set_mat4("model", z_axis->model);
	z_axis->draw();
}
shapes::shapes() : render_object() {
	height = 0;
	base = 0;

};
shapes::shapes(int vertices, color_delegate* color, normal_delegate* normal, float* measurements, glm::vec3 position) : render_object(vertices, color, normal, position){
	height = measurements[0];
	base = measurements[1];
};
void shapes::gen_position_buffer() {}; //empty 

triangle::triangle() : shapes(){
	
};
triangle::triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements) : shapes(3, new has_color(color, 3), new has_normal(3), measurements, position) {
	gen_position_buffer();
	if (has_normals)
		gen_normal_buffer();
};
void triangle::gen_position_buffer() {
	glm::vec3 vertex;
	position_buffer = new glm::vec3[total_vertices];

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

