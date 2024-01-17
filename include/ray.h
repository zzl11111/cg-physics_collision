#pragma once
#include "utility.h"

class Ray {
public:
Ray(){
    
}
  Ray(glm::vec3 _o, glm::vec3 _d) : origin(_o), dir(_d) 
  {


  }
  glm::vec3 origin = glm::vec3(0, 0, 0);
  glm::vec3 dir = glm::vec3(0, 0, 0);
};
