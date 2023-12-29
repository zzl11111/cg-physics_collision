#ifndef SCENE_H
#define SCENE_H
#include"body.h"
#include "utility.h"
#include "sphere.h"
#include "contact.h"
glm::vec3 gravity(0, -10.0, 0);
bool check_collision( Body& body1,  Body& body2,Contact &contact) {
	glm::vec3 position_between_body2_body1 = body2.m_position - body1.m_position;
	contact.normal = normalize(position_between_body2_body1);//the collision point vector (b-a)

	double length = std::sqrt(dot(position_between_body2_body1,position_between_body2_body1));
	auto sphere_1 = (const Sphere*)body1.shape;
	auto sphere_2 = (const Sphere*)body2.shape;
	contact.A = &body1;
	contact.B = &body2;

	contact.A_potential_collision_point_world_space = body1.m_position + contact.normal * sphere_1->radius;
	contact.B_potential_collision_point_world_space = body2.m_position - contact.normal * sphere_2->radius;
	double radius_AB = sphere_1->radius + sphere_2->radius;
	return radius_AB> length;

}
void Process_collision(Contact& contact) {
	Body* A = contact.A;
	Body* B = contact.B;
	float elasticity = A->elasticity * B->elasticity;
	//glm::vec3 J = (1 + elasticity) * (B->m_velocity - A->m_velocity) / (A->m_inv_mass + B->m_inv_mass);
	glm::vec3 J = (1 + elasticity) * dot((B->m_velocity - A->m_velocity),contact.normal)*contact.normal / (A->m_inv_mass + B->m_inv_mass);//we only apply impulse through the vector of contact_normal


	A->Process_Impulse(J);
	B->Process_Impulse(-J);
	float tA = A->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
	float tB = B->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
	glm::vec3 ds = contact.B_potential_collision_point_world_space - contact.A_potential_collision_point_world_space;
	A->m_position += tA * ds;
	B->m_position -= tB * ds;
}


class Scene {
public:
	Scene() {
		Create_Scene();

	}
	void Create_Scene() {
		int num_objs = 1;
		Body obj(new Sphere(1), glm::vec3(0, 0,0), glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), 1.0);
		obj.elasticity = 0.95;
		objs.push_back(obj);
		Body ground(new Sphere(100), glm::vec3(0, -103, 0), glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), 0);
		ground.elasticity = 0.99;
		objs.push_back(ground);
		
	
	}
	void Update(float delta_time) {
		for (int i = 0; i < objs.size();i++) {
			if (objs[i].m_inv_mass == 0)continue;//not move
			bool collision = false;
			for (int j = i + 1; j < objs.size(); j++) {
				Contact contact;
				collision = check_collision(objs[i], objs[j],contact);
				if (collision) {
					Process_collision(contact);
				}
				else {
					glm::vec3 impulse = 1.0f / objs[i].m_inv_mass * gravity * delta_time;
					objs[i].Process_Impulse(impulse);
				}
			}
				

		}
		for (auto& obj : objs) {
			obj.m_position += obj.m_velocity * delta_time;
		}

	}
	void Draw(const Shader &shader_prog) {
		
		for (auto& obj : objs) {
			glm::mat4 model = obj.get_model_matrix();
			shader_prog.setMat4("model", model);
			obj.shape->Draw();
		}
	}
	std::vector<Body> objs;

};
#endif