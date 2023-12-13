# RayTracing
A ray tracing engine written with OpenGL \
Tutorial: https://raytracing.github.io/books/RayTracingInOneWeekend.html

![Diffuse materials - Random reflection](/pics/random_reflection.gif "Diffuse materials using random refelction")
![Diffuse materials - Lambertian reflection](/pics/lambertian_reflection.gif "Diffuse materials using Lambertian refelction")
#### Ray tracing parameters (in `fshader.glsl`):
 - `t_min = 0.1`: The minimum `t` to solve for ray intersection with object. Larger than 0 to deal with shadow acne.
  - `rayStrength = 0.7`: Timed with strength of ray after each bounce. The lower the darker the scene.
  - `maxRayBounce = 10`: Limit number of ray bounce.
#### Anti-aliasing parameters (in `RenderAttributes.h`):
- `samples_per_pixel = 100`

### Issues
* Variance: 
https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders-(Part-I)#variance

## Ray Tracing computed in CPU with multithreading
![RayTraceing_CPU](/pics/RayTracing_CPU.png "Ray Tracing computed in CPU with multithreading")

## Ray Tracing computed in GPU (fragment shader)
![RayTraceing_CPU](/pics/RayTracing_GPU.png "Ray Tracing computed in CPU with multithreading")

Anti-aliasing and scatter ray directions look jaggy and filled with shadow acne because the GPU doesn't generate good random numbers. \
TO solve this, I generated a texture of random numbers on the CPU side and smapled the random texture inside fragment shader. \
The differences: \
![Compare_shadow_acne](/pics/compare_shadow_acne.png "Compare_shadow_acne")

![Compare_shadow_acne](/pics/compare_silhouette.png "Compare_shadow_acne")

### TODOs:
- Make dielectric material
- Add metal material fuzziness
- Add other primitive shapes (e.g. cube)
