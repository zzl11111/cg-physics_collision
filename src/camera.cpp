#include "camera.h"
Camera::Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 up)
{
    camera_pos = origin;
    camera_front = normalize(lookat - origin);
    pitch = asin(camera_front.y);
    glm::vec2 v(camera_front.x, camera_front.z);
    v = glm::normalize(v);
    if (camera_front.z <= 0) {
        yaw = acos(v.x);

    }
    else {
        yaw = 2 * PI - acos(v.x);
    }
    pitch = radian_to_degree(pitch);
    yaw = radian_to_degree(yaw);
    camera_right = normalize(cross(camera_front, up));
    camera_up = normalize(cross(camera_right, camera_front));
    initial_camera_front = camera_front;
    initial_camera_up = camera_up;
    initial_camera_right = camera_right;
    Update_vector();
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
    else if (direction == (Direction::UP)) {
        camera_pos += camera_up * camera_speed * delta_time;
    }
    else if (direction == (Direction::DOWN)) {
        camera_pos -= camera_up * camera_speed * delta_time;
    }
}
void Camera::Process_camera_mouse(float xoffset, float yoffset) {
    xoffset *= mouse_sensity;
    yoffset *= mouse_sensity;
    yaw -= xoffset;
    pitch += yoffset;
    Update_vector();
}
void Camera::Process_camera_scroll(float yoffset) {
    yoffset *= scroll_sensity;
    fov -= yoffset;
    fov = clamp(fov, 1.0, 45.0);
}
void Camera::Process_camera(unsigned int direction, float delta_time) {
    Process_camera_keyboard(direction, delta_time);

}
glm::mat4 Camera::get_View_Matrix() {
    return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

}
void Camera::Update_vector() {
    yaw = clamp(yaw, 0.01, 359.0);
    pitch = clamp(pitch, -89.0, 89.0);
    camera_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front.y = sin(glm::radians(pitch));
    camera_front.z = -cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camera_right = normalize(cross(camera_front, camera_up));
    camera_up = normalize(cross(camera_right, camera_front));
}
void Camera::return_to_initial() {
    camera_front = initial_camera_front;
    camera_right = initial_camera_right;
    camera_up = initial_camera_up;
    pitch = asin(camera_front.y);
    glm::vec2 v(camera_front.x, camera_front.z);
    v = glm::normalize(v);
    if (camera_front.z <= 0) {
        yaw = acos(v.x);

    }
    else {
        yaw = 2 * PI - acos(v.x);
    }
    pitch = radian_to_degree(pitch);
    yaw = radian_to_degree(yaw);
    camera_right = normalize(cross(camera_front, camera_up));
    camera_up = normalize(cross(camera_right, camera_front));
}