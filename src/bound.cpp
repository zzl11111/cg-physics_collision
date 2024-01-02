#include "bound.h"

bool Bounds::doesIntersect(const Bounds &rhs) const {
	if (maxs.x < rhs.mins.x || maxs.y < rhs.mins.y || maxs.z < rhs.mins.z)
		return false;
	if (mins.x > rhs.maxs.x || mins.y > rhs.maxs.y || mins.z > rhs.maxs.z)
		return false;
	return true;
}

void Bounds::expand(const glm::vec3 * pts, const int num) {
	for (int i = 0; i < num; i++) {
		expand((pts[i]));
	}
}

void Bounds::expand(const glm::vec3 & rhs) {
	if (rhs.x < mins.x) mins.x = rhs.x;
	if (rhs.y < mins.y) mins.y = rhs.y;
	if (rhs.z < mins.z) mins.z = rhs.z;
	if (rhs.x > maxs.x) maxs.x = rhs.x;
	if (rhs.y > maxs.y) maxs.y = rhs.y;
	if (rhs.z > maxs.z) maxs.z = rhs.z;
}

void Bounds::expand(const Bounds & rhs) {
	expand(rhs.mins);
	expand(rhs.maxs);
}