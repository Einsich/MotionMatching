#include "ray_tracing_scene.h"
#include "Camera/cameras.h"
#include "Application/application.h"
#include "Components/MeshRender/mesh_render.h"
#include "skybox.h"

RayTracingScene::RayTracingScene(Shader shader):PostFX(shader)
{
  SkyBox sky(common_resources_path("Textures/Skyboxes/CloudSky"));
  skyBox = sky.skybox;
}
void RayTracingScene::postfx_render() 
{
  CameraPtr camera = main_camera();
  if (!camera)
    return;
  const Camera& mainCam = *camera;
  mat4 camTransform = mainCam.get_transform_matrix();
  mat4 ProjViewInv =  camTransform * inverse(mainCam.get_projection());
  shader.set_mat4x4("ProjViewInv", ProjViewInv);
  vec3 camPos = camTransform[3];
  shader.set_vec3("CameraPos", camPos);
  skyBox.bind(shader, "skybox");
}
void RayTracingScene::postfx_unbind()
{
  skyBox.unbind();
}

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight&)
{

  

  Input &input = Application::get_input();
  {
    shared_ptr<ArcballCamera> arcballCam;
  
    GameObjectPtr camera = make_game_object();
    arcballCam = camera->add_component<ArcballCamera>(vec3(0), 10, vec2(0.f, 30.f*DegToRad));
    arcballCam->set_perspective(90.f * DegToRad, 0.01f, 5000.f);
    add_camera(arcballCam);
    input.mouse_move_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_move_handler);
    input.mouse_click_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_click_handler);
    gameObjects.push_back(camera);
  }
  {
    GameObjectPtr postfx = make_game_object();
    postfx->add_component<RayTracingScene>(get_shader("ray_tracing_scene"));
    gameObjects.push_back(postfx);
  }
      GameObjectPtr sphere = make_game_object();
      sphere->add_component<Transform>(vec3(0.f), vec3(), vec3(0.4f));
      sphere->add_component<MeshRender>(create_sphere(20, true, false));
      gameObjects.push_back(sphere);


}