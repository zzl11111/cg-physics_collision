#pragma once

#include "body.h"
#include "collision_pair.h"
#include "contact.h"
#include "ray.h"
#include "scene.h"
#include "shader_program.h"
#include "sphere.h"
#include "utility.h"
#include <algorithm>
#include "collision_pair.h"



inline bool check_collision(Body &body1, Body &body2, Contact &contact, float dt);

inline void Process_collision(Contact &contact);

class Scene {
public:
  Scene() { Create_Scene(); }
  void Create_Scene();
  void Update(float delta_time);
  void Draw(const Shader &shader_prog);

private:
  std::vector<Body> objs;
};

inline bool check_collision(Body &body1, Body &body2, Contact &contact, float dt) {
	contact.A = &body1;
	contact.B = &body2;
	if (body1.shape->Get_Type() == SPHERE && body2.shape->Get_Type() == SPHERE) {
		const Sphere *sphere_A = (const Sphere *)body1.shape;
		const Sphere *sphere_B = (const Sphere *)body2.shape;
		if (Sphere_Sphere_Dynamic(sphere_A, sphere_B, body1.m_position,
								body2.m_position, body1.m_linear_velocity,
								body2.m_linear_velocity, dt,
								contact.A_potential_collision_point_world_space,
								contact.B_potential_collision_point_world_space,
								contact.time_of_impact)) {
			// the collision happen in time_of_impact;
			contact.normal = glm::normalize(body2.m_position - body1.m_position);
			body1.Update(contact.time_of_impact);
			body2.Update(contact.time_of_impact);
			contact.A_potential_collision_point_local_space =
				body1.World_To_Local_space(
					contact.A_potential_collision_point_world_space);
			contact.B_potential_collision_point_local_space =
				body2.World_To_Local_space(
					contact.B_potential_collision_point_world_space);
			body1.Update(-contact.time_of_impact);
			body2.Update(-contact.time_of_impact);
			// return back
			glm::vec3 p_a_b = body2.m_position - body1.m_position;
			float r = glm::length(p_a_b) - (sphere_A->radius + sphere_B->radius);
			contact.seperation_distance = r;
			return true;
		} else { return false; }
	}
	return false;
	// this function will check collision and update the collision point in
	// contact
}

inline void Process_collision(Contact &contact) {
	Body *A = contact.A;
	Body *B = contact.B;
	float elasticity = A->elasticity * B->elasticity;
	// glm::vec3 J = (1 + elasticity) * (B->m_linear_velocity -
	// A->m_linear_velocity) / (A->m_inv_mass + B->m_inv_mass);
	glm::vec3 ra = contact.A_potential_collision_point_world_space -
					contact.A->get_CenterofWorldSpace();
	glm::vec3 rb = contact.B_potential_collision_point_world_space -
					contact.B->get_CenterofWorldSpace();
	glm::vec3 n = contact.normal;
	glm::vec3 angular_Impulse_A =
		cross(contact.A->get_Inertial_inverse_mat3_world() * cross(ra, n), ra);
	glm::vec3 angular_Impulse_B =
		cross(contact.B->get_Inertial_inverse_mat3_world() * cross(rb, n), rb);
	float angularFactor = dot((angular_Impulse_A + angular_Impulse_B),
								n); // calculate the angular_part
	glm::vec3 v_A = A->m_linear_velocity + cross(A->m_angular_velocity, ra);
	glm::vec3 v_B = B->m_linear_velocity + cross(B->m_angular_velocity, rb);
	glm::vec3 v_ab = v_A - v_B;
	float Impulse_J_amplitude = (1.0f + elasticity) * dot(v_ab, n) /
								(angularFactor + A->m_inv_mass + B->m_inv_mass);
	glm::vec3 Impulse_J = Impulse_J_amplitude * n;

	A->Process_Impulse(contact.A_potential_collision_point_world_space,
						-Impulse_J);
	B->Process_Impulse(contact.B_potential_collision_point_world_space,
						Impulse_J);
	// process the impulse according to the impulse point and the  Impulse
	// put the body A and B to outside
	// Apply the frictionx	
	float friction = A->m_friction * B->m_friction;
	glm::vec3 v_relate_tangent = v_ab - dot(v_ab, n) * n;
	glm::vec3 v_relate_tangent_normalized;
	if (glm::length(v_relate_tangent) == 0) {
		v_relate_tangent_normalized = v_relate_tangent;
	} else {
		v_relate_tangent_normalized = glm::normalize(v_relate_tangent);
	}
	// calculate the friction_part
	glm::vec3 inertiaA = cross(A->get_Inertial_inverse_mat3_world() *
									cross(ra, v_relate_tangent_normalized),
								ra);
	glm::vec3 inertiaB = cross(B->get_Inertial_inverse_mat3_world() *
									cross(rb, v_relate_tangent_normalized),
								rb);
	float invInertia = dot((inertiaA + inertiaB), v_relate_tangent_normalized);
	float reducedMass = 1.0f / (A->m_inv_mass + B->m_inv_mass + invInertia);
	glm::vec3 impulse_Friction = v_relate_tangent * reducedMass * friction;
	A->Process_Impulse(contact.A_potential_collision_point_world_space,
						-impulse_Friction);
	B->Process_Impulse(contact.B_potential_collision_point_world_space,
						impulse_Friction);
	// std::cout << A->m_linear_velocity << B->m_linear_velocity;
	if(0.0f == contact.time_of_impact){
		float tA = A->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
		float tB = B->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
		glm::vec3 ds = contact.B_potential_collision_point_world_space -
						contact.A_potential_collision_point_world_space;
		A->m_position += ds * tA;
		B->m_position -= ds * tB; // push the A and B to outside
	}
}
