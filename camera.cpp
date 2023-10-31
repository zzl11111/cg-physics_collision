#include "camera.h"
#include "camera.h"
Camera::Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up)
{
    camera_pos = origin;
    camera_front = normalize(lookat - origin);
    camera_right = normalize(cross(camera_front, up));
    camera_up = normalize(cross(camera_right, camera_front));
}
void Camera::Process_camera_keyboard(unsigned int direction, float delta_time) {
    if (direction == (Direction::FRONT)) {
        camera_pos += camera_front * camera_speed * delta_time;

    }
    else if (direction == (Direction::BACK)) {
        camera_pos -= camera_front * camera_speed * delta_time;
    }
    else if (direction == (Direction::RIGHT)) {
        camera_pos += camera_right * camera_speed * delta_time;

    }
    else if (direction == (Direction::LEFT)) {
        camera_pos -= camera_right * camera_speed * delta_time;
    }
}
void Camera::Process_camera_mouse() {

}
void Camera::Process_camera(unsigned int direction, float delta_time) {
    Process_camera_keyboard(direction, delta_time);

}
glm::mat4 Camera::get_View_Matrix() {
    return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

}
void Camera::Update_vector() {

}