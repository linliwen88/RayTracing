#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient  = ambientStrength * lightColor;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5;
	int shininess = 32; // reflective property of the object
	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;


	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);	
}