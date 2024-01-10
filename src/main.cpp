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
std::string name = "rigid body simulator";
//WE use glm instead
glm::vec3 camera_pos(2, 3, 30);
bool is_first = false;
float last_x;
float last_y;
Camera cam(camera_pos, glm::vec3(0, 0, 0));
glm::vec3 lightPos(1, 3, 2);

void Process_Input(GLFWwindow* window, float delta_time) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
	if (glfwGetKey(window, GLFW_KEY_A)) { cam.Process_camera(LEFT, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_D)) { cam.Process_camera(RIGHT, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_W)) { cam.Process_camera(FRONT, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_S)) { cam.Process_camera(BACK, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_E)) { cam.Process_camera(UP, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_Q)) { cam.Process_camera(DOWN, delta_time); }
	if (glfwGetKey(window, GLFW_KEY_B)) { cam.return_to_initial(); }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	float x = (float)xPos;
	float y = (float)yPos;
	if (is_first)
	{
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

int main() {
	Show_Window opengl_window(800, 600, name);
	Shader shaderProgram("shader\\blin.vs", "shader\\blin.fs");
	Scene sphere_scene;
	GLFWwindow* window = opengl_window.getWindow();
	glEnable(GL_DEPTH_TEST);
	//glfwSetCursorPosCallback(window, mouse_callback);
	while (!glfwWindowShouldClose(window)) {
		float current_frame_time = glfwGetTime();
		
		glClearColor(0.2, 0.3, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
		glm::mat4 view= cam.get_View_Matrix();

		shaderProgram.use();
	
		//shaderProgram.setMat4("view", view);
		/*shaderProgram.setMat4("projection", projection);*/

		//shaderProgram.setOpenGl_Mat4("model", opengl_model);
		glm::mat4 model = glm::mat4(1.0f);
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setVec3("lightPos", lightPos);
		shaderProgram.setVec3("viewPos", camera_pos);
		shaderProgram.setVec3("lightColor",glm::vec3(2,2,2));
		shaderProgram.setVec3("objectColor", glm::vec3(1, 1, 1));
		// set sampler2D
		shaderProgram.setInt("diffuse_sampler", 0); // texture unit 0
		float delta_time = glfwGetTime() - current_frame_time;
		int  num = 100;
		for (int i = 0; i < num; i++) {
			sphere_scene.Update(delta_time);
		}
		sphere_scene.Draw(shaderProgram);
		Process_Input(window,delta_time);
	
		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}
	glfwTerminate();

	return 0;
}