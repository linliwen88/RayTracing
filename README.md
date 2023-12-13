# RayTracing
A ray tracing engine written with OpenGL \
Tutorial: https://raytracing.github.io/books/RayTracingInOneWeekend.html

## Ray Tracing computed in CPU with multithreading
![RayTraceing_CPU](/pics/RayTracing_CPU.png "Ray Tracing computed in CPU with multithreading")

## Ray Tracing computed in GPU (fragment shader)
![RayTraceing_CPU](/pics/RayTracing_GPU.png "Ray Tracing computed in CPU with multithreading")

### Issues
#### Not random enough
Anti-aliasing (done with multi-sampling) creates jaggy silhouettes and the scattered ray directions converge into shadow acnes because GPU isn't good at generating random numbers.

To solve this, I generated a texture of random numbers on the CPU side and sampled the it inside fragment shader when multi-sampling and scattering ray directions.

The difference: 
![Compare_shadow_acne](/pics/compare_shadow_acne.png "Compare_shadow_acne")

![Compare_shadow_acne](/pics/compare_silhouette.png "Compare_shadow_acne")

### TODOs:
- Make dielectric material
- Add metal material fuzziness
- Add other primitive shapes (e.g. cube)
