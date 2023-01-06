#include <daScript/daScript.h>
#include <ecs/ecs.h>
#include <ecs/archetype_manager.h>
#include "das_load.h"
#include "das_modules/ecs_module.h"

static ecs::vector<ecs::string> to_ecs_string_array(const das::Array &a, const char *descr)
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
    v.emplace_back(data[i].name, ecs::type_name_to_index(data[i].type));
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


static thread_local das::vector<ecs::SystemDescription> unresolvedSystems;
static thread_local das::vector<ecs::QueryDescription> unresolvedQueries;
static thread_local das::vector<eastl::pair<ecs::EventDescription, ecs::vector<ecs::string>>> unresolvedEvents;
static thread_local das::vector<eastl::pair<ecs::RequestDescription, ecs::vector<ecs::string>>> unresolvedRequests;


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
    const das::FunctionPtr &system)
{
  const char *name = system->name.c_str();
  printf("system %s, stage %s\n", name, stage);
  printf("%s\n", system->describe().c_str());

  unresolvedSystems.emplace_back(system->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(system->arguments),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      stage,
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr);
}

bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event)
{
  if (event->arguments.size() == 0)
    return false;

  const char *name = event->name.c_str();
  printf("event %s\n", name);
  printf("%s\n", event->describe().c_str());

  unresolvedEvents.emplace_back(ecs::EventDescription(event->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(event->arguments, false),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr,
      nullptr), get_event_types("Event", event->arguments[0], to_ecs_string_array(on_event, "on_event")));

  return true;
}

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request)
{
  if (request->arguments.size() == 0)
    return false;

  const char *name = request->name.c_str();
  printf("event %s\n", name);
  printf("%s\n", request->describe().c_str());

  unresolvedRequests.emplace_back(ecs::RequestDescription(request->at.fileInfo->name.c_str(), name, new ecs::QueryCache(),
      to_ecs_arguments(request->arguments, false),
      to_ecs_components(require_args, "required"),
      to_ecs_components(require_not_args, "not required"),
      to_ecs_string_array(before, "before"),
      to_ecs_string_array(after, "after"),
      to_ecs_string_array(tags, "tags"),
      nullptr,
      nullptr), get_event_types("Request", request->arguments[0], to_ecs_string_array(on_request, "on_request")));

  return true;
}

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block)
{
  auto name = block->at.fileInfo->name + eastl::to_string(block->at.line);
  printf("query %s\n", name.c_str());
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
    file.resolvedQueries.emplace_back(ecs::register_query(std::move(system)));
  }

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
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);});
      };
    file.resolvedSystems.emplace_back(ecs::register_system(std::move(system)));
  }
  
  for (auto &[system, types] : unresolvedEvents)
  {
    auto systemName = system.name + "`implementation";
    auto loopFunc = ctx->findFunction(systemName.c_str());
    if (loopFunc == nullptr)
    {
      continue;
    }
    system.broadcastEventHandler = [cache = system.cache, ctx, loopFunc](const ecs::Event &event)
      {
        vec4f eventArg = das::cast<const ecs::Event &>::from(event);
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      };
    
    system.unicastEventHandler = [cache = system.cache, ctx, loopFunc](ecs:: EntityId eid, const ecs::Event &event)
      {
        vec4f eventArg = das::cast<const ecs::Event &>::from(event);
        perform_ecs_eid_query(eid, *cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      };
    for (const auto &type : types)
    {
      auto eventCopy = system;
      int eventId = ecs::event_name_to_index(type.c_str());
      file.resolvedEvents.emplace_back(ecs::register_event(std::move(eventCopy), eventId));
    }
  }

  for (auto &[system, types] : unresolvedRequests)
  {
    auto systemName = system.name + "`implementation";
    auto loopFunc = ctx->findFunction(systemName.c_str());
    if (loopFunc == nullptr)
    {
      continue;
    }
    system.broadcastRequestHandler = [cache = system.cache, ctx, loopFunc](ecs::Request &request)
      {
        vec4f eventArg = das::cast<ecs::Request &>::from(request);
        perform_ecs_loop(*cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      };
    
    system.unicastRequestHandler = [cache = system.cache, ctx, loopFunc](ecs:: EntityId eid, ecs::Request &event)
      {
        vec4f eventArg = das::cast<ecs::Request &>::from(event);
        perform_ecs_eid_query(eid, *cache, [&](vec4f *args) { ctx->call(loopFunc, args, nullptr);}, &eventArg);
      };
    for (const auto &type : types)
    {
      auto eventCopy = system;
      int eventId = ecs::request_name_to_index(type.c_str());
      file.resolvedRequests.emplace_back(ecs::register_request(std::move(eventCopy), eventId));
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

void register_das_event(const das::StructurePtr &st)
{
  printf("register_das_event %s\n", st->name.c_str());
  int idx = ecs::event_name_to_index(st->name.c_str());
  if (idx < 0)
    ecs::register_event(ecs::get_next_event_index(), get_event_desc(st));
  else
    ecs::update_event(idx, get_event_desc(st));
}

void register_das_request(const das::StructurePtr &st)
{
  printf("register_das_request %s\n", st->name.c_str());
  int idx = ecs::request_name_to_index(st->name.c_str());
  if (idx < 0)
    ecs::register_request(ecs::get_next_request_index(), get_request_desc(st));
  else
    ecs::update_request(idx, get_request_desc(st));
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

static int verify_event(const char *name, int &eventId)
{
  eventId = ecs::event_name_to_index(name);
  if (eventId < 0)
  {
    printf("unsupported event %s\n", name);
  }
  return eventId < 0;
}

void builtin_send_event(int size_of, const char *name, const void *event)
{
  int eventId;
  if (verify_event(name, eventId))
    ecs::get_query_manager().eventsQueue.push(DasDefferedEvent(size_of, event, eventId));
}

void builtin_send_event_immediate(int size_of, const char *name, const void *event)
{
  int eventId;
  if (verify_event(name, eventId))
    ecs::get_query_manager().sendEventImmediate(*((const ecs::Event *)event), eventId);
}

void builtin_send_eid_event(ecs::EntityId eid, int size_of, const char *name, const void *event)
{
  int eventId;
  if (verify_event(name, eventId))
    ecs::get_query_manager().eventsQueue.push(DasDefferedEidEvent(eid, size_of, event, eventId));
}

void builtin_send_eid_event_immediate(ecs::EntityId eid, int size_of, const char *name, const void *event)
{
  int eventId;
  if (verify_event(name, eventId))
    ecs::get_query_manager().sendEventImmediate(eid, *((const ecs::Event *)event), eventId);
}

void builtin_send_request(int size_of, const char *name, const void *event)
{
  int eventId;
  if (verify_event(name, eventId))
    ecs::get_query_manager().sendRequest(*((ecs::Request *)event), eventId);
}
