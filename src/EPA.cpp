#include "EPA.h"

glm::vec3 barycentricCoordinates(glm::vec3 s1, glm::vec3 s2, glm::vec3 s3, const glm::vec3 &p) {
	s1 = s1 - p;
	s2 = s2 - p;
	s3 = s3 - p;

	glm::vec3 normal = glm::cross(s2 - s1, s3 - s1);
	glm::vec3 p0 = normal * glm::dot(normal, s1) / glm::dot(normal, normal);

	// Find the axis with the greatest projected area
	int index = 0;
	float area_max = 0;
	for (int i =0; i < 3; i++) {
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		glm::vec2 a = glm::vec2(s1[j], s1[k]);
		glm::vec2 b = glm::vec2(s2[j], s2[k]);
		glm::vec2 c = glm::vec2(s3[j], s3[k]);
		glm::vec2 a2b = b - a;
		glm::vec2 a2c = c - a;

		float area = a2b.x * a2c.y - a2b.y * a2c.x;
		if (area * area > area_max * area_max) {
			area_max = area;
			index = i;
		}
	}

	// project onto the axis with the greatest projected area
	int x = (index + 1) % 3;
	int y = (index + 2) % 3;
	glm::vec2 s[3];
	s[0] = glm::vec2(s1[x], s1[y]);
	s[1] = glm::vec2(s2[x], s2[y]);
	s[2] = glm::vec2(s3[x], s3[y]);
	glm::vec2 p2 = glm::vec2(p0[x], p0[y]); 

	// Get the sub_areas of the triangles formed by the projected point and the vertices of the triangle
	glm::vec3 areas;
	for (int i = 0; i < 2; i++) {
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		glm::vec2 a = p2;
		glm::vec2 b = s[j];
		glm::vec2 c = s[k];
		glm::vec2 a2b = b - a;
		glm::vec2 a2c = c - a;

		areas[i] = a2b.x * a2c.y - a2b.y * a2c.x;
	}

	glm::vec3 lambda = areas / area_max;
	if (lambda.x * 0.0f != lambda.x * 0.0f || lambda.y * 0.0f != lambda.y * 0.0f || lambda.z * 0.0f != lambda.z * 0.0f) {
		return glm::vec3(1, 0, 0);
	}
	return lambda;
}

// normalDirection
