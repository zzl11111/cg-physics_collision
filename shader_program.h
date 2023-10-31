#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include <fstream>
#include <string>
#include "utility.h"
std::string ReadFromFile(std::string name) {
	std::ifstream ifs(name);
	std::string out;
	if (ifs.is_open()) {
		std::string s;
		while (std::getline(ifs, s)) {
			out += s+"\n";
		}
	}
	else {
		std::cerr << "file :" << name << "cannot open" << std::endl;

	}
	ifs.close();
	return out;
}
class Shader_Program {
public:
	Shader_Program(std::string vertex_shader_source,std::string fragment_shader_source) {
		unsigned int vertexShader;
		unsigned int fragmentShader;
		int success;
		char infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		std::string v_shader = ReadFromFile(vertex_shader_source);
		const char* vertexShaderSource = v_shader.c_str();
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
	
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(-1);
		}
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string f_shader = ReadFromFile(fragment_shader_source);
		const char* fragmentShaderSource = f_shader.c_str();
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);





		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(-1);
		}
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		program_id = shaderProgram;
	
	}

	Shader_Program(std::string vertex_shader_source, std::string geometry_shader_source, std::string fragment_shader_source) {
		unsigned int vertexShader;
		unsigned int geometryShader;
		unsigned int fragmentShader;
		int success;
		char infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		std::string v_shader = ReadFromFile(vertex_shader_source);
		const char* vertexShaderSource = v_shader.c_str();
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(-1);
		}



		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		std::string g_shader = ReadFromFile(geometry_shader_source);
		const char* geometryShaderSource = v_shader.c_str();
		glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
		glCompileShader(geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(-1);
		}




		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string f_shader = ReadFromFile(fragment_shader_source);
		const char* fragmentShaderSource = f_shader.c_str();
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			exit(-1);
		}
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, geometryShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		program_id = shaderProgram;


	}
	void Use() {
		glUseProgram(program_id);
	}
	void setBool(const std::string& name, bool value)const {
		glUniform1i(glGetUniformLocation(program_id,name.c_str()) ,(int)value);
	}
	void setInt(const std::string& name, int value)const {
		glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);

	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name,  vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(program_id, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name,vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name,mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setOpenGl_Mat4(const std::string& name, glm::mat4& mat)const {
		glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
public:
	unsigned int program_id;
};



#endif