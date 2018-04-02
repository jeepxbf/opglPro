#version 330 core

uniform vec3 lightColor;
uniform vec3 cubeColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

float specularStrength = 0.5;

void main()
{	
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
		
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(lightDir,norm),0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (specular + ambient + diffuse) * cubeColor;
	color = vec4(result,1.0);
}