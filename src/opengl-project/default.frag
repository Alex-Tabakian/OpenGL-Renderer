#version 330 core

out vec4 FragColor;

in vec3 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform sampler2D tex0;

uniform vec4 lightColor;

uniform vec3 lightPos;
uniform bool isLight;

void main()
{
	if (isLight)
	{
		FragColor = lightColor;
		return;
	}

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);

	float diffuse = max(dot(normal, lightDirection), 0.2);

    FragColor = vec4(color, 1.0) * lightColor * diffuse;
}
