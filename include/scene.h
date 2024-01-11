#pragma once

#include "body.h"
#include "collision_pair.h"
#include "contact.h"
#include "ray.h"
#include "scene.h"
#include "shader_program.h"
#include "sphere.h"
#include "utility.h"
#include <algorithm>
#include "collision_pair.h"
#include "box.h"
#include "gjk.h"
#include "EPA.h"
inline bool check_collision(Body &body1, Body &body2, Contact &contact, float dt);
inline void Process_collision(Contact &contact);

class Scene {
public:
  Scene() { Create_Scene(); }
  void Create_Scene();
  void Update(float delta_time);
  void Draw(const Shader &shader_prog);

private:
  std::vector<Body> objs;
  glm::vec3 gravity = glm::vec3(0, -100.0f, 0);
};

