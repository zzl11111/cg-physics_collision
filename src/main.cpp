#include "GLFW/glfw3.h"
#include "camera.h"
#include "gjk.h"
#include "glm/geometric.hpp"
#include "scene.h"
#include "shader_program.h"
#include "sphere.h"
#include "utility.h"
#include "window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <ostream>

constexpr const int width = 800;
constexpr const int height = 600;
std::string name = "rigid body simulator";
// WE use glm instead
glm::vec3 camera_pos(2, 0, 30);
bool is_first = false;
float last_x;
float last_y;
float last_y_scroll = 0;
float y_scroll_offset = 0;
Camera cam(camera_pos, glm::vec3(0, 0, 0));
glm::vec3 lightPos(1, 3, 2);

bool start = false;
bool add_wind = false;
bool wind_to_change = false;
bool interactive_mode = false;
void Process_Input(GLFWwindow *window, float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_A)) {
    cam.Process_camera(LEFT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_D)) {
    cam.Process_camera(RIGHT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_W)) {
    cam.Process_camera(FRONT, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_S)) {
    cam.Process_camera(BACK, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_E)) {
    cam.Process_camera(UP, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_Q)) {
    cam.Process_camera(DOWN, delta_time);
  }
  if (glfwGetKey(window, GLFW_KEY_B)) {
    cam.return_to_initial();
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE)) {
    start =true;
  }
  if (glfwGetKey(window, GLFW_KEY_1)) {
    if (wind_to_change == false) {
      wind_to_change = true;
    }
    add_wind = !add_wind;
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) {
    if (add_wind) {
      add_wind = false;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    interactive_mode = true;
  }
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

  y_scroll_offset += yoffset;
}

void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
  float x = (float)xPos;
  float y = (float)yPos;
  if (is_first) {
    is_first = false;
    last_x = x;
    last_y = y;
  }
  float xoffset = x - last_x;
  float yoffset = last_y - y;
  last_x = x;
  last_y = y;
  cam.Process_camera_mouse(xoffset, yoffset);
}
void Interactive(GLFWwindow *window, const Camera &cam,
                 std::vector<Body> &objects, glm::mat4 model, glm::mat4 view,
                 glm::mat4 projection) {
  float aspect_ratio = (float)w / (float)h;
  float near = 0.1f;
  float far = 100.0f;
  float ndc_z = -1.0;
  //-1 to 1 and
  glm::vec3 ndc = glm::vec3((2.0 * last_x - w) / (float)w,
                            (2.0 * last_y - h) / (float)h, ndc_z);

  float clip = far * near / (near + far + ndc_z);
  glm::vec3 perspective_coords = clip * ndc;
  glm::vec4 perspective_homogenous = glm::vec4(
      perspective_coords.x, perspective_coords.y, perspective_coords.z, clip);
  glm::vec4 world_mouse_homogenous = inverse(model) * inverse(view) *
                                     inverse(projection) *
                                     perspective_homogenous;
  float we = world_mouse_homogenous.w;
  glm::vec3 mouse_world =
      glm::vec3(world_mouse_homogenous.x / we, -world_mouse_homogenous.y / we,
                world_mouse_homogenous.z / we);
  glm::mat4 mvp = projection * view * model;

  // we get the world
  glm::vec3 mouse_camera_vector = mouse_world - cam.camera_pos;
  // mouse_camera_vector = cam.camera_front;
  float mini_dist = 0.3;
  mouse_camera_vector = glm::normalize(mouse_camera_vector);
  for (auto &obj : objects) {
    Ray r(cam.camera_pos, glm::normalize(mouse_camera_vector));

    if (obj.is_changeing != 0) {
      if (!obj.shape->Get_Type() == SPHERE) {
        continue;
        // we only change the obj which is sphere
      }
      Sphere *sphere = (Sphere *)obj.shape;
      if (y_scroll_offset != 0) {
        float radius = sphere->radius + y_scroll_offset * 1.0f / 3.0f;
        radius = std::max(radius, 1e-2f);
        delete sphere;
        obj.shape = new Sphere(radius);
      }
      if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        float cos_theta = dot(cam.camera_front, mouse_camera_vector);
        obj.m_position = cam.camera_pos + obj.before_changeing_distance /
                                              cos_theta *
                                              normalize(mouse_camera_vector);

        obj.is_changeing = false;
        break;
      }
    }

    if (!obj.shape->Get_Type() == SPHERE) {
      continue;
      // we only change the obj which is sphere
    }
    Sphere *sphere = (Sphere *)obj.shape;
    float t_in, t_out;
    glm::vec3 sphere_center = obj.m_position;
    float min_dist = 0.6f;
    glm::vec3 sphere_cam = sphere_center - cam.camera_pos;

    float dist = glm::length(cross(sphere_cam, mouse_camera_vector)) /
                 glm::length(mouse_camera_vector);
    if (dist < min_dist &&
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

      float _distance = dot(cam.camera_front, sphere_cam);
      if (!obj.is_changeing) {
        y_scroll_offset = 0.0f;
      }
      obj.before_changeing_distance = _distance;
      obj.is_changeing = true;
    }
  }
}
int main() {
  Show_Window opengl_window(800, 600, name);
  Shader shaderProgram("shader\\blin.vs", "shader\\blin.fs");
  Scene sphere_scene;
  GLFWwindow *window = opengl_window.getWindow();
  glEnable(GL_DEPTH_TEST);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  // test();
  // glfwSetCursorPosCallback(window, mouse_callback);
  while (!glfwWindowShouldClose(window)) {
    float current_frame_time = glfwGetTime();

    glClearColor(0.2, 0.3, 0.3, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 view = cam.get_View_Matrix();

    shaderProgram.use();

    // shaderProgram.setMat4("view", view);
    /*shaderProgram.setMat4("projection", projection);*/

    glm::mat4 model = glm::mat4(1.0f);
    shaderProgram.setMat4("model", model);
    shaderProgram.setMat4("view", view);
    shaderProgram.setMat4("projection", projection);
    shaderProgram.setVec3("lightPos", lightPos);
    shaderProgram.setVec3("viewPos", camera_pos);
    shaderProgram.setVec3("lightColor", glm::vec3(2, 2, 2));
    shaderProgram.setVec3("objectColor", glm::vec3(1, 1, 1));
    // set sampler2D
    shaderProgram.setInt("diffuse_sampler", 0); // texture unit 0
    float delta_time = glfwGetTime() - current_frame_time;
    int num = 5;
    float fixed_time = 0.00005f;
    if (start) {
      for (int i = 0; i < num; i++) {
        sphere_scene.Update(fixed_time, add_wind);
      }
    }
    sphere_scene.Draw(shaderProgram);
    Process_Input(window, fixed_time);
    if (interactive_mode) {

      Interactive(window, cam, sphere_scene.objs, model, view, projection);
    }
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();

  return 0;
}