#ifndef SPHERE_H
#define SPHERE_H
#include "body.h"


constexpr const int sector_count = 32;
constexpr const int stack_count = 32;
constexpr const float sectorstep = 2 * PI / sector_count;
constexpr const float stackstep = PI / stack_count;
class Sphere :public Model {
public:
	virtual unsigned int Get_Type()const {
		return SPHERE;
	}
	Sphere(float _radius):radius(_radius) {
		mass_center = glm::vec3(0,0,0);
		setup_Mesh();
		mesh.Set_VAO();
	}
	virtual void Draw() {
		mesh.Draw();
	}
	void setup_Mesh() {
		float sector_Angle, stack_Angle;
		glm::vec3 v;
		glm::vec3 n;
		glm::vec2 texcoords;
		// initialize the vertices
		for (int i = 0; i <= stack_count; i++) {
			stack_Angle = PI / 2 - i * stackstep;
			float xy_plane = radius * cos(stack_Angle);
			v[2] = radius * sin(stack_Angle);
			for (int j = 0; j <= sector_count; ++j) {
				sector_Angle = j * sectorstep;
				v[0] = xy_plane * cos(sector_Angle);
				v[1] = xy_plane * sin(sector_Angle);
				n[0] = v[0] / radius;
				n[1] = v[1] / radius;
				n[2] = v[2] / radius;
				texcoords[0] = ((float)j) / sector_count;
				texcoords[1] = ((float)i) / stack_count;
				Vertex vertex{ v,n,texcoords };
				mesh.vertices.push_back(vertex);
			}
		}
		int k1, k2;
		//get ebo;
		for (int i = 0; i < stack_count; ++i) {
			k1 = i * (sector_count + 1);
			k2 = k1 + sector_count + 1;

			for (int j = 0; j < sector_count; ++j) {
				if (i != 0) {
					mesh.indices.push_back(k1);
					mesh.indices.push_back(k2);
					mesh.indices.push_back(k1 + 1);

				}
				if (i != (stack_count)-1) {
					mesh.indices.push_back(k1 + 1);
					mesh.indices.push_back(k2);
					mesh.indices.push_back(k2 + 1);

				}
				k1++;
				k2++;
			}
		}


	}
	float radius;


};


#endif