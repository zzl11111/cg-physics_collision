#pragma once
#include <vector>
#include "body.h"
struct Body_t {
  int id;
  float value;
  bool is_min;
};
class CollisionPair_t {
public:
  bool operator==(const CollisionPair_t &rhs) const{
    return ((a==rhs.a)&&(b==rhs.b))||((a==rhs.b)&&(b==rhs.a));
  }
  bool operator!=(const CollisionPair_t &rhs) const{
    return !(*this==rhs);
  }

public:
  int a, b;
};
void Build_Pairs(std::vector<CollisionPair_t> &collisionPairs,
                 const std::vector<Body_t> &bodys_t,const int num);
void SortBodiesBounds(const std::vector<Body>& bodies,const int num,std::vector<Body_t>&sorted_bodys_t,float dt);
bool Compare_SAP(const Body_t &body1,const Body_t &body2);
void SweepAndPrune1D(const std::vector<Body>&bodies,const int num,std::vector<CollisionPair_t>&pairs,float dt );
void BroadPhase(const std::vector<Body>&bodies,const int num,std::vector<CollisionPair_t>&pairs,float dt );