#include"../header/camera.h"

camera::camera(glm::vec3 camera_position, glm::vec3 camera_front, glm::vec3 world_up, float yaw, float pitch) : speed(SPEED), mouse_sensivity(SENSITIVITY), zoom(ZOOM) {
	position = camera_position;
	front = camera_front;
	this->world_up = world_up;

	update_camera_vectors();
};
void camera::process_keyboard(camera_movement movement, float delta_time) {
	float velocity = speed * delta_time;  

	switch (movement)
	{
	case FORWARD:
		position += front * velocity;
		break;
	case BACKWARD:
		position -= front * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	}
	
};
void camera::process_mouse(float x_offest, float y_offset, bool constrain_pitch) {
	yaw += x_offest * mouse_sensivity;
	pitch += y_offset * mouse_sensivity;

	if (constrain_pitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}

	update_camera_vectors();
};
void camera::update_camera_vectors() {
	glm::vec3 new_front;

	new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_front.y = sin(glm::radians(pitch));
	new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(new_front);
	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));	
};
glm::mat4 camera::get_view_matrix() {
	return glm::lookAt(position, position + front, up);
}