#include "ray_tracing_scene.h"
#include "Camera/cameras.h"
#include "Application/application.h"
void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun)
{
  Input &input = Application::get_input();
  {
    shared_ptr<ArcballCamera> arcballCam;
  
    GameObjectPtr camera = make_game_object();
    arcballCam = camera->add_component<ArcballCamera>(vec3(10.f,50.f,-150.f), 40, vec2(0.f, 30.f*DegToRad));
    arcballCam->set_perspective(70.f * DegToRad, 0.01f, 5000.f);
    add_camera(arcballCam);
    input.mouse_move_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_move_handler);
    input.mouse_click_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_click_handler);
    gameObjects.push_back(camera);
  }
}