#pragma once
#include "body.h"
#include "ray.h"
#include "utility.h"

const int sector_count = 40;
const int stack_count = 40;
const float sectorstep = 2 * 3.1415 / sector_count;
const float stackstep = 3.1415 / stack_count;

class Sphere : public Model {
public:
	virtual unsigned int Get_Type() const { return SPHERE; }
	unsigned int load_texture( const char * path);
	virtual glm::mat3 get_Ineritial_mat3(float mass) const;
	Bounds getBounds(const glm::vec3 & pos, const glm::quat & orient) const override;
	Bounds getBounds() const override;
	Sphere(float _radius) : radius(_radius) {
		mass_center = glm::vec3(0, 0, 0);
		setup_Mesh();
		mesh.Set_VAO();
	}
	virtual void Draw() { mesh.Draw(); }
	void setup_Mesh();
	// this function gives us the farest point on the particular direction
	glm::vec3 support(const glm::vec3 & dir, const glm::vec3 & pos, const glm::quat & orient, const float bias) const override;
	// fastestLinearSpeed only useful for those convex shape, so we did not implement it here

public:
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