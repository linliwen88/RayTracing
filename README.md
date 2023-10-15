# RayTracing
A ray tracing engine written with OpenGL

![Diffuse materials](/pics/record.gif "Diffuse materials")
#### Ray tracing parameters (in `fshader.glsl`):
 - `t_min = 0.1`: The minimum `t` to solve for ray intersection with object. Larger than 0 to deal with shadow acne.
  - `rayStrength = 0.7`: Timed with strength of ray after each bounce. The lower the darker the scene.
  - `maxRayBounce = 10`: Limit number of ray bounce.
#### Anti-aliasing parameters (in `RenderAttributes.h`):
- `samples_per_pixel = 100`
