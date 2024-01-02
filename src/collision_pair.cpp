#include "collision_pair.h"
#include <algorithm>
bool Compare_SAP(const Body_t &body1, const Body_t &body2) {
  return body1.value < body2.value;
}
void Build_Pairs(std::vector<CollisionPair_t> &collisionPairs,
                 const std::vector<Body_t> &sorted_bodys_t, const int num) {
  collisionPairs.clear();
  for (int i = 0; i < num * 2; i++) {
    auto &body_t = sorted_bodys_t[i];
    if (!body_t.is_min) {
      continue;
    }
    CollisionPair_t pair;
    pair.a = body_t.id;
    for (int j = i + 1; j < num * 2; j++) {
      auto &body_t_2 = sorted_bodys_t[j];

      if (body_t_2.id == body_t.id) {
        break;
      }
      if (!body_t_2.is_min) {
        continue;
      }
      pair.b = body_t_2.id;
      collisionPairs.push_back(pair);
    }
  }
}
void SortBodiesBounds(const std::vector<Body> &bodies, const int num,
                      std::vector<Body_t> &sorted_bodys_t, float dt) {
  glm::vec3 axis(1, 1, 1);
  axis = glm::normalize(axis);
  for (int i = 0; i < num; i++) {
    const auto &body = bodies[i];
    Bounds bounds = body.shape->getBounds(body.m_position, body.m_rotation);
    bounds.expand(bounds.mins + body.m_linear_velocity * dt);
    bounds.expand(bounds.maxs + body.m_linear_velocity * dt);
    bounds.expand(bounds.mins + axis * epsilon);
    bounds.expand(bounds.maxs + axis * epsilon);
    Body_t body_min;
    body_min.id = i;
    body_min.value = dot(axis, bounds.mins);
    body_min.is_min = true;
    sorted_bodys_t.push_back(body_min);
    Body_t body_max;
    body_max.id = i;
    body_max.value = dot(axis, bounds.maxs);
    body_max.is_min = false;
     sorted_bodys_t.push_back(body_max);
  }
  std::sort(sorted_bodys_t.begin(), sorted_bodys_t.end(), Compare_SAP);
}
void SweepAndPrune1D(const std::vector<Body>&bodies,const int num,std::vector<CollisionPair_t>&pairs,float dt ){
    std::vector<Body_t>bodys_t;
SortBodiesBounds(bodies,  num, bodys_t,  dt);
Build_Pairs(pairs,bodys_t,num);
}

void BroadPhase(const std::vector<Body> &bodies, const int num,
                std::vector<CollisionPair_t> &pairs, float dt){
pairs.clear();
SweepAndPrune1D(bodies,num,pairs,dt);
}