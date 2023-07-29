#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform vec3 cameraPos;
uniform vec3 sphereOrigin;
uniform float sphereRadius;

bool hit_sphere() {
    vec3 rayDirection = fragPos - cameraPos;
    vec3 oc = cameraPos - sphereOrigin;
    float a = dot(rayDirection, rayDirection);
    float b = 2.0 * dot(oc, rayDirection);
    float c = dot(oc, oc) - sphereRadius*sphereRadius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0.0);
}


void main()
{    
    if(hit_sphere()) { // draw sphere
        FragColor = vec4(0.83, 0.2, 0.2, 1.0);
    }
    else { // draw canvas
        float windowHeight = 800.0;
	    float t = gl_FragCoord.y / windowHeight;

	    vec4 lightBlue = vec4(0.1, 0.1, 1.0, 1.0);
	    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

	    FragColor = (1.0 - t) * lightBlue + t * white;
    }
}