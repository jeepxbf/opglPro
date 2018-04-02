#version 330 core

uniform vec3 lightColor;
uniform vec3 cubeColor;


out vec4 color;
in vec3 Normal;

void main()
{	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = ambient * cubeColor; 
	color = vec4(result,1.0);
}