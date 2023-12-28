#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include<string>
#include <iostream>
// a basic class to handle the initialize the window;
int w, h;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
class Show_Window {
public:
	Show_Window(int width, int height, std::string name) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		w = width;
		h = height;
		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (window == nullptr) {
			std::cerr << "Failed to creat the window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return;
		}
	} 
	GLFWwindow* getWindow() {
		return window;
	}
public:
private:
	GLFWwindow* window;

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	w = width;
	h = height;
}
#endif