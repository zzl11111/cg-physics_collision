#pragma once
#include "body.h"
#include "ray.h"
#include "utility.h"

const int sector_count = 540;
const int stack_count = 540;
const float sectorstep = 2 * 3.1415 / sector_count;
const float stackstep = 3.1415 / stack_count;

class Sphere : public Model {
public:
	virtual unsigned int Get_Type() const { return SPHERE; }
	unsigned int load_texture( const char * path);
	virtual glm::mat3 get_Ineritial_mat3(float mass)const ;
	Sphere(float _radius) : radius(_radius) {
		mass_center = glm::vec3(0, 0, 0);
		setup_Mesh();
		mesh.Set_VAO();
	}
	virtual void Draw() { mesh.Draw(); }
	void setup_Mesh();
	float radius;
};

bool Sphere_Sphere_Dynamic(
    const Sphere *sphereA, 
	const Sphere *sphereB, 
	const glm::vec3 &position_A,
    const glm::vec3 &position_B, 
	const glm::vec3 &v_A, 
	const glm::vec3 &v_B,
    const float dt, 
	glm::vec3 &A_potential_collision_point_world_space,
    glm::vec3 &B_potential_collision_point_world_space, 
	float &time_of_impact
);

bool Ray_Sphere_Collision(
	Ray &r, 
	const glm::vec3 &sphere_Center,
	float Sphere_radius, 
	float &t_in,
	float &t_out
);