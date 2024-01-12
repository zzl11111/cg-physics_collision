#include "EPA.h"
#include "body.h"
#include "gjk.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "scene.h"
#include "utility.h"
#include <cmath>
#include <vector>

glm::vec3 barycentricCoordinates(glm::vec3 s1, glm::vec3 s2, glm::vec3 s3,
                                 const glm::vec3 &p) {
  s1 = s1 - p;
  s2 = s2 - p;
  s3 = s3 - p;

  glm::vec3 normal = glm::cross(s2 - s1, s3 - s1);
  glm::vec3 p0 = normal * glm::dot(normal, s1) / glm::dot(normal, normal);

  // Find the axis with the greatest projected area
  int index = 0;
  float area_max = 0;
  for (int i = 0; i < 3; i++) {
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;

    glm::vec2 a = glm::vec2(s1[j], s1[k]);
    glm::vec2 b = glm::vec2(s2[j], s2[k]);
    glm::vec2 c = glm::vec2(s3[j], s3[k]);
    glm::vec2 a2b = b - a;
    glm::vec2 a2c = c - a;

    float area = a2b.x * a2c.y - a2b.y * a2c.x;
    if (area * area > area_max * area_max) {
      area_max = area;
      index = i;
    }
  }

  // project onto the axis with the greatest projected area
  int x = (index + 1) % 3;
  int y = (index + 2) % 3;
  glm::vec2 s[3];
  s[0] = glm::vec2(s1[x], s1[y]);
  s[1] = glm::vec2(s2[x], s2[y]);
  s[2] = glm::vec2(s3[x], s3[y]);
  glm::vec2 p2 = glm::vec2(p0[x], p0[y]);

  // Get the sub_areas of the triangles formed by the projected point and the
  // vertices of the triangle
  glm::vec3 areas;
  for (int i = 0; i < 3; i++) {
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;

    glm::vec2 a = p2;
    glm::vec2 b = s[j];
    glm::vec2 c = s[k];
    glm::vec2 a2b = b - a;
    glm::vec2 a2c = c - a;

    areas[i] = a2b.x * a2c.y - a2b.y * a2c.x;
  }

  glm::vec3 lambda = areas / area_max;
  if (lambda.x * 0.0f != lambda.x * 0.0f ||
      lambda.y * 0.0f != lambda.y * 0.0f ||
      lambda.z * 0.0f != lambda.z * 0.0f) {
    return glm::vec3(1, 0, 0);
  }
  return lambda;
}

// normalDirection
glm::vec3 normalDirection(const Triangle_t &triangle,
                          const std::vector<Point_t> &points) {
  const glm::vec3 a = points[triangle.a].xyz;
  const glm::vec3 b = points[triangle.b].xyz;
  const glm::vec3 c = points[triangle.c].xyz;
  const glm::vec3 normal = glm::cross(b - a, c - a);
  return glm::normalize(normal);
}

// signedDistanceToTriangle
float signedDistanceToTriangle(const Triangle_t &triangle, const glm::vec3 p,
                               const std::vector<Point_t> &points) {
  const glm::vec3 normal = normalDirection(triangle, points);
  const glm::vec3 &a = points[triangle.a].xyz;
  const glm::vec3 a2p = p - a;
  return dot(normal, a2p);
}

// closestTriangle
int closestTriangle(const std::vector<class Triangle_t> &triangles,
                    const std::vector<Point_t> &points) {
  float min_distance = 1e10;
  int min_index = -1;
  for (int i = 0; i < triangles.size(); i++) {
    float distance =
        signedDistanceToTriangle(triangles[i], glm::vec3(0, 0, 0), points);
    if (distance * distance < min_distance * min_distance) {
      min_distance = distance;
      min_index = i;
    }
  }
  return min_index;
}

// hasPoint
bool hasPoint(const glm::vec3 &w, const std::vector<Triangle_t> &triangles,
              const std::vector<Point_t> &points) {
  const float epsilon = 1e-6;
  glm::vec3 delta;

  for (int i = 0; i < triangles.size(); i++) {
    const Triangle_t &triangle = triangles[i];

    delta = w - points[triangle.a].xyz;
    if (glm::dot(delta, delta) < epsilon) {
      return true;
    }
    delta = w - points[triangle.b].xyz;
    if (glm::dot(delta, delta) < epsilon) {
      return true;
    }
    delta = w - points[triangle.c].xyz;
    if (glm::dot(delta, delta) < epsilon) {
      return true;
    }
  }
  return false;
}

// removeTriangleFacingPoint
int removeTriangleFacingPoint(const glm::vec3 &pt,
                              std::vector<Triangle_t> &triangles,
                              const std::vector<Point_t> &points) {
  int removed = 0;
  for (int i = 0; i < triangles.size(); i++) {
    const Triangle_t &triangle = triangles[i];
    float dist = signedDistanceToTriangle(triangle, pt, points);
    if (dist > 0.0f) {
      triangles.erase(triangles.begin() + i);
      removed++;
      i--;
    }
  }
  return removed;
}

// findDanglingEdges
void findDanglingEdges(std::vector<Edge_t> &danglingEdges,
                       const std::vector<Triangle_t> &triangles) {
  danglingEdges.clear();
  for (int i = 0; i < triangles.size(); i++) {
    const Triangle_t &triangle = triangles[i];
    Edge_t edge[3];
    edge[0].a = triangle.a;
    edge[0].b = triangle.b;
    edge[1].a = triangle.b;
    edge[1].b = triangle.c;
    edge[2].a = triangle.c;
    edge[2].b = triangle.a;

    int counts[3] = {0, 0, 0};

    for (int j = 0; j < triangles.size(); j++) {
      if (i == j) {
        continue;
      }
      const Triangle_t &other = triangles[j];
      Edge_t other_edge[3];
      other_edge[0].a = other.a;
      other_edge[0].b = other.b;
      other_edge[1].a = other.b;
      other_edge[1].b = other.c;
      other_edge[2].a = other.c;
      other_edge[2].b = other.a;

      for (int k = 0; k < 3; k++) {
        if (edge[k] == other_edge[0]) {
          counts[k]++;
        }
        if (edge[k] == other_edge[1]) {
          counts[k]++;
        }
        if (edge[k] == other_edge[2]) {
          counts[k]++;
        }
      }
    }
    for (int k = 0; k < 3; k++) {
      if (counts[k] == 0) {
        danglingEdges.push_back(edge[k]);
      }
    }
  }
}

// EPA_expand
float EPA_expand(const Body *bodyA, const Body *bodyB, const float bias,
                 std::vector<Point_t> &simplexPoints, glm::vec3 &ptOnA,
                 glm::vec3 &ptOnB) {
  std::vector<Point_t> points;
  std::vector<Triangle_t> triangles;
  std::vector<Edge_t> danglingEdges;

  glm::vec3 center(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    points.push_back(simplexPoints[i]);
    center += simplexPoints[i].xyz;
  }
  center /= 4.0f;

  // Build the triangls
  for (int i = 0; i < 4; i++) {
    int j = (i + 1) % 4;
    int k = (i + 2) % 4;
    Triangle_t triangle;
    triangle.a = i;
    triangle.b = j;
    triangle.c = k;

    int unusedPt = (i + 3) % 4;
    float dist =
        signedDistanceToTriangle(triangle, points[unusedPt].xyz, points);

    // The unused point is always on the inside of the triangle
    if (dist > 0.0f) {
      std::swap(triangle.a, triangle.b);
    }
    triangles.push_back(triangle);
  }

  // Expand the simplex to find the closest face of the CSO to the origin
  while (1) {
    const int index = closestTriangle(triangles, points);
    glm::vec3 normal = normalDirection(triangles[index], points);

    const Point_t newPoint = get_support(*bodyA, *bodyB, normal, bias);

    if (hasPoint(newPoint.xyz, triangles, points)) {
      break;
    }

    float dist =
        signedDistanceToTriangle(triangles[index], newPoint.xyz, points);
    if (dist < 1e-6) {
      break;
    }

    const int newIndex = (int)points.size();
    points.push_back(newPoint);

    // Remove triangles facing the new point
    int numRemoved = removeTriangleFacingPoint(newPoint.xyz, triangles, points);
    if (numRemoved == 0) {
      break;
    }

    // Find the dangling edges
    danglingEdges.clear();
    findDanglingEdges(danglingEdges, triangles);
    if (danglingEdges.size() == 0) {
      break;
    }

    // In theory the edges should be a proper CCW order
    // So we only need to add the new point as 'a' in order
    // to create new triangles that face away from the origin
    for (int i = 0; i < danglingEdges.size(); i++) {
      const Edge_t &edge = danglingEdges[i];

      Triangle_t triangle;
      triangle.a = newIndex;
      triangle.b = edge.a;
      triangle.c = edge.b;

      // Make sure it's oriented properly
      float dist = signedDistanceToTriangle(triangle, center, points);
      if (dist > 0.0f) {
        std::swap(triangle.b, triangle.c);
      }

      triangles.push_back(triangle);
    }
  }

  // Get the projection of the origin on the closet triangle
  const int index = closestTriangle(triangles, points);
  const Triangle_t &triangle = triangles[index];
  glm::vec3 ptA_w = points[triangle.a].xyz;
  glm::vec3 ptB_w = points[triangle.b].xyz;
  glm::vec3 ptC_w = points[triangle.c].xyz;
  glm::vec3 lambdas =
      barycentricCoordinates(ptA_w, ptB_w, ptC_w, glm::vec3(0, 0, 0));

  // Get the point on shape A
  glm::vec3 ptA_a = points[triangle.a].point_A;
  glm::vec3 ptB_a = points[triangle.b].point_A;
  glm::vec3 ptC_a = points[triangle.c].point_A;
  ptOnA = ptA_a * lambdas[0] + ptB_a * lambdas[1] + ptC_a * lambdas[2];

  // Get the point on shape B
  glm::vec3 ptA_b = points[triangle.a].point_B;
  glm::vec3 ptB_b = points[triangle.b].point_B;
  glm::vec3 ptC_b = points[triangle.c].point_B;
  ptOnB = ptA_b * lambdas[0] + ptB_b * lambdas[1] + ptC_b * lambdas[2];

  // Return the penetration depth
  glm::vec3 delta = ptOnA - ptOnB;
  return glm::length(delta);
}

// GJK_doesIntersect
bool GJK_doesIntersect(const Body *bodyA, const Body *bodyB, const float bias,
                       glm::vec3 &ptOnA, glm::vec3 &ptOnB) {
  const glm::vec3 origin(0, 0, 0);

  int numPoints = 1;
  std::vector<Point_t> simplexPoints(4);
  glm::vec3 temp = glm::vec3(1, 1, 1);
  simplexPoints[0] = get_support(*bodyA, *bodyB, temp, 0.0f);

  float closetDist = 1e10f;
  bool doesContainOrigin = false;
  glm::vec3 newDir = simplexPoints[0].xyz * -1.0f;
  while (!doesContainOrigin) {
    // Get the new point to check on
    Point_t newPoint = get_support(*bodyA, *bodyB, newDir, 0.0f);

    // If the noew point is the same as a previous point, then we can't expand
    // any further

    if (HasPoint(simplexPoints, newPoint)) {
	  break;
    }
    simplexPoints[numPoints] = newPoint;
    numPoints++;

    // If this new point hasn't moved passed the origin, then the
    // origin cannot be in the set. And therefore there is no collision.
    float dotdot = dot(newDir, newPoint.xyz - origin);
    if (dotdot < 0.0f) {
      break;
    }

    glm::vec4 lambdas;
    doesContainOrigin =
        SimplexSignedVolumes(simplexPoints, numPoints, newDir, lambdas);
    
	if (doesContainOrigin) {
	  break;
    }

    // Check that the new projection of the origin onto the simplex is closer
    // than the previous
    float dist = glm::dot(newDir, newDir);
    if (dist >= closetDist) {
      break;
    }
    closetDist = dist;

    // Use the lambdas that support thee new search direction, and invalidate
    // any points that dont't support it
    SortValids(simplexPoints, lambdas);
    numPoints = NumValids(lambdas);
    doesContainOrigin = numPoints == 4;
  }

  if (!doesContainOrigin) {
    return false;
  }

  // Check that we have a 3-simplex (EPA expects a tetrahedron)
  if (numPoints == 1) {
    glm::vec3 searchDir = simplexPoints[0].xyz * -1.0f;
    Point_t newPoint = get_support(*bodyA, *bodyB, searchDir, 0.0f);
    simplexPoints[numPoints] = newPoint;
    numPoints++;
  }
  if (numPoints == 2) {
    glm::vec3 ab = simplexPoints[1].xyz - simplexPoints[0].xyz;
    glm::vec3 u, v;
    [](const glm::vec3 &ab, glm::vec3 &u, glm::vec3 &v) {
      glm::vec3 n = glm::normalize(ab);
      const glm::vec3 w =
          (n.z * n.z > 0.9f * 0.9f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 0, 1);
      u = glm::cross(w, n);
      u = glm::normalize(u);
      v = glm::cross(n, u);
      v = glm::normalize(v);
    }(ab, u, v);

    glm::vec3 newDir = u;
    Point_t newPoint = get_support(*bodyA, *bodyB, newDir, 0.0f);
    simplexPoints[numPoints] = newPoint;
    numPoints++;
  }
  if (numPoints == 3) {
    glm::vec3 ab = simplexPoints[1].xyz - simplexPoints[0].xyz;
    glm::vec3 ac = simplexPoints[2].xyz - simplexPoints[0].xyz;
    glm::vec3 norm = glm::cross(ab, ac);

    glm::vec3 newDir = norm;
    Point_t newPoint = get_support(*bodyA, *bodyB, newDir, 0.0f);
    simplexPoints[numPoints] = newPoint;
    numPoints++;
  }

  // Expand the simplex by the bias amount

  glm::vec3 avg = glm::vec3(0, 0, 0);
  for (int i = 0; i < numPoints; i++) {
    avg += simplexPoints[i].xyz;
  }
  avg /= 4.0;

  for (int i = 0; i < numPoints; i++) {
    Point_t &pt = simplexPoints[i];

    glm::vec3 dir = pt.xyz - avg;
    glm::normalize(dir);
    pt.point_A += dir * bias;
    pt.point_B -= dir * bias;
    pt.xyz = pt.point_A - pt.point_B;
  }

  EPA_expand(bodyA, bodyB, bias, simplexPoints, ptOnA, ptOnB);
  return true;
}

bool check_collision(Body &body1, Body &body2, Contact &contact) {
  contact.A = &body1;
  contact.B = &body2;
  contact.time_of_impact = 0.0f;
  if (body1.shape->Get_Type() == SPHERE && body2.shape->Get_Type() == SPHERE) {
    const Sphere *sphereA = (const Sphere *)body1.shape;
    const Sphere *sphereB = (const Sphere *)body2.shape;
    glm::vec3 position_A = body1.m_position;
    glm::vec3 position_B = body2.m_position;
    if (Sphere_Sphere_Static(sphereA, sphereB, position_A, position_B,
                             contact.A_potential_collision_point_world_space,
                             contact.B_potential_collision_point_world_space)) {
      contact.normal = position_A - position_B;
      if (glm::length(contact.normal) != 0) {
        contact.normal = glm::normalize(contact.normal);
      }
      contact.A_potential_collision_point_local_space =
          body1.World_To_Local_space(
              contact.A_potential_collision_point_world_space);
      contact.B_potential_collision_point_local_space =
          body2.World_To_Local_space(
              contact.B_potential_collision_point_world_space);
      glm::vec3 ab = body2.m_position - body1.m_position;
      float r = glm::length(ab) - (sphereA->radius + sphereB->radius);
      contact.seperation_distance = r;
      return true;
    }

  } else {
    glm::vec3 pt_A;
    glm::vec3 pt_B;
    float bias = 0.001f;
    if (GJK_doesIntersect(contact.A, contact.B, bias, pt_A, pt_B)) {
      glm::vec3 norm = pt_B - pt_A;
      if (glm::length(norm) != 0) {
        norm = glm::normalize(norm);
      }
      pt_A -= norm * bias;
      pt_B += norm * bias;
      contact.normal = norm;
      contact.A_potential_collision_point_world_space = pt_A;
      contact.B_potential_collision_point_world_space = pt_B;
      // set the local
      contact.A_potential_collision_point_local_space =
          body1.World_To_Local_space(
              contact.A_potential_collision_point_world_space);
      contact.B_potential_collision_point_local_space =
          body2.World_To_Local_space(
              contact.B_potential_collision_point_world_space);
      glm::vec3 ab = body2.m_position - body1.m_position;
      float r = glm::length(pt_A - pt_B);
      contact.seperation_distance = -r;
      return true;
    }

    GJK_Closest_Points(body1, body2, pt_A, pt_B);
    contact.A_potential_collision_point_world_space = pt_A;
    contact.B_potential_collision_point_world_space = pt_B;
    // set the local
    contact.A_potential_collision_point_local_space =
        body1.World_To_Local_space(
            contact.A_potential_collision_point_world_space);
    contact.B_potential_collision_point_local_space =
        body2.World_To_Local_space(
            contact.B_potential_collision_point_world_space);
    glm::vec3 ab = body2.m_position - body1.m_position;
    float r = glm::length(pt_A - pt_B);

    contact.seperation_distance = r;
  }

  return false;
}

bool ConservativeAdvance(Body &bodyA, Body &bodyB, float dt, Contact &contact) {
  contact.A = &bodyA;
  contact.B = &bodyB;

  float toi = 0.0f;

  int numberOfIterations = 0;

  // Advance the positions of the bodies untill they touch or there is not time
  // left
  while (dt > 0.0f) {
    // Check for intersection
    bool didIntersect = check_collision(bodyA, bodyB, contact);
    if (didIntersect) {
      contact.time_of_impact = toi;
      bodyA.Update(-toi);
      bodyB.Update(-toi);

      return true;
    }

    numberOfIterations++;
    if (numberOfIterations > 10) {
      break;
    }

    // Get the vector from the closest point on A to the closest point on B
    glm::vec3 ab = contact.B_potential_collision_point_world_space -
                   contact.A_potential_collision_point_world_space;
    if(glm::length(ab)!=0){
	glm::normalize(ab);
	ab=glm::normalize(ab);
	}

    // project the relative velocity onto the ray of shortest distance
    glm::vec3 relativeVelocity =
        bodyA.m_linear_velocity - bodyB.m_linear_velocity;
    float orthoSpeed = dot(relativeVelocity, ab);

    // Add to the orthoSpeed the maximum angular speeds of the relative shapes
    float angularSpeedA =
        bodyA.shape->fastestLinearSpeed(bodyA.m_angular_velocity, ab);
    float angularSpeedB =
        bodyB.shape->fastestLinearSpeed(bodyB.m_angular_velocity, ab * -1.0f);
    orthoSpeed += angularSpeedA + angularSpeedB;
    if (orthoSpeed <= 0.0f) {
      break;
    }

    float timeToGo = contact.seperation_distance / orthoSpeed;
    if (timeToGo > dt) {
      break;
    }

    dt -= timeToGo;
    toi += timeToGo;
    bodyA.Update(timeToGo);
    bodyB.Update(timeToGo);
  }

  // unwind the clock
  bodyA.Update(-toi);
  bodyB.Update(-toi);
  return false;
}