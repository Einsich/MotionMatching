#include <ecs/ecs.h>
#include "world_renderer.h"
#include <transform2d.h>
#include <imgui.h>
#include <application/application_data.h>
#include "game_structs.h"
#include <ecs/event_registration.h>


ECS_EVENT_REGISTRATION(StartGameEvent)
ECS_EVENT_REGISTRATION(LoadSceneEvent)
ECS_EVENT_REGISTRATION(KillTargetEvent)

EVENT() setup_camera(
  const ecs::OnEntityCreated &,
  mat3 &cameraProjection,
  vec3 &zoom,
  Transform2D &transform,
  WorldRenderer &wr)
{
  //camera creation
  Resolution r = Application::get_context().get_resolution();
  wr.resolution = vec2(r.width, r.height);
  float orthoScale = 1.f/100.f;//100 pixel per unit
  cameraProjection = mat3(0.f);
  cameraProjection[0][0] = 1.f / r.width;
  cameraProjection[1][1] = 1.f / r.height;

  zoom = vec3(orthoScale * 0.5f, orthoScale * 10.f, 1/20.f);
  transform = Transform2D(vec2(0), vec2(orthoScale));
}

//update menu
SYSTEM(require=ecs::Tag startGameButton; stage=imgui_render) start_game_button(ecs::EntityId eid, const ScoreBoard &sb)
{
  constexpr int N = 64;
  char buf[N];
  if (ImGui::Begin("Menu"))
  {
    snprintf(buf, N, "Start level %d", sb.curentLevel);
    if (ImGui::Button(buf))
    {
      ecs::destroy_entity(eid);
      ecs::send_event<StartGameEvent>(StartGameEvent());
    }
    ImGui::End();
  }
}


SYSTEM(stage=imgui_render) exit_menu_button(bool isWinner, int killsCount, ScoreBoard &sb)
{
  if (isWinner && ImGui::Begin("Result"))
  {
    ImGui::Text("You win the game, %d kills", killsCount);
    if (ImGui::Button("Exit menu"))
    {
      ecs::send_event<LoadSceneEvent>(LoadSceneEvent());
      sb.curentLevel ++;
      sb.kills.push_back(killsCount);
    }
    ImGui::End();
  }
}
