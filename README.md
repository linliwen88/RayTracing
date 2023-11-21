# RayTracing
A ray tracing engine written with OpenGL

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

* Tutorial: https://raytracing.github.io/books/RayTracingInOneWeekend.html
