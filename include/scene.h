#pragma once

#include "body.h"
#include "contact.h"
#include "shader_program.h"
#include "sphere.h"
#include "utility.h"

glm::vec3 gravity(0, -10.0, 0);
inline bool check_collision(Body &body1, Body &body2, Contact &contact) {
  glm::vec3 position_between_body2_body1 = body2.m_position - body1.m_position;
  contact.normal =
      normalize(position_between_body2_body1); // the collision point vector
                                               // (b-a) maintain normalize

  double length = std::sqrt(
      dot(position_between_body2_body1, position_between_body2_body1));
  auto sphere_1 = (const Sphere *)body1.shape;
  auto sphere_2 = (const Sphere *)body2.shape;
  contact.A = &body1;
  contact.B = &body2;

  contact.A_potential_collision_point_world_space =
      body1.m_position + contact.normal * sphere_1->radius;
  contact.B_potential_collision_point_world_space =
      body2.m_position - contact.normal * sphere_2->radius;
  double radius_AB = sphere_1->radius + sphere_2->radius;
  return radius_AB > length;
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
  // Apply the friction
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
  float reducedMass = 3.0f / (A->m_inv_mass + B->m_inv_mass + invInertia);
  glm::vec3 impulse_Friction = v_relate_tangent * reducedMass * friction;
  A->Process_Impulse(contact.A_potential_collision_point_world_space,
                     -impulse_Friction);
  B->Process_Impulse(contact.B_potential_collision_point_world_space,
                     impulse_Friction);
  //std::cout << A->m_linear_velocity << B->m_linear_velocity;
  float tA = A->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
  float tB = B->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
  glm::vec3 ds = contact.B_potential_collision_point_world_space -
                 contact.A_potential_collision_point_world_space;
  A->m_position += ds * tA;
  B->m_position -= ds * tB; // push the A and B to outside
}

class Scene {
public:
  Scene() { Create_Scene(); }
  void Create_Scene() {
    int num_objs = 1;
    Body obj(new Sphere(2), glm::vec3(0, 2, 0), glm::vec3(1, 0, 0),
             glm::quat(1, 0, 0, 0), 1.0);
    obj.elasticity = 0.9;
    obj.m_friction=1.0;
    objs.push_back(obj);
       Body obj2(new Sphere(2), glm::vec3(1, 3, 0), glm::vec3(1, 0, 0),
             glm::quat(1, 0, 0, 0), 1.0);
    obj2.elasticity = 0.9;
    obj2.m_friction=1.0;
    objs.push_back(obj2);
    Body ground(new Sphere(8000), glm::vec3(0, -8003, 0), glm::vec3(0, 0, 0),
                glm::quat(1, 0, 0, 0), 0);

    ground.elasticity = 0.99;
    ground.m_friction=0.9;
    objs.push_back(ground);
  }
  void Update(float delta_time) {
    for (int i = 0; i < objs.size(); i++) {
      if (objs[i].m_inv_mass == 0)
        continue; // not move
      bool collision = false;
      for (int j = i + 1; j < objs.size(); j++) {
        Contact contact;
        collision = check_collision(objs[i], objs[j], contact);
        if (collision) {
          Process_collision(contact);
        } else {
          glm::vec3 impulse = 1.0f / objs[i].m_inv_mass * gravity * delta_time;
          objs[i].Process_Linear_Impulse(impulse);
        }
      }
    }
    for (auto &obj : objs) {
      obj.Update(delta_time);
    }
  }
  void Draw(const Shader &shader_prog) {

    for (auto &obj : objs) {
      glm::mat4 model = obj.get_model_matrix();
      shader_prog.setMat4("model", model);
      obj.shape->Draw();
    }
  }
  std::vector<Body> objs;
};
