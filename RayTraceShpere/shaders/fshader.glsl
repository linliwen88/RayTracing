#version 330 core
out vec4 FragColor;

// shading mode
uniform int SHADING_MODE; // 0: object normal; 1: diffuse material

// world objects 
uniform int hittableCount;
uniform vec3 sphereOrigins[100];
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

int maxRayBounce = 15;
vec3 lightBlue = vec3(0.5, 0.7, 1.0);

vec3 white = vec3(1.0, 1.0, 1.0);

// data stuctures
struct Ray {
    vec3 origin;
    vec3 direction;
};

struct HitRecord {
    vec3 point;
    float t;
    vec3 normal;
    bool isFrontFace;
};

struct Sphere {
    vec3 origin;
    float radius;
};

// utility functions
// TODO: better random functions
float rand(vec2 co, float min, float max){
    // return random float between [min, max]
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453123) * (max - min) + min;
}

vec3 randVec3(vec3 seed) {
    return vec3(rand(seed.xy, -0.5, 0.5), rand(seed.yz, -0.5, 0.5), rand(seed.xz, -0.5, 0.5));
}

vec3 random_in_unit_sphere(in vec3 seed) {
    vec3 p = randVec3(seed);
    
    // supposed to reject random vector when length > 1.0, but random function is lowzy so not practical
    // for(float i = 0.0f; i < 100.0f; i += 1.0f) {
    //     p = randVec3(seed + vec3(i));
    //     if(p.length() <= 2.0f) return p;
    // }
    return p;
}

vec3 random_on_hemisphere(in vec3 seed, in vec3 normal) {
    vec3 p = normalize(random_in_unit_sphere(seed));
    if(dot(p, normal) < 0.0) {
        p = -p;
    }

    return p;
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
    rec.point = ray.origin + (root * ray.direction);

    vec3 outward_normal = (rec.point - sphere.origin) / sphere.radius;
    rec.isFrontFace = dot(ray.direction, outward_normal) < 0.0;
    rec.normal = rec.isFrontFace ? outward_normal : -outward_normal;

    return true;
}

// Bounce the ray and determine color by whether it hits object in the scene.
vec3 ray_color(inout Ray ray) {
    vec3 rayColor = vec3(0.0);
    HitRecord rec;
    float t_min = 0.1;
    float t_max = 1000.0;
    float closest_so_far = t_max;
    bool hitAnything = false;

    if(SHADING_MODE == 0) { // show the object's normal
        for(int i = 0; i < hittableCount; i++) {
            HitRecord tmp_rec;
            Sphere sphere = Sphere(sphereOrigins[i], sphereRadiuses[i]);

            if(sphere_hit(sphere, ray, t_min, closest_so_far, tmp_rec)) {
                closest_so_far = tmp_rec.t;
                hitAnything = true;
                rec = tmp_rec;
            }
        }
        if(hitAnything) { // draw sphere normal
            rayColor = 0.5 * (rec.normal + 1.0);
        }
        else { // draw background
            vec3 unitDirection = normalize(ray.direction);
            float t = (unitDirection.y + 1.0) * 0.5;

            rayColor = (1.0 - t) * white + t * lightBlue;
        }
    }
    else if(SHADING_MODE == 1) { // diffuse material
        // use iteration for ray tracing without recursion in shader
        bool bounceEnd = false;
        int bounceCount = 0;
        float rayStrength = 1.0;

        while(!bounceEnd) {
            if(bounceCount >= maxRayBounce) {
                rayColor = vec3(0.0);
                break;
            }

            for(int i = 0; i < hittableCount; i++) { // check all objects for ray hit
                HitRecord tmp_rec;
                Sphere sphere = Sphere(sphereOrigins[i], sphereRadiuses[i]);

                if(sphere_hit(sphere, ray, t_min, closest_so_far, tmp_rec)) {
                    closest_so_far = tmp_rec.t;
                    hitAnything = true;
                    rec = tmp_rec;
                }
            }

            if(hitAnything) { // draw sphere
                ray.origin = rec.point;
                // ray.direction = normalize(rec.normal + random_in_unit_sphere(rec.point)); // lambertian reflection
                ray.direction = random_on_hemisphere(rec.point, rec.normal); // random reflection
                rayStrength *= 0.5;
            }
            else { // draw background
                bounceEnd = true;
                vec3 unitDirection = normalize(ray.direction);
                float t = (unitDirection.y + 1.0) * 0.5;

                if(bounceCount == 0) {
                    rayColor = (1.0 - t) * white + t * lightBlue;
                    break;
                }
                rayColor = rayStrength * ((1.0 - t) * white + t * lightBlue);
            }

            // clear hit information for next iteration
            hitAnything = false;
            closest_so_far = t_max;

            // increase ray bounce count
            bounceCount++;
        }
    }

    return rayColor;
}

// Set the ray's origin point and direction, which is from the camera to a pixel on the screen.
void get_ray(inout Ray ray) {
    float u = gl_FragCoord.x;
    float v = gl_FragCoord.y;
    vec3 pixelCenter = lowerLeftCorner + ((u / screenWidth) * horizontal) + ((v / screenHeight) * vertical);

    if(ANTI_ALIAS) {
        float px = -0.5 + rand(gl_FragCoord.xy, 0.0, 1.0);
        float py = -0.5 + rand(gl_FragCoord.xz, 0.0, 1.0);
        vec3 pixel_delta_u = horizontal / screenWidth;
        vec3 pixel_delta_v = vertical / screenHeight;
        
        vec3 pixelSample = pixelCenter + (px * pixel_delta_u) + (py * pixel_delta_v);
        ray = Ray(cameraPos, pixelSample - cameraPos);

    }
    else {
        ray = Ray(cameraPos, pixelCenter - cameraPos);
    }    
}


void main()
{
    vec4 pixelColor = vec4(0.0);
    for(int s = 0; s < samples_per_pixel; s++) {
        Ray ray;
        get_ray(ray);

        pixelColor += vec4(ray_color(ray), 1.0);
    }
    FragColor = clamp(pixelColor / float(samples_per_pixel), 0.0, 1.0);
}