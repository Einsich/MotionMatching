#include <ecs.h>
#include <imgui.h>
#include "game_structs.h"

EVENT(ecs::Tag mainHero) collect_kills(
  const KillTargetEvent&,
  int &killsCount
)
{
  killsCount++;
}

SYSTEM(ecs::SystemOrder::UI) show_kill_stat(int killsCount)
{
  if (ImGui::Begin("Score"))
  {
    ImGui::Text("%d kills", killsCount);
    ImGui::End();
  }
}

template<typename Callable> void gather_all_targets(Callable);

SYSTEM(ecs::SystemOrder::LOGIC, ecs::Tag mainHero) check_winner(bool &isWinner)
{
  if (isWinner)
    return;
  int aliveTargets = 0;
  QUERY(ecs::Tag target) gather_all_targets([&]()
  {
    aliveTargets++;
  });
  if (aliveTargets == 0)
    isWinner = true;
}