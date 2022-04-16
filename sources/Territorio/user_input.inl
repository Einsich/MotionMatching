#include <ecs.h>
#include <input.h>
#include "map_arrays.h"
#include "game_events.h"
#include "game_structs.h"

template<typename Callable>
static void is_game_started(Callable);

EVENT(require=ecs::Tag isPlayer)start_game(const KeyDownEvent<SDLK_RETURN>&)
{
  QUERY()is_game_started([&](bool &gameStarted)
  {
    if (!gameStarted)
    {
      gameStarted = true;
      ecs::send_event(OnGameStarted());
    }
  });
}

vec2 screen_to_world(int x, int y);
ecs::EntityId spawn_player(const ecs::Template *player_template, ivec2 start_point);

static bool get_world_pos(int x, int y, const MapArrays &map_arrays, ivec2 &world_pos)
{
  auto [h, w] = map_arrays.color_indices.dimension();
  world_pos = vec2(w, h) * (vec2(1.f) + screen_to_world(x, y)) * 0.5f;

  return map_arrays.color_indices.test(world_pos.y, world_pos.x);
}

template<typename Callable>
static void select_map_query(Callable);

EVENT(require=ecs::Tag player_spawning) select_spawn_point(
  const MouseButtonDownEvent<MouseButton::RightButton> &event,
  ecs::EntityId eid
)
{
  QUERY()select_map_query([&](
    const MapArrays &map_arrays
  )
  {
    ivec2 worldPos;
    if (get_world_pos(event.x, event.y, map_arrays, worldPos))
    {
      const ecs::Template *playerTemplate = ecs::get_template("player");
      spawn_player(playerTemplate, worldPos);
      ecs::destroy_entity(eid);
    }
  });
}

void try_add_invasion(uint &forces, vector<Invasion> &invasions, Invasion invasion);

EVENT(require=ecs::Tag isPlayer) select_invasion(
  const MouseButtonDownEvent<MouseButton::LeftButton> &event,
  uint landIndex,
  uint &forces,
  const vector<ecs::EntityId> &neighbors,
  const vector<uint> &neighborsIdx,
  vector<Invasion> &invasions,
  float invasion_weight
)
{
  select_map_query([&](
    const MapArrays &map_arrays
  )
  {
    ivec2 worldPos;
    if (get_world_pos(event.x, event.y, map_arrays, worldPos))
    {
      uint targetIdx = map_arrays.color_indices[worldPos.y][worldPos.x];
      if (targetIdx != landIndex)
      {
        auto it = std::find(neighborsIdx.begin(), neighborsIdx.end(), targetIdx);
        if (it != neighborsIdx.end())
        {
          int i = it - neighborsIdx.begin();
          try_add_invasion(forces, invasions, Invasion{neighbors[i], neighborsIdx[i], (uint)(forces * invasion_weight), false});
        }
      }
    }
  });
}

#include <imgui.h>

SYSTEM(stage=ui; require=ecs::Tag isPlayer) change_invasion_weight(float &invasion_weight, uint forces)
{
  if (ImGui::Begin("invasion forces"))
  {
    int invasionForces = forces * invasion_weight;
    if (ImGui::SliderInt("", &invasionForces, 0, forces))
    {
      invasion_weight = (float)invasionForces / forces;
    }
    ImGui::End();
  }
  
}
SYSTEM(stage=ui) check_mouse_over_ui()
{
  Input::input().mouseOverUI = ImGui::GetIO().WantCaptureMouse;
}