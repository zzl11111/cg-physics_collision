#ifndef HITTABLE_H
#define HITTABLE_H
#include "mesh.h"
#include <glm/gtx/rotate_vector.hpp>
enum model_type {
  SPHERE = 0,
};

class Model 
{
public:
  virtual unsigned int Get_Type() const = 0;
  glm::vec3 get_CenterOfMass() const {
    return mass_center; // it is the local space center
  }
  virtual glm::mat3 get_Ineritial_mat3(float mass = 1e5f) const = 0;
  virtual void Draw() = 0;
  glm::vec3 mass_center;
  Mesh mesh;
};

class Body {
public:
  Body(Model *_shape, glm::vec3 position,
       glm::vec3 velocity = glm::vec3(0, 0, 0),
       glm::quat rotation = glm::quat(1, 0, 0, 0), float inv_mass = 1.0)
      : shape(_shape), m_position(position), m_velocity(velocity),
        m_rotation(rotation), m_inv_mass(inv_mass) {}
  glm::vec3 get_CenterofWorldSpace() const {
    // glm::vec4 v = glm::mat4_cast(m_rotation) *
    // glm::vec4(shape->get_CenterOfMass(), 1);
    return m_position + m_rotation * shape->get_CenterOfMass();
    ;
  }

  glm::vec3 get_CenterofLocalSpace() const {
    return shape->get_CenterOfMass();
  } // modelspace and world space
  glm::vec3 World_To_Local_space(const glm::vec3 &position) const {
    glm::vec3 position_to_center = position - get_CenterofWorldSpace();
    glm::quat inverse_quat =
        glm::quat(m_rotation.w, -m_rotation.x, -m_rotation.y, -m_rotation.z);
    return inverse_quat * position_to_center;
  }
  glm::vec3 Local_To_World_space(const glm::vec3 &position) const {
    glm::vec3 position_to_center = position - get_CenterofLocalSpace();
    glm::vec3 world_vector = m_rotation * position_to_center;
    return world_vector + get_CenterofWorldSpace();
  }
  glm::mat4 get_model_matrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    return model; // model matrix is related to the model
  }
  void Process_Impulse(const glm::vec3 &impulse) {
    m_velocity += impulse * m_inv_mass;
  }
  glm::mat3 get_Inertial_mat3_local() {
    glm::mat3 Inertial;
    if (m_inv_mass != 0) {
      Inertial = shape->get_Ineritial_mat3(1 / m_inv_mass);
    } else {
      Inertial = shape->get_Ineritial_mat3();
    }
    return Inertial;
  }
  glm::mat3 get_Inertial_mat3_world() {
    glm::mat3 Inertial = get_Inertial_mat3_local();
    glm::mat3 rotation_matrix = glm::mat3(m_rotation);
    return rotation_matrix * Inertial * glm::transpose(rotation_matrix);
    // R*I*R^T
  }

public:
  Model *shape;
  glm::vec3 m_position;
  glm::vec3 m_velocity;
  glm::quat m_rotation;
  float elasticity = 1;
  float m_inv_mass;
};
#endif