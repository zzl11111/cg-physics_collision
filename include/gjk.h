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
Point_t get_support(const Body &bodyA, const Body &bodyB, glm::vec3 &dir,
                    float bias);
bool SimplexSignedVolumes(std::vector<Point_t> &pts, int num,
                          glm::vec3 &new_dir, glm::vec4& lambda_out);
bool HasPoint(const std::vector<Point_t> &simplexPoints, const Point_t &new_pt);
void SortValids(std::vector<Point_t> &simplexPoints, glm::vec4 &lambdas);
int NumValids(const glm::vec4 &lambdas);
bool GJK_Intersect(const Body &bodyA, const Body &bodyB);