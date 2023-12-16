#ifndef CAMERA_H
#define CAMERA_H
#include "utility.h"
constexpr const float camera_speed = 80.0f;
enum   Direction
{
	FRONT, BACK, LEFT, RIGHT, UP, DOWN
};
class Camera {
public:
	Camera(vec3 origin, vec3 lookat, vec3 up = vec3(0, 1, 0));
	void Process_camera_keyboard(unsigned int direction, float delta_time);
	void Process_camera_mouse(float xoffset, float yoffset);
	void Process_camera(unsigned int direction, float delta_time);
	mat4 get_View_Matrix();
	void Update_vector();
	void Process_camera_scroll(float yoffset);
	void return_to_initial();
public:
	vec3 camera_pos;
	vec3 camera_front;
	vec3 camera_up;
	vec3 camera_right;
	vec3 initial_camera_front;
	vec3 initial_camera_up;
	vec3 initial_camera_right;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouse_sensity = 0.1;
	float scroll_sensity = 0.2;
	float fov = 45.0f;
};



#endif