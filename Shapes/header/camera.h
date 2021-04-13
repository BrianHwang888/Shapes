#ifndef __CAMERA_H__
#define __CAMERA_H__

#include"resource_manager.h"

enum camera_movement {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};

/*----- CAMERA CLASS -----*/
class camera{
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 world_up;

	float yaw;
	float pitch;

	float speed;
	float mouse_sensivity;

	camera(glm::vec3 camera_position, glm::vec3 camera_right, glm::vec3 world_up);

	void process_keyboard(camera_movement movement, float delta_time);
	void process_mouse(float x_offset, float y_offest, bool constrain_pitch = true);

private:
	void update_camera_vectors();	
};


#endif // !__CAMERA_H__
