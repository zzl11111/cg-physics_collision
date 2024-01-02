#include "body.h"

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void *)0);
}

void Mesh::Set_VAO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
				&vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
				&indices[0], GL_STATIC_DRAW);
	// read the data in vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						(void *)sizeof(glm::vec3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
				&indices[0], GL_STATIC_DRAW);
	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glm::vec3 Body::get_CenterofWorldSpace() const {
	// glm::vec4 v = glm::mat4_cast(m_rotation) *
	// glm::vec4(shape->get_CenterOfMass(), 1);
	return m_position + m_rotation * shape->get_CenterOfMass();
}

glm::vec3 Body::get_CenterofLocalSpace() const {
	return shape->get_CenterOfMass();
}

glm::vec3 Body::World_To_Local_space(const glm::vec3 &position) const {
	glm::vec3 position_to_center = position - get_CenterofWorldSpace();
	glm::quat inverse_quat =
		glm::quat(m_rotation.w, -m_rotation.x, -m_rotation.y, -m_rotation.z);
	return inverse_quat * position_to_center;
}

glm::vec3 Body::Local_To_World_space(const glm::vec3 &position) const {
	glm::vec3 position_to_center = position - get_CenterofLocalSpace();
	glm::vec3 world_vector = m_rotation * position_to_center;
	return world_vector + get_CenterofWorldSpace();
}

glm::mat4 Body::get_model_matrix() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_position);
	model = model * glm::mat4(m_rotation);
	return model; // model matrix is related to the model
}

void Body::Process_Impulse(const glm::vec3 &impulse_point,
					const glm::vec3 &impulse) {
	if (m_inv_mass == 0) { return; }
	Process_Linear_Impulse(impulse);
	glm::vec3 r = impulse_point - get_CenterofWorldSpace();
	glm::vec3 angular_impulse = cross(r, impulse);
	Process_Angular_Impulse(angular_impulse);
}

void Body::Process_Linear_Impulse(const glm::vec3 &linear_impulse) {
    if (m_inv_mass == 0) { return; }
    m_linear_velocity += linear_impulse * m_inv_mass; // v+=J/m
}

void Body::Process_Angular_Impulse(const glm::vec3 &angular_impulse) {
	if (m_inv_mass == 0) { return; }
	m_angular_velocity += get_Inertial_inverse_mat3_world() * angular_impulse; // w+=L/I
	// std::cout << m_angular_velocity;
	float max_angular_speed = 30.0f;
	if (glm::length(m_angular_velocity) >= max_angular_speed) {
		m_angular_velocity =
			max_angular_speed * glm::normalize(m_angular_velocity);
	}
}

glm::mat3 Body::get_Inertial_mat3_local() {
	glm::mat3 Inertial;
	if (m_inv_mass != 0) {
		Inertial = shape->get_Ineritial_mat3(1 / m_inv_mass);
	} else { Inertial = shape->get_Ineritial_mat3(); }
	return Inertial;
}

glm::mat3 Body::get_Inertial_mat3_world() {
    glm::mat3 Inertial = get_Inertial_mat3_local();
    glm::mat3 rotation_matrix = glm::mat3(m_rotation);
    return rotation_matrix * Inertial * glm::transpose(rotation_matrix);
    // R*I*R^T
}

glm::mat3 Body::get_Inertial_inverse_mat3_world() {
	glm::mat3 Inertial = get_Inertial_mat3_local();
	glm::mat3 rotation_matrix = glm::mat3(glm::normalize(m_rotation));
	return rotation_matrix * glm::inverse(Inertial) *
		glm::transpose(rotation_matrix);
	// R *I-1*R^T
}

void Body::Update(float dt)
  // update the position
  {
    glm::vec3 cmTopos = m_position - get_CenterofWorldSpace();
    glm::vec3 alpha = get_Inertial_inverse_mat3_world() *
                      (cross(m_angular_velocity,
                             get_Inertial_mat3_world() * m_angular_velocity));
    // process the internal torque
    m_angular_velocity += alpha * dt;

    glm::vec3 dAngle = m_angular_velocity * dt;

    glm::quat dq = get_quat_from_n_and_angle(dAngle, glm::length(dAngle));
    m_rotation = dq * m_rotation;
    m_rotation = glm::normalize(m_rotation);
    m_position = m_position + m_linear_velocity * dt;
    m_position = get_CenterofWorldSpace() + dq * cmTopos;
  }