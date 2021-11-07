#include <ecs.h>
#include "world_renderer.h"
#include <transform2d.h>
#include <imgui.h>
#include <application/application_data.h>
#include "game_structs.h"

EVENT() init_menu(const StartMenuEvent &, WorldRenderer &wr)
{
  //camera creation
  Resolution r = Application::get_context().get_resolution();
  wr.resolution = vec2(r.width, r.height);
  float orthoScale = 1.f/100.f;//100 pixel per unit
  mat3 projection(0.f);
  projection[0][0] = 1.f / r.width;
  projection[1][1] = 1.f / r.height;
  
  ecs::create_entity<mat3, vec3, Transform2D>(
    {"cameraProjection", projection},
    {"zoom", vec3(orthoScale * 0.5f, orthoScale * 10.f, 1/20.f)},
    {"transform", Transform2D(vec2(0), vec2(orthoScale))}
  );
  ecs::create_entity<ecs::Tag>({"startGameButton", {}});
}

//update menu
SYSTEM(ecs::SystemOrder::UI, ecs::Tag startGameButton) start_game_button(ecs::EntityId eid, const ScoreBoard &sb)
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


SYSTEM(ecs::SystemOrder::UI) exit_menu_button(bool isWinner, int killsCount, ScoreBoard &sb)
{
  if (isWinner && ImGui::Begin("Result"))
  {
    ImGui::Text("You win the game, %d kills", killsCount);
    if (ImGui::Button("Exit menu"))
    {
      ecs::destroy_scene();
      ecs::send_event<StartMenuEvent>(StartMenuEvent());
      sb.curentLevel ++;
      sb.kills.push_back(killsCount);
    }
    ImGui::End();
  }
}
