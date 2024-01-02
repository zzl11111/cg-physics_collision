#include "utility.h"

class Bounds {
public:
	Bounds() { clear(); }
	Bounds(const Bounds &rhs) : mins(rhs.mins), maxs(rhs.maxs) {}
	const Bounds & operator = (const Bounds & rhs);
	~Bounds() {}

	void clear() { mins = glm::vec3(1e6); maxs = glm::vec3(-1e6); }
	bool doesIntersect(const Bounds &rhs) const;
	void expand(const glm::vec3 * pts, const int num);
	void expand(const glm::vec3 &rhs);
	void expand(const Bounds & rhs);
	float widthX() const { return maxs.x - mins.x; }
	float widthY() const { return maxs.y - mins.y; }
	float widthZ() const { return maxs.z - mins.z; }

public:
	glm::vec3 mins;
	glm::vec3 maxs;
};