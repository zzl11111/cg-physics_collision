#include "contact.h"
#include "utility.h"
#include "body.h"
#include <vector>
#include "gjk.h"

glm::vec3 barycentricCoordinates(glm::vec3 s1, glm::vec3 s2, glm::vec3 s3, const glm::vec3 &p);

glm::vec3 normalDirection(const Triangle_t & triangle, const std::vector<Point_t> & points);

float signedDistanceToTriangle(const Triangle_t & triangle, const glm::vec3 pt, const std::vector<Point_t> & points);

int closestTriangle(const std::vector<class Triangle_t> & traingles, const std::vector<Point_t> & points);

bool hasPoint(const glm::vec3 & w, const std::vector<Triangle_t> & triangles, const std::vector<Point_t> & points);

int removeTriangleFacingPoint(const glm::vec3 & pt, std::vector<Triangle_t> & triangles, const std::vector<Point_t> & points);

void findDanglingEdges(std::vector<Edge_t> & danglingEdges, const std::vector<Triangle_t> & triangles);

float EPA_expand(const Body * bodyA, const Body * bodyB, const float bias, const Point_t simplexPoints[4], glm::vec3 & ptOnA, glm::vec3 & ptOnB);

bool GJK_doesIntersect(const Body * bodyA, const Body * bodyB, const float bias, glm::vec3 & ptOnA, glm::vec3 & ptOnB);

bool conservativeAdvance(Body * bodyA, Body * bodyB, float dt, Contact & contact);