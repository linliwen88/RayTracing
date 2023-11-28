#version 330 core
#define PI      3.1415926535

out vec4 FragColor;

// shading mode
uniform int SHADING_MODE; // 0: object normal; 1: ray tracing lighting

// world objects 
uniform int hittableCount;
uniform vec3  sphereOrigins[100];
uniform float sphereRadiuses[100];
uniform int   sphereMaterialTypes[100]; // 0: metal; 1: lambertial; 2: dielectric
uniform vec3  sphereMaterialAlbedos[100];

// camera and viewport
uniform bool ANTI_ALIAS;
uniform int samples_per_pixel;
uniform float screenWidth;
uniform float screenHeight;

uniform vec3 cameraPos;

uniform vec3 pixel_delta_u;
uniform vec3 pixel_delta_v;
uniform vec3 pixel00_location;

uniform vec3 viewport_u;
uniform vec3 viewport_v;
uniform vec3 viewport_lower_left;

uniform int maxRayBounce;

vec3 lightBlue = vec3(0.5, 0.7, 1.0);
vec3 white = vec3(1.0, 1.0, 1.0);

// define materials
const int metal      = 0;
const int lambertian = 1;
const int dielectric = 2;

// data stuctures
struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Material {
     int type;
     vec3 albedo;
};

struct HitRecord {
    vec3 point;
    float t;
    vec3 normal;
    bool isFrontFace;
    Material mat;
};

struct Sphere {
    vec3 origin;
    float radius;
    Material mat;
};



// utility functions
// TODO: sample random texture for random numbers
float rand(vec2 co, float min, float max){
    // return random float between [min, max]
    return  fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453123) * (max - min) + min;

}

vec3 randVec3(vec3 seed) {
    return vec3(rand(seed.xy, -0.5, 0.5), rand(seed.yz, -0.5, 0.5), rand(seed.xz, -0.5, 0.5));
    // return vec3(rand(seed.xy, 0.0, 1.0), rand(seed.yz, 0.0, 1.0), rand(seed.xz, 0.0, 1.0));
}

vec3 random_in_unit_sphere(in vec3 seed) {
    vec3 p = randVec3(seed);
    return p;

//    // supposed to reject random vector when length > 1.0, but random function is lowzy so not practical
//    vec3 p;
//    
//    for(float i = 0.0f; i < 100.0f; i += 1.0f) {
//        p = randvec3(seed + vec3(i));
//        if(p.length() <= 2.0f) return p;
//    }
//    return p;
}

vec3 random_on_hemisphere(in vec3 seed, in vec3 normal) {
    vec3 p = normalize(random_in_unit_sphere(seed));
    if(dot(p, normal) < 0.0) {
        p = -p;
    }

    return p;
}

void linear_to_gamma_space(inout vec3 linear_component) {
    linear_component = sqrt(linear_component);
}


void scatter(inout Ray ray, in HitRecord rec) {

    if(rec.mat.type == metal) {
        vec3 reflected = reflect(ray.direction, rec.normal);
        // ray = Ray(rec.point, reflected);
        ray = Ray(rec.point, reflected + random_in_unit_sphere(rec.normal));
    }
    else if(rec.mat.type == lambertian) {
        vec3 scattered_direction = rec.normal + random_in_unit_sphere(rec.normal);
        scattered_direction = normalize(scattered_direction);

        ray = Ray(rec.point, scattered_direction);
    }
    else if(rec.mat.type == dielectric) {

    }
}

bool sphere_hit(in Sphere sphere, inout Ray ray, in float t_min, inout float t_max, inout HitRecord rec) {
    // if(length(ray.origin - sphere.origin) - sphere.radius < 0.01) {
    //     return false;
    // }
    vec3 oc = ray.origin - sphere.origin;
    // ray.direction = normalize(ray.direction);
    float a = dot(ray.direction, ray.direction);
    float half_b = dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = half_b*half_b - a*c;

    if(discriminant < 0.0) {
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
    rec.point = ray.origin + (root * ray.direction);

    vec3 outward_normal = (rec.point - sphere.origin) / sphere.radius;
    rec.isFrontFace = dot(ray.direction, outward_normal) < 0.0;
    rec.normal = rec.isFrontFace ? outward_normal : -outward_normal;
    rec.mat = sphere.mat;

    return true;
}

bool world_hit(in Ray ray, in float t_min, in float t_max, inout HitRecord rec) {
    bool hitAnything = false;
    float closest_so_far = t_max;

    for(int i = 0; i < hittableCount; i++) {
        HitRecord tmp_rec;
        Sphere sphere = Sphere(sphereOrigins[i], sphereRadiuses[i], 
                                Material(sphereMaterialTypes[i], sphereMaterialAlbedos[i]));

        if(sphere_hit(sphere, ray, t_min, closest_so_far, tmp_rec)) {
            closest_so_far = tmp_rec.t;
            hitAnything = true;
            rec = tmp_rec;
        }
    }
    return hitAnything;
}

// Bounce the ray and determine color by whether it hits object in the scene.
vec3 ray_color(inout Ray ray) {
    vec3 rayColor = vec3(1.0);
    HitRecord rec;
    float t_min = 0.1;
    float t_max = 1000.0;
    // float closest_so_far = t_max;
    bool hitAnything = false;

    if(SHADING_MODE == 0) { // show the object's normal

        hitAnything = world_hit(ray, t_min, t_max, rec);

        if(hitAnything) { // draw sphere normal
            rayColor = 0.5 * (rec.normal + 1.0);
        }
        else { // draw background
            vec3 unitDirection = normalize(ray.direction);
            float t = (unitDirection.y + 1.0) * 0.5;

            rayColor = (1.0 - t) * white + t * lightBlue;
        }
    }
    else if(SHADING_MODE == 1) { // ray tracing
        for(int bounce = 0; bounce < maxRayBounce; bounce++) {
            hitAnything = world_hit(ray, t_min, t_max, rec);

            if(hitAnything) {
                // add hit object color
                ray.origin = rec.point;
                scatter(ray, rec);
                rayColor = rayColor * rec.mat.albedo;
            }
            else {
                vec3 unitDirection = normalize(ray.direction);
                float t = (unitDirection.y + 1.0) * 0.5;

                if(bounce > 0) {
                    rayColor = rayColor * ((1.0 - t) * white + t * lightBlue);
                }
                else {
                    // ray directly hits the background
                    rayColor = (1.0 - t) * white + t * lightBlue;
                }
                break;
            }
        }
    }

    return rayColor;
}

// Set the ray's origin point and direction, which is from the camera to a pixel on the screen.
void get_ray(inout Ray ray) {
    float u;
    float v;

    if(ANTI_ALIAS) {
        u = (gl_FragCoord.x + rand(gl_FragCoord.xy, -0.5, 0.5)) / screenWidth;
        v = (gl_FragCoord.y + rand(gl_FragCoord.xy, -0.5, 0.5)) / screenHeight;
    }
    else {
        u = gl_FragCoord.x / screenWidth;
        v = gl_FragCoord.y / screenHeight;
    }
    
    vec3 pixel_sample = viewport_lower_left + (u * viewport_u) + (v * viewport_v);
    ray = Ray(cameraPos, pixel_sample - cameraPos);
}


void main() {
    vec3 pixelColor = vec3(0.0);
    for(int s = 0; s < samples_per_pixel; s++) {
        Ray ray;
        get_ray(ray);

        pixelColor += ray_color(ray);
    }
    pixelColor = pixelColor / float(samples_per_pixel);
    linear_to_gamma_space(pixelColor);

    FragColor = vec4(clamp(pixelColor, 0.0, 1.0), 1.0);
}