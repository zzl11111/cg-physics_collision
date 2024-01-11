#pragma once
#include "body.h"
#include "utility.h"

glm::vec2 signedVolume1D(const glm::vec3 &s1, const glm::vec3 &s2);
bool CompareSigns(float a, float b);
// projection on 2d
glm::vec3 signedVolume2D(const glm::vec3 &s1, const glm::vec3 &s2,
                         const glm::vec3 &s3);
glm::vec4 signedVolume3D(const glm::vec3 &s1, const glm::vec3 &s2,
                         const glm::vec3 &s3, const glm::vec3 &s4);
void test();
class Point_t {
public:
  glm::vec3 xyz;
  glm::vec3 point_A;
  glm::vec3 point_B;
  Point_t() : xyz(glm::vec3(0, 0, 0)), point_A(0, 0, 0), point_B(0, 0, 0) {}
  bool operator==(const Point_t &rhs) const {
    return (xyz == rhs.xyz) && (point_A == rhs.point_B) &&
           (point_B == rhs.point_B);
  }
};
Point_t get_support(const Body &bodyA, const Body &bodyB, glm::vec3 dir,
                    float bias);
bool SimplexSignedVolumed(std::vector<Point_t> &pts, int num,
                          glm::vec3 &new_dir, glm::vec4 lambda_out);
bool HasPoint(const std::vector<Point_t> &simplexPoints, const Point_t &new_pt);