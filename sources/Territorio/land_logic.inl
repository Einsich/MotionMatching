#include <set>
#include <ecs/ecs.h>
#include <ecs/registration.h>
#include <application/time.h>
#include "map_arrays.h"
#include "game_events.h"
#include "game_structs.h"
#include "profiler/profiler.h"
#include <ecs/event_registration.h>

ECS_EVENT_REGISTRATION(OnGameStarted)

ECS_REGISTER_TYPE_AND_VECTOR(Invasion, "Invasion", ecs::DefaultType);


template<typename Callable>
static void query_neighbor(ecs::EntityId, Callable);

static Invasion find_target(const ecs::vector<ecs::EntityId> &neighbors, uint forces)
{
  ecs::EntityId target;
  uint minForces = forces;
  uint targetIdx = -1;
  for (const auto &neighbor : neighbors)
  {
    QUERY()query_neighbor(neighbor, [&](
      uint forces,
      ecs::EntityId eid,
      uint landIndex
    )
    {
      if (forces < minForces)
      {
        minForces = forces;
        target = eid;
        targetIdx = landIndex;
      }
    });
  }
  if (minForces < forces)
  {
    int delta = forces - minForces;
    return Invasion {target, targetIdx, delta / 2u, false};
  }
  return Invasion {ecs::EntityId(), -1u, 0, false};
}

EVENT(require=ecs::Tag isPlayableLand) game_started(
  const OnGameStarted&,
  uint &forces,
  uint startForces,
  int &landCount
)
{
  forces = startForces;
  landCount = 1;
}

void try_add_invasion(uint &forces, ecs::vector<Invasion> &invasions, Invasion invasion)
{
  if (invasion.enemyEid.valid())
  {
    auto prevInv = std::find_if(invasions.begin(), invasions.end(),
        [&](const Invasion &inv){return inv.enemyIndex == invasion.enemyIndex;});
    if (prevInv == invasions.end())
      invasions.emplace_back(invasion);
    else
      prevInv->forces += invasion.forces;
    forces -= invasion.forces;
  }
}

SYSTEM(stage=act;) update_bot_invasions(
  float invasionPeriod,
  float &invasionTime,
  ecs::vector<Invasion> &invasions,
  const ecs::vector<ecs::EntityId> &neighbors,
  uint &forces
)
{
  invasionTime += Time::delta_time();
  if (invasionTime < invasionPeriod)
    return;
  invasionTime = rand_float(0, invasionPeriod);

  Invasion invasion = find_target(neighbors, forces);
  try_add_invasion(forces, invasions, invasion);
}

template<typename Callable>
static void lands_economic(Callable);

template<typename Callable>
static void gather_invaders(Callable);
template<typename Callable>
static void query_victim(ecs::EntityId, Callable);
template<typename Callable>
static void gather_invaders2(Callable);
template<typename Callable>
static void query_victim2(Callable);

static void conquer(MapArrays &map_arrays, uint conquerer, ivec2 p)
{
  auto &conquerBorders = map_arrays.borders[conquerer];
  conquerBorders.erase(p);
  for (ivec2 d : {ivec2(1, 0), ivec2(-1, 0), ivec2(0, -1), ivec2(0, 1)})
  {
    ivec2 next = p + d;
    if (map_arrays.color_indices.test(next.y, next.x))
    {
      uint neighborColor = map_arrays.color_indices[next.y][next.x];
      if (neighborColor != conquerer)
      {
        conquerBorders.insert(next);
        if (neighborColor < map_arrays.borders.size())
          map_arrays.borders[neighborColor].insert(p);
      }
      //else erase it later
    }
  }
}
//stage=act+2
SYSTEM(stage=act) map_update(
  float forceFromCell,
  float forceFromPeople,
  float maxForceFromCell,
  float updatePeriod,
  float &updateTime,
  MapArrays &map_arrays,
  bool &mapWasChanged,
  bool &needUpdateBorder,
  bool gameStarted
)
{
  if (!gameStarted)
    return;
  updateTime += Time::delta_time();
  if (updateTime < updatePeriod)
    return;

  updateTime -= updatePeriod;

  QUERY(require=ecs::Tag isPlayableLand)lands_economic([&](
    int landCount,
    uint &forces
  )
  {
    float income = glm::min(landCount * forceFromCell + forces * forceFromPeople, landCount * maxForceFromCell);
    forces += (uint)income;
  });

  QUERY()gather_invaders([&](
    ecs::vector<Invasion> &invasions,
    uint landIndex,
    int &landCount
  )
  {
    uint invadersIndex = landIndex;
    int &invadersLandsCount = landCount;
    for (Invasion &invasion : invasions)
    {
      auto victimEid = invasion.enemyEid;
      QUERY()query_victim(victimEid, [&](
        uint &forces,
        uint landIndex,
        int &landCount
      )
      {
        int invadersLandsBefore = invadersLandsCount;
        if (landCount > 0)
        {
          uint defendersPerCell = forces / landCount ;
          uint defendersPerCellWithGarrison = defendersPerCell + 1;
          const auto &border = map_arrays.borders[invadersIndex];
          vector<ivec2> borderTmp(border.size());
          std::copy(border.begin(), border.end(), borderTmp.begin());
          for (ivec2 p : borderTmp)
          {
            if (map_arrays.color_indices[p.y][p.x] != landIndex)
              continue;
            //debug_log("player %d own %d in %d %d", invadersIndex, landIndex, p.x, p.y);
            if (defendersPerCellWithGarrison <= invasion.forces)
            {
              invasion.forces -= defendersPerCellWithGarrison;
              forces -= defendersPerCell;
              landCount--;
              invadersLandsCount++;
              map_arrays.color_indices[p.y][p.x] = invadersIndex;
              conquer(map_arrays, invadersIndex, p);
            }
            else
            {
              forces -= invasion.forces;
              invasion.forces = 0;
              break;
            }
          }
        }
        mapWasChanged |= invadersLandsBefore != invadersLandsCount;
        invasion.stoped = invasion.forces == 0 || invadersLandsBefore == invadersLandsCount;
      });
    }
    invasions.erase(
      std::remove_if(invasions.begin(), invasions.end(), [](const Invasion &inv){return inv.stoped;}),
      invasions.end());
  });
  needUpdateBorder = mapWasChanged;
}
//stage=act+1
SYSTEM(stage=act) border_update(
  MapArrays &map_arrays,
  bool &needUpdateBorder
)
{
  if (!needUpdateBorder)
    return;
  needUpdateBorder = false;
  for (uint i = 0; i < map_arrays.borders.size(); i++)
    std::erase_if(map_arrays.borders[i], [&](const ivec2 &p)
    {
      if (map_arrays.color_indices[p.y][p.x] == i)
        return true;
      bool isBorder = false;
      for (ivec2 d : {ivec2(1, 0), ivec2(-1, 0), ivec2(0, -1), ivec2(0, 1)})
      {
        ivec2 next = p + d;
        isBorder |= (map_arrays.color_indices.test(next.y, next.x) && map_arrays.color_indices[next.y][next.x] == i);
      }
      return !isBorder;
    });

  QUERY(require=ecs::Tag isPlayableLand)gather_invaders2([&](
    uint landIndex,
    ecs::vector<ecs::EntityId> &neighbors,
    ecs::vector<uint> &neighborsIdx
  )
  {
    uint invadersIndex = landIndex;
    set<uint> uniqNeighborsIdx;
    for (ivec2 p : map_arrays.borders[landIndex])
    {
      uniqNeighborsIdx.insert(map_arrays.color_indices[p.y][p.x]);
    }
    neighborsIdx.resize(uniqNeighborsIdx.size());
    std::copy(uniqNeighborsIdx.begin(), uniqNeighborsIdx.end(), neighborsIdx.begin());
    neighbors.resize(neighborsIdx.size());
    QUERY()query_victim2([&](
      ecs::EntityId eid,
      uint landIndex,
      int landCount
    )
    {
      if (invadersIndex != landIndex && landCount > 0)
      {
        auto it = std::find(neighborsIdx.begin(), neighborsIdx.end(), landIndex);
        if (it != neighborsIdx.end())
          neighbors[it - neighborsIdx.begin()] = eid;
      }
    });
  });
}