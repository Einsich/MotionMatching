#include "test.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache ecs_view__cache__;

static ecs::QueryCache init__cache__;

static void ecs_view_implementation()
{
  ecs::perform_system(ecs_view__cache__, ecs_view);
}

static void init_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void init_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init__cache__, init);
}

static void registration_pull_test()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSMultiThreading/test.inl:331",
  "ecs_view",
  &ecs_view__cache__,
  {},
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &ecs_view_implementation);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/ECSMultiThreading/test.inl:167",
  "init",
  &init__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &init_handler, &init_single_handler,
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_test)
ECS_PULL_DEFINITION(variable_pull_test)
