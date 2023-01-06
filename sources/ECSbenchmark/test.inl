#include <ecs/ecs.h>
#include <ecs/registration.h>
#include <application/time.h>
#include <application/application.h>
#include <3dmath.h>
#include "constanta.h"
#include "data_block/data_block.h"
#include "transform.h"

ECS_REGISTER_TYPE(Data, "Data", ecs::PODType)

struct TestClass
{
  uint index;
  vec3 pos;
  vec3 vel;
  vec3 center;
  float m;
  Data data;
};

static std::vector<TestClass> list0;
static std::vector<TestClass*> list1;

EVENT() init(const ecs::OnSceneCreated &)
{
/*   DataBlock blk;

  blk.add<int>("a", 1);
  blk.add<float>("b", 2);
  blk.add<float>("b", 3);
  blk.add<string>("s", "kek");
  {
    DataBlock *block = blk.addBlock("bl");
  }
  debug_log("%d", blk.get<int>("a", 0));
  debug_log("%f", blk.get<float>("b", 0));
  debug_log("%f", blk.get<float>("c", 0));
  debug_log("%s", blk.get<string>("c", "lol").c_str());
  debug_log("%d", blk.getBlock("bl") ? 1 : 0);
  DataBlock blk2(root_path("ECSbenchmark/test.blk")); */

  debug_log("struct sizeof = %d", sizeof(TestClass));
  {
    TimeScope a("ecs_create");
    ecs::prefab_id tmpl = ecs::get_prefab_id("test_template");
    for (uint i = 0; i < entityCount; i++)
    {
      ecs::create_entity(tmpl, {
        {"index", i},
        {"pos", rand_vec3()},
        {"vel", rand_vec3()},
        {"center", rand_vec3()},
        {"m", rand_float(1.f, 10.f)}
      });
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

  ecs::prefab_id tmpl2 = ecs::create_entity_prefab(ecs::EntityPrefab("test2", {
    {"counter", 7},
    {"name", ecs::string("lolik")}
  }));

  ecs::create_entity(tmpl2);
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
SYSTEM(stage=act;) ecs_update(vec3 &pos, vec3 &vel, const vec3 &center, float m)
{
  process(Time::delta_time(), pos, vel, center, m);
}

SYSTEM(stage=act;) vector_structs_update()
{
  for (auto &entity : list0)
  {
    process(Time::delta_time(), entity.pos, entity.vel, entity.center, entity.m);
  }
}

SYSTEM(stage=act;) vector_pointers_update()
{
  for (auto entityPrt : list1)
  {
    auto &entity = *entityPrt;
    process(Time::delta_time(), entity.pos, entity.vel, entity.center, entity.m);
  }
}
#include <daScript/daScript.h>
#include <das_load.h>
#include <daScript/daScriptModule.h>


void require_game_das_modules()
{
  NEED_MODULE(Test_ECS);
}

void load_das_files()
{
  load_das_script(root_path("sources/ECSbenchmark/init.das").c_str());
}

#include <ecs/event_registration.h>
struct TestEvent : ecs::Event
{
  int inc;
};

ECS_EVENT_REGISTRATION(TestEvent)

SYSTEM(stage=act) test_event_send()
{
  ecs::send_event_immediate(TestEvent{{},13});
}


#include <ecs/request_registration.h>
struct TestRequest : ecs::Request
{
  int sum = 0;
};

ECS_REQUEST_REGISTRATION(TestRequest)

SYSTEM(stage=act) test_request_send()
{
  TestRequest r;
  ecs::send_request(r);
  //debug_log("sum = %d", r.sum);
}