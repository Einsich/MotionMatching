#include <ecs.h>
#include <application/time.h>
#include <3dmath.h>
struct Data
{
  //mat4 data[8];
};
REGISTER_TYPE(Data)
EDIT_STUB(Data)

struct TestClass
{
  uint index;
  Data data0;
  vec3 pos;
  Data data1;
  vec3 vel;
  Data data2;
  vec3 center;
  Data data3;
  float m;
  Data data4;
};

static vector<TestClass> list0;
static vector<TestClass*> list1;

EVENT(ecs::SystemTag::GameEditor) init(const ecs::OnSceneCreated &)
{
  debug_log("struct sizeof = %d", sizeof(TestClass));
  uint entityCount = 500000;
  {
    TimeScope a("ecs_create");
    for (uint i = 0; i < entityCount; i++)
    {
      ecs::create_entity<uint, Data, vec3, Data, vec3, Data, vec3, Data, float, Data>(
        {"index", i},
        {"data0", Data{}},
        {"pos", rand_vec3()},
        {"data1", Data{}},
        {"vel", rand_vec3()},
        {"data2", Data{}},
        {"center", rand_vec3()},
        {"data3", Data{}},
        {"m", rand_float(1.f, 10.f)},
        {"data4", Data{}}
      );
    }
  }
  {
    TimeScope b("vector_structs_create");
    for (uint i = 0; i < entityCount; i++)
    {
      list0.emplace_back(TestClass{
        i,
        Data{},
        rand_vec3(),
        Data{},
        rand_vec3(),
        Data{},
        rand_vec3(),
        Data{},
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
        Data{},
        rand_vec3(),
        Data{},
        rand_vec3(),
        Data{},
        rand_vec3(),
        Data{},
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