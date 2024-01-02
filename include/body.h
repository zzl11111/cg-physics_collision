#pragma once

#include "utility.h"
#include "bound.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

enum model_type {
  SPHERE = 0,
};

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
	unsigned int VAO, VBO, EBO, texture_id;
	void Draw();
	void Set_VAO(); // initialize the VAO
	unsigned int Get_VAO() { return VAO; }
};

class Model {
public:
	virtual unsigned int Get_Type() const = 0;
	// center of the local space
	glm::vec3 get_CenterOfMass() const { return mass_center; }
	virtual glm::mat3 get_Ineritial_mat3(float mass = 1.0f) const = 0;
	virtual void Draw() = 0;
	virtual Bounds getBounds(const glm::vec3 & pos, const glm::quat & orient) const = 0;
	virtual Bounds getBounds() const = 0;

public:
	glm::vec3 mass_center;
	Mesh mesh;
};

class Body {
public:
	Body(Model *_shape, glm::vec3 position,
		glm::vec3 velocity = glm::vec3(0, 0, 0),
		glm::quat rotation = glm::quat(1, 0, 0, 0),
		float inv_mass = 1.0) // notice that the last is
			:shape(_shape), 
			m_position(position), 
			m_linear_velocity(velocity),
			m_rotation(rotation), 
			m_inv_mass(inv_mass),
			m_angular_velocity(glm::vec3(0, 0, 0)) {}

	glm::vec3 get_CenterofWorldSpace() const;
	glm::vec3 get_CenterofLocalSpace() const; // modelspace and world space
	glm::vec3 World_To_Local_space(const glm::vec3 &position) const;
	glm::vec3 Local_To_World_space(const glm::vec3 &position) const;
	glm::mat4 get_model_matrix();
	void Process_Impulse(const glm::vec3 &impulse_point,
						 const glm::vec3 &impulse);
 	void Process_Linear_Impulse(const glm::vec3 &linear_impulse);
  	void Process_Angular_Impulse(const glm::vec3 &angular_impulse);
  	glm::mat3 get_Inertial_mat3_local();
	glm::mat3 get_Inertial_mat3_world();
  	glm::mat3 get_Inertial_inverse_mat3_world();
  	void Update(float dt);

public:
	Model *shape;
	glm::vec3 m_position;
	glm::vec3 m_linear_velocity;
	glm::quat m_rotation;
	glm::vec3 m_angular_velocity;
	float elasticity = 1;
	float m_inv_mass;
	float m_friction = 0.8f;
};