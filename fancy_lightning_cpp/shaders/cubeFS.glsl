#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
  
uniform vec3 viewPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.specular;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	vec3 ambient = light.ambient * material.ambient;;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
