#include <ecs/ecs.h>
#include "Camera/camera.h"
#include "Postfx/postfx.h"
#include "Texture/textures.h"
#include "Application/application.h"


EVENT() ray_tracing_init(
  const ecs::OnEntityCreated &,
  PostFX &rayTracing,
  CubeMap &skyBox,
  Texture3D &perlinNoise3D)
{
  skyBox = CubeMap(common_resources_path("Textures/Skyboxes/CloudSky"));
  perlinNoise3D = Texture3D(256, 256, 256, "perlinNoise3D");
  Shader perlinNoise3dGeneration = get_shader("perlin3d_generation");
  perlinNoise3dGeneration.use();
  perlinNoise3D.execute_dispatch();

}
SYSTEM(ecs::SystemOrder::LATE_RENDER) ray_tracing(
  PostFX &rayTracing,
  CubeMap &skyBox,
  Texture3D &perlinNoise3D
)
{
  mat4 camTransform, projection;
  if (!main_camera(camTransform, projection))
    return;
  Shader &shader = rayTracing.shader;
  shader.use();
  mat4 ProjViewInv =  camTransform * inverse(projection);
  rayTracing.shader.set_mat4x4("ProjViewInv", ProjViewInv);
  vec3 camPos = camTransform[3];

  shader.set_vec3("CameraPos", camPos);
  shader.set_float("Time", Time::time());
  skyBox.bind(shader, "skybox");
  perlinNoise3D.bind(shader, "perlin3D");

  rayTracing.render();

  skyBox.unbind();
  perlinNoise3D.unbind();
}