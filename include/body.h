#pragma once

#include "mesh.h"
#include "utility.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

enum model_type {
  SPHERE = 0,
};

class Model {
public:
  virtual unsigned int Get_Type() const = 0;
  glm::vec3 get_CenterOfMass() const {
    return mass_center; // it is the local space center
  }
  virtual glm::mat3 get_Ineritial_mat3(float mass = 1.0f) const = 0;
  virtual void Draw() = 0;
  glm::vec3 mass_center;
  Mesh mesh;
};

class Body {
public:
  Body(Model *_shape, glm::vec3 position,
       glm::vec3 velocity = glm::vec3(0, 0, 0),
       glm::quat rotation = glm::quat(1, 0, 0, 0),
       float inv_mass = 1.0) // notice that the last is
      : shape(_shape), m_position(position), m_linear_velocity(velocity),
        m_rotation(rotation), m_inv_mass(inv_mass),
        m_angular_velocity(glm::vec3(0, 0, 0)) {}
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

    model =  model*glm::mat4(m_rotation) ;
    return model; // model matrix is related to the model
  }
  void Process_Impulse(const glm::vec3 &impulse_point,
                       const glm::vec3 &impulse) {
    if (m_inv_mass == 0) {
      return;
    }

    Process_Linear_Impulse(impulse);
    glm::vec3 r = impulse_point - get_CenterofWorldSpace();
    glm::vec3 angular_impulse = cross(r, impulse);

    Process_Angular_Impulse(angular_impulse);
  }
  void Process_Linear_Impulse(const glm::vec3 &linear_impulse) {
    if (m_inv_mass == 0) {
      return;
    }
    m_linear_velocity += linear_impulse * m_inv_mass; // v+=J/m
  }
  void Process_Angular_Impulse(const glm::vec3 &angular_impulse) {
    if (m_inv_mass == 0) {
      return;
    }
    m_angular_velocity +=
        get_Inertial_inverse_mat3_world()  * angular_impulse; // w+=L/I
    // std::cout << m_angular_velocity;
    float max_angular_speed=30.0f;
    if(glm::length(m_angular_velocity)>=max_angular_speed){
      m_angular_velocity=max_angular_speed*glm::normalize(m_angular_velocity);
    }
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
  glm::mat3 get_Inertial_inverse_mat3_world() {
    glm::mat3 Inertial = get_Inertial_mat3_local();
    glm::mat3 rotation_matrix = glm::mat3(glm::normalize(m_rotation));
    return rotation_matrix * glm::inverse(Inertial) *
           glm::transpose(rotation_matrix);
    // R *I-1*R^T
  }
  void Update(float dt)
  // update the position
  {

    glm::vec3 cmTopos = m_position - get_CenterofWorldSpace();
    glm::vec3 alpha = get_Inertial_inverse_mat3_world() *
                      (cross(m_angular_velocity,
                             get_Inertial_mat3_world() * m_angular_velocity));
    // process the internal torque
    m_angular_velocity += alpha * dt;
    
    glm::vec3 dAngle = m_angular_velocity * dt;

    glm::quat dq = get_quat_from_n_and_angle(dAngle, glm::length(dAngle));
    m_rotation = dq * m_rotation;
    m_rotation = glm::normalize(m_rotation);
    m_position = m_position + m_linear_velocity * dt;
    m_position =get_CenterofWorldSpace()+ dq * cmTopos;
  }

public:
  Model *shape;
  glm::vec3 m_position;
  glm::vec3 m_linear_velocity;
  glm::quat m_rotation;
  glm::vec3 m_angular_velocity;
  float elasticity = 1;
  float m_inv_mass;
  float m_friction=0.8f;
};