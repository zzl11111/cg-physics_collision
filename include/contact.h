#pragma once

#include "body.h"
class Contact {
public:

	Body* A;
	Body* B;
	glm::vec3 A_potential_collision_point_world_space;
	glm::vec3 B_potential_collision_point_world_space;
	glm::vec3 A_potential_collision_point_local_space;
	glm::vec3 B_potential_collision_point_local_space;
	float seperation_distance=0;
	float time_of_impact=0;

	glm::vec3 normal;
};
