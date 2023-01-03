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

#include <parallel/thread_pool.h>

EVENT() init_das(const ecs::OnSceneCreated &)
{
  das::daScriptEnvironment::ensure();
  das::daScriptEnvironment::bound->das_def_tab_size = 2;
  NEED_ALL_DEFAULT_MODULES;
  NEED_MODULE(Module_ECS);
  NEED_MODULE(Test_ECS);
  das::Module::Initialize();

  das::setDasRoot(eastl::string(root_path("sources/3rd_party/daScript").c_str()));
  
  setup_das_watcher();

  add_main_thread_job([]() {
    load_das_script(root_path("sources/ECSbenchmark/init.das").c_str());
  });
}

EVENT() term_das(const ecs::OnSceneTerminated &)
{
  clear_das_files();
  // shut-down daScript, free all memory
  das::Module::Shutdown();
  fflush(stdout);
}