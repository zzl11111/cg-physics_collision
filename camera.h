#ifndef CAMERA_H
#define CAMERA_H
#include "utility.h"
constexpr const float camera_speed = 2.0f;
enum   Direction
{
	FRONT, BACK, LEFT, RIGHT
};

class Camera {
public:
	Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up = glm::vec3(0, 1, 0));
	void Process_camera_keyboard(unsigned int direction, float delta_time);
	void Process_camera_mouse();
	void Process_camera(unsigned int direction, float delta_time);
	glm::mat4 get_View_Matrix();
	void Update_vector();
public:
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_right;
};



#endif