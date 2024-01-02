#version 330 core
out vec4 FragColor;

in vec3 world_normal;  
in vec3 world_position;  
in vec2 texcoords;
  
<<<<<<< HEAD:shader/blin.fs
uniform sampler2D diffuse_sampler;
=======
uniform sampler2D diffuse_sampler; // texture0
>>>>>>> 4de40939ac534abd9fe7c1f40733ee98161a7f94:src/blin.fs
uniform vec3 lightPos; 
uniform vec3 viewPos; //viewPos is the camer
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    // ambient
    float ambientStrength = 0.4;//ambient_power
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 norm = normalize(world_normal);
    vec3 lightDir = normalize(lightPos - world_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 2*diff * lightColor * texture(diffuse_sampler, texcoords).rgb;
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - world_position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    int time = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), time);
    vec3 specular = specularStrength * spec * lightColor;  
        
    // vec3 result = (ambient + diffuse + specular) * objectColor;  // white
<<<<<<< HEAD:shader/blin.fs
	vec3 result = (ambient + diffuse + specular) * texture(diffuse_sampler, texcoords).rgb;
=======
	vec3 result = ambient + diffuse + specular;
>>>>>>> 4de40939ac534abd9fe7c1f40733ee98161a7f94:src/blin.fs
    FragColor = vec4(result, 1.0);
} 