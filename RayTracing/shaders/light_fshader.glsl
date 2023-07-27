#version 330 core
in vec2 outTexCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, outTexCoord), texture(texture1, outTexCoord), 0.2);	
}