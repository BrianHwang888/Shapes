#include"../header/render_object.h"

void non_color::gen_color_buffer(int vertices) {};
has_color::has_color(glm::vec4 color, int vertices) {
	this->color = color;
	color_buffer = new glm::vec4[vertices];

	for (int i = 0; i < vertices; i++) {
		color_buffer[i] = color;
	}
}
void has_color::gen_color_buffer(int vertices) {
	
};


void non_normal::gen_normal_buffer(int vertices, glm::vec3* position_buffer) {};
has_normal::has_normal(int vertices) {
	normal_buffer = new glm::vec3[vertices];
}
void has_normal::gen_normal_buffer(int vertices, glm::vec3* position) {
	glm::vec3 u, v, norm;

	for (int i = 0; i < vertices; i += 3) {
		u = position[i + 2] - position[i];
		v = position[i + 1] - position[i];
		norm = glm::normalize(glm::cross(u, v));

		normal_buffer[i] = norm;
		normal_buffer[i + 1] = norm;
		normal_buffer[i + 2] = norm;
	}
};

render_object::render_object(int vertices, color_delegate* color, normal_delegate* normal) {
	total_vertices = vertices;
	this->color = color;
	this->normal = normal;

}
void render_object::gen_vertices_buffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), position_buffer, GL_STATIC_DRAW);

}

shapes::shapes(int vertices, color_delegate* color, normal_delegate* normal, float* measurements) : render_object(vertices, color, normal){
	height = measurements[0];
	base = measurements[1];
};

triangle::triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements) : shapes(3, new has_color(color, 3), new has_normal(3), measurements) {
	this->position = position;

	gen_position_buffer();
	if(typeid(this->color) == typeid(has_color))
		((has_color*)(this->color))->gen_color_buffer(3);

	if (has_normals)
		((has_normal*)(this->normal))->gen_normal_buffer(3, position_buffer);
	
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

