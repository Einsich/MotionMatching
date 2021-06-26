#include "ecs/ecs.h"
#include "Camera/camera.h"
#include "Postfx/postfx.h"
#include "Texture/textures.h"

EVENT() init_scene(const ecs::OnSceneCreated &)
{

  printf("scene created\n");
  {
    ecs::EntityId mainCamera = create_arcball_camera(10, vec2(0.f, 30.f*DegToRad), vec3(0,0,0));
    ecs::send_event(OnSetMainCamera(mainCamera));
  }
  {
    ecs::ComponentInitializerList list;
    list.add<PostFX>("rayTracing") = PostFX(get_shader("ray_tracing_scene"));
    list.add<CubeMap>("skyBox") = CubeMap();
    list.add<Texture3D>("perlinNoise3D") = Texture3D();
    ecs::create_entity(list);
  }
}
