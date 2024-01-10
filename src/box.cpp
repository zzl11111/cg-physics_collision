#include "box.h"

// set up bounds and points
void Box::setUpMesh(const std::vector<glm::vec3> &pts, const int num) {
  for (int i = 0; i < num; i++) {
    m_bounds.expand(pts[i]);
  }

  m_points.clear();
  m_points.push_back(
      glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z));
  m_points.push_back(
      glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z));
  m_points.push_back(
      glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z));
  m_points.push_back(
      glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z));

  m_points.push_back(
      glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z));
  m_points.push_back(
      glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z));
  m_points.push_back(
      glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z));
  m_points.push_back(
      glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z));
  // deal with up and down
  glm::vec3 up_normal(0, 1, 0);
  Vertex vertex;
  vertex.position = m_points[0];
  vertex.normal = up_normal;
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[1];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[2];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[3];
  vertex.texcoords = {0.0f, 1.0f};
  mesh.vertices.push_back(vertex);
  mesh.indices.push_back(0);
  mesh.indices.push_back(1);
  mesh.indices.push_back(2);
  mesh.indices.push_back(2);
  mesh.indices.push_back(3);
  mesh.indices.push_back(0);
  // down
  vertex.position = m_points[4];
  vertex.normal = -up_normal;
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[5];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[6];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[7];
  vertex.texcoords = {0.0f, 1.0f};
  mesh.vertices.push_back(vertex);
  mesh.indices.push_back(4);
  mesh.indices.push_back(5);
  mesh.indices.push_back(6);
  mesh.indices.push_back(6);
  mesh.indices.push_back(7);
  mesh.indices.push_back(4);
  // left
  glm::vec3 left_normal(-1, 0, 0);
  vertex.normal = left_normal;
  vertex.position=m_points[0];
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[4];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[5];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[1];
  vertex.texcoords = {0.0f, 1.0f};
  mesh.vertices.push_back(vertex);
  mesh.indices.push_back(8);
  mesh.indices.push_back(9);
  mesh.indices.push_back(10);
  mesh.indices.push_back(10);
  mesh.indices.push_back(11);
  mesh.indices.push_back(8);
  //right
  vertex.normal = -left_normal;
  vertex.position=m_points[3];
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[7];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[6];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[2];
  vertex.texcoords = {0.0f, 1.0f};
  mesh.vertices.push_back(vertex);
  mesh.indices.push_back(12);
  mesh.indices.push_back(13);
  mesh.indices.push_back(14);
  mesh.indices.push_back(14);
  mesh.indices.push_back(15);
  mesh.indices.push_back(12);
  //forward
	glm::vec3 forward_normal(0,0,1);
  vertex.normal = forward_normal;
  vertex.position=m_points[1];
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[5];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[6];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[2];
  vertex.texcoords = {0.0f, 1.0f};
  mesh.vertices.push_back(vertex);
  mesh.indices.push_back(16);
  mesh.indices.push_back(17);
  mesh.indices.push_back(18);
  mesh.indices.push_back(18);
  mesh.indices.push_back(19);
  mesh.indices.push_back(16);
  //backward
    //forward
  vertex.normal =-forward_normal;
  vertex.position=m_points[0];
  vertex.texcoords = {0.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[4];
  vertex.texcoords = {1.0f, 0.0f};
  mesh.vertices.push_back(vertex);
  vertex.position = m_points[7];
  vertex.texcoords = {1.0f, 1.0f};

  mesh.vertices.push_back(vertex);
  vertex.position = m_points[3];
  vertex.texcoords = {0.0f, 1.0f};
    mesh.vertices.push_back(vertex);
  mesh.indices.push_back(20);
  mesh.indices.push_back(21);
  mesh.indices.push_back(22);
  mesh.indices.push_back(22);
  mesh.indices.push_back(23);
  mesh.indices.push_back(20);
  mesh.texture_id=load_texture("resources\\container2.png");
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
glm::vec3 Box::support(const glm::vec3 &dir, const glm::vec3 &pos,
                       const glm::quat &orient, const float bias) const {
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

Bounds Box::getBounds(const glm::vec3 &pos, const glm::quat &orient) const {
  glm::vec3 corners[8];
  // the sequence of corners is the different with the book code, so bug may
  // exist here
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

Bounds Box::getBounds() const { return m_bounds; }

float Box::fastestLinearSpeed(const glm::vec3 &angularVelocity,
                              const glm::vec3 &dir) const {
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
