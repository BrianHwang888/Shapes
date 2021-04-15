#include"../header/render_object.h"

void non_color::gen_color_buffer(int vertices) {};
has_color::has_color(glm::vec4 color, int vertices) {
	this->color = color;
	color_buffer = new glm::vec4[vertices];
}
void has_color::gen_color_buffer(int vertices) {
	
};


void non_normal::gen_normal_buffer(int vertices) {};
has_normal::has_normal(int vertices) {
	normal_buffer = new glm::vec3[vertices];
}
void has_normal::gen_normal_buffer(int vertices) {
};


void render_object::gen_vertices_buffer() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(glm::vec3), position_buffer, GL_STATIC_DRAW);

}

triangle::triangle(glm::vec3 position, glm::vec4 color, bool has_normals, float* measurements) {
	total_vertices = 3;
	height = *measurements;
	base = *(measurements + 1);
	position_buffer = new glm::vec3[3];
	this->color = new has_color(color, 3);
	if (has_normals)
		this->normal = new has_normal();

	gen_vertices_buffer();
};

