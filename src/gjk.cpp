#include "gjk.h"
#include "utility.h"
glm::vec2 signedVolume1D(const glm::vec3 &s1, const glm::vec3 &s2) {
  glm::vec3 ab = s2 - s1;
  glm::vec3 ap = -s1;
  glm::vec3 p0 = s1 + dot(ab, ap) / dot(ab, ab) * ab;
  // choose the maximum difference axis
  unsigned int index = 0;
  float projection_max = 0;
  for (int i = 0; i < 3; i++) {
    float projection = s2[i] - s1[i];
    if (projection * projection > projection_max * projection_max) {
      projection_max = projection;
      index = i;
    }
  }
  // project according to the corrsponding axis
  float a = s1[index];
  float b = s2[index];
  float p = p0[index];
  float c1 = p - a;
  float c2 = b - p;
  if ((p > a && p < b) || (p > b && p < a)) {
    glm::vec2 lambda;
    lambda[0] = c2 / projection_max;
    lambda[1] = c1 / projection_max;
    return lambda;
  }
  if ((a <= b && p <= a) || (a >= b && p >= a)) {
    return glm::vec2(1, 0);
  }
  return glm::vec2(0, 1);
}
glm::vec3 signedVolume2D(const glm::vec3 &s1, const glm::vec3 &s2,
                         const glm::vec3 &s3) {
  glm::vec3 normal = glm::cross(s2 - s1, s3 - s1);
  glm::vec3 p0 = normal * dot(s1, normal) / dot(normal, normal);
  int index = 0;
  float projection_max = 0;
  for (int i = 0; i < 3; i++) {
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;
    glm::vec2 a(s1[j], s1[k]);
    glm::vec2 b(s2[j], s2[k]);
    glm::vec2 c(s3[j], s3[k]);
    glm::vec2 ab = b - a;
    glm::vec2 ac = c - a;
    float projection = ab.x * ac.y - ab.y * ac.x;
    if (projection * projection > projection_max) {
      index = i;
      projection_max = projection;
    }
  }
  int x = (index + 1) % 3;
  int y = (index + 2) % 3;
  glm::vec2 s[3];
  s[0] = glm::vec2(s1[x], s1[y]);
  s[1] = glm::vec2(s2[x], s2[y]);
  s[2] = glm::vec2(s3[x], s3[y]);
  glm::vec2 p = glm::vec2(p0[x], p0[y]);
  glm::vec3 areas;
  for (int i = 0; i < 3; i++) {
    int j = (i + 1) % 3;
    int k = (i + 2) % 3;
    glm::vec2 a = p;
    glm::vec2 b = s[j];
    glm::vec2 c = s[k];
    glm::vec2 ab = b - a;
    glm::vec2 ac = c - a;
    areas[i] = ab.x * ac.y - ab.y * ac.x;
  }
  for (int i = 0; i < 3; i++) {
    if (!CompareSigns(projection_max, areas[i])) {
      break;
    }

    if (i == 2) {
      glm::vec3 lambdas = areas / projection_max;
      return lambdas;
    }
  }
  float dist = 1e10;
  glm::vec3 lambdas(1, 0, 0);
  for (int i = 0; i < 3; i++) {
    int k = (i + 1) % 3;
    int l = (i + 2) % 3;
    glm::vec3 edgesPts[3];
    edgesPts[0] = s1;
    edgesPts[1] = s2;
    edgesPts[2] = s3;
    glm::vec2 lambdaEdge = signedVolume1D(edgesPts[k], edgesPts[l]);
    glm::vec3 pt = edgesPts[k] * lambdaEdge[0] + edgesPts[l] * lambdaEdge[1];
    if (dot(pt, pt) < dist) {
      dist = dot(pt, pt);
      lambdas[i] = 0;
      lambdas[k] = lambdaEdge[0];
      lambdas[l] = lambdaEdge[1];
    }
  }
  return lambdas;
}
glm::vec4 signedVolume3D(const glm::vec3 &s1, const glm::vec3 &s2,
                         const glm::vec3 &s3, const glm::vec3 &s4) {
  glm::mat4 M;
  for (int i = 0; i < 3; i++) {
    M[0][i] = s1[i];
    M[1][i] = s2[i];
    M[2][i] = s3[i];
    M[3][i] = s4[i];
  }
  M[0][3] = 1.0f;
  M[1][3] = 1.0f;
  M[2][3] = 1.0f;
  M[3][3] = 1.0f;
  float det_M = glm::determinant(M);
  glm::vec4 C4;
  C4.x = get_Cofactor(M, 3, 0);
  C4.y = get_Cofactor(M, 3, 1);
  C4.z = get_Cofactor(M, 3, 2);
  C4.w = get_Cofactor(M, 3, 3);
  float test_det = C4[0] + C4[1] + C4[2] + C4[3];
  if (CompareSigns(det_M, C4[0]) && CompareSigns(det_M, C4[1]) &&
      CompareSigns(det_M, C4[2]) && CompareSigns(det_M, C4[3])) {
    glm::vec4 lambdas = C4 * (1.0f / det_M);

    return lambdas;
  }
  glm::vec4 lambdas(0, 0, 0, 0);
  float dist = 1e10;
  for (int i = 0; i < 4; i++) {
    int j = (i + 1) % 4;
    int k = (i + 2) % 4;
    glm::vec3 facePts[4];
    facePts[0] = s1;
    facePts[1] = s2;
    facePts[2] = s3;
    facePts[3] = s4;
    glm::vec3 lambdasFace = signedVolume2D(facePts[i], facePts[j], facePts[k]);
    glm::vec3 pt = facePts[i] * lambdasFace[0] + facePts[j] * lambdasFace[1] +
                   facePts[k] * lambdasFace[2];
    if (dot(pt, pt) < dist) {
      dist = dot(pt, pt);
      lambdas = glm::vec4(0, 0, 0, 0);
      lambdas[i] = lambdasFace[0];
      lambdas[j] = lambdasFace[1];
      lambdas[k] = lambdasFace[2];
    }
  }
  return lambdas;
}
bool CompareSigns(float a, float b) { return a * b > 0; }
void test() {
  const glm::vec3 orgPts[4] = {
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(0, 0, 1),
  };
  glm::vec3 pts[4];
  glm::vec4 lambdas;
  glm::vec3 v(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    pts[i] = orgPts[i] + glm::vec3(1, 1, 1);
  }
  lambdas = signedVolume3D(pts[0], pts[1], pts[2], pts[3]);

  for (int i = 0; i < 4; i++) {
    v += pts[i] * lambdas[i];
  }
  printf("lambda:%.3f %.3f %.3f%.3f, v:%.3f %.3f %.3f  \n", lambdas.x,
         lambdas.y, lambdas.z, lambdas.w, v.x, v.y, v.z);
  for (int i = 0; i < 4; i++) {
    pts[i] = orgPts[i] + glm::vec3(-1, -1, -1) * 0.25f;
  }
  lambdas = signedVolume3D(pts[0], pts[1], pts[2], pts[3]);
  v = glm::vec3(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    v += pts[i] * lambdas[i];
  }
  printf(" lambdas:%.3f %.3f %.3f %.3f  v:%.3f %.3f %.3f  \n ", lambdas.x,
         lambdas.y, lambdas.z, lambdas.w, v.x, v.y, v.z);
  for (int i = 0; i < 4; i++) {
    pts[i] = orgPts[i] + glm::vec3(-1, -1, -1);
  }
  lambdas = signedVolume3D(pts[0], pts[1], pts[2], pts[3]);
  v = glm::vec3(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    v += pts[i] * lambdas[i];
  }
  printf(" lambdas:%.3f%.3f%.3f%.3f v:%.3f %.3f %.3f \n ", lambdas.x, lambdas.y,
         lambdas.z, lambdas.w, v.x, v.y, v.z);
  for (int i = 0; i < 4; i++) {
    pts[i] = orgPts[i] + glm::vec3(1, 1, -0.5f);
  }
  lambdas = signedVolume3D(pts[0], pts[1], pts[2], pts[3]);
  v = glm::vec3(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    v += pts[i] * lambdas[i];
  }
  printf(" lambdas:%.3f%.3f%.3f%.3f v:%.3f%.3f%.3f \n ", lambdas.x, lambdas.y,
         lambdas.z, lambdas.w, v.x, v.y, v.z);
  pts[0] = glm::vec3(51.1996613f, 26.1989613f, 1.91339576f);
  pts[1] = glm::vec3(-51.0567360f, -26.0565681f, -0.436143428f);
  pts[2] = glm::vec3(50.8978920f, -24.1035538f, -1.04042661f);
  pts[3] = glm::vec3(-49.1021080f, 25.8964462f, -1.04042661f);
  lambdas = signedVolume3D(pts[0], pts[1], pts[2], pts[3]);
  v = glm::vec3(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    v += pts[i] * lambdas[i];
  }
  printf("lambdas:%.3f%.3f%.3f%.3f v:%.3f%.3f%.3f \n ", lambdas.x, lambdas.y,
         lambdas.z, lambdas.w, v.x, v.y, v.z);
}
Point_t get_support(const Body &bodyA, const Body &bodyB, glm::vec3 &dir,
                    float bias) {
  Point_t point;
  if (glm::length(dir) != 0) {
    dir = glm::normalize(dir);
  }
  point.point_A =
      bodyA.shape->support(dir, bodyA.m_position, bodyA.m_rotation, bias);
  point.point_B =
      bodyB.shape->support(-dir, bodyB.m_position, bodyB.m_rotation, bias);
  point.xyz = point.point_A - point.point_B;
  return point;
}
bool SimplexSignedVolumes(const std::vector<Point_t> &pts, int num,
                          glm::vec3 &new_dir, glm::vec4 &lambda_out) {
  float epsilon = 1e-8;
  lambda_out = glm::vec4(0, 0, 0, 0);
  bool Intersect = false;
  if (num == 2) {
    glm::vec2 lambdas = signedVolume1D(pts[0].xyz, pts[1].xyz);
    glm::vec3 v(0, 0, 0);
    for (int i = 0; i < 2; i++) {
      v += pts[i].xyz * lambdas[i];
    }
    new_dir = v * -1.0f;
    Intersect = dot(v, v) < epsilon;
    lambda_out[0] = lambdas[0];
    lambda_out[1] = lambdas[1];
  } else if (num == 3) {
    glm::vec3 lambda = signedVolume2D(pts[0].xyz, pts[1].xyz, pts[2].xyz);
    glm::vec3 v(0.0f);
    for (int i = 0; i < 3; i++) {
      v += pts[i].xyz * lambda[i];
    }
    new_dir = v * -1.0f;
    Intersect = dot(v, v) < epsilon;
    lambda_out[0] = lambda[0];
    lambda_out[1] = lambda[1];
    lambda_out[2] = lambda[2];
  } else if (num == 4) {
    glm::vec4 lambda =
        signedVolume3D(pts[0].xyz, pts[1].xyz, pts[2].xyz, pts[3].xyz);
    glm::vec3 v(0.0f);
    for (int i = 0; i < 4; i++) {
      v += pts[i].xyz * lambda[i];
    }
    new_dir = v * -1.0f;
    Intersect = dot(v, v) < epsilon;
    for (int i = 0; i < 4; i++) {
      lambda_out[i] = lambda[i];
    }
  }
  return Intersect;
}
bool HasPoint(const std::vector<Point_t> &simplexPoints,
              const Point_t &new_pt) {
  float epsilon = 1e-6f;
  for (int i = 0; i < 4; i++) {
    glm::vec3 delta = simplexPoints[i].xyz;
    if (dot(delta, delta) < epsilon) {
      return true;
    }
  }
  return false;
}
void SortValids(std::vector<Point_t> &simplexPoints, glm::vec4 &lambdas) {
  bool valids[4];
  for (int i = 0; i < 4; i++) {
    valids[i] = true;
    if (lambdas[i] == 0.0f) {
      valids[i] = false;
    }
  }
  glm::vec4 validLambdas(0.0f);
  int validCount = 0;
  std::vector<Point_t> validPts(4);
  for (int i = 0; i < 4; i++) {
    if (valids[i]) {
      validPts[validCount] = simplexPoints[i];
      validLambdas[validCount] = lambdas[i];
      validCount++;
    }
  }
  for (int i = 0; i < 4; i++) {
    simplexPoints[i] = validPts[i];
    lambdas[i] = validLambdas[i];
  }
}
int NumValids(const glm::vec4 &lambdas) {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (0.0f != lambdas[i]) {
      count++;
    }
  }
  return count;
}
bool GJK_Intersect(const Body &bodyA, const Body &bodyB) {
  glm::vec3 origin(0.0f);
  int numPts = 1;
  std::vector<Point_t> simplexPoints(4);
  glm::vec3 dir(1, 1, 1);
  simplexPoints[0] = get_support(bodyA, bodyB, dir, 0.0f);
  float closetDist = 1e10f;
  bool ContainOrigin = false;
  glm::vec3 new_dir = simplexPoints[0].xyz * -1.0f;
  while (!ContainOrigin) {
    Point_t new_Pt = get_support(bodyA, bodyB, new_dir, 0.0f);
    // if same then break
    if (HasPoint(simplexPoints, new_Pt)) {
      break;
    }
    simplexPoints[numPts] = new_Pt;
    numPts++;
    float dir_dot = dot(new_Pt.xyz - origin, new_dir);
    if (dir_dot < 0.0f) {
      break;
    }
    glm::vec4 lambdas;
    ContainOrigin =
        SimplexSignedVolumes(simplexPoints, numPts, new_dir, lambdas);
    if (ContainOrigin) {
      break;
    }
    float dist = dot(new_dir, new_dir);
    if (dist >= closetDist) {
      break;
    }
    closetDist = dist;
    SortValids(simplexPoints, lambdas);
    numPts = NumValids(lambdas);
    if (numPts == 4) {
      ContainOrigin = true;
      break;
    }
  }

  return ContainOrigin;
}
void GJK_Closest_Points(const Body &BodyA, const Body &BodyB, glm::vec3 &pt_A,
                        glm::vec3 pt_B) {
  glm::vec3 origin(0);
  float closetDist = 1e10f;
  float bias = 0.0f;
  int numPts = 1;
  std::vector<Point_t> simplexPoints(4);
  glm::vec3 dir(1, 1, 1);
  simplexPoints[0] = get_support(BodyA, BodyB, dir, bias);
  glm::vec4 lambdas(1, 0, 0, 0);
  glm::vec3 new_dir = simplexPoints[0].xyz * (-1.0f);
  while (numPts < 4) {
    Point_t new_Pt = get_support(BodyA, BodyB, new_dir, bias);
    if (HasPoint(simplexPoints, new_Pt)) {
      break;
    }
    simplexPoints[numPts] = new_Pt;
    numPts++;
    SimplexSignedVolumes(simplexPoints, numPts, new_dir, lambdas);
    numPts = NumValids(lambdas);
    float dist = dot(new_dir, new_dir);
    if (dist >= closetDist) {
      break;
    }
    closetDist = dist;
  }
  pt_A = glm::vec3(0);
  pt_B = glm::vec3(0);
  for (int i = 0; i < 4; i++) {
    pt_A += simplexPoints[i].point_A * lambdas[i];
    pt_B += simplexPoints[i].point_B * lambdas[i];
  }
}