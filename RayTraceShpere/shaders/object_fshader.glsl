#version 330 core
out vec4 FragColor;

in vec3 fragPos; // world space

uniform vec3 cameraPos;
uniform int hittableCount;
uniform float sphereOrigins[300];
uniform float sphereRadiuses[100];

struct HitRecord {
    vec3 point;
    float t;
    vec3 normal;
    bool isFrontFace;
};

bool sphereHit(in vec3 s_origin, in float s_radius, in float t_min, inout float t_max, inout HitRecord rec) {
    vec3 rayDirection = fragPos - cameraPos;
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

    for(int i = 0; i < hittableCount; i++) {
        HitRecord tmp_rec;

        vec3 s_origin = vec3(sphereOrigins[3 * i], sphereOrigins[(3 * i) + 1], sphereOrigins[(3 * i) + 2]);
        float s_radius = sphereRadiuses[i];

        if(sphereHit(s_origin, s_radius, t_min, closest_so_far, tmp_rec)) {
            closest_so_far = tmp_rec.t;
            hitAnything = true;
            rec = tmp_rec;
        }
    }

    if(hitAnything)
    {
        FragColor = vec4(0.5 * (rec.normal + 1.0), 1.0);
    }
    else // draw canvas
    { 
        float windowHeight = 800.0;
	    float t = gl_FragCoord.y / windowHeight;

	    vec4 lightBlue = vec4(0.1, 0.1, 1.0, 1.0);
	    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);

	    FragColor = (1.0 - t) * white + t * lightBlue;
    }

}