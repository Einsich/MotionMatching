#include <ecs/ecs.h>
#include <ecs/ecs_perform.h>
#include <ecs/registration.h>

constexpr int TotalNames = 100;
constexpr int totalPrefabs = 100;

std::vector<std::string> generate_names()
{
  std::vector<std::string> names;
  names.reserve(TotalNames);
  for (int i = 0; i < TotalNames; i++)
  {
    names.push_back("a_" + std::to_string(i));
  }
  return names;
}

static std::vector<std::string> possibleNames = generate_names();
static std::vector<std::pair<ecs::prefab_id, int>> prefabs;

template<typename C>
static void select_random_names(int count, C &&f)
{
  ECS_ASSERT(count <= TotalNames);
  bool selected[TotalNames];
  memset(selected, 0, TotalNames);

  int i = rand() % TotalNames;
  while (count > 0)
  {
    if (!selected[i])
    {
      selected[i] = true;
      f(possibleNames[i].c_str());
      count--;
    }
    i = rand() % TotalNames;
  }
}

static ecs::prefab_id generate_prefab(int template_idx, int component_count)
{

  ecs::vector<ecs::ComponentPrefab> components;

  select_random_names (component_count,
  [&](const char *name) { components.emplace_back(name, 0); });

  std::string name = "prefab_" + std::to_string(template_idx);
  return ecs::create_entity_prefab(name.c_str(), std::move(components));
}

void do_some_work(int &x)
{
  for (int i = 0, n = rand() % 250; i < n;i++)
  {
    x = (x ^ 18798871) + 1087987;
  }
}

static void synthetic_system_1(int &a1)
{
  do_some_work(a1);
}
static void synthetic_system_2(int &a1, int &a2)
{
  do_some_work(a1);
  do_some_work(a2);
}
static void synthetic_system_3(int &a1, int &a2, int &a3)
{
  do_some_work(a1);
  do_some_work(a2);
  do_some_work(a3);
}
static void synthetic_system_4(int &a1, int &a2, int &a3, int &a4)
{
  do_some_work(a1);
  do_some_work(a2);
  do_some_work(a3);
  do_some_work(a4);
}

constexpr int systemArgsCount = 4;

template<typename F>
static void create_system(const char *name_str, int system_idx, int args_count, const char *stage, F f)
{
  std::string name = name_str + std::to_string(system_idx);

  ecs::vector<ecs::string> after;
  for (int i = 0, n = rand() % 3; system_idx != 0 && i < n; i++)
  {
    after.emplace_back(name_str + std::to_string(rand() % system_idx));
  }
  ecs::QueryCache *cache = new ecs::QueryCache();
  ecs::vector<ecs::ArgumentDescription> args;

  int intTypeIdx = ecs::get_type_index<int>();

  select_random_names (args_count,
  [&](const char *name)
  { args.emplace_back(name, intTypeIdx, ecs::AccessType::ReadWrite, false, false); });

  ECS_LOG("%s%d", name_str, system_idx);
  for (const auto &arg : args)
    printf("%s, ", arg.name.c_str());
  printf("\n");

  ecs::register_system(
    "", name.c_str(), cache,
    std::move(args),
    {}, {}, stage, {}, std::move(after), {},
    [cache, f]() { ecs::perform_system(*cache, f); });
}

static void init_entities()
{
  const int cmpCount = 5;
  const int cmpInc = 3;
  const int cmpBatch = 4;
  static_assert(cmpCount+(totalPrefabs/cmpBatch)*cmpInc < TotalNames);
  const int entCount = 50;
  const int entpInc = 100;

  for (int i = 0, count = cmpCount; i < totalPrefabs; i+=cmpBatch, count+=cmpInc)
  {
    for (int j = 0, ents = entCount; j < cmpBatch; j++, ents += entpInc)
    {
      ecs::prefab_id prefab = generate_prefab(i+j, count);
      ECS_ASSERT(prefab != ecs::invalidPrefabId);
      prefabs.push_back({prefab, ents});
    }
  }
  int totalEntityCount = 0;
  for (auto [prefab, entityCount] : prefabs)
  {
    totalEntityCount += entityCount;
    for (int i = 0; i < entityCount; i++)
      ecs::create_entity_immediate(prefab);
  }

  ECS_LOG("created %d entities", totalEntityCount);
}

static void init_systems(const char *name, const char *stage, int system_count[systemArgsCount])
{

  int sysIdx = 0;
  for (int i = 0; i < system_count[0]; i++)
    create_system(name, ++sysIdx, 1, stage, &synthetic_system_1);
  for (int i = 0; i < system_count[1]; i++)
    create_system(name, ++sysIdx, 2, stage, &synthetic_system_2);
  for (int i = 0; i < system_count[2]; i++)
    create_system(name, ++sysIdx, 3, stage, &synthetic_system_3);
  for (int i = 0; i < system_count[3]; i++)
    create_system(name, ++sysIdx, 4, stage, &synthetic_system_4);

  int totalSystemCount = 0;
  for (int i = 0; i < systemArgsCount; i++)
    totalSystemCount += system_count[i];

  ECS_LOG("created %d systems", totalSystemCount);
}

EVENT() init(const ecs::OnSceneCreated &)
{
  int actSystemCount[systemArgsCount] = {20, 15, 10, 5};
  int renderSystemCount[systemArgsCount] = {10, 10, 10, 10};
  init_systems("act_", "act_stage", actSystemCount);
  init_systems("render_", "render_stage", renderSystemCount);
  init_entities();
}

#include <profiler.h>
#include <parallel/thread_pool.h>
#include <mutex>
#include <shared_mutex>

static std::shared_mutex m;
static ecs::vector<ecs::QueryDescription*> currentTasks;

static bool is_executing(const ecs::QueryDescription *test_query)
{
  for (auto task : currentTasks)
    if (task == test_query)
      return true;
  return false;
}

static bool can_execute_query(const ecs::QueryDescription *test_query, int thread_id)
{
  for (auto previous_task : test_query->mtDescription.shouldWait)
    if (!previous_task->mtDescription.executed)
      return false;
  for (int i = 0, n = currentTasks.size(); i < n; i++)
  {
    if (currentTasks[i] == test_query)
      return false;
    if (i != thread_id)
    {
      for (auto task_with_race : test_query->mtDescription.hasDataRace)
        if (currentTasks[i] == task_with_race)
          return false;
    }
  }
  return true;
}

struct SystemQueue
{
  const ecs::vector<ecs::SystemDescription*> &systems;
  uint from, end, current;
  SystemQueue(const ecs::vector<ecs::SystemDescription*> &systems) :
    systems(systems), from(0), end(systems.size()), current(0)
  {

    for (auto sys : systems)
      sys->mtDescription.executed = false;
  }
  ~SystemQueue()
  {
    for (auto sys : systems)
      ECS_ASSERT(sys->mtDescription.executed==true);
  }

  ecs::SystemDescription* get_next(int worker_id)
  {
    while (from != end && (systems[from]->mtDescription.executed || is_executing(systems[from])))
      ++from;
    if (from == end)
      return nullptr;

    current = from;

    while (current != end)
    {
      if (!systems[current]->mtDescription.executed && can_execute_query(systems[current], worker_id))
      {
        ECS_ASSERT(!systems[current]->mtDescription.executed);
        currentTasks[worker_id] = systems[current];
        return systems[current];
      }
      else
      {
        ++current;
      }
    }
    return nullptr;
  }


};
static void finish_task(ecs::SystemDescription *task, int worker_id)
{
  OPTICK_EVENT("lock2");
  std::unique_lock write_lock(m);
  OPTICK_EVENT("under_lock2");
  currentTasks[worker_id] = nullptr;
  ECS_ASSERT(!task->mtDescription.executed);
  task->mtDescription.executed = true;

}

ecs::SystemDescription * select_task(ecs::list<SystemQueue> &queues, int worker_id)
{
  OPTICK_EVENT("lock");
  std::unique_lock read_write_lock(m);
  OPTICK_EVENT("under_lock");

  for (auto &q : queues)
    if (auto task = q.get_next(worker_id))
      return task;

  return nullptr;
}

void process_stage_queue(ecs::list<SystemQueue> &q, int worker_id)
{
  while (auto task = select_task(q, worker_id))
  {
    {
      OPTICK_EVENT_DYNAMIC(task->name.c_str());
      task->system();
    }
    finish_task(task, worker_id);
  }
}

void game_main_loop()
{
  static bool one_thread = false;
  one_thread = !one_thread;
  if (one_thread)
  {
    ecs::perform_stage("act_stage");
    ecs::perform_stage("render_stage");
    return;
  }
  auto &mgr = ecs::get_query_manager();
  auto actId = mgr.findStageId("act_stage");
  auto renderId = mgr.findStageId("render_stage");
  const auto &actSystems = mgr.activeSystems[actId];
  const auto &renderSystems = mgr.activeSystems[renderId];


  OPTICK_EVENT("multi_threading");
  int workerCount = get_worker_count()-1;
  currentTasks.assign(workerCount+1, nullptr);

  ecs::list<SystemQueue> q;
  q.emplace_back(actSystems);
  q.emplace_back(renderSystems);

  for (int workerId = 1; workerId <= workerCount; workerId++)
  {
    add_job([&q, workerId]()
    {
      process_stage_queue(q, workerId);
    });
  }

  process_stage_queue(q, 0);

  wait_jobs();

}


SYSTEM(stage=imgui_render) ecs_view()
{
  static std::optional<ImVec2> prevPressedCursor;
  static ImVec2 mouseDelta;

  if (ImGui::Begin("ecs view"))
  {
    ImVec2 curDelta;
    if (ImGui::IsMouseDown(1))
    {
      ImVec2 curPos = ImGui::GetMousePos();
      if (prevPressedCursor)
      {
        curDelta = ImVec2(curPos.x - prevPressedCursor->x, curPos.y - prevPressedCursor->y);
      }
      prevPressedCursor = curPos;
    }
    else
      prevPressedCursor = {};
    ImDrawList &list = *ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetWindowPos();
    mouseDelta.x += curDelta.x;
    mouseDelta.y += curDelta.y;
    pos.x += mouseDelta.x;
    pos.y += mouseDelta.y;
    list.AddText(ImVec2(pos.x + 20, pos.y + 20), IM_COL32_WHITE, "lol");



  }
  ImGui::End();
}