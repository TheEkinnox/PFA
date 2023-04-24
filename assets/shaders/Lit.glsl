#shader vertex
#version 330 core

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoords;

out VS_OUT
{
	vec3	FragPos;
	vec3	Normal;
	vec2	TexCoords;
} vs_out;

uniform mat4 u_mvp;
uniform mat4 u_modelMat;
uniform mat4 u_normalMat;

void main()
{
	gl_Position = u_mvp * vec4(_pos, 1.0);
	
	vs_out.FragPos = (u_modelMat * vec4(_pos, 1.0)).xyz;
	vs_out.Normal = (u_normalMat * vec4(_normal, 0)).xyz;
	vs_out.TexCoords = _texCoords;
}

#shader fragment
#version 430 core

struct DirLight
{
	vec4	color;

	vec3	direction;
};

struct PointLight
{
	vec4	color;

	vec3	position;

	float	constant;
	float	linear;
	float	quadratic;
};

struct SpotLight
{
	vec4	color;

	vec3	direction;
	vec3	position;

	float	constant;
	float	linear;
	float	quadratic;

	float	cutoff;
	float	outerCutoff;
};

struct Material
{
	vec4		tint;
	vec4		specularColor;
	vec2		uvOffset;
	vec2		uvScale;
	sampler2D	diffuse;
	sampler2D	specular;
	sampler2D	normal;
	float		shininess;
	int			usedMaps;
};

in VS_OUT
{
	vec3	FragPos;
	vec3	Normal;
	vec2	TexCoords;
} fs_in;

out vec4 FragColor;

layout(std430, binding = 0) buffer LightSSBO
{
	mat4 ssbo_lights[];
};

uniform vec3		u_viewPos;

uniform Material	u_material;

vec3 g_normal;
vec3 g_viewDir;
vec4 g_diffColor;
vec4 g_specColor;

int  unpack(int data, int offset, int bitCount);
vec3 calculateBlinnPhong(vec3 lightDir, vec4 diffuse, vec4 specular);

vec3 calculateLight(mat4 lightMat);
vec3 calculateDirLight(mat4 lightMat);
vec3 calculatePointLight(mat4 lightMat);
vec3 calculateSpotLight(mat4 lightMat);

void main()
{
	vec2 texCoords = fs_in.TexCoords * u_material.uvScale + u_material.uvOffset;

	bool useNormalMap = unpack(u_material.usedMaps, 0, 1) == 1;
	bool useSpecMap = unpack(u_material.usedMaps, 1, 1) == 1;
	bool useDiffuseMap = unpack(u_material.usedMaps, 2, 1) == 1;

	// TODO(NTH): Implement normal mapping properly
	//if (useNormalMap)
	//	g_normal = normalize(texture(u_material.normal, texCoords).rgb);
	//else
		g_normal = normalize(fs_in.Normal);

	if (useSpecMap)
		g_specColor = texture(u_material.specular, texCoords);
	else
		g_specColor = u_material.specularColor;

	if (useDiffuseMap)
		g_diffColor = texture(u_material.diffuse, texCoords) * u_material.tint;
	else
		g_diffColor = u_material.tint;

	g_viewDir = normalize(u_viewPos - fs_in.FragPos);

	vec3 litColor = vec3(0);

	for (int i = 0; i < ssbo_lights.length(); i++)
		litColor += calculateLight(ssbo_lights[i]);

	FragColor = vec4(litColor, g_diffColor.a);
}

int unpack(int data, int offset, int bitCount)
{
	int mask = (1 << bitCount) - 1;
	return (data >> offset) & mask;
}

vec3 calculateBlinnPhong(vec3 lightDir, vec4 lightColor)
{
	float lambertian = max(dot(lightDir, g_normal), 0);
	float specularIntensity = 0;

	if (lambertian > 0)
	{
		vec3 halfDir = normalize(lightDir + g_viewDir);
		float specularAngle = max(dot(halfDir, g_normal), 0);

		specularIntensity = pow(specularAngle, u_material.shininess);
	}

	vec3 diffuseColor = lightColor.rgb * g_diffColor.rgb * lambertian;
	vec3 specColor = lightColor.rgb * g_specColor.rgb * specularIntensity;

	return (diffuseColor + specColor) * lightColor.a;
}

vec3 calculateLight(mat4 lightMat)
{
	switch(int(lightMat[3][3]))
	{
	case 0:
		return lightMat[0].rgb * lightMat[0].a * g_diffColor.rgb;
	case 1:
		return calculateDirLight(lightMat);
	case 2:
		return calculatePointLight(lightMat);
	case 3:
		return calculateSpotLight(lightMat);
	default:
		return vec3(0);
	}
}

vec3 calculateDirLight(mat4 lightMat)
{
	DirLight light =
	{
		lightMat[0],
		{ lightMat[1][0], lightMat[2][0], lightMat[3][0] }
	};

	vec3 lightDir = normalize(-light.direction);

	return calculateBlinnPhong(lightDir, light.color);
}

vec3 calculatePointLight(mat4 lightMat)
{
	PointLight light =
	{
		lightMat[0],
		{ lightMat[1][1], lightMat[2][1], lightMat[3][1] },
		lightMat[1][2], lightMat[2][2], lightMat[3][2]
	};

	vec3 lightDir = normalize(light.position - fs_in.FragPos);

	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				 light.quadratic * (distance * distance));

	return calculateBlinnPhong(lightDir, light.color) * attenuation;
}

vec3 calculateSpotLight(mat4 lightMat)
{
	SpotLight light =
	{
		lightMat[0],
		{ lightMat[1][0], lightMat[2][0], lightMat[3][0] },
		{ lightMat[1][1], lightMat[2][1], lightMat[3][1] },
		lightMat[1][2], lightMat[2][2], lightMat[3][2],
		lightMat[1][3], lightMat[2][3]
	};

	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	vec3 spotDir = normalize(-light.direction);

	float spotAngle = dot(lightDir, spotDir);

	if (spotAngle < light.outerCutoff)
		return vec3(0);

	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((spotAngle - light.outerCutoff) / epsilon, 0.0, 1.0);

	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
				light.quadratic * (distance * distance));

	return calculateBlinnPhong(lightDir, light.color) * attenuation * intensity;
}