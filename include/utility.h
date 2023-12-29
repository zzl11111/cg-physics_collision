#ifndef UTILITY_H
#define UTILITY_H
//#include <random>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>
const float PI = 3.1415926f;
inline float degrees_to_radians(float degrees) {
	return degrees * PI / 180;
}

inline float clamp(float x, float min, float max) {
	if (x < min)return min;
	if (x > max)return max;
	return x;
}
inline float radian_to_degree(float radian) {
	return radian * 180 / PI;
}
#endif