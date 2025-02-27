#include <daScript/daScript.h>
#include <ecs/ecs.h>
#include <ecs/archetype_manager.h>
#include "das_load.h"
#include "das_modules/ecs_module.h"

static bool log_system_arguments = false;

int get_event_sizeof(int type_id)
{
  const auto &types = ecs::get_all_registered_events();
  return (uint32_t)type_id < types.size() ? types[type_id].sizeOf : 0;
}

int get_request_sizeof(int type_id)
{
  const auto &types = ecs::get_all_registered_requests();
  return (uint32_t)type_id < types.size() ? types[type_id].sizeOf : 0;
}

static ecs::vector<ecs::string> to_ecs_string_array(const das::Array &a, const char *descr)
{
  ecs::vector<ecs::string> v(a.size);
  const char **data = (const char **)a.data;
  for (int i = 0, n = a.size; i < n; i++)
  {
    v[i] = data[i];
    if (log_system_arguments)
      printf("%s %s\n", descr, data[i]);
  }
  return v;
}

const char * remap_type_name(const char *name)
{
  //remap
  constexpr eastl::string_view ecsStr = "ecs_string";
  if (name == ecsStr)
    return "string";
  return name;
}

static ecs::vector<ecs::ComponentDescription> to_ecs_components(const das::Array &a, const char *descr)
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
    int typeId = ecs::type_name_to_index(remap_type_name(data[i].type));
    if (typeId >= 0)
      v.emplace_back(data[i].name, typeId);
    else
      ECS_ERROR("undefined type \"%s\"", data[i].name);
    if (log_system_arguments)
      printf("%s %s : %s\n", descr, data[i].name, data[i].type);
  }
  return v;
}


static ecs::vector<ecs::ArgumentDescription> to_ecs_arguments(const das::vector<das::VariablePtr> &arguments, bool with_first_arg = true)
{
  ecs::vector<ecs::ArgumentDescription> v;
  for (auto it = arguments.begin() + (with_first_arg ? 0 : 1), to = arguments.end(); it != to; ++it)
  {
    const auto &arg = *it;
    ecs::AccessType access = ecs::AccessType::Copy;
    const auto&type = *arg->type;
    if (type.baseType == das::Type::tString)
      access = ecs::AccessType::ReadOnly;
    if (type.isRef())
    {
       access = type.isConst() ? ecs::AccessType::ReadOnly : ecs::AccessType::ReadWrite;
    }
    bool optional = type.isPointer();
    const char *typeName = get_das_type_name(optional ? *type.firstType : type);


    const auto &a = v.emplace_back(arg->name.c_str(),
      ecs::type_name_to_index(typeName), access, optional, false);

    if (log_system_arguments)
    {
      printf("comp %s, type %s<%d>, optional %d, %s\n", a.name.c_str(), typeName, a.typeIndex, optional,
        access == ecs::AccessType::Copy ? "Copy" :
        (access == ecs::AccessType::ReadOnly ? "ReadOnly" : "ReadWrite"));
    }
  }
  return v;
}

static ecs::vector<ecs::string> get_event_types(const char *def_name, const das::VariablePtr &var, ecs::vector<ecs::string> &&on_event)
{
  ecs::vector<ecs::string> v;
  ecs::string name = get_das_type_name(*var->type);
  if (name == def_name)
  {
    assert(!on_event.empty());
    return std::move(on_event);
  }
  else
  {
    assert(on_event.empty() && "on_event|on_request should be empty");
    return { name };
  }
}

struct UnresolvedSystem
{
  ecs::SystemDescription descr;
  uint64_t mangledHash;
  UnresolvedSystem(ecs::SystemDescription&&descr, uint64_t hash) : descr(std::move(descr)), mangledHash(hash) {}
};
struct UnresolvedEvent
{
  ecs::EventDescription descr;
  ecs::vector<ecs::string> types;
  uint64_t mangledHash;
  UnresolvedEvent(ecs::EventDescription &&descr, ecs::vector<ecs::string> &&types, uint64_t hash) :
     descr(std::move(descr)), types(std::move(types)), mangledHash(hash) {}
};
struct UnresolvedRequest
{
  ecs::RequestDescription descr;
  ecs::vector<ecs::string> types;
  uint64_t mangledHash;
  UnresolvedRequest(ecs::RequestDescription &&descr, ecs::vector<ecs::string> &&types, uint64_t hash) :
     descr(std::move(descr)), types(std::move(types)), mangledHash(hash) {}
};

static thread_local das::vector<ecs::QueryDescription> unresolvedQueries;
static thread_local das::vector<UnresolvedSystem> unresolvedSystems;
static thread_local das::vector<UnresolvedEvent> unresolvedEvents;
static thread_local das::vector<UnresolvedRequest> unresolvedRequests;


static void clear_unresolved_systems()
{
  unresolvedSystems.clear();
  unresolvedQueries.clear();
  unresolvedEvents.clear();
  unresolvedRequests.clear();
}


void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system,
    uint64_t mangled_hash)
{
  const char *name = system->name.c_str();
  stage = stage ? stage : "";
  printf("System(stage=%s): %s\n", stage, system->describe().c_str());

  unresolvedSystems.emplace_back(ecs::SystemDescription(system->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(system->arguments),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      stage,
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr), mangled_hash);
}

bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event,
    uint64_t mangled_hash)
{
  if (event->arguments.size() == 0)
    return false;

  const char *name = event->name.c_str();
  printf("Event: %s\n", event->describe().c_str());

  unresolvedEvents.emplace_back(ecs::EventDescription(event->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(event->arguments, false),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr,
      nullptr),
      get_event_types("Event", event->arguments[0], to_ecs_string_array(on_event, "on_event")),
      mangled_hash);

  return true;
}

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request,
    uint64_t mangled_hash)
{
  if (request->arguments.size() == 0)
    return false;

  const char *name = request->name.c_str();
  printf("Request: %s\n", request->describe().c_str());

  unresolvedRequests.emplace_back(ecs::RequestDescription(request->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(request->arguments, false),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr,
      nullptr),
      get_event_types("Request", request->arguments[0], to_ecs_string_array(on_request, "on_request")),
      mangled_hash);

  return true;
}

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block)
{
  auto name = block->at.fileInfo->name + ":" + eastl::to_string(block->at.line);
  printf("Query: %s\n", name.c_str());
  auto cache = new ecs::QueryCache();
  unresolvedQueries.emplace_back(block->at.fileInfo->name.c_str(), name.c_str(), cache,
      to_ecs_arguments(block->arguments),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"));
  block->annotationData = intptr_t(cache);
  auto mangledName = block->getMangledName(true,true);

  block->annotationDataSid = das::hash_blockz64((uint8_t *)mangledName.c_str());
}

template<typename C>
static void perform_ecs_loop(const ecs::QueryCache &cache, C call, vec4f *event = nullptr)
{
  vec4f args[DAS_MAX_FUNCTION_ARGUMENTS];
  uint32_t argumentOffset = 1;
  if (event)
  {
    args[1] = *event;
    argumentOffset = 2;
  }
  if (cache.noArchetype)
  {
    args[0] = das::cast<uint32_t>::from(1u);
    call(args);
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
      args[0] = das::cast<uint32_t>::from(archetype.chunkSize);
      vec4f *argPtr = args + argumentOffset;
      for (int cmpIdx : cachedArchetype)
      {
        *argPtr++ = das::cast<void*>::from(archetype.components[cmpIdx].data[binIdx]);
      }
      call(args);

    }
    ecs::uint lastBinSize = archetype.entityCount - (binN << archetype.chunkPower);
    if (lastBinSize > 0)
    {
      args[0] = das::cast<uint32_t>::from(lastBinSize);
      vec4f *argPtr = args + argumentOffset;
      for (int cmpIdx : cachedArchetype)
      {
        *argPtr++ = das::cast<void*>::from(archetype.components[cmpIdx].data[binN]);
      }
      call(args);
    }
  }
}

template<typename C>
static void perform_ecs_eid_query(ecs::EntityId eid, const ecs::QueryCache &cache, C call, vec4f *event = nullptr)
{
  vec4f args[DAS_MAX_FUNCTION_ARGUMENTS];
  uint32_t argumentOffset = 1;
  if (event)
  {
    args[1] = *event;
    argumentOffset = 2;
  }
  ecs::uint archetypeIdx, index;
  ecs::EntityState state;
  bool acceptDestoyQueue = false;
  if (eid.get_info(archetypeIdx, index, state) &&
      (state == ecs::EntityState::CreatedAndInited || (acceptDestoyQueue && state == ecs::EntityState::InDestroyQueue)))
  {
    auto it = cache.archetypes.find(archetypeIdx);
    if (it != cache.archetypes.end())
    {
      const ecs::ArchetypeManager &manager = ecs::get_archetype_manager();
      const ecs::Archetype &archetype = manager.archetypes[archetypeIdx];
      const auto &types = ecs::get_all_registered_types();
      const auto &cachedArchetype = it->second;
      ecs::uint binIdx = index >> archetype.chunkPower;
      ecs::uint inBinIdx = index & archetype.chunkMask;

      args[0] = das::cast<uint32_t>::from(1);
      vec4f *argPtr = args + argumentOffset;
      for (int cmpIdx : cachedArchetype)
      {
        const auto &cmp = archetype.components[cmpIdx];

        *argPtr++ = das::cast<void*>::from(cmp.get(binIdx, inBinIdx, types[cmp.description.typeIndex].sizeOf));
      }
      call(args);
    }
  }
}

void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line)
{
  auto closureBlock = (das::SimNode_ClosureBlock*)block.body;
  auto cache = (ecs::QueryCache*)closureBlock->annotationData;
  if (cache == nullptr)
  {
    ECS_ERROR("missed [eq] annotation in ecs query", line->fileInfo->name.c_str(), line->line);
    return;
  }

  perform_ecs_loop(*cache, [&](vec4f *args) { context->invoke(block, args, nullptr, line);});
}

void perform_eid_query(ecs::EntityId eid, const das::Block &block, das::Context *context, das::LineInfoArg *line)
{
  auto closureBlock = (das::SimNode_ClosureBlock*)block.body;
  auto cache = (ecs::QueryCache*)closureBlock->annotationData;
  perform_ecs_eid_query(eid, *cache, [&](vec4f *args) { context->invoke(block, args, nullptr, line);});
}


void resolve_systems(const das::ContextPtr &ctx, DasFile &file)
{
  for(auto &system : file.resolvedQueries)
  {
    bool result = ecs::remove_query(system, true);
    ECS_ASSERT(result);
  }
  for(auto &system : file.resolvedSystems)
  {
    bool result = ecs::remove_system(system, true);
    ECS_ASSERT(result);
  }
  for(auto &system : file.resolvedEvents)
  {
    bool result = ecs::remove_event(system, true);
    ECS_ASSERT(result);
  }
  for(auto &system : file.resolvedRequests)
  {
    bool result = ecs::remove_request(system, true);
    ECS_ASSERT(result);
  }
  file.clearResolvedSystems();

  for(auto &system : unresolvedQueries)
  {
    file.resolvedQueries.emplace_back(ecs::register_query(
      std::move(system.file),
      std::move(system.name),
      system.cache,
      std::move(system.arguments),
      std::move(system.requiredComponents),
      std::move(system.requiredNotComponents)
      ));
  }

  for(auto &[system, mangledHash] : unresolvedSystems)
  {
    auto loopFunc = ctx->fnByMangledName(mangledHash);
    if (loopFunc == nullptr)
    {
      continue;
    }

    file.resolvedSystems.emplace_back(ecs::register_system(
      std::move(system.file),
      std::move(system.name),
      system.cache,
      std::move(system.arguments),
      std::move(system.requiredComponents),
      std::move(system.requiredNotComponents),
      std::move(system.stage),
      std::move(system.before),
      std::move(system.after),
      std::move(system.tags),
      [cache = system.cache, ctx, loopFunc]()
      {
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);});
      }));
  }

  for (auto &[system, types, mangledHash] : unresolvedEvents)
  {
    auto loopFunc = ctx->fnByMangledName(mangledHash);
    if (loopFunc == nullptr)
    {
      continue;
    }

    for (const auto &type : types)
    {
      auto copy = system;
      int eventId = ecs::event_name_to_index(type.c_str());
      file.resolvedEvents.emplace_back(ecs::register_event(
      std::move(copy.file),
      std::move(copy.name),
      copy.cache,
      std::move(copy.arguments),
      std::move(copy.requiredComponents),
      std::move(copy.requiredNotComponents),
      std::move(copy.before),
      std::move(copy.after),
      std::move(copy.tags),
      [cache = copy.cache, ctx, loopFunc](const ecs::Event &event)
      {
        vec4f eventArg = das::cast<const ecs::Event &>::from(event);
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      },
      [cache = copy.cache, ctx, loopFunc](ecs:: EntityId eid, const ecs::Event &event)
      {
        vec4f eventArg = das::cast<const ecs::Event &>::from(event);
        perform_ecs_eid_query(eid, *cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      }
      , eventId));
    }
  }

  for (auto &[system, types, mangledHash] : unresolvedRequests)
  {
    auto loopFunc = ctx->fnByMangledName(mangledHash);
    if (loopFunc == nullptr)
    {
      continue;
    }

    for (const auto &type : types)
    {
      auto copy = system;
      int eventId = ecs::request_name_to_index(type.c_str());
      file.resolvedRequests.emplace_back(ecs::register_request(
      std::move(copy.file),
      std::move(copy.name),
      copy.cache,
      std::move(copy.arguments),
      std::move(copy.requiredComponents),
      std::move(copy.requiredNotComponents),
      std::move(copy.before),
      std::move(copy.after),
      std::move(copy.tags),
      [cache = system.cache, ctx, loopFunc](ecs::Request &request)
      {
        vec4f eventArg = das::cast<ecs::Request &>::from(request);
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      },
      [cache = system.cache, ctx, loopFunc](ecs:: EntityId eid, ecs::Request &event)
      {
        vec4f eventArg = das::cast<ecs::Request &>::from(event);
        perform_ecs_eid_query(eid, *cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      }
      , eventId));
    }
  }
  clear_unresolved_systems();
}


namespace ecs
{
  int get_next_event_index();
  void register_event(int, EventTypeDescription);
  void update_event(int, EventTypeDescription);

  int get_next_request_index();
  void register_request(int, RequestTypeDescription);
  void update_request(int, RequestTypeDescription);
}

static ecs::EventTypeDescription get_event_desc(const das::StructurePtr &st)
{
  return ecs::EventTypeDescription{st->name.c_str(), (ecs::uint)st->getSizeOf(), true};
}

static ecs::RequestTypeDescription get_request_desc(const das::StructurePtr &st)
{
  return ecs::RequestTypeDescription{st->name.c_str(), (ecs::uint)st->getSizeOf()};
}

int register_das_event(const das::StructurePtr &st)
{
  int idx = ecs::event_name_to_index(st->name.c_str());
  if (idx < 0)
    ecs::register_event(idx = ecs::get_next_event_index(), get_event_desc(st));
  else
    ecs::update_event(idx, get_event_desc(st));
  printf("register_das_event %s <%d>\n", st->name.c_str(), idx);
  return idx;
}

int register_das_request(const das::StructurePtr &st)
{
  int idx = ecs::request_name_to_index(st->name.c_str());
  if (idx < 0)
    ecs::register_request(idx = ecs::get_next_request_index(), get_request_desc(st));
  else
    ecs::update_request(idx, get_request_desc(st));
  printf("register_das_request %s <%d>\n", st->name.c_str(), idx);
  return idx;
}


struct DasDefferedEvent
{
  char *data;
  int eventId;
  DasDefferedEvent(int size_of, const void *src_data, int event_id) : data(new char[size_of]), eventId(event_id)
  {
    memcpy(data, src_data, size_of);
  }
  void operator()() const
  {
    ecs::get_query_manager().sendEventImmediate(*((const ecs::Event *)data), eventId);
    delete[] data;
  }
};

struct DasDefferedEidEvent
{
  char *data;
  int eventId;
  ecs::EntityId eid;
  DasDefferedEidEvent(ecs::EntityId eid, int size_of, const void *src_data, int event_id) :
  data(new char[size_of]), eventId(event_id), eid(eid)
  {
    memcpy(data, src_data, size_of);
  }
  void operator()() const
  {
    ecs::get_query_manager().sendEventImmediate(eid, *((const ecs::Event *)data), eventId);
    delete[] data;
  }
};


void builtin_send_event(int size_of, int eventId, const void *event)
{
  ecs::get_query_manager().eventsQueue.push(DasDefferedEvent(size_of, event, eventId));
}

void builtin_send_event_immediate(int size_of, int eventId, const void *event)
{
  ecs::get_query_manager().sendEventImmediate(*((const ecs::Event *)event), eventId);
}

void builtin_send_eid_event(ecs::EntityId eid, int size_of, int eventId, const void *event)
{
  ecs::get_query_manager().eventsQueue.push(DasDefferedEidEvent(eid, size_of, event, eventId));
}

void builtin_send_eid_event_immediate(ecs::EntityId eid, int size_of, int eventId, const void *event)
{
  ecs::get_query_manager().sendEventImmediate(eid, *((const ecs::Event *)event), eventId);
}

void builtin_send_request(int size_of, int eventId, const void *event)
{
  ecs::get_query_manager().sendRequest(*((ecs::Request *)event), eventId);
}


ecs::EntityId create_entity_with_init(ecs::prefab_id id, const InitBlock &block, das::Context *context, das::LineInfoArg *at)
{
  if (id == ecs::invalidPrefabId)
    return ecs::EntityId();

  ComponentInitializer overrides_list;
  vec4f args = das::cast<ComponentInitializer&>::from(overrides_list);
  context->invoke(block, &args, nullptr, at);
  return ecs::create_entity(id, std::move(overrides_list));
}

ecs::EntityId create_entity_with_init_n(const char *prefab_name, const InitBlock &block, das::Context *context, das::LineInfoArg *at)
{
  return create_entity_with_init(ecs::get_prefab_id(prefab_name), block, context, at);
}

ecs::EntityId create_entity(const char *prefab_name)
{
  return ecs::create_entity(prefab_name);
}

ecs::EntityId create_entity_n(ecs::prefab_id id)
{
  return ecs::create_entity(id);
}

ecs::EntityId create_entity_immediate_with_init(ecs::prefab_id id, const InitBlock &block, das::Context *context, das::LineInfoArg *at)
{
  if (id == ecs::invalidPrefabId)
    return ecs::EntityId();

  ComponentInitializer overrides_list;
  vec4f args = das::cast<ComponentInitializer&>::from(overrides_list);
  context->invoke(block, &args, nullptr, at);
  return ecs::create_entity_immediate(id, std::move(overrides_list));
}

ecs::EntityId create_entity_immediate_with_init_n(const char *prefab_name, const InitBlock &block, das::Context *context, das::LineInfoArg *at)
{
  return create_entity_immediate_with_init(ecs::get_prefab_id(prefab_name), block, context, at);
}

ecs::EntityId create_entity_immediate(const char *prefab_name)
{
  return ecs::create_entity_immediate(prefab_name);
}

ecs::EntityId create_entity_immediate_n(ecs::prefab_id id)
{
  return ecs::create_entity_immediate(id);
}

ecs::prefab_id create_entity_prefab(const char *name, const InitBlock &block, das::Context *context, das::LineInfoArg *at)
{
  ComponentInitializer overrides_list;
  vec4f args = das::cast<ComponentInitializer&>::from(overrides_list);
  context->invoke(block, &args, nullptr, at);
  return ecs::create_entity_prefab(name, std::move(overrides_list));
}

#include <3dmath.h>

vec4f init_component(das::Context &context, das::SimNode_CallBase *call, vec4f *args)
{
  auto typeDecl = das::cast<const das::TypeDeclPtr>::to(args[0]);
  auto typeId = das::cast<int>::to(args[1]);
  auto&list = das::cast<ComponentInitializer&>::to(args[2]);
  auto name = das::cast<const char *>::to(args[3]);
  auto rawDataPtr = das::cast<const void *>::to(args[4]);
  auto rawData = args[4];
  //ECS_LOG("init_component %s:<%s> type<%d>", name, get_das_type_name(*typeDecl), typeId);
  if (typeId >= 0)
  {
    #define BASE_TYPE(enumType, cppType) case das::Type::enumType: list.emplace_back(name, typeId, *(const cppType*)&rawData); break

    switch (typeDecl->baseType)
    {
    //case das::Type::tStructure: return type.structType->name.c_str(); break;
    //case das::Type::tHandle: return type.annotation->name != "ecs_string" ? type.annotation->name.c_str() : "string"; break;
    BASE_TYPE(tInt, int);
    BASE_TYPE(tInt2, ivec2);
    BASE_TYPE(tInt3, ivec3);
    BASE_TYPE(tInt4, ivec4);
    BASE_TYPE(tFloat, float);
    BASE_TYPE(tFloat2, vec2);
    BASE_TYPE(tFloat3, vec3);
    BASE_TYPE(tFloat4, vec4);
    BASE_TYPE(tDouble, double);
    BASE_TYPE(tBool, bool);
    case das::Type::tString: list.emplace_back(name, typeId, ecs::string((const char*)rawDataPtr)); break;

    default: ECS_LOG("unsopported init_component %s:<%s> type<%d>", name, get_das_type_name(*typeDecl), typeId); break;

    }
  }
  else
  {
    ECS_LOG("not registered type init_component %s:<%s> type<%d>", name, get_das_type_name(*typeDecl), typeId);
  }
  return v_zero();
}
/*
#include <ecs/type_registration.h>

struct DasTypeFabric final : public ecs::TypeFabric
{
  das::StructurePtr annotation;
  DasTypeFabric(ecs::Type type, bool has_awaiter = false):
    ecs::TypeFabric(type, true, has_awaiter)
  {}

  void copy_constructor(void *raw_memory, const void *source) const override
  {
    annotation->canCopy()
    new (raw_memory) T(*(const T *)source);
  }
  void move_constructor(void *raw_memory, void *source) const override
  {
    new (raw_memory) T(std::move(*(T *)source));
  }
  void destructor(void *memory) const override
  {
    ((T *)memory)->~T();
  }
  void prefab_constructor(void *raw_memory, const ecs::ComponentPrefab &prefab, bool r_value) const override
  {
    return template_blk_constructor<T>(raw_memory, prefab, r_value);
  }
}; */