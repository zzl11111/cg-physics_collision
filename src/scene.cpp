#include "scene.h"
#include "sphere.h"

void Scene::Create_Scene() {
  // for (int x = 0; x < 6; x++) {
  //   for (int y = 0; y < 6; y++) {
  //     float radius = 2.0f;
  //     Body obj;
  // 	obj.shape = new Sphere(radius);

  //     float xx = float(x - 1) * radius * 2.0f;
  //     float yy = float(y - 1) * radius * 2.0f;
  // 	obj.m_position = glm::vec3(xx, 2.0f, yy);
  // 	obj.m_linear_velocity = glm::vec3(0, 0, 0);
  // 	obj.m_angular_velocity = glm::vec3(0, 0, 0);
  // 	obj.m_inv_mass = 1.0f;
  // 	obj.elasticity = 1.1f;
  // 	obj.m_friction = 0.5f;
  // 	obj.m_rotation = glm::quat(1,0,0,0);
  // 	objs.push_back(obj);
  //   }
  // }
  // massive_sphere
  Body box;
  box.m_position = glm::vec3(0, 1, 10);
  box.m_linear_velocity = glm::vec3(0, 0, 0);
  box.m_angular_velocity = glm::vec3(0, 0, 0);
  box.m_inv_mass = 0.0002f;
  box.elasticity = 1.1f;
  box.m_friction = 0.5f;
  box.m_rotation = glm::quat(1, 0, 0, 0);
  std::vector<glm::vec3> box_points;
  glm::vec3 mins(-1, 0, -3);
  glm::vec3 maxs(0, 1, 1);
  Bounds m_bounds;
  m_bounds.mins = mins;
  m_bounds.maxs = maxs;
  box.shape = new Box(mins, maxs);
  // test for box
  objs.push_back(box);
  Body ground(new Sphere(1000), glm::vec3(0, -1003, 0), glm::vec3(0, 0, 0),
              glm::quat(1, 0, 0, 0), 0);
  ground.elasticity = 0.9f;
  ground.m_friction = 1.03f;
  objs.push_back(ground);
}

void Scene::Update(float delta_time) {
  for (int i = 0; i < objs.size(); i++) {
    Body &body = objs[i];
    if (body.m_inv_mass == 0) {
      continue;
    }
    glm::vec3 impulse_gravity = gravity * (1.0f / body.m_inv_mass) * delta_time;
    body.Process_Linear_Impulse(impulse_gravity);
    // process the gravity
  }
  std::vector<CollisionPair_t> collisionPairs;
  BroadPhase(objs, objs.size(), collisionPairs, delta_time);
  int num_Contacts = 0;
  std::vector<Contact> contacts;
  for (auto &pair : collisionPairs) {
    auto &body1 = objs[pair.a];
    auto &body2 = objs[pair.b];
    if (body1.m_inv_mass == 0 && body2.m_inv_mass == 0) {
      continue;
    } // find next pair
    Contact contact;
    bool collision = check_collision(body1, body2, contact, delta_time);
    if (collision) {
      contacts.push_back(contact);
    }
  }
  if (contacts.size() > 1) {
    std::sort(contacts.begin(), contacts.end(), CompareContacts);
  }
  float accumulated_Time = 0.0f;
  for (auto &contact : contacts) {
    float dt = contact.time_of_impact - accumulated_Time;
    Body *A = contact.A;
    Body *B = contact.B;
    if (A->m_inv_mass == 0.0f && B->m_inv_mass == 0.0f) {
      continue;
    } // not resolve
    for (int j = 0; j < objs.size(); j++) {
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
bool check_collision(Body &body1, Body &body2, Contact &contact) {
  contact.A = &body1;
  contact.B = &body2;
  contact.time_of_impact = 0.0f;
  if (body1.shape->Get_Type() == SPHERE && body2.shape->Get_Type() == SPHERE) 
  {
    const Sphere *sphereA = (const Sphere *)body1.shape;
    const Sphere *sphereB = (const Sphere *)body2.shape;
    glm::vec3 position_A = body1.m_position;
    glm::vec3 position_B = body2.m_position;
    if (Sphere_Sphere_Static(sphereA, sphereB, position_A, position_B,
                             contact.A_potential_collision_point_world_space,
                             contact.B_potential_collision_point_world_space)) {
      contact.normal = position_A - position_B;
      if (glm::length(contact.normal) != 0) {
        contact.normal = glm::normalize(contact.normal);
      }
      contact.A_potential_collision_point_local_space =
          body1.World_To_Local_space(
              contact.A_potential_collision_point_world_space);
      contact.B_potential_collision_point_local_space =
          body2.World_To_Local_space(
              contact.B_potential_collision_point_world_space);
      glm::vec3 ab = body2.m_position - body1.m_position;
      float r = glm::length(ab) - (sphereA->radius + sphereB->radius);
      contact.seperation_distance = r;
      return true;
    }
    
  } 
else {
    glm::vec3 pt_A;
    glm::vec3 pt_B;
    float bias = 0.001f;
    if (GJK_doesIntersect(contact.A, contact.B, bias, pt_A, pt_B)) {
      glm::vec3 norm = pt_B - pt_A;
      if (glm::length(norm) != 0) {
        norm = glm::normalize(norm);
      }
      pt_A -= norm * bias;
      pt_B += norm * bias;
      contact.normal = norm;
      contact.A_potential_collision_point_world_space = pt_A;
      contact.B_potential_collision_point_world_space = pt_B;
      // set the local
      contact.A_potential_collision_point_local_space =
          body1.World_To_Local_space(
              contact.A_potential_collision_point_world_space);
      contact.B_potential_collision_point_local_space =
          body2.World_To_Local_space(
              contact.B_potential_collision_point_world_space);
      glm::vec3 ab = body2.m_position - body1.m_position;
      float r = glm::length(pt_A - pt_B);
      contact.seperation_distance = -r;
      return true;
    }

  GJK_Closest_Points(body1, body2, pt_A, pt_B);
  contact.A_potential_collision_point_world_space = pt_A;
  contact.B_potential_collision_point_world_space = pt_B;
  // set the local
  contact.A_potential_collision_point_local_space = body1.World_To_Local_space(
      contact.A_potential_collision_point_world_space);
  contact.B_potential_collision_point_local_space = body2.World_To_Local_space(
      contact.B_potential_collision_point_world_space);
  glm::vec3 ab = body2.m_position - body1.m_position;
  float r = glm::length(pt_A - pt_B);

  contact.seperation_distance = r;
  }

  return false;
}
bool check_collision(Body &body1, Body &body2, Contact &contact, float dt) {
  contact.A = &body1;
  contact.B = &body2;
  if (body1.shape->Get_Type() == SPHERE && body2.shape->Get_Type() == SPHERE) {
    const Sphere *sphere_A = (const Sphere *)body1.shape;
    const Sphere *sphere_B = (const Sphere *)body2.shape;
    if (Sphere_Sphere_Dynamic(sphere_A, sphere_B, body1.m_position,
                              body2.m_position, body1.m_linear_velocity,
                              body2.m_linear_velocity, dt,
                              contact.A_potential_collision_point_world_space,
                              contact.B_potential_collision_point_world_space,
                              contact.time_of_impact)) {
      // the collision happen in time_of_impact;
      contact.normal = glm::normalize(body2.m_position - body1.m_position);
      body1.Update(contact.time_of_impact);
      body2.Update(contact.time_of_impact);
      contact.A_potential_collision_point_local_space =
          body1.World_To_Local_space(
              contact.A_potential_collision_point_world_space);
      contact.B_potential_collision_point_local_space =
          body2.World_To_Local_space(
              contact.B_potential_collision_point_world_space);
      body1.Update(-contact.time_of_impact);
      body2.Update(-contact.time_of_impact);
      // return back
      glm::vec3 p_a_b = body2.m_position - body1.m_position;
      float r = glm::length(p_a_b) - (sphere_A->radius + sphere_B->radius);
      contact.seperation_distance = r;
      return true;
    } else {
      return false;
    }
  }
  return false;
  // this function will check collision and update the collision point in
  // contact
}

void Process_collision(Contact &contact) {
  Body *A = contact.A;
  Body *B = contact.B;
  float elasticity = A->elasticity * B->elasticity;
  // glm::vec3 J = (1 + elasticity) * (B->m_linear_velocity -
  // A->m_linear_velocity) / (A->m_inv_mass + B->m_inv_mass);
  glm::vec3 ra = contact.A_potential_collision_point_world_space -
                 contact.A->get_CenterofWorldSpace();
  glm::vec3 rb = contact.B_potential_collision_point_world_space -
                 contact.B->get_CenterofWorldSpace();
  glm::vec3 n = contact.normal;
  glm::vec3 angular_Impulse_A =
      cross(contact.A->get_Inertial_inverse_mat3_world() * cross(ra, n), ra);
  glm::vec3 angular_Impulse_B =
      cross(contact.B->get_Inertial_inverse_mat3_world() * cross(rb, n), rb);
  float angularFactor = dot((angular_Impulse_A + angular_Impulse_B),
                            n); // calculate the angular_part
  glm::vec3 v_A = A->m_linear_velocity + cross(A->m_angular_velocity, ra);
  glm::vec3 v_B = B->m_linear_velocity + cross(B->m_angular_velocity, rb);
  glm::vec3 v_ab = v_A - v_B;
  float Impulse_J_amplitude = (1.0f + elasticity) * dot(v_ab, n) /
                              (angularFactor + A->m_inv_mass + B->m_inv_mass);
  glm::vec3 Impulse_J = Impulse_J_amplitude * n;

  A->Process_Impulse(contact.A_potential_collision_point_world_space,
                     -Impulse_J);
  B->Process_Impulse(contact.B_potential_collision_point_world_space,
                     Impulse_J);
  // process the impulse according to the impulse point and the  Impulse
  // put the body A and B to outside
  // Apply the frictionx
  float friction = A->m_friction * B->m_friction;
  glm::vec3 v_relate_tangent = v_ab - dot(v_ab, n) * n;
  glm::vec3 v_relate_tangent_normalized;
  if (glm::length(v_relate_tangent) == 0) {
    v_relate_tangent_normalized = v_relate_tangent;
  } else {
    v_relate_tangent_normalized = glm::normalize(v_relate_tangent);
  }
  // calculate the friction_part
  glm::vec3 inertiaA = cross(A->get_Inertial_inverse_mat3_world() *
                                 cross(ra, v_relate_tangent_normalized),
                             ra);
  glm::vec3 inertiaB = cross(B->get_Inertial_inverse_mat3_world() *
                                 cross(rb, v_relate_tangent_normalized),
                             rb);
  float invInertia = dot((inertiaA + inertiaB), v_relate_tangent_normalized);
  float reducedMass = 1.0f / (A->m_inv_mass + B->m_inv_mass + invInertia);
  glm::vec3 impulse_Friction = v_relate_tangent * reducedMass * friction;
  A->Process_Impulse(contact.A_potential_collision_point_world_space,
                     -impulse_Friction);
  B->Process_Impulse(contact.B_potential_collision_point_world_space,
                     impulse_Friction);
  // std::cout << A->m_linear_velocity << B->m_linear_velocity;
  if (0.0f == contact.time_of_impact) {
    float tA = A->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
    float tB = B->m_inv_mass / (A->m_inv_mass + B->m_inv_mass);
    glm::vec3 ds = contact.B_potential_collision_point_world_space -
                   contact.A_potential_collision_point_world_space;
    A->m_position += ds * tA;
    B->m_position -= ds * tB; // push the A and B to outside
  }
}
