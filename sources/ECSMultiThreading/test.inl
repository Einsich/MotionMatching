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
static void create_system(int system_idx, int args_count, const char *stage, F f)
{
  std::string name = "system_" + std::to_string(system_idx);

  ecs::vector<ecs::string> after;
  for (int i = 0, n = rand() % 2; system_idx != 0 && i < n; i++)
  {
    after.emplace_back("system_" + std::to_string(rand() % system_idx));
  }
  ecs::QueryCache *cache = new ecs::QueryCache();
  ecs::vector<ecs::ArgumentDescription> args;

  int intTypeIdx = ecs::get_type_index<int>();

  select_random_names (args_count,
  [&](const char *name)
  { args.emplace_back(name, intTypeIdx, ecs::AccessType::ReadWrite, false, false); });

  ECS_LOG("system_%d", system_idx);
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

static void init_systems()
{
  int systemCount[systemArgsCount] = {20, 15, 10, 5};

  int sysIdx = 0;
  for (int i = 0; i < systemCount[0]; i++)
    create_system(++sysIdx, 1, "one_thread", &synthetic_system_1);
  for (int i = 0; i < systemCount[1]; i++)
    create_system(++sysIdx, 2, "one_thread", &synthetic_system_2);
  for (int i = 0; i < systemCount[2]; i++)
    create_system(++sysIdx, 3, "one_thread", &synthetic_system_3);
  for (int i = 0; i < systemCount[3]; i++)
    create_system(++sysIdx, 4, "one_thread", &synthetic_system_4);

  int totalSystemCount = 0;
  for (int i = 0; i < systemArgsCount; i++)
    totalSystemCount += systemCount[i];

  ECS_LOG("created %d systems", totalSystemCount);
}

EVENT() init(const ecs::OnSceneCreated &)
{
  init_systems();
  init_entities();
}

#include <profiler.h>
#include <parallel/thread_pool.h>
#include <mutex>

static std::mutex m;
static ecs::vector<ecs::QueryDescription*> currentTasks;

static bool can_execute_query(const ecs::QueryDescription *test_query, int thread_id)
{
  for (auto previous_task : test_query->mtDescription.shouldWait)
    if (previous_task->mtDescription.pendingCount > 0)
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



void game_main_loop()
{
  static bool one_thread = false;
  one_thread = !one_thread;
  if (one_thread)
  {
    ecs::perform_stage("one_thread");
    return;
  }
  auto &mgr = ecs::get_query_manager();
  auto id = mgr.findStageId("one_thread");
  const auto &systems = mgr.activeSystems[id];

  if (systems.empty())
    return;
  OPTICK_EVENT("multi_threading");
  int workerCount = get_worker_count();
  currentTasks.assign(workerCount, nullptr);
  for (auto sys : systems)
    sys->mtDescription.pendingCount++;

  for (int workerId = 0; workerId < workerCount; workerId++)
  {
    add_job([&systems, workerId]()
    {
      uint from = 0;
      uint end = systems.size();
      while (from != end)
      {
        uint cur;
        {
          std::unique_lock read_write_lock(m);

          while (from != end && (systems[from]->mtDescription.pendingCount == 0))
            ++from;
          if (from == end)
            break;
          cur = from;
          while (cur != end)
          {
            if (systems[cur]->mtDescription.pendingCount == 0)
            {
              ++cur;
              continue;
            }

            if (can_execute_query(systems[cur], workerId))
            {
              ECS_ASSERT(systems[cur]->mtDescription.pendingCount == 1);
              currentTasks[workerId] = systems[cur];
              break;
            }
            else
            {
              ++cur;
            }
          }
        }
        if (cur == end)
        {
          //no task found
          continue;
        }
        auto system = systems[cur];
        {
          OPTICK_EVENT_DYNAMIC(system->name.c_str());
          system->system();
        }
        {
          std::unique_lock write_lock(m);
          currentTasks[workerId] = nullptr;
          system->mtDescription.pendingCount--;
          ECS_ASSERT(systems[cur]->mtDescription.pendingCount == 0);
        }
      }
    });
  }
  wait_jobs();

  for (auto sys : systems)
    ECS_ASSERT(sys->mtDescription.pendingCount==0);
}

