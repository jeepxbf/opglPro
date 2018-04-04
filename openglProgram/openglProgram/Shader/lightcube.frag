#version 330 core

uniform vec3 lightColor;
//uniform vec3 cubeColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

//float specularStrength = 0.5;

//材质
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

//光照强度
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

in vec2 TexCoords;
void main()
{	
	//环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

	//漫反射	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(lightDir,norm),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	//镜面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));

	vec3 result = specular + ambient + diffuse;
	color = vec4(result,1.0);
}