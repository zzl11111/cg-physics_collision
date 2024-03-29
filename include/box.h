#include "body.h"
#include <vector>


class Box : public Model {
public:
	Box(const std::vector<glm::vec3> & pts, const int num) {
		setUpMesh(pts, num);
		mesh.Set_VAO();
	}
	Box (Bounds bounds);
	Box(glm::vec3 mins,glm::vec3 maxs);
	void setUpMesh(const std::vector<glm::vec3> & pts, const int num); 
	void setUpMesh();
	glm::vec3 support(const glm::vec3 & dir, const glm::vec3 & pos, const glm::quat & orient, const float bias) const override;
	glm::mat3 get_Ineritial_mat3(float mass) const override;
	Bounds getBounds(const glm::vec3 & pos, const glm::quat & orient) const override;
	Bounds getBounds() const override;
	float fastestLinearSpeed(const glm::vec3 & angularVelocity, const glm::vec3 & dir) const override;
	unsigned int Get_Type() const override { return BOX; }
	void Draw() override { mesh.Draw(); }

public:
	std::vector<glm::vec3> m_points;
	Bounds m_bounds;
};