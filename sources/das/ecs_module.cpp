#include <daScript/daScript.h>
#include <ecs/ecs.h>
#include <ecs/archetype_manager.h>
#include "das_load.h"

MAKE_EXTERNAL_TYPE_FACTORY(Function, das::Function)

using namespace das;

static das::ContextPtr sharedContext;

ecs::vector<ecs::string> to_ecs_string_array(const das::Array &a, const char *descr)
{
  ecs::vector<ecs::string> v(a.size);
  const char **data = (const char **)a.data;
  for (int i = 0, n = a.size; i < n; i++)
  {
    v[i] = data[i];
    printf("%s %s\n", descr, data[i]);
  }
  return v;
}

ecs::vector<ecs::ComponentDescription> to_ecs_components(const das::Array &a, const char *descr)
{
  ecs::vector<ecs::ComponentDescription> v;
  struct Arg
  {
    const char *name;
    const char *type;
  };
  const Arg *data = (const Arg *)a.data;
  for (int i = 0, n = a.size; i < n; i++)
  {
    v.emplace_back(data[i].name, ecs::type_name_to_index(data[i].type));
    printf("%s %s : %s\n", descr, data[i].name, data[i].type);
  }
  return v;
}

const char* get_das_type_name(const das::TypeDecl &type)
{
  type.baseType;
  switch (type.baseType)
  {
  case das::Type::tHandle: return type.annotation->name.c_str(); break;
  case das::Type::tInt: return "int"; break;
  case das::Type::tInt2: return "int2"; break;
  case das::Type::tInt3: return "int3"; break;
  case das::Type::tInt4: return "int4"; break;
  case das::Type::tFloat: return "float"; break;
  case das::Type::tFloat2: return "vec2"; break;
  case das::Type::tFloat3: return "vec3"; break;
  case das::Type::tFloat4: return "vec4"; break;
  case das::Type::tDouble: return "double"; break;
  case das::Type::tString: return "string"; break;
  case das::Type::tBool: return "bool"; break;
  
  default: return "unsopported type"; break;
  } 
}
ecs::vector<ecs::ArgumentDescription> to_ecs_arguments(const das::FunctionPtr &function)
{
  printf("%s\n", function->describe().c_str());
  ecs::vector<ecs::ArgumentDescription> v;
  for (const auto &arg : function->arguments)
  {
    ecs::AccessType access = ecs::AccessType::Copy;
    const auto&type = *arg->type;
    if (type.isRef())
    {
       access = type.isConst() ? ecs::AccessType::ReadOnly : ecs::AccessType::ReadWrite;
    }
    bool optional = type.isPointer();
    const char *typeName = get_das_type_name(optional ? *type.firstType : type);
    const auto &a = v.emplace_back(arg->name.c_str(),
      ecs::type_name_to_index(typeName), access, optional, false);

    printf("comp %s, type %s<%d>, optional %d, %s\n", a.name.c_str(), typeName, a.typeIndex, optional, 
    access == ecs::AccessType::Copy ? "Copy" :
     (access == ecs::AccessType::ReadOnly ? "ReadOnly" : "ReadWrite"));
  }
  return v;
}


static thread_local das::vector<ecs::SystemDescription> unresolvedSystems;

void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system)
{
  const char *name = system->name.c_str();
  printf("system %s, stage %s\n", name, stage);

  
  unresolvedSystems.emplace_back(system->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(system),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      stage,
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr);
       
}

static void perform_ecs_loop(const ecs::QueryCache &cache, const das::ContextPtr &ctx, const das::SimFunction *loopFunc)
{
  constexpr int ArgsCnt = 32;
  vec4f args[ArgsCnt];
  if (cache.noArchetype)
  {
    *args = das::cast<uint32_t>::from(1u);
    ctx->call(loopFunc, args, nullptr);
    return;
  }
  const ecs::ArchetypeManager &manager = ecs::get_archetype_manager();
 
  for (const auto &p : cache.archetypes)
  {
    const auto &cachedArchetype = p.second;
    const ecs::Archetype &archetype = manager.archetypes[p.first];
    if (archetype.entityCount == 0)
      continue;


    ecs::uint binN = archetype.entityCount >> archetype.chunkPower;
    for (ecs::uint binIdx = 0; binIdx < binN; ++binIdx)
    {
      vec4f *argPtr = args;
      *argPtr++ = das::cast<uint32_t>::from(archetype.chunkSize);
      for (int cmpIdx : cachedArchetype)
      {
        *argPtr++ = das::cast<void*>::from(archetype.components[cmpIdx].data[binIdx]);
      }
      ctx->call(loopFunc, args, nullptr);
      
    }
    ecs::uint lastBinSize = archetype.entityCount - (binN << archetype.chunkPower);
    if (lastBinSize > 0)
    {
      vec4f *argPtr = args;
      *argPtr++ = das::cast<uint32_t>::from(lastBinSize);
      for (int cmpIdx : cachedArchetype)
      {
        *argPtr++ = das::cast<void*>::from(archetype.components[cmpIdx].data[binN]);
      }
      ctx->call(loopFunc, args, nullptr);
    }
  }
}


void resolve_systems(const das::ContextPtr &ctx, DasFile &file)
{

  for(auto &system : file.resolvedSystems)
  {
    ECS_ASSERT(ecs::remove_system(system, true));
  }
  file.resolvedSystems.clear();


  for(auto &system : unresolvedSystems)
  {
    auto systemName = system.name + "`implementation";
    auto loopFunc = ctx->findFunction(systemName.c_str());
    if (loopFunc == nullptr)
    {
      continue;
    }
    system.system = [cache = system.cache, ctx, loopFunc]()
      {
        perform_ecs_loop(*cache, ctx, loopFunc);
      };
    file.resolvedSystems.emplace_back(ecs::register_system(std::move(system)));
  }
  unresolvedSystems.clear();
}

class Module_ECS : public Module
{
public:
  Module_ECS() : Module("ecs_impl")
  {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(register_system)>(*this, lib, "register_system", SideEffects::accessExternal, "register_system");

    verifyAotReady();
    sharedContext = das::make_shared<das::Context>(4 * 1024);
  }
  virtual ModuleAotType aotRequire(TextWriter &tw) const override
  {
    // specifying which include files are required for this module
    // tw << "#include \"tutorial02aot.h\"\n";
    // specifying AOT type, in this case direct cpp mode (and not hybrid mode)
    return ModuleAotType::cpp;
  }
};

REGISTER_MODULE(Module_ECS);