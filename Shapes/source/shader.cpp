#include "../header/shader.h"

shader_program::shader_program(const char* vertex_path, const char* fragment_path) {
	const char* shader_source[2];  //[0]: vertex source; [1]: fragment source
	GLuint* shader_IDs;
	GLint linked;

	shader_source[0] = vertex_path;
	shader_source[1] = fragment_path;

	shader_IDs = compile_shader(shader_source);
	
	ID = glCreateProgram();
	if (!ID) {
		fprintf(stderr, "Failed to create shader program using IDs: %d, %d\n", shader_IDs[0], shader_IDs[1]);
		exit(EXIT_FAILURE);
	}
	glAttachShader(ID, shader_IDs[0]);
	glAttachShader(ID, shader_IDs[1]);
	
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint log_size;
		char* log_msg;
		fprintf(stderr, "Shader program failed to link; ID: %d\n", ID);
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &log_size);
		log_msg = new char[log_size];
		glGetProgramInfoLog(ID, log_size, NULL, log_msg);
		perror(log_msg);
		delete[] log_msg;
		system("pause");
		exit(EXIT_FAILURE);
	}

	glDeleteShader(shader_IDs[0]);
	glDeleteShader(shader_IDs[1]);
	free(shader_IDs);
}	
void shader_program::use() {
	glUseProgram(ID);
}
GLuint* shader_program::compile_shader(const char** shader_paths) {
	GLint compiled;
	GLuint* shader_IDs = new GLuint[2]; //[0]: vertex id; [1]: fragment id

	printf("Attempting to read: %s\n", shader_paths[0]);
	shader_paths[0] = read_file(shader_paths[0]);
	printf("Attempting to read: %s\n", shader_paths[1]);
	shader_paths[1] = read_file(shader_paths[1]);

	shader_IDs[0] = glCreateShader(GL_VERTEX_SHADER);
	shader_IDs[1] = glCreateShader(GL_FRAGMENT_SHADER);

	for (int i = 0; i < 2; i++) {
		glShaderSource(shader_IDs[i], 1, (const GLchar**)&shader_paths[i], NULL);
		glCompileShader(shader_IDs[i]);
		glGetShaderiv(shader_IDs[i], GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			GLint log_size;
			char* log_msg;
			fprintf(stderr, "%s failed to compile\n", shader_paths[i]);
			glGetShaderiv(shader_IDs[i], GL_INFO_LOG_LENGTH, &log_size);
			log_msg = new char[log_size];
			glGetShaderInfoLog(shader_IDs[i], log_size, NULL, log_msg);
			fprintf(stderr, "\nReason:\n%s\n", log_msg);
			delete[] log_msg;
			system("pause");
			exit(EXIT_FAILURE);
		}
		printf("Successfully compiled %s\nID: %d\n", shader_paths[i], shader_IDs[i]);
	}

	return shader_IDs;
}