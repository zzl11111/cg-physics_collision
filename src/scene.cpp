#include "scene.h"


void Scene::Create_Scene() {
    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        float radius = 2.0f;
        Body obj;
  		obj.shape = new Sphere(radius);

        float xx = float(x - 1) * radius * 2.0f;
        float yy = float(y - 1) * radius * 2.0f;
		obj.m_position = glm::vec3(xx, 1.5f, yy);
		obj.m_linear_velocity = glm::vec3(0, 0, 0);
		obj.m_angular_velocity = glm::vec3(0, 0, 0);
		obj.m_inv_mass = 1.0f;
		obj.elasticity = 0.5f;
		obj.m_friction = 0.5f;
		obj.m_rotation = glm::quat(1,0,0,0);
		objs.push_back(obj);
      }
    }
    Body ground(new Sphere(10000), glm::vec3(0, -10003, 0), glm::vec3(0, 0, 0),
                glm::quat(1, 0, 0, 0), 0);
    ground.elasticity = 0.9f;
    ground.m_friction = 1.03f;
    objs.push_back(ground);
}

void Scene::Update(float delta_time) {
    for (int i = 0; i < objs.size(); i++) {
      Body &body = objs[i];
      if (body.m_inv_mass == 0) { continue; }
      glm::vec3 impulse_gravity =
          gravity * (1.0f / body.m_inv_mass) * delta_time;
      body.Process_Linear_Impulse(impulse_gravity);
      // process the gravity
    }
    std::vector<CollisionPair_t> collisionPairs;
    BroadPhase(objs,objs.size(),collisionPairs,delta_time);
    int num_Contacts = 0;
    std::vector<Contact> contacts;
	for (auto &pair : collisionPairs){
		auto &body1=objs[pair.a];
		auto &body2=objs[pair.b];
		if(body1.m_inv_mass == 0 && body2.m_inv_mass == 0) { continue; }//find next pair
		Contact contact;
		bool collision=check_collision(body1,body2,contact,delta_time);
		if (collision) {
			contacts.push_back(contact);
		}
	}
	if (contacts.size() > 1) {
		std::sort(contacts.begin(), contacts.end(), CompareContacts);
	}
	float accumulated_Time=0.0f;
	for(auto &contact : contacts ){
		float dt = contact.time_of_impact-accumulated_Time;
		Body *A = contact.A;
		Body *B = contact.B;
		if (A->m_inv_mass == 0.0f && B->m_inv_mass == 0.0f) { continue; } // not resolve
		for (int j = 0; j < objs.size(); j++){
			objs[j].Update(dt);
		}
		Process_collision(contact);
		accumulated_Time += dt;
	}
	float timeRemaining = delta_time - accumulated_Time;
	if (timeRemaining > 0.0f) {
		for (auto &obj : objs) {
			obj.Update(timeRemaining);
		}
    }
}

void Scene::Draw(const Shader &shader_prog) {
    for (auto &obj : objs) {
		glm::mat4 model = obj.get_model_matrix();
		shader_prog.setMat4("model", model);
		obj.shape->Draw();
    }
}