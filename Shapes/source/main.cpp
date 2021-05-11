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

void init_window(int option);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void mouse_input_callback(GLFWwindow* window, double x_pos, double y_pos);
void process_input(GLFWwindow* window);

camera main_camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f));
double last_x = WINDOW_WIDTH / 2.0f;
double last_y = WINDOW_HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

shader_program* basic_program;
bool draw_tri = false;

struct shape_node {
	shapes* object;
	shape_node* next;
};

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
	glm::mat4 model;
	light main_light(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f);

	int shiny = 32;
	
	float right_triangle_measurements[2];
	right_triangle_measurements[0] = 10.0f;
	right_triangle_measurements[1] = 10.0f;

	shape_node triangles;
	triangles.object = new triangle(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), true, right_triangle_measurements);
	triangles.object->set_shader_program(basic_program);
	triangles.object->gen_vertices_buffer();

	float current_frame;
	glClearColor(0.529f, 0.807f, 0.92f, 1.0f);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		current_frame = glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(window); 
		basic_program->set_vec3("light_color", main_light.color);
		basic_program->set_vec3("light_pos", main_light.position);
		basic_program->set_float("ambient_stren", main_light.ambient_str);
		basic_program->set_float("specular_stren", main_light.specular_str);
		basic_program->set_int("shininess", shiny);

		basic_program->set_vec3("viewer_pos", main_camera.position);
		basic_program->set_mat4("projection", projection);
		basic_program->set_mat4("view", main_camera.get_view_matrix());
		basic_program->set_mat4("model", triangles.object->model);

		triangles.object->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
		glm::vec3 position(main_camera.position.x + 5.0f, main_camera.position.y, main_camera.position.z);
		draw_tri = true;
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