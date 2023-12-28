#ifndef CONTACT_H
#define CONTACT_H
#include "body.h"
class Contact {
public:

	Body* A;
	Body* B;
	vec3 A_potential_collision_point_world_space;
	vec3 B_potential_collision_point_world_space;
	vec3 A_potential_collision_point_local_space;
	vec3 B_potential_collision_point_local_space;
	float distance;
	float time;
	vec3 normal;
};

#endif