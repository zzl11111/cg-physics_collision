#ifndef UTILITY_H
#define UTILITY_H
#include <random>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;
constexpr const float PI = 3.1415926f;
 inline  float random_float() {
	static std::uniform_real_distribution<float>  distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}
inline  float random_float(float min, float max) {
	static std::uniform_real_distribution<float> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

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