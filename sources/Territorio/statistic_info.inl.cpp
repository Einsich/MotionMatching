#include "statistic_info.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache gather_lands__cache__;

static ecs::QueryCache show_statistic__cache__;

static ecs::QueryCache move_text__cache__;

template<typename Callable>
static void gather_lands(Callable lambda)
{
  ecs::perform_query<uint, int, uint>(gather_lands__cache__, lambda);
}

static void show_statistic_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), show_statistic__cache__, show_statistic);
}

static void show_statistic_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), show_statistic__cache__, show_statistic);
}

static void move_text_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), move_text__cache__, move_text);
}

static void move_text_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), move_text__cache__, move_text);
}

static void registration_pull_statistic_info()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_lands",
  &gather_lands__cache__,
  {
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()}
  },
  {
    {"isPlayableLand", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_event(ecs::EventDescription(
  "",
  "show_statistic",
  &show_statistic__cache__,
  {
    {"land_colors", ecs::get_type_index<ecs::vector<vec3>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec3>>()}
  },
  {},
  {},
  {},
  {},
  {},
  &show_statistic_handler, &show_statistic_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "move_text",
  &move_text__cache__,
  {
    {"position", ecs::get_type_index<ivec2>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ivec2>()},
    {"text", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::string>()}
  },
  {},
  {},
  {},
  {},
  {},
  &move_text_handler, &move_text_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_statistic_info)
