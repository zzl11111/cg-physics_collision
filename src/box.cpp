#include "box.h"

// set up bounds and points
void Box::setUpMesh(const glm::vec3 * pts, const int num) {
	for (int i = 0; i < num; i++) {
		m_bounds.expand(pts[i]);
	}

	m_points.clear();
	m_points.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z));
	m_points.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z));
	m_points.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z));
	m_points.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z));

	m_points.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z));
	m_points.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z));
	m_points.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z));
	m_points.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z));

	mass_center = (m_bounds.mins + m_bounds.maxs) / 2.0f;
}

// we calculate the inertia tensor with parallel axis theorem
glm::mat3 Box::get_Ineritial_mat3(float mass) const {
	const float dx = m_bounds.maxs.x - m_bounds.mins.x;
	const float dy = m_bounds.maxs.y - m_bounds.mins.y;
	const float dz = m_bounds.maxs.z - m_bounds.mins.z;

	glm::mat3 tensor(0.0f); // inertia tensor
	tensor[0][0] = (dy * dy + dz * dz) / 12.0f;
	tensor[1][1] = (dx * dx + dz * dz) / 12.0f;
	tensor[2][2] = (dx * dx + dy * dy) / 12.0f;

	glm::vec3 cm; // may be center of mass? we can change this part of code later
	cm.x = (m_bounds.mins.x + m_bounds.maxs.x) / 2.0f;
	cm.y = (m_bounds.mins.y + m_bounds.maxs.y) / 2.0f;
	cm.z = (m_bounds.mins.z + m_bounds.maxs.z) / 2.0f;

	const glm::vec3 r = glm::vec3(0) - cm;
	const float r2 = r.length();
	glm::mat3 patTensor;
	patTensor[0] = glm::vec3(r2 - r.x * r.x, r.x * r.y, r.x * r.z);
	patTensor[1] = glm::vec3(r.y * r.x, r2 - r.y * r.y, r.y * r.z);
	patTensor[2] = glm::vec3(r.z * r.x, r.z * r.y, r2 - r.z * r.z);

	return mass * (tensor + patTensor);
}

// return the vertex which is the farthest in the direction of dir
glm::vec3 Box::support(const glm::vec3 & dir, const glm::vec3 & pos, const glm::quat & orient, const float bias) const {
	glm::vec3 maxPt = glm::vec3(0);
	float maxDist = -FLT_MAX;
	for (int i = 0; i < m_points.size(); i++) {
		// different with the book code, so bug may exist here
		glm::vec3 pt = pos + orient * m_points[i];
		float dist = glm::dot(pt, dir);
		if (dist > maxDist) {
			maxDist = dist;
			maxPt = pt;
		}
	}
	glm::vec3 norm = glm::normalize(dir);
	return maxPt + norm * bias;
}


Bounds Box::getBounds(const glm::vec3 & pos, const glm::quat & orient) const {
	glm::vec3 corners[8];
	// the sequence of corners is the different with the book code, so bug may exist here
	corners[0] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z);
	corners[1] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z);
	corners[2] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z);
	corners[3] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z);
	corners[4] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z);
	corners[5] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z);	
	corners[6] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z);
	corners[7] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z);

	Bounds bounds;
	for (int i = 0; i < 8; i++) {
		bounds.expand(pos + orient * corners[i]);
	}

	return bounds;
}

Bounds Box::getBounds() const {
	return m_bounds;
}

float Box::fastestLinearSpeed(const glm::vec3 & angularVelocity, const glm::vec3 & dir) const {
	float maxSpeed = -FLT_MAX;
	for (int i = 0; i < m_points.size(); i++) {
		glm::vec3 pt = m_points[i] - mass_center;
		glm::vec3 vel = glm::cross(angularVelocity, pt);
		float speed = glm::dot(vel, dir);
		if (speed > maxSpeed) {
			maxSpeed = speed;
		}
	}
	return maxSpeed;
}