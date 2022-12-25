#include <ecs.h>
#include <imgui.h>
#include "game_structs.h"

EVENT(require=ecs::Tag mainHero) collect_kills(
  const KillTargetEvent&,
  int &killsCount
)
{
  killsCount++;
}

EVENT() show_kill_stat(const ImguiRender&, int killsCount)
{
  if (ImGui::Begin("Score"))
  {
    ImGui::Text("%d kills", killsCount);
    ImGui::End();
  }
}

template<typename Callable> void gather_all_targets(Callable);

SYSTEM(stage=act; require=ecs::Tag mainHero) check_winner(bool &isWinner)
{
  if (isWinner)
    return;
  int aliveTargets = 0;
  QUERY(require=ecs::Tag target) gather_all_targets([&]()
  {
    aliveTargets++;
  });
  if (aliveTargets == 0)
    isWinner = true;
}