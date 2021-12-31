#include <ecs.h>
#include <application/time.h>
#include <3dmath.h>
#include "constanta.h"


struct DagorTestEntity
{
  mat4 transform;
  int iv = 10, ic2 = 10;
  vec3 p={1,0,0};
  mat4 d[9];
  vec3 v={1,0,0};
  int ivCopy = 10;
  virtual void update(float dt){ p += dt*v;}
  DagorTestEntity(int i) : iv(i), p(rand_vec3()), v(rand_vec3()), ivCopy(i){}
};

static vector<DagorTestEntity> list0;
static vector<DagorTestEntity*> list1;
static vector<DagorTestEntity*> list2;

volatile int cache0 = 0;
void prune_cache()
{
  return;
  static vector<int> memory;
  if (!memory.size())
    memory.resize(1<<20, 1);
  for (auto i:memory)
    cache0 += i;
}

EVENT(ecs::SystemTag::GameEditor) dag_init(const ecs::OnSceneCreated &)
{

  debug_log("struct sizeof = %d", sizeof(DagorTestEntity));

  {
    TimeScope a("ecs_create");
    for (uint i = 0; i < dagorEntityCount; i++)
    {
      ecs::create_entity<vec3, vec3>(
        {"p", rand_vec3()},
        {"v", rand_vec3()}
      );
    }
  }
  {
    TimeScope b("vector_structs_create");
    for (uint i = 0; i < dagorEntityCount; i++)
    {
      list0.emplace_back(DagorTestEntity(i));
    }
  }
  {
    TimeScope b("vector_pointers_create");
    for (uint i = 0; i < dagorEntityCount; i++)
    {
      list1.emplace_back(new DagorTestEntity(i));
    }
  }
  {
    TimeScope b("vector_pointers_create");
    for (uint i = 0; i < dagorEntityCount; i++)
    {
      list2.emplace_back(new DagorTestEntity(i));
    }
  }
  fflush(stdout);
}

static void process(float dt, vec3 &pos, const vec3 &vel)
{
  pos += vel * dt;
}

SYSTEM(ecs::SystemTag::GameEditor) dag_ecs_update(vec3 &p, const vec3 &v)
{
  process(Time::delta_time(), p, v);
}

SYSTEM(ecs::SystemTag::GameEditor) prune_cache0()
{
  prune_cache();
}

SYSTEM(ecs::SystemTag::GameEditor) dag_vector_structs_update()
{
  for (DagorTestEntity &entity : list0)
  {
    process(Time::delta_time(), entity.p, entity.v);
  }
}

SYSTEM(ecs::SystemTag::GameEditor) prune_cache1()
{
  prune_cache();
}

SYSTEM(ecs::SystemTag::GameEditor) dag_vector_pointers_update()
{
  for (auto &entity : list1)
  {
    process(Time::delta_time(), entity->p, entity->v);
  }
}

SYSTEM(ecs::SystemTag::GameEditor) prune_cache2()
{
  prune_cache();
}

SYSTEM(ecs::SystemTag::GameEditor) dag_vector_pointers_virtual_update()
{
  for (auto &entity : list2)
  {
    entity->update(Time::delta_time());
  }
}

SYSTEM(ecs::SystemTag::GameEditor) prune_cache3()
{
  prune_cache();
}
