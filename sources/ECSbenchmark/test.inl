#include <ecs.h>
#include <application/time.h>
#include <3dmath.h>
#include "constanta.h"
REGISTER_TYPE(Data)
EDIT_STUB(Data)

struct TestClass
{
  uint index;
  vec3 pos;
  vec3 vel;
  vec3 center;
  float m;
  Data data4;
};

static vector<TestClass> list0;
static vector<TestClass*> list1;

EVENT(ecs::SystemTag::GameEditor) init(const ecs::OnSceneCreated &)
{
  debug_log("struct sizeof = %d", sizeof(TestClass));

  {
    TimeScope a("ecs_create");
    for (uint i = 0; i < entityCount; i++)
    {
      ecs::create_entity<uint, vec3, vec3, vec3, float, Data>(
        {"index", i},
        {"pos", rand_vec3()},
        {"vel", rand_vec3()},
        {"center", rand_vec3()},
        {"m", rand_float(1.f, 10.f)},
        {"data", Data{}}
      );
    }
  }
  {
    TimeScope b("vector_structs_create");
    for (uint i = 0; i < entityCount; i++)
    {
      list0.emplace_back(TestClass{
        i,
        rand_vec3(),
        rand_vec3(),
        rand_vec3(),
        rand_float(1.f, 10.f),
        Data{}
      });
    }
  }
  {
    TimeScope b("vector_pointers_create");
    for (uint i = 0; i < entityCount; i++)
    {
      list1.push_back(new TestClass{
        i,
        rand_vec3(),
        rand_vec3(),
        rand_vec3(),
        rand_float(1.f, 10.f),
        Data{}
      });
    }
  }
  fflush(stdout);
}
static void process(float dt, vec3 &pos, vec3 &vel, const vec3 &center, float m)
{
  vec3 dir = center - pos;
  float distance = length(dir);
  vec3 F = m * 0.0001f / (distance*distance*distance) * dir;
  vec3 a = F / m;
  vel += a * dt;
  pos += vel * dt;
}
SYSTEM(ecs::SystemTag::GameEditor) ecs_update(vec3 &pos, vec3 &vel, const vec3 &center, float m)
{
  process(Time::delta_time(), pos, vel, center, m);
}

SYSTEM(ecs::SystemTag::GameEditor) vector_structs_update()
{
  for (auto &entity : list0)
  {
    process(Time::delta_time(), entity.pos, entity.vel, entity.center, entity.m);
  }
}

SYSTEM(ecs::SystemTag::GameEditor) vector_pointers_update()
{
  for (auto entityPrt : list1)
  {
    auto &entity = *entityPrt;
    process(Time::delta_time(), entity.pos, entity.vel, entity.center, entity.m);
  }
}