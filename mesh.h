#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "utility.h"
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};
struct Texture {
	unsigned int tex_id;
	std::string texture_name;

};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> texures;
	unsigned int VAO, VBO, EBO;
	void Draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size() , GL_UNSIGNED_INT, (void*)0);
	}
	void Set_VAO() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	unsigned int Get_VAO() {
		return VAO;
	}
};
#endif