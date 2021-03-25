#version 440
in vec2 fragUV;
out vec4 FragColor;

uniform mat4 ProjViewInv;
uniform vec3 CameraPos;

uniform samplerCube skybox;
uniform sampler3D perlin3D;

#define FAR_INF 1e9
#define EPS 1e-4
#define RAY_DEPTH 7
#define STACK_SIZE 130
struct Ray
{
    vec3 pos, dir;
    float transparent;
    int depth;
};
struct Sphere
{
    vec3 pos;
    float radius;
};
struct Collision
{
    bool hit;
    vec3 pos, normal;
    float dist;
};
struct SphereMaterial
{
  float transparent;
  float reflectionFactor;
  float refractionFactor;
  vec3 color;
};
struct RayTracingStack
{
  Ray rays[STACK_SIZE];
  int top;
};
RayTracingStack rayTracingStack;
const int SphereN = 4;
Sphere spheres[SphereN] = Sphere[SphereN](
  Sphere(vec3(0,0,0), 1), 
  Sphere(vec3(1.5,-0.5,1), 0.5), 
  Sphere(vec3(1.4,-0.2,-0.3), 0.25), 
  Sphere(vec3(1.2,-0.1,-0.9), 0.2));
SphereMaterial spheresMat[SphereN] = SphereMaterial[SphereN](
  SphereMaterial(0.99, 0.5, 0.98, vec3(0.1,0.1,0.3)), 
  SphereMaterial(1, 0.5, 0.98, vec3(0.2,0.4,0.2)),
  SphereMaterial(0, 1, 1.1, vec3(0.9,0.6,0.9)),
  SphereMaterial(0, 0, 0.98, vec3(0.4,0.9,0.9)));

void clear_stack()
{
  rayTracingStack.top = 0;
}
bool stack_empty()
{
  return rayTracingStack.top == 0;
}
Ray get_ray_from_stack()
{
  Ray ray = rayTracingStack.rays[rayTracingStack.top-1];
  rayTracingStack.top--;
  return ray;
}
void pop_ray_to_stack(Ray ray)
{
  if (rayTracingStack.top < STACK_SIZE && ray.depth < RAY_DEPTH)
  {
    ray.depth++;
    rayTracingStack.rays[rayTracingStack.top] = ray;
    rayTracingStack.top++;
  }
}
void raycast_sphere(inout Collision collision, in Sphere sphere, in Ray ray)
{
  vec3 center = ray.pos - sphere.pos;
  float c = dot(center, center) - sphere.radius*sphere.radius;
  float b = dot(center, ray.dir);
  float a = dot(ray.dir, ray.dir);
  float D = b * b - a * c;
  if (D < 0)
  {
    collision.hit = false;
    return;
  }
  D = sqrt(D);
  float t = 0;
  float t1 = (-D - b) / a;
  if (t1 >= 0)
    t = t1;
  else
  {
    float t2 = (D - b) / a;
    if (t2 >= 0)
      t = t2;
    else
    {
      collision.hit = false;
      return;
    }
  }
  collision.hit = true;
  collision.dist = t;
  collision.pos = ray.pos + ray.dir * t;
  collision.normal = normalize(collision.pos - sphere.pos);
}

Ray get_ray(vec2 uv)
{
    Ray ray;
    vec4 pos = ProjViewInv * vec4(fragUV, 0, 1.0);
    ray.dir = normalize(pos.xyz / pos.w - CameraPos);
    ray.pos = CameraPos;
    ray.transparent = 0;
    ray.depth = 0;
    return ray;
}
float simple_noise(vec3 pos)
{
  float noise = sin(pos.x * 10.1) + cos(pos.y * 20.2) + sin(13*pos.z);
  return abs(noise) / 3;
}
bool find_collision_spheres(in Ray ray, out Collision bestCollision, inout int sphereInd)
{
  bestCollision.hit = false;
  bestCollision.dist = FAR_INF;
  for (int i = 0; i < SphereN; i++)
  {
    Collision collision;
    raycast_sphere(collision, spheres[i], ray);
    if (collision.hit && collision.dist < bestCollision.dist)
    {
      bestCollision = collision;
      sphereInd = i;
    }
  }
  return bestCollision.hit;
}
void create_rays_spheres(in Ray ray, in Collision collision, in int sphereInd, inout vec4 color)
{
  SphereMaterial material = spheresMat[sphereInd];
  vec3 normal = collision.normal;
  bool outedRay = dot(ray.dir, normal) > 0;
  float refractionFactor = outedRay ? 1 / material.refractionFactor : material.refractionFactor;

  float noise = textureLod(perlin3D, ray.pos.xzy * 2.7, 0).x;

  normal.xyz += (vec3(noise) - vec3(0.5))*0.3;
  normal = normalize(normal);
color = vec4(noise,0,0, 1);
return;
  vec3 reflected = reflect(ray.dir, normal);
  if (outedRay)
    normal = -normal;



  vec3 refracted = refract(ray.dir, normal, refractionFactor);
  bool foolInsideReflection = dot(refracted, refracted) < 0.1;
  float transparent = material.transparent;

  float t,r,d;
  t = transparent == 1 ? 0 : transparent;
  r = (1 - t) * (1 - material.reflectionFactor);
  d = (1 - t) * material.reflectionFactor;

  if (transparent * material.reflectionFactor > EPS)
  {
    Ray reflectedRay = ray;
    reflectedRay.dir = reflected;
    reflectedRay.pos += reflected * EPS;
    reflectedRay.transparent += r * (1 - ray.transparent);
    pop_ray_to_stack(reflectedRay);
  }
  
  if ((1 - transparent) > EPS)
  {
    if (foolInsideReflection)
    {
      color += vec4(material.color, 1) * (1 - transparent) * (1 - ray.transparent);
    }
    else
    {
      Ray refractedRay = ray;
      refractedRay.dir = refracted;
      refractedRay.pos += refracted * EPS;
      refractedRay.transparent += t * (1 - ray.transparent);
      pop_ray_to_stack(refractedRay);
    }
  }
  color += vec4(material.color, 1) * transparent * (1 - material.reflectionFactor) * (1 - ray.transparent);
}
void main()
{
  clear_stack();
  pop_ray_to_stack(get_ray(fragUV));
  vec4 color = vec4(0);
  while (!stack_empty())
  {
    Ray ray = get_ray_from_stack();
    Collision bestCollision;
    int sphereInd = -1;
    
    if (find_collision_spheres(ray, bestCollision, sphereInd))
    {
      ray.pos = bestCollision.pos;
      create_rays_spheres(ray, bestCollision, sphereInd, color);
    }
    else
    {
      color += vec4(textureLod(skybox, ray.dir, 0).rgb, 1) * (1 - ray.transparent);
    }
    if (color.a >= 0.99)
      break;
  }
  FragColor = vec4(color.rgb , 1.0);
}