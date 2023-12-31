#pragma once
#include "utility.h"
constexpr const float camera_speed = 100.0f;
enum   Direction
{
	FRONT, BACK, LEFT, RIGHT, UP, DOWN
};
class Camera {
public:
	Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up = glm::vec3(0, 1, 0));
	void Process_camera_keyboard(unsigned int direction, float delta_time);
	void Process_camera_mouse(float xoffset, float yoffset);
	void Process_camera(unsigned int direction, float delta_time);
	glm::mat4 get_View_Matrix();
	void Update_vector();
	void Process_camera_scroll(float yoffset);
	void return_to_initial();
public:
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_right;
	glm::vec3 initial_camera_front;
	glm::vec3 initial_camera_up;
	glm::vec3 initial_camera_right;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouse_sensity = 0.1;
	float scroll_sensity = 0.2;
	float fov = 45.0f;
};
