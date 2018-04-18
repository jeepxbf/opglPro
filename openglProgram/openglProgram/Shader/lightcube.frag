#version 330 core

uniform vec3 lightColor;
//uniform vec3 cubeColor;
uniform vec3 viewPos;
uniform vec3 lightPos;
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
	vec3 direction;
	float cutOff; //内切角
	float outerCutOff;//外切角
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;

in vec2 TexCoords;
void main()
{	
	
	float distance = length(lightPos - FragPos);
	float attenuation  = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance); //定点光衰减
	
	//vec3 lightDir = normalize(-light.direction);

	vec3 lightDir = normalize(lightPos - FragPos);
	float theta = dot(lightDir,normalize(-light.direction));
	if(theta > light.cutOff)
	{
		//环境光
		vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse,TexCoords));

		//漫反射	
		vec3 norm = normalize(Normal);
		float diff = max(dot(lightDir,norm),0.0);
		vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));

		//镜面高光
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
		vec3 specular = attenuation * light.specular * spec * vec3(texture(material.specular,TexCoords));

		vec3 result = specular + ambient + diffuse;
		color = vec4(result,1.0);
	}
	else
	{
		color = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0f);
	}

}