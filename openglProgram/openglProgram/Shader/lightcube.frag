#version 330 core

uniform vec3 lightColor;
//uniform vec3 cubeColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

//float specularStrength = 0.5;

//����
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

//����ǿ��
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

void main()
{	
	//������
	vec3 ambient = light.ambient * material.ambient * lightColor;
	//������	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(lightDir,norm),0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse * lightColor;
	//����߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
	vec3 specular = light.specular * material.specular * spec * lightColor;

	vec3 result = specular + ambient + diffuse;
	color = vec4(result,1.0);
}