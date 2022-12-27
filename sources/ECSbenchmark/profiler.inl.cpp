#include "profiler.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache menu_profiler__cache__;

static void menu_profiler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiMenuRender &>(event), menu_profiler__cache__, menu_profiler);
}

static void menu_profiler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiMenuRender &>(event), menu_profiler__cache__, menu_profiler);
}

static void registration_pull_profiler()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "menu_profiler",
  &menu_profiler__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &menu_profiler_handler, &menu_profiler_single_handler),
  ecs::EventIndex<ImguiMenuRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_profiler)
ECS_PULL_DEFINITION(variable_pull_profiler)
