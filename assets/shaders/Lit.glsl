#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 mvp;
uniform mat4 modelMat;
uniform mat4 normalMat;

void main()
{
	gl_Position = mvp * vec4(_pos, 1.0);
	FragPos = (modelMat * vec4(_pos, 1.0)).xyz;
	Normal = (normalMat * vec4(_normal, 0)).xyz;
	TexCoord = _texCoord;
}

#shader fragment
#version 330 core

#define NB_POINT_LIGHTS 4

struct DirLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	direction;
};

struct PointLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	position;

	float	constant;
	float	linear;
	float	quadratic;
};

struct SpotLight
{
	vec4	ambient;
	vec4	diffuse;
	vec4	specular;

	vec3	position;
	vec3	direction;
	float	cutOff;
	float	outerCutOff;

	float	constant;
	float	linear;
	float	quadratic;
};

struct Material
{
	sampler2D	diffuse;
	sampler2D	specular;
	float		shininess;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NB_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material _material;

vec4 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material);
vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material);
vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material);

void main()
{
	vec3 fragPos = FragPos;
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec4 litColor = vec4(0);

	litColor += calculateDirLight(dirLight, normal, viewDir, _material);

	for (int i = 0; i < NB_POINT_LIGHTS; i++)
		litColor += calculatePointLight(pointLights[i], normal, fragPos, viewDir, _material);

	litColor += calculateSpotLight(spotLight, normal, fragPos, viewDir, _material);

	FragColor = litColor;
}

vec4 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(-light.direction);

	float lambertian = max(dot(lightDir, normal), 0);
	float specular = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + viewDir);
		float specularAngle = max(dot(halfDir, normal), 0);

		specular = pow(specularAngle, material.shininess);
	}

	vec4 fragColor = texture(material.diffuse, TexCoord);
	vec4 specColor = texture(material.specular, TexCoord);

	vec4 ambientColor = light.ambient * fragColor;
	vec4 diffuseColor = light.diffuse * fragColor;
	vec4 specularColor = light.specular * specColor;

	return (ambientColor + diffuseColor * lambertian + specularColor * specular);
}

vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float lambertian = max(dot(lightDir, normal), 0);
	float specular = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + viewDir);
		float specularAngle = max(dot(halfDir, normal), 0);

		specular = pow(specularAngle, material.shininess);
	}

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				 light.quadratic * (distance * distance));

	vec4 fragColor = texture(material.diffuse, TexCoord);
	vec4 specColor = texture(material.specular, TexCoord);

	vec4 ambientColor = light.ambient * fragColor;
	vec4 diffuseColor = light.diffuse * fragColor;
	vec4 specularColor = light.specular * specColor;

	return (ambientColor + diffuseColor * lambertian + specularColor * specular) * attenuation;
}

vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 spotDir = normalize(-light.direction);

	float spotAngle = dot(lightDir, spotDir);

	if (spotAngle < light.outerCutOff)
		return vec4(0);

	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((spotAngle - light.outerCutOff) / epsilon, 0.0, 1.0);

	float lambertian = max(dot(lightDir, normal), 0);
	float specular = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + viewDir);
		float specularAngle = max(dot(halfDir, normal), 0);

		specular = pow(specularAngle, material.shininess);
	}

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				light.quadratic * (distance * distance));

	vec4 fragColor = texture(material.diffuse, TexCoord);
	vec4 specColor = texture(material.specular, TexCoord);

	vec4 ambientColor = light.ambient * fragColor;
	vec4 diffuseColor = light.diffuse * fragColor;
	vec4 specularColor = light.specular * specColor;

	return (ambientColor + diffuseColor * lambertian + specularColor * specular) * attenuation * intensity;
}