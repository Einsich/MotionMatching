#include "system_description.h"
#include "ecs_event_impl.h"

namespace ecs
{

  static void dfs(uint v, const vector<vector<uint>> &edges, vector<bool> &used, vector<uint> &answer)
  {
    used[v] = true;
    for (uint to : edges[v])
    {
      if (!used[to])
        dfs(to, edges, used, answer);
    }
    answer.push_back(v);
  }
  template<typename Description>
  static void topological_sort(Description** begin, Description** end)
  {
    auto size = end - begin;
    vector<vector<uint>> edge(size);
    vector<bool> used(size, false);
    vector<uint> answer;
    answer.reserve(size);
    map<string, int> nameMap;
    for (uint i = 0; i < size; i++)
      if (!begin[i]->isQuery)
        nameMap[begin[i]->name] = i;
    for (uint i = 0; i < size; i++)
    {
      for (const string &before : begin[i]->before)
      {
        auto it = nameMap.find(before);
        if (it != nameMap.end())
        {
          edge[it->second].push_back(i);
        }
        else
        {
          ECS_LOG("%s didn't exist for before %s", before.c_str(), begin[i]->name.c_str());
        }
      }
      for (const string &after : begin[i]->after)
      {
        auto it = nameMap.find(after);
        if (it != nameMap.end())
        {
          edge[i].push_back(it->second);
        }
        else
        {
          ECS_LOG("%s didn't exist for after %s", after.c_str(), begin[i]->name.c_str());
        }
      }
    }
    for (uint i = 0; i < size; i++)
    {
      if (!used[i])
        dfs(i, edge, used, answer);
    }
    ecs::vector<Description *> rightOrder(size);

    for (uint i = 0; i < size; i++)
      rightOrder[answer[i]] = begin[i];
    for (uint i = 0; i < size; i++)
      begin[i] = rightOrder[i];
  }

  static bool tagSatisfaction(const ecs::vector<ecs::string> &applicationTags, const ecs::vector<ecs::string> &systemTags)
  {
    for (const auto &tag : systemTags)
    {
      if (eastl::find(applicationTags.begin(), applicationTags.end(), tag) == applicationTags.end())
        return false;
    }
    return true;
  }
  static bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return strcmp(a->stage, b->stage) < 0;
  }

  void system_sort(const ecs::string &currentSceneTags, const ecs::vector<ecs::string> &applicationTags)
  {
    auto &systems = ecs::get_all_systems();
    systems.clear();
    bool isEditor = currentSceneTags != "editor";
    auto callableTest = [isEditor, &applicationTags, &currentSceneTags](const CallableDescription *callable)
    {
      return tagSatisfaction(applicationTags, callable->tags) &&
        ((callable->scenes.empty() && isEditor) ||std::find(callable->scenes.begin(), callable->scenes.end(), currentSceneTags)
          != callable->scenes.end());
    };
    for (SystemDescription *callable : get_all_mutable_systems())
    {
      if (callableTest(callable))
        systems.push_back(callable);
    }
    for (auto &[srcHandlers, filteredHandlers] : get_all_event_handlers())
    {
      filteredHandlers.clear();
      for (EventDescription *handler : srcHandlers)
        if (callableTest(handler))
          filteredHandlers.push_back(handler);
    }

    std::sort(systems.begin(), systems.end(), system_comparator);

    auto cmp = [](const SystemDescription *stage, const SystemDescription *a){return strcmp(a->stage, stage->stage) > 0;};

    auto begin = systems.begin();
    while (begin != systems.end())
    {
      auto end = eastl::upper_bound(begin, systems.end(), *begin, cmp);
      topological_sort(begin, end);
      begin = end;
    }
    for (auto &[srcHandlers, filteredHandlers] : get_all_event_handlers())
    {
      topological_sort(filteredHandlers.begin(), filteredHandlers.end());
    }
  }

  void clear_system_archetypes_cache()
  {

    for (QueryDescription *query: ecs::all_queries())
      query->archetypes.clear();
    for (SystemDescription *system: ecs::get_all_mutable_systems())
      system->archetypes.clear();

    for (auto &[srcHandlers, filteredHandlers] : get_all_event_handlers())
    {
      for (EventDescription *handler : srcHandlers)
        handler->archetypes.clear();
    }
  }
}