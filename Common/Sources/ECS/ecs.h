#include <vector>
#include <map>
#include <set>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <assert.h>
using namespace std;

#define type_id(T) type_index(typeid(T))

class Entity
{
private:
  int entityID;
public:
  Entity():
  entityID(-1)
  {}
  Entity(int eid):
  entityID(eid)
  {}
  int eid() const
  {
    return entityID;
  }
  int operator<(const Entity & other) const
  {
    return entityID < other.entityID;
  }
};
class EntitySignature
{
  map<pair<type_index, string>, int> signature;
public:
  EntitySignature()
  {
    signature.try_emplace({type_id(Entity), "eid"}, 0);
  }
  template<typename T>
  int get_component_index(const string& name)
  {
    auto it = signature.find({type_id(T), name});
    return it == signature.end() ? -1 : it->second;
  }
  bool contains(const type_index& type, const string& name) const
  {
    return signature.find({type, name}) != signature.end();
  }
  template<typename T>
  bool add_type(const string& name, int index)
  {
    auto it = signature.try_emplace({type_id(T), name}, index);
    if (!it.second)
    {
      printf("Add error type %s with name %s", typeid(T).name(), name.c_str());
    }
    return it.second;
  }
  template<typename T>
  bool remove_type(const string& name)
  {
    auto it = signature.find({type_id(T), name});
    bool erased = it != signature.end();
    if (erased)
    {
      signature.erase(it);
    }
    else
    {
      printf("Remove error type %s with name %s", typeid(T).name(), name.c_str());
    }
    return erased;
  }
};
class SystemSignature
{
  set<tuple<type_index, string, bool>> signature;
public:
  SystemSignature(const set<tuple<type_index, string, bool>>& signature):
    signature(signature){}
  bool allow_entity(const EntitySignature& entity_signature)
  {
    for(auto &s : signature)
    {
      if (!get<2>(s) && !entity_signature.contains(get<0>(s), get<1>(s)))
        return false;
    }
    return true;
  }
};
template<typename T>
class ComponentStore
{
private:
  inline static vector<T> components;
public:
  static T* get_component(int i)
  {
    assert(i < (int)components.size());
    return i < 0 ? nullptr : &components[i];
  }
  template<typename ...Args>
  static int add_component(Args...args)
  {
    components.emplace_back(T(args...));
    return components.size() - 1;
  }
};

class System
{
private:
  SystemSignature signature;
  set<Entity> entities;
public:
  System(const SystemSignature & signature):
  signature(signature){}

  void update_entity(const Entity &entity, const EntitySignature & entity_signature)
  {
    if (signature.allow_entity(entity_signature))
    {
      entities.insert(entity);
    }
    else
    {
      entities.erase(entity);
    }    
  }
  const set<Entity>& get_entities() const
  {
    return entities;
  }
};
void foo()
{

}
void test_system(Entity eid, int x, float&y, string * s)
{
  printf("%s\n", __PRETTY_FUNCTION__);
  printf("eid = %d, x = %d, y = %f, s = %s \n", eid.eid(), x, y, s == nullptr ? "null" : s->c_str());
  y++;
  x++;
  if (s)
    *s += ")";
}
class EntityManager
{
  map<Entity, EntitySignature> entities;
  int entityCounter = 1;
#define GetComponent(T, name, entity) (ComponentStore<T>::get_component(entities[entity].get_component_index<T>(#name)))
#define AddComponent(T, name, entity, ...) (entities[entity].add_type<T>(#name, ComponentStore<T>::add_component(__VA_ARGS__)))
#define RemoveComponent(T, name, eid) entities[eid].remove_type<T>(#name)

  vector<System> systems;
  const Entity& add_entity()
  {
    auto e = entities.emplace(Entity(entityCounter), EntitySignature());
    return e.first->first;
  }
  void init_entities()
  {
    auto e = add_entity();
    AddComponent(int, x, e, 1);
    AddComponent(float, y, e, 1.4f);
    for(System& system : systems)
    {
      system.update_entity(e, entities[e]);
    }
  }
  void init_systems()
  {
    systems.emplace_back(SystemSignature({{type_id(Entity), "eid", false}, {type_id(int), "x", false}, {type_id(float&), "y", false}, {type_id(string), "s", true}}));

  }
  void execute_systems()
  {
    for(const System& system : systems)
    {
      for (const Entity& entity: system.get_entities())
      {
        test_system(entity, *GetComponent(int, x, entity), *GetComponent(float, y, entity), GetComponent(string, s, entity));

      }
    }
  }
public:

  void test()
  {
    init_systems();
    init_entities();
    auto e = entities.begin()->first;
    AddComponent(string, s, e, "kek");
    execute_systems();
    execute_systems();
    execute_systems();
    execute_systems();
    RemoveComponent(string, s, e);
    execute_systems();
    fflush(stdout);
  }

};

