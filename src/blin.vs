#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
    vec3 world_normal;
    vec3 world_position;
} vs_out;

out vec3 world_normal;
out vec3 world_position;
out vec2 texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	world_normal = mat3(transpose(inverse(model))) * normal;
	world_position = vec3(model * vec4(position,1.0f));
	gl_Position = projection * view * model * vec4(position, 1.0f);
	texcoords = aTexCoords;
	vs_out.world_normal = world_normal;
	vs_out.world_position = world_position;
}