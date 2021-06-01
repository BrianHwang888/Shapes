#include"../header/render_object.h"

has_color::has_color(glm::vec4 color, int vertices) {
	this->color = color;
	color_buffer = new glm::vec4[vertices];

	for (int i = 0; i < vertices; i++) {
		color_buffer[i] = color;
	}
}

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

}
glm::vec3* render_object::get_position_buffer() {
	return position_buffer;
};
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

line::line() {
	length = 1.0f;
};
line::line(glm::vec4 color, float len) : render_object(2, new has_color(color, 2), new non_normal){
	length = len;

};
void line::gen_position_buffer() {
	
};

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
	position_buffer = new glm::vec3[3];

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

