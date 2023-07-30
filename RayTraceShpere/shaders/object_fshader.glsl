#version 330 core
out vec4 FragColor;

in vec3 fragPos; // world space

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
    vec3 rayDirection = fragPos - cameraPos;
    vec3 oc = cameraPos - sphereOrigin;
    float a = dot(rayDirection, rayDirection);
    float b = 2.0 * dot(oc, rayDirection);
    float c = dot(oc, oc) - sphereRadius*sphereRadius;
    float discriminant = b*b - 4*a*c;

    // Ray hit sphere, draw sphere
    if(discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2 * a);
        vec3 hitPoint = cameraPos + (rayDirection * t);
        vec3 N = normalize(hitPoint - sphereOrigin);
        N = 0.5 * (N + 1.0); // map normal to [0, 1]
        FragColor = vec4(N, 1.0);
    }
    // Draw canvas
    else {
        float windowHeight = 800.0;
	    float t = gl_FragCoord.y / windowHeight;

	    vec4 lightBlue = vec4(0.1, 0.1, 1.0, 1.0);
	    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

	    FragColor = (1.0 - t) * white + t * lightBlue;
    }
}