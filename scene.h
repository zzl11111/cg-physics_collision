#ifndef SCENE_H
#define SCENE_H
#include"body.h"
#include "utility.h"
#include "sphere.h"
glm::vec3 gravity(0, -0.5, 0);
class Scene {
public:
	Scene() {
		Create_Scene();

	}
	void Create_Scene() {
		int num_objs = 1;
		Body obj(std::make_shared<Sphere>(0.2), vec3(0, 0, 0), vec3(0, 0, 0), quat(1, 0, 0, 0), 1);
		objs.push_back(obj);
	}
	void Update(float delta_time) {
		for (auto &obj : objs) {
			obj.m_velocity += gravity * delta_time;
			obj.m_position += obj.m_velocity * delta_time;

		}

	}
	void Draw(const Shader_Program &shader_prog) {
		
		for (auto& obj : objs) {
			glm::mat4 model = obj.get_model_matrix();
			shader_prog.setMat4("model", model);
			obj.shape->Draw();
		}
	}
	std::vector<Body> objs;

};
#endif