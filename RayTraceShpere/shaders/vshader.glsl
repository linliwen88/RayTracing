#version 330 core
layout (location = 0) in vec3 aPos;		// canvas coordinates in clip space
layout (location = 1) in vec2 aTexCoord; // texture coordinaes

out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoord = aTexCoord;
	gl_Position = vec4(aPos, 1.0);
}