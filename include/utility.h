#pragma once

// #include <random>
#include <assert.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

const float PI = 3.1415926f;
inline float degrees_to_radians(float degrees) { return degrees * PI / 180; }

inline float clamp(float x, float min, float max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}
inline float radian_to_degree(float radian) { return radian * 180 / PI; }
inline glm::quat get_quat_from_n_and_angle(glm::vec3 n, float angle) {
  float half_theta = angle / 2;
  glm::quat q;
  if (angle == 0||glm::length(n)==0) {
      q.x = 0;
      q.y = 0;
      q.z = 0;
      q.w = 1;
      return q;
  }
  n = glm::normalize(n);
  q.w = cos(half_theta);
  q.x = n.x * sin(half_theta);
  q.y = n.y * sin(half_theta);
  q.z = n.z * sin(half_theta);
  return q;
}
inline std::ostream &operator<<(std::ostream &cout, glm::vec3 v) {
  cout << v.x << " " << v.y << " " << v.z << std::endl;
  return cout;
}
inline std::ostream &operator<<(std::ostream &cout, glm::vec4 v) {
  cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
  return cout;
}
inline std::ostream &operator<<(std::ostream &cout, glm::quat v) {
  cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
  return cout;
}
inline std::ostream &operator<<(std::ostream &cout, glm::mat3 mat) {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      cout << mat[i][j];
    }
    cout<<std::endl;
  }

  return cout;
}
inline std::ostream &operator<<(std::ostream &cout, glm::mat4 mat) {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      cout << mat[i][j];
    }
    cout<<std::endl;
  }

  return cout;
}