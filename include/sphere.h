#pragma once
#include "body.h"

const int sector_count = 540;
const int stack_count = 540;
const float sectorstep = 2.0 * PI / sector_count;
const float stackstep = PI / stack_count;

class Sphere : public Model {
public:
	Sphere(float _radius) : radius(_radius) {
		mass_center = glm::vec3(0, 0, 0);
		setup_Mesh();
		mesh.Set_VAO();
	}
	unsigned int Get_Type() const { return model_type::SPHERE; }
	glm::mat3 get_Ineritial_mat3(float mass) const;
	virtual void Draw() { mesh.Draw(); }
	unsigned int load_texture(char const * path);
	void setup_Mesh();
	
	float radius;
};
