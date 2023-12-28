#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
out VS_OUT{
    vec3 world_normal;
    vec3 world_position;
}vs_out;
out vec3 world_normal;
out vec3 world_position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
world_normal=mat3(transpose(inverse(model)))*normal;
world_position=vec3(model*vec4(position,1.0f));
gl_Position=projection*view*model*vec4(position,1.0f);
vs_out.world_normal=world_normal;
vs_out.world_position=world_position;
}