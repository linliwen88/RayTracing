#version 330 core
out vec4 FragColor;

// world objects 
uniform int hittableCount;
uniform float sphereOrigins[300];
uniform float sphereRadiuses[100];

// camera and viewport
uniform bool ANTI_ALIAS;
uniform int samples_per_pixel;
uniform float screenWidth;
uniform float screenHeight;

uniform vec3 cameraPos;
uniform vec3 horizontal;
uniform vec3 vertical;
uniform vec3 lowerLeftCorner;



//float rand(){
//    float noise = noise1(gl_FragCoord.x); // [-1, 1]
//    noise += 1.0f; // [0, 2]
//    noise /= 2.00f; // [0, 1)
//
//    return noise;
// }

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

struct HitRecord {
    vec3 point;
    float t;
    vec3 normal;
    bool isFrontFace;
};

bool sphereHit(in vec3 s_origin, in float s_radius, in float t_min, inout float t_max, inout HitRecord rec, in vec3 pointInViewport) {
    vec3 rayDirection = pointInViewport - cameraPos;
    vec3 oc = cameraPos - s_origin;
    float a = dot(rayDirection, rayDirection);
    float half_b = dot(oc, rayDirection);
    float c = dot(oc, oc) - s_radius * s_radius;
    float discriminant = half_b*half_b - a*c;

    if(discriminant < 0.0)
    {
        return false;
    }

    // Find the nearest root that lies in the acceptable range.
    float root = (-half_b - sqrt(discriminant)) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt(discriminant)) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.point = cameraPos + root * rayDirection;

    vec3 outward_normal = (rec.point - s_origin) / s_radius;
    rec.isFrontFace = dot(rayDirection, outward_normal) < 0;
    rec.normal = rec.isFrontFace ? outward_normal :-outward_normal;

    return true;
}


void main()
{
    HitRecord rec;
    float t_min = 0.0;
    float t_max = 1000.0;
    float closest_so_far = t_max;
    bool hitAnything = false;
    vec4 rayColor = vec4(0.0);

    for(int s = 0; s < samples_per_pixel; s++) {
        float u, v;
        if(ANTI_ALIAS) {
            u = gl_FragCoord.x + rand(vec2(gl_FragCoord.x, gl_FragCoord.y));
            v = gl_FragCoord.y + rand(vec2(gl_FragCoord.y, gl_FragCoord.x));
        }
        else {
            u = gl_FragCoord.x;
            v = gl_FragCoord.y;
        }

        vec3 pointInViewport = lowerLeftCorner + ((u / screenWidth) * horizontal) + ((v / screenHeight) * vertical);
        for(int i = 0; i < hittableCount; i++) {
            HitRecord tmp_rec;

            vec3 s_origin = vec3(sphereOrigins[3 * i], sphereOrigins[(3 * i) + 1], sphereOrigins[(3 * i) + 2]);
            float s_radius = sphereRadiuses[i];

            if(sphereHit(s_origin, s_radius, t_min, closest_so_far, tmp_rec, pointInViewport)) {
                closest_so_far = tmp_rec.t;
                hitAnything = true;
                rec = tmp_rec;
            }
        }

        if(hitAnything)
        {
            rayColor += vec4(0.5 * (rec.normal + 1.0), 1.0);
        }
        else // draw canvas
        {
	        float t = gl_FragCoord.y / screenHeight;

	        vec4 lightBlue = vec4(0.1, 0.1, 1.0, 1.0);
	        vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

	        rayColor += (1.0 - t) * white + t * lightBlue;
        }
    }
    FragColor = clamp(rayColor / float(samples_per_pixel), 0.0, 1.0);
}