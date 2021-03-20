#version 440
in vec2 fragUV;
out vec4 FragColor;

uniform mat4 ProjViewInv;
uniform vec3 CameraPos;

uniform samplerCube skybox;

#define FAR_INF 1e9
struct Ray
{
    vec3 pos, dir;
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
    return ray;
}
float simple_noise(vec3 pos)
{
  float noise = sin(pos.x * 0.1) + cos(pos.y * 0.2) + sin(pos.z);
  return abs(noise) / 3;
}
void main()
{
  const int SphereN = 1;
  Sphere spheres[SphereN] = Sphere[SphereN](Sphere(vec3(0,0,0), 1));

  Ray ray = get_ray(fragUV);
  vec3 color = ray.dir;
  
  Collision bestCollision;
  bestCollision.hit = false;
  bestCollision.dist = FAR_INF;
  for (int i = 0; i < SphereN; i++)
  {
    Collision collision;
    raycast_sphere(collision, spheres[i], ray);
    if (collision.hit && collision.dist < bestCollision.dist)
    {
      bestCollision = collision;
    }
  }
  if (bestCollision.hit)
  {
    vec3 normal = bestCollision.normal;
    vec3 reflection = textureLod(skybox, normal, 0).rgb;
    color = mix(normal, reflection, simple_noise(bestCollision.pos));
  }
  else
  {
    color = textureLod(skybox, ray.dir, 0).rgb;
  }
  FragColor = vec4(color, 1.0);
}