#pragma once
#include <iterator>
#include "../system_order.h"
#include "archetype.h"

namespace ecs
{
  struct FunctionArgument
  {
    string_hash descr;
    bool optional = false;
  };
  struct SystemCashedArchetype
  {
    Archetype *archetype;
    std::vector<ComponentContainer*> containers;
    SystemCashedArchetype(Archetype *archetype, std::vector<ComponentContainer*> &&containers);
  };

  struct QueryIterator;

  struct QueryDescription
  {
    std::string name;
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    bool withArgs;
    uint realArgs;
    QueryDescription(const char *name, const std::vector<FunctionArgument> &args, bool query = true);
    QueryIterator begin();
    QueryIterator end();
  };
  struct SingleQueryDescription
  {
    std::string name;
    std::vector<FunctionArgument> args;
    std::vector<SystemCashedArchetype> archetypes;
    void (*function)();
    bool withArgs;
    uint realArgs;
    SingleQueryDescription(const char *name, const std::vector<FunctionArgument> &args, bool query = true);
    QueryIterator begin();
    QueryIterator end();
  };
  struct SystemDescription : QueryDescription
  {
    void (*function)();
    int order;
    uint tags;
    SystemDescription(const char *name, const std::vector<FunctionArgument> &args, void (*function_pointer)(), int order, uint tags);
    void execute();
  };

  
  struct QueryIterator
  {
    friend struct QueryDescription;
  public:
    QueryIterator() = default;
    QueryIterator(const QueryDescription *query, int archetype, int component);

    bool operator!=(const QueryIterator &other) const;
    void operator++();
    template<typename T, uint ind>
    T *get_component()
    {
      if constexpr (is_singleton<T>::value)
        return &get_singleton<T>();
      else
      {
        return (dataArrays[ind].second) ? (T*)(*dataArrays[ind].first)[binIndex] + inBinIndex : nullptr;
      }
    }
    void set_archetype();
  private:
    const QueryDescription *query;
    const SystemCashedArchetype *archetype;
    vector<pair<vector<void*>*, bool>>dataArrays;
    uint componentCount;
    uint archetypeIndex, componentIndex, binIndex, inBinIndex;
    void skip_empty_archetype();
  };
}