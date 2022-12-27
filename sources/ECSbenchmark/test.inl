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

static vector<TestClass> list0;
static vector<TestClass*> list1;

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


das::shared_ptr<DasFile> file;
das::SimFunction *das_process_function;

EVENT() init_das(const ecs::OnSceneCreated &)
{
  das::daScriptEnvironment::ensure();
  das::daScriptEnvironment::bound->das_def_tab_size = 2;
  NEED_ALL_DEFAULT_MODULES;
  NEED_MODULE(Module_ECS);
  das::Module::Initialize();

  das::setDasRoot(root_path("sources/3rd_party/daScript"));
  auto ecsFile = load_das_script(root_path("sources/ECSbenchmark/ecs.das").c_str());
  file = load_das_script(root_path("sources/ECSbenchmark/init.das").c_str());
return;

  if (file)
  {
    for (uint32_t i = 0, n = file->get_function_count(); i < n; i++)
    {
      das::SimFunction *function = file->get_function(i);
      das::FuncInfo &info = *function->debugInfo;
      debug_log("name %s stackSize %d, count %d, locCount %d", function->name, function->stackSize, info.count, info.localCount);
      for (int j = 0, m = info.count; j < m; j++)
      {
        break;
        das::VarInfo &var = *info.fields[j];
        debug_log("    name %s size %d const %d pod %d ref %d cppName %s",
          var.name, var.size, var.isConst(), var.isPod(), var.isRef(), das::das_to_string(var.type).c_str());
      }
      bool verification = verify<void, float, float, das::float3 &, das::float3 &, das::float3>(*file, function);
      debug_log("verification %d", verification);
      if (verification)
      {
        float dt = 0.1;
        float m = 1.f;
        das::float3 p(0,1,2), v(-1,1,-1), c(100,100,100);

        vec4f args[5] = {
          das::cast<float>::from(dt),
          das::cast<float>::from(m),
          das::cast<das::float3&>::from(p),
          das::cast<das::float3&>::from(v),
          das::cast<das::float3>::from(c)
        };
        file->ctx->eval(function, args);
        if (auto ex = file->ctx->getException()) {       // if function cased panic, report it
          debug_log("exception: %s", ex);
        }
        else
        {
          das_process_function = function;
          debug_log("dt = %f, m = %f, pos = %f,%f,%f, vel = %f,%f,%f, center = %f,%f,%f",
          dt, m, p.x, p.y, p.z, v.x, v.y, v.z, c.x, c.y,c.z);
        }
      }
    }
  }
}

EVENT() term_das(const ecs::OnSceneTerminated &)
{
  clear_das_files();
  file.reset();
  // shut-down daScript, free all memory
  das::Module::Shutdown();
}


SYSTEM(stage=act;) das_update(vec3 &pos, vec3 &vel, const vec3 &center, float m)
{return;
  vec4f args[5] = {
    das::cast<float>::from(Time::delta_time()),
    das::cast<float>::from(m),
    das::cast<das::float3&>::from((das::float3&)pos),
    das::cast<das::float3&>::from((das::float3&)vel),
    das::cast<das::float3>::from((const das::float3&)center)
  };
  if (file)
    file->ctx->eval(das_process_function, args);
}
