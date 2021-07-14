#include<stdio.h>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"../header/shader.h"
#include"../header/render_object.h"
#include"../header/camera.h"
#include"../header/light.h"
#include<glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct shape_node {
	shapes* object = NULL;
	shape_node* next = NULL;
};

void init_window(int option);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void mouse_input_callback(GLFWwindow* window, double x_pos, double y_pos);
void process_input(GLFWwindow* window);
void save_file();
void create_file();

camera main_camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f));
double last_x = WINDOW_WIDTH / 2.0f;
double last_y = WINDOW_HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

shader_program* basic_program;
shape_node shape_linked_list; 
shape_node* shape_tail = &shape_linked_list;

int main(void) {
	glfwInit();
	printf("OpenGL version supported %s\n", glfwGetVersionString());

	init_window(3);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Shapes", NULL, NULL);
	if (window == NULL) {
		perror("Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Failed to load GLAD\n");
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyboard_input_callback);
	glfwSetCursorPosCallback(window, mouse_input_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	const char* shader_paths[2] = { "source/shader/basic_vertex.glsl", "source/shader/basic_fragment.glsl" };
	basic_program = new shader_program(shader_paths[0], shader_paths[1]);
	
	glm::mat4 projection = glm::perspective(glm::radians(main_camera.zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	light main_light(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f);

	int shiny = 32;
	
	float right_triangle_measurements[2];
	right_triangle_measurements[0] = 10.0f;
	right_triangle_measurements[1] = 10.0f;

	shape_linked_list.object = new triangle(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), true, right_triangle_measurements);
	shape_linked_list.object->set_shader_program(basic_program);
	shape_linked_list.object->gen_vertices_buffer();

	glm::vec4 grid_colors[3];
	grid_colors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	grid_colors[1] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	grid_colors[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	int non_shaded = 1;
	int shaded = 0;
	grid object_orentation(glm::vec3(0.0f, 0.0f, 0.0f), grid_colors, 5, 5, 5);
	object_orentation.set_shader_program(basic_program);
	object_orentation.gen_vertices_buffer();

	float current_frame;
	shape_node* current_node;
	glClearColor(0.529f, 0.807f, 0.92f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		current_node = &shape_linked_list;
		process_input(window); 
		basic_program->set_vec3("light_color", main_light.color);
		basic_program->set_vec3("light_pos", main_light.position);
		basic_program->set_float("ambient_stren", main_light.ambient_str);
		basic_program->set_float("specular_stren", main_light.specular_str);
		basic_program->set_int("shininess", shiny);

		basic_program->set_vec3("viewer_pos", main_camera.position);
		basic_program->set_mat4("projection", projection);
		basic_program->set_mat4("view", main_camera.get_view_matrix());

		while(current_node != NULL) {
			basic_program->set_int("is_non_shaded", shaded);
			basic_program->set_mat4("model", current_node->object->model);
			current_node->object->draw();
			current_node = current_node->next;
		} 
		basic_program->set_int("is_non_shaded", non_shaded);
		object_orentation.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	save_file();

	glfwTerminate();
	return 0;
}

void init_window(int option) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, option);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, option);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	GLfloat current_time = (GLfloat)glfwGetTime();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}
void process_input(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		main_camera.process_keyboard(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		main_camera.process_keyboard(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		main_camera.process_keyboard(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		main_camera.process_keyboard(RIGHT, delta_time);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		float measurement[2];
		measurement[0] = 2.0f;
		measurement[1] = 2.0f;

		glm::vec3 position(main_camera.position.x, main_camera.position.y, main_camera.position.z);
		position = position + glm::vec3(glm::normalize(main_camera.front) * 5.0f);
		float y_angle = glm::radians(main_camera.pitch);
		float x_angle = glm::radians(main_camera.yaw);
		
		shape_tail->next = new shape_node;
		shape_tail->next->object = new triangle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), true, measurement);
		shape_tail->next->object->set_shader_program(basic_program);
		shape_tail->next->object->gen_vertices_buffer();
		shape_tail->next->object->model = glm::translate(shape_tail->next->object->model, position);
		shape_tail->next->object->model = glm::rotate(shape_tail->next->object->model, y_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		shape_tail->next->object->model = glm::rotate(shape_tail->next->object->model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		
		shape_tail = shape_tail->next;

	}
}
void mouse_input_callback(GLFWwindow* window, double x_pos, double y_pos) {
	if (first_mouse) {
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	double x_diff = x_pos - last_x;
	double y_diff = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	main_camera.process_mouse(x_diff, y_diff);
}
void save_file() {
	FILE* save_file;
	char filename[81];
	char decision = '\0';


	printf("Do you want to save? (y/n)\n");
	scanf_s("%c", &decision, 1);
	if ((int)decision >= 97)
		decision = decision - 32;
	while (decision != 'Y' && decision != 'N') {
		printf("\nPlease enter a vaild choice\n");
		scanf_s("%c", &decision, 1);
		if ((int)decision >= 97)
			decision = decision - 32;
	}
	
	if (decision == 'N') //exits program without saving
		return;

	printf("Enter the name of file: ");
	scanf_s("%s", &filename, 80);

	fopen_s(&save_file, filename, "r");

	if (save_file != NULL) {
		while (decision != 'Y' && decision != 'N') {
			printf("Do you want to overwrite the file? (y/n)");
			scanf_s(" %c", &decision, 1);

			if ((int)decision >= 97) {
				decision = decision - 32;
			}
			if (decision != 'Y' && decision != 'N')
				printf("Please enter a valid choice\n");
		}
	}

	if (save_file == NULL || decision == 'Y') {
		glm::vec3 shape_pos;
		glm::vec3 shape_rot;
		glm::vec3 shape_trans;
		glm::vec3 shape_scal;
		glm::vec4 shape_color;
		glm::mat4 shape_model;
		shape_node* shape_ptr;

		fopen_s(&save_file, filename, "w");
		shape_ptr = &shape_linked_list;
		while (shape_ptr != NULL) {
			shape_pos = shape_ptr->object->get_position();
			fprintf_s(save_file, "%f %f %f\n", shape_pos.x, shape_pos.y, shape_pos.z);

			shape_color = shape_ptr->object->get_color();
			fprintf(save_file, "%f %f %f %f\n", shape_color.r, shape_color.g, shape_color.b, shape_color.a);

			shape_rot = shape_ptr->object->rotation;
			fprintf(save_file, "%f %f %f\n", shape_rot.x, shape_rot.y, shape_rot.z);

			shape_trans = shape_ptr->object->translation;
			fprintf(save_file, "%f %f %f\n", shape_trans.x, shape_trans.y, shape_trans.z);

			shape_scal = shape_ptr->object->scale;
			fprintf(save_file, "%f %f %f\n", shape_scal.x, shape_scal.y, shape_scal.z);

			shape_model = shape_ptr->object->model;
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					fprintf(save_file, "%f ", shape_model[row][col]);
				}
				fprintf(save_file, "\n");
			}
			shape_ptr = shape_ptr->next;
		}
	}

	fclose(save_file);
}
void create_file() {

}