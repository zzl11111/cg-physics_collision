#pragma once
#include "body.h"
#include "ray.h"
const int sector_count = 540;
const int stack_count = 540;
const float sectorstep = 2 * 3.1415 / sector_count;
const float stackstep = 3.1415 / stack_count;

class Sphere : public Model {
public:
  virtual unsigned int Get_Type() const { return SPHERE; }
  virtual glm::mat3 get_Ineritial_mat3(float mass) const {
    // set the ineria in local world
    glm::mat3 mat(1.0f);
    float ineria = 2.0 / 5.0 * mass * radius * radius;
    for (int i = 0; i < 3; i++) {
      mat[i][i] = ineria;
    }
    return mat;
  }
  Sphere(float _radius) : radius(_radius) {
    mass_center = glm::vec3(0, 0, 0);
    setup_Mesh();
    mesh.Set_VAO();
  }
  virtual void Draw() { mesh.Draw(); }
  void setup_Mesh() {
    float sector_Angle, stack_Angle;
    glm::vec3 v;
    glm::vec3 n;
    glm::vec2 texcoords;
    // initialize the vertices
    for (int i = 0; i <= stack_count; i++) {
      stack_Angle = PI / 2 - i * stackstep;
      float xy_plane = radius * cos(stack_Angle);
      v[2] = radius * sin(stack_Angle);
      for (int j = 0; j <= sector_count; ++j) {
        sector_Angle = j * sectorstep;
        v[0] = xy_plane * cos(sector_Angle);
        v[1] = xy_plane * sin(sector_Angle);
        n[0] = v[0] / radius;
        n[1] = v[1] / radius;
        n[2] = v[2] / radius;
        texcoords[0] = ((float)j) / sector_count;
        texcoords[1] = ((float)i) / stack_count;
        Vertex vertex{v, n, texcoords};
        mesh.vertices.push_back(vertex);
      }
    }
    int k1, k2;
    // get ebo;
    for (int i = 0; i < stack_count; ++i) {
      k1 = i * (sector_count + 1);
      k2 = k1 + sector_count + 1;

      for (int j = 0; j < sector_count; ++j) {
        if (i != 0) {
          mesh.indices.push_back(k1);
          mesh.indices.push_back(k2);
          mesh.indices.push_back(k1 + 1);
        }
        if (i != (stack_count)-1) {
          mesh.indices.push_back(k1 + 1);
          mesh.indices.push_back(k2);
          mesh.indices.push_back(k2 + 1);
        }
        k1++;
        k2++;
      }
    }
  }
  float radius;
};
inline bool Ray_Sphere_Collision(Ray &r, const glm::vec3 &sphere_Center,
                                 float Sphere_radius, float &t_in,
                                 float &t_out) {
  glm::vec3 OR = sphere_Center - r.origin;
  float a = dot(r.dir, r.dir);
  float b = dot(OR, r.dir);
  float c = dot(OR, OR) - Sphere_radius * Sphere_radius;
  float delta = b * b - a * c;
  if (delta < 0) {
    return false;
  }
  delta = sqrt(delta);
  float invA = 1.0f / a;
  t_in = invA * (b - delta);
  t_out = invA * (b + delta);
  return true;
}
inline bool Sphere_Sphere_Dynamic(
    const Sphere *sphereA, const Sphere *sphereB, const glm::vec3 &position_A,
    const glm::vec3 &position_B, const glm::vec3 &v_A, const glm::vec3 &v_B,
    const float dt, glm::vec3 &A_potential_collision_point_world_space,
    glm::vec3 &B_potential_collision_point_world_space, float &time_of_impact) {
  glm::vec3 related_v = v_A - v_B;
  Ray r;
  r.origin = position_A;
  r.dir = related_v * dt;
  if (glm::length(r.dir) < 0.001f * 0.001f) {
    glm::vec3 p_a_b = position_B - position_A;
    // check collision directly
    float radius = sphereA->radius + sphereB->radius + 0.001f;
    if (glm::length(p_a_b) > radius * radius) {
      return false;
    }
  }
  float t0, t1;
  if (!Ray_Sphere_Collision(r, position_B, sphereA->radius + sphereB->radius,
                            t0, t1)) {
    return false;
  }
  t0 *= dt;
  t1 *= dt;
  if (t1 < 0.0f) {

    return false;
  }
  time_of_impact = t0 < 0.0f ? 0.0f : t0;
  if (time_of_impact > dt) {
    return false;
  }
  glm::vec3 newP_A = position_A + v_A * time_of_impact;
  glm::vec3 newP_B = position_B + v_B * time_of_impact;
  glm::vec3 normal = newP_B - newP_A;
  if (glm::length(normal) != 0) {
    normal = glm::normalize(normal);
  }
  A_potential_collision_point_world_space = newP_A + normal * sphereA->radius;
  B_potential_collision_point_world_space = newP_B - normal * sphereB->radius;
  return true;
}