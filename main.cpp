#include "window.h"
#include "shader_program.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "scene.h"
#include "utility.h"
constexpr const int width = 800;
constexpr const int height = 600;
std::string name = "aa";
//WE use glm instead
void Process_Input(GLFWwindow* window,Camera &cam,float delta_time=0.01);

void Process_Input(GLFWwindow* window,Camera &cam,float delta_time) {
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
}
glm::vec3 lightPos = glm::vec3(0,0.3, -0.5f);
glm::vec3 camera_pos = glm::vec3(0, 0,3);
glm::vec3 pos(0, 0, 3);
int main() {
	Camera cam(pos, glm::vec3(0, 0, 0));
	Show_Window opengl_window(800, 600, name);
	Shader_Program shaderProgram("blin.vs", "blin.fs");
	Scene sphere_scene;
	GLFWwindow* window = opengl_window.getWindow();

	while (!glfwWindowShouldClose(window)) {
		float current_frame_time = glfwGetTime();
		
		glClearColor(0.2, 0.3, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
		glm::mat4 view= cam.get_View_Matrix();

		shaderProgram.Use();
	
		//shaderProgram.setMat4("view", view);
		/*shaderProgram.setMat4("projection", projection);*/
		shaderProgram.setOpenGl_Mat4("projection", projection);
		//shaderProgram.setOpenGl_Mat4("model", opengl_model);
		shaderProgram.setOpenGl_Mat4("view", view);
		shaderProgram.setVec3("lightPos", lightPos);
		shaderProgram.setVec3("viewPos", camera_pos);
		shaderProgram.setVec3("lightColor",1,1,1);
		shaderProgram.setVec3("objectColor", 0, 1, 0);
		float delta_time = glfwGetTime() - current_frame_time;
		sphere_scene.Update(delta_time);
		sphere_scene.Draw(shaderProgram);
		Process_Input(window, cam, delta_time);
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		
	}
	glfwTerminate();

	return 0;
}