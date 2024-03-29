#pragma once

#include "utility.h"
#include "bound.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

struct Triangle_t {
	int a, b, c;
};

struct Edge_t {
	int a, b;
	bool operator==(const Edge_t &rhs) const {
		return (a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a);
	}
};

class Point_t {
public:
	glm::vec3 xyz;
	glm::vec3 point_A;
	glm::vec3 point_B;
	Point_t() : xyz(glm::vec3(0, 0, 0)), point_A(0, 0, 0), point_B(0, 0, 0) {}
	Point_t &operator=(const Point_t&rhs){
		xyz=rhs.xyz;
		point_A=rhs.point_A;
		point_B=rhs.point_B;
	}
  	bool operator==(const Point_t &rhs) const 
	{
		return (xyz == rhs.xyz) && (point_A == rhs.point_B) &&
			(point_B == rhs.point_B);
	}
};
enum model_type {
  SPHERE = 0,
  BOX = 1,
  CONVEX = 2,
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
	// virtual void build(const glm::vec3 * pts, const int num) = 0; // build the shape from points
	// use the following two functions to detect collision between convex shapes
	virtual glm::vec3 support(const glm::vec3 & dir, const glm::vec3 & pos, const glm::quat & orient, const float bias) const = 0;
	virtual float fastestLinearSpeed(const glm::vec3 & angularVelocity, const glm::vec3 & dir) const { return 0.0f; }
	unsigned int load_texture(const char *path);
public:
	glm::vec3 mass_center;
	Mesh mesh;
};

class Body {
public:
Body(){
	
}
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
	bool is_changeing=false;
	float before_changeing_distance=0;
};